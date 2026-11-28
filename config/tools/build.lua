-- tools/build.lua - building the compiler: LLVM discovery, build, set-llvm, extension.

local base = require("base")

local quote, quote_all, capture, succeeds = base.quote, base.quote_all, base.capture, base.succeeds
local read_file, write_file = base.read_file, base.write_file
local lines, command_lines, words, append = base.lines, base.command_lines, base.words, base.append
local trim, starts_with = base.trim, base.starts_with
local green, die = base.green, base.die
local REPO, SRC_DIR, BIN_DIR, BIN = base.REPO, base.SRC_DIR, base.BIN_DIR, base.BIN

local ENABLE_SANITIZER = true -- AddressSanitizer in every build

-- ---------------------------------------------------------------------------
-- LLVM: find the versions on this machine
-- ---------------------------------------------------------------------------

local function version_less(a, b)
    local x, y = {}, {}
    for n in a:gmatch("%d+") do x[#x + 1] = tonumber(n) end
    for n in b:gmatch("%d+") do y[#y + 1] = tonumber(n) end
    for i = 1, math.max(#x, #y) do
        local p, q = x[i] or 0, y[i] or 0
        if p ~= q then return p < q end
    end
    return false
end

-- every llvm-config in sight as {version, config}, oldest first, one per version
local function llvm_list()
    local names = { "llvm-config" }
    for v = 3, 40 do names[#names + 1] = "llvm-config-" .. v end
    local command = "for c in " .. table.concat(names, " ") .. "; do command -v $c; done; " ..
        "ls /usr/lib/llvm-*/bin/llvm-config /usr/local/opt/llvm*/bin/llvm-config " ..
        '/opt/homebrew/opt/llvm*/bin/llvm-config "$HOME"/.local/opt/llvm*/bin/llvm-config 2>/dev/null; true'

    local found, seen = {}, {}
    for _, config in ipairs(command_lines(command)) do
        local version = trim((capture(quote(config) .. " --version 2>/dev/null")))
        if version ~= "" and not seen[version] then
            seen[version] = true
            found[#found + 1] = { version = version, config = config }
        end
    end
    table.sort(found, function(a, b) return version_less(a.version, b.version) end)
    return found
end

-- the llvm-config of the newest LLVM matching want: exact (15.0.6) or a prefix
-- ending at a dot (15, 15.0); no want: the newest
local function llvm_find(want)
    local pick
    for _, llvm in ipairs(llvm_list()) do
        if want == "" or llvm.version == want or starts_with(llvm.version, want .. ".") then
            pick = llvm.config
        end
    end
    return pick
end

-- LLVM_CONFIG from the environment wins, then LLVM_VERSION (see set-llvm), then the newest
local function pick_llvm_config()
    local from_env = os.getenv("LLVM_CONFIG")
    if from_env and from_env ~= "" then
        if not succeeds("command -v " .. quote(from_env)) then die("build: " .. from_env .. " not found") end
        return from_env
    end
    local want = os.getenv("LLVM_VERSION") or ""
    local config = llvm_find(want)
    if not config and want ~= "" then
        die("build: LLVM " .. want .. " (LLVM_VERSION) not found, run set-llvm to list them")
    elseif not config then
        die("build: no llvm-config found, run as: LLVM_CONFIG=/path/to/llvm-config lua main.lua build")
    end
    return config
end

-- ---------------------------------------------------------------------------
-- build
-- ---------------------------------------------------------------------------

-- the flags that link LLVM; also returns a temporary folder of library links
-- (or nil) that the caller removes once the compile is done
local function link_flags(llvm)
    local libs = words(llvm("--ldflags"))
    libs[#libs + 1] = "-Wl,-rpath," .. llvm("--libdir")
    append(libs, words(llvm("--libs core native bitwriter passes ipo")))

    -- static LLVM needs the C++ runtime
    if llvm("--shared-mode") == "static" then
        libs[#libs + 1] = trim((capture("uname"))) == "Darwin" and "-lc++" or "-lstdc++"
    end

    -- LLVM's system libraries; one that can't be linked by name (no libX.so,
    -- only libX.so.N) gets a link to the installed runtime in a temporary folder
    local shim
    for _, lib in ipairs(words(llvm("--system-libs"))) do
        libs[#libs + 1] = lib
        local name = lib:match("^%-l(.+)")
        local links = name and succeeds("echo 'int main(void) { return 0; }' | clang -x c - " ..
            quote(lib) .. " -o /dev/null")
        if name and not links then
            for _, entry in ipairs(lines((capture("ldconfig -p 2>/dev/null")))) do
                local file, runtime = entry:match("^%s*(%S+)%s.*=>%s*(%S+)$")
                if file and starts_with(file, "lib" .. name .. ".so.") then
                    if not shim then
                        shim = trim((capture("mktemp -d")))
                        table.insert(libs, 1, "-L" .. shim)
                    end
                    os.execute("ln -sf " .. quote(runtime) .. " " .. quote(shim .. "/lib" .. name .. ".so"))
                    break
                end
            end
        end
    end

    libs[#libs + 1] = "-lm" -- log2, sin, cos, ...
    return libs, shim
end

-- the function prototypes of a C file, found with Universal Ctags
local function prototypes(file)
    local out = {}
    local command = "cd " .. quote(SRC_DIR) .. " && ctags -x --c-kinds=f --language-force=C " ..
        "--_xformat='%{typeref} %{name}%{signature};' " .. file
    for _, line in ipairs(command_lines(command)) do
        line = line:gsub("^%l+:", ""):gsub(" (%*+) ", " %1"):gsub(",", ", ")
        if not line:find(" main(", 1, true) then out[#out + 1] = line end
    end
    return out
end

local function generate_prototypes()
    if not capture("ctags --version 2>/dev/null"):find("Universal Ctags") then
        io.stderr:write("build: Universal Ctags not found, protos.h not regenerated\n")
        return
    end

    local out = { "// generated by build in main.lua - do not edit", "#pragma once" }

    for _, file in ipairs({ "arena.h", "fmt.h", "fs.h", "ps.h", "string.h" }) do
        append(out, prototypes("includes/" .. file))
    end

    append(out, prototypes("ucg.c"))
    out[#out + 1] = "#if !UCG"
    for _, file in ipairs({ "main.c", "assert.c" }) do
        append(out, prototypes(file))
    end
    out[#out + 1] = "#endif"

    write_file(SRC_DIR .. "/includes/protos.h", table.concat(out, "\n") .. "\n")
end

-- compiles <source> with LLVM into bin/<name>; extra_flags go to the C compiler
local function build_binary(source, name, extra_flags)
    local llvm_config = pick_llvm_config()
    local function llvm(args) return trim((capture(quote(llvm_config) .. " " .. args))) end
    print("using LLVM " .. llvm("--version") .. " (" .. llvm_config .. ")")

    local flags = {}
    if ENABLE_SANITIZER then append(flags, { "-fsanitize=address", "-g3" }) end
    flags[#flags + 1] = "-Werror"
    append(flags, extra_flags)

    local cflags = words(llvm("--cflags"))
    local libs, shim = link_flags(llvm)

    generate_prototypes()

    os.execute("mkdir -p " .. quote(BIN_DIR))
    local ok = base.run("cd " .. quote(SRC_DIR) .. " && clang " .. quote_all(flags) .. " " ..
        quote_all(cflags) .. " " .. quote(source) .. " " .. quote_all(libs) ..
        " -o " .. quote(BIN_DIR .. "/" .. name))
    if shim then os.execute("rm -rf " .. quote(shim)) end

    if not ok then die("build: failed") end
    green("build: bin/" .. name .. " ready")
end

local function build(flags)
    build_binary("main.c", "ura", flags)
    -- `ura` from any folder: ~/.local/bin is on PATH
    os.execute('mkdir -p "$HOME/.local/bin" && ln -sf ' .. quote(BIN) .. ' "$HOME/.local/bin/ura"')
end

-- rebuild only when a source is newer than bin/ura
local function build_if_changed()
    if succeeds("test -x " .. quote(BIN)) then
        local newer = capture("find " .. quote(SRC_DIR) .. " -maxdepth 2 \\( -name '*.c' -o -name '*.h' " ..
            "\\) ! -name protos.h -newer " .. quote(BIN) .. " -print -quit")
        if newer == "" then return end
    end
    build({})
end

-- ---------------------------------------------------------------------------
-- set-llvm and extension
-- ---------------------------------------------------------------------------

-- with no version, lists the LLVMs on this machine (* marks the one build uses);
-- with one, checks that it exists. config.sh's set-llvm then exports LLVM_VERSION,
-- because only the shell can change its own environment.
local function set_llvm(args)
    local want = args[1]
    if not want then
        local wanted = os.getenv("LLVM_VERSION") or ""
        local active = llvm_find(wanted)
        for _, llvm in ipairs(llvm_list()) do
            print(string.format("%s %-8s %s", llvm.config == active and "*" or " ", llvm.version, llvm.config))
        end
        if wanted == "" then print("LLVM_VERSION not set: build uses the newest") end
        return
    end
    local config = llvm_find(want) or die("set-llvm: no LLVM " .. want .. ", run set-llvm to list them")
    green("set-llvm: build uses LLVM " .. trim((capture(quote(config) .. " --version"))) .. " (" .. config .. ")")
end

local function extension()
    if base.run("make -C " .. quote(REPO .. "/_extension") .. " install") then
        green("extension: installed, reload VS Code to use it")
    else
        die("extension: failed")
    end
end

return {
    build = build,
    build_if_changed = build_if_changed,
    set_llvm = set_llvm,
    extension = extension,
}