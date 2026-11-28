-- tools/tests.lua - everything about tests: compiling one test, the markdown
-- test files, and the commands tests, record, copy and run.

local base = require("base")
local build = require("build")

local quote, capture, succeeds = base.quote, base.capture, base.succeeds
local read_file, write_file = base.read_file, base.write_file
local lines, command_lines, append = base.lines, base.command_lines, base.append
local trim, starts_with = base.trim, base.starts_with
local green, red, die = base.green, base.red, base.die
local BIN, TESTS_DIR, have_timeout = base.BIN, base.TESTS_DIR, base.have_timeout

-- ---------------------------------------------------------------------------
-- compiling one test
-- ---------------------------------------------------------------------------

local function require_compiler()
    if not succeeds("test -x " .. quote(BIN)) then
        die("compile: bin/ura not found - run 'build' first")
    end
end

-- compiler stderr without colors, and "main.c:1165 find_function message"
-- written as "find_function: message": the line number moves on every edit, the name doesn't
local function clean_stderr(text)
    text = text:gsub("\27%[[0-9;]*m", "")
    local out = {}
    for _, line in ipairs(lines(text)) do
        out[#out + 1] = line:gsub("^[^ :]+%.[ch]:%d+ ([%a_][%w_]*) ", "%1: ")
    end
    return (table.concat(out, "\n"):gsub("\n+$", ""))
end

-- LLVM IR without the lines that name the module and the machine
local function clean_ir(text)
    local skipped = { "; ModuleID = ", "source_filename = ", "target datalayout = ", "target triple = " }
    local out = {}
    for _, line in ipairs(lines(text)) do
        local skip = false
        for _, prefix in ipairs(skipped) do
            if starts_with(line, prefix) then skip = true end
        end
        if not skip and (#out > 0 or line:match("%S")) then out[#out + 1] = line end
    end
    return (table.concat(out, "\n"):gsub("\n+$", ""))
end

-- the text the compiler printed under a "──── name ────" header, up to the next header
local function printed_part(printed, name)
    local start = printed:find("──── " .. name .. " ", 1, true)
    if not start then return "" end
    local line_end = printed:find("\n", start, true) or #printed
    local body = printed:sub(line_end + 1)
    local next_header = body:find("──── ", 1, true)
    if next_header then body = body:sub(1, next_header - 1) end
    return (body:gsub("\n+$", ""))
end

-- Compiles `code` as test.ura in a temporary folder with -ucg -ll, then runs
-- the program (its stdin is the "// stdin:" lines). Returns
-- { ok, ucg, ll, stdout, stderr, status }: when the compile fails, stderr and
-- status are the compiler's, else the program's.
local function compile_case(code)
    local dir = trim((capture("mktemp -d")))
    write_file(dir .. "/test.ura", code)

    -- a compiler or program that loops forever fails its test instead of the whole run
    local limit = have_timeout and "timeout 10 " or ""
    local cd = "cd " .. quote(dir) .. " && " .. limit

    -- the compiler runs from the test's folder, so messages say "test.ura:2:11",
    -- and the program is ./test
    local _, status = capture(cd .. quote(BIN) .. " test.ura -ucg -ll >printed.txt 2>stderr.txt")
    local printed = (read_file(dir .. "/printed.txt") or ""):gsub("\27%[[0-9;]*m", "")
    local result = {
        ok = status == 0,
        ucg = printed_part(printed, "ucg"),
        ll = clean_ir(printed_part(printed, "llvm")),
        stdout = "",
        stderr = clean_stderr(read_file(dir .. "/stderr.txt") or ""),
        status = status,
    }

    if result.ok then
        local input = {}
        for _, line in ipairs(lines(code)) do
            local text = line:match("^// stdin: ?(.*)$")
            if text then input[#input + 1] = text .. "\n" end
        end
        write_file(dir .. "/stdin.txt", table.concat(input))
        result.stdout, result.status = capture(cd .. "./test < stdin.txt 2>err.txt")
        result.stderr = read_file(dir .. "/err.txt") or ""
    end

    os.execute("rm -rf " .. quote(dir))
    return result
end

-- a failed compile's stderr, indented and short: for a sanitizer crash only
-- the error line and the first two stack frames, otherwise the first 8 lines
local function print_failure(stderr)
    local shown = {}
    if stderr:find("AddressSanitizer") then
        for _, line in ipairs(lines(stderr)) do
            if line:find("ERROR: AddressSanitizer") or line:match("^ *#[01] ") then
                shown[#shown + 1] = line
            end
        end
    else
        for i, line in ipairs(lines(stderr)) do
            if i <= 8 then shown[#shown + 1] = line end
        end
    end
    for _, line in ipairs(shown) do print("      " .. (line:gsub("^ +", ""))) end
end

local function show_diff(expected, actual)
    local a, b = os.tmpname(), os.tmpname()
    write_file(a, expected .. "\n")
    write_file(b, actual .. "\n")
    local output = capture("diff " .. a .. " " .. b .. " | head -6")
    os.remove(a)
    os.remove(b)
    for _, line in ipairs(lines(output)) do print("      " .. line) end
end

-- ---------------------------------------------------------------------------
-- test files (markdown): "## NNN — description" sections
-- ---------------------------------------------------------------------------

-- Splits a markdown file into sections, each starting at a "## NNN — description"
-- heading: { number, description, line, lines }. The "---" rules and blank
-- lines between sections are not part of them.
local function parse_sections(path)
    local sections, current = {}, nil
    for i, line in ipairs(lines(read_file(path) or "")) do
        local number, description = line:match("^## (%d%d%d) — (.*)$")
        if number then
            current = { number = number, description = description, line = i, lines = {} }
            sections[#sections + 1] = current
        end
        if current then current.lines[#current.lines + 1] = line end
    end
    for _, section in ipairs(sections) do
        local last = section.lines
        while last[#last] == "" or last[#last] == "---" do last[#last] = nil end
    end
    return sections
end

local function find_section(sections, number)
    for _, section in ipairs(sections) do
        if section.number == number then return section end
    end
end

local function find_by_description(sections, description)
    for _, section in ipairs(sections) do
        if section.description == description then return section end
    end
end

-- the contents of the first code block of a section, or of the first one
-- opened with ```<kind>; nil when there is none
local function fence_block(section_lines, kind)
    local block, inside = {}, false
    for _, line in ipairs(section_lines) do
        if inside then
            if starts_with(line, "```") then return block end
            block[#block + 1] = line
        elseif (kind and line == "```" .. kind) or (not kind and starts_with(line, "```")) then
            inside = true
        end
    end
    return inside and block or nil
end

local function entry_code(section)
    return table.concat(fence_block(section.lines) or {}, "\n") .. "\n"
end

-- the name a test file is shown under: its path below tests/, without ".md"
local function label_of(md_path)
    return (md_path:sub(#TESTS_DIR + 2):gsub("%.md$", ""))
end

-- "<path>:<line>" of an entry, which terminals turn into a link
local function entry_link(md_path, section)
    return md_path:sub(#TESTS_DIR + 2) .. ":" .. section.line
end

-- writes the test file: header, "## index" bullet list, then every entry after a "---" rule
local function write_tests(md_path, sections)
    local out = { "# " .. label_of(md_path):match("^[^/]+"), "", "## index", "" }
    for _, section in ipairs(sections) do
        out[#out + 1] = "- " .. section.number .. " — " .. section.description
    end
    out[#out + 1] = ""
    for _, section in ipairs(sections) do
        out[#out + 1] = "---"
        out[#out + 1] = ""
        append(out, section.lines)
        out[#out + 1] = ""
    end
    write_file(md_path, table.concat(out, "\n") .. "\n")
end

-- each folder records its tests in three files next to them:
--   ucg.md     the -ucg output
--   llvm.md    the LLVM IR
--   output.md  the program's stdout, stderr and exit status (the compiler's when it fails)
local RECORDINGS = { "ucg", "llvm", "output" }
local NOT_RECORDINGS = "! -name llvm-ir.md ! -name ucg.md ! -name llvm.md ! -name output.md"

local function recordings_path(md_path, name)
    return md_path:match("^(.*)/[^/]*$") .. "/" .. name .. ".md"
end

local function fenced(kind, text)
    local out = { "```" .. kind }
    if text ~= "" then append(out, lines(text)) end
    out[#out + 1] = "```"
    return out
end

-- the recorded lines of one result for one recordings file
local function recorded_lines(result, name)
    if name == "ucg" then return fenced("ucg", result.ucg) end
    if name == "llvm" then return fenced("llvm", result.ll) end
    local out = fenced("stdout", result.stdout)
    append(out, fenced("stderr", result.stderr))
    append(out, fenced("status", tostring(result.status)))
    return out
end

-- every recordings file of a test file: { ucg = sections, llvm = sections, output = sections }
local function load_recordings(md_path)
    local all = {}
    for _, name in ipairs(RECORDINGS) do all[name] = parse_sections(recordings_path(md_path, name)) end
    return all
end

-- sets a result as the entry's recording in every recordings file
local function set_recording(all, section, result)
    for _, name in ipairs(RECORDINGS) do
        local recorded = { "## " .. section.number .. " — " .. section.description, "" }
        append(recorded, recorded_lines(result, name))
        local existing = find_section(all[name], section.number)
        if existing then
            existing.lines = recorded
        else
            all[name][#all[name] + 1] = { number = section.number, description = section.description, lines = recorded }
        end
    end
end

local function save_recordings(md_path, all)
    local folder = md_path:match("^(.*)/[^/]*$"):match("[^/]+$")
    for _, name in ipairs(RECORDINGS) do
        local out = { "# " .. folder .. " " .. name, "" }
        for _, recording in ipairs(all[name]) do
            append(out, recording.lines)
            out[#out + 1] = ""
        end
        write_file(recordings_path(md_path, name), table.concat(out, "\n") .. "\n")
    end
end

-- ---------------------------------------------------------------------------
-- choosing test files
-- ---------------------------------------------------------------------------

local function md_files_in(folder)
    return command_lines("find " .. quote(folder) .. " -name '*.md' " .. NOT_RECORDINGS .. " | sort")
end

-- The test files <target> names, and <number> padded to 3 digits:
--   (nothing)  every .md under tests/, except tests/pending/
--   m01        every .md in tests/m01/
--   m01 42     only the file of tests/m01/ that holds entry 042
local function select_tests(target, number)
    if number then
        number = string.format("%03d", tonumber(number) or die("not a number: " .. number))
    end

    local files
    if not target then
        files = command_lines("find " .. quote(TESTS_DIR) .. " -path " .. quote(TESTS_DIR .. "/pending") ..
            " -prune -o -name '*.md' " .. NOT_RECORDINGS .. " -print | sort")
    elseif read_file(TESTS_DIR .. "/" .. target .. ".md") then
        files = { TESTS_DIR .. "/" .. target .. ".md" }
    elseif succeeds("test -d " .. quote(TESTS_DIR .. "/" .. target)) then
        files = md_files_in(TESTS_DIR .. "/" .. target)
    else
        files = command_lines("find " .. quote(TESTS_DIR) .. " -name " .. quote(target .. ".md") ..
            " " .. NOT_RECORDINGS .. " | sort")
    end

    if number then
        local holding = {}
        for _, file in ipairs(files) do
            if find_section(parse_sections(file), number) then holding[#holding + 1] = file end
        end
        files = holding
    end

    if #files == 0 then die("no tests named '" .. (target or "") .. "' in tests/") end
    if number and #files ~= 1 then die("a test number needs one folder, like: m01 " .. tonumber(number)) end
    return files, number
end

-- the sections of a file, or only the one with that number
local function wanted_sections(md_path, number)
    local sections = parse_sections(md_path)
    if not number then return sections end
    local section = find_section(sections, number)
    if not section then die("no test " .. number .. " in " .. label_of(md_path)) end
    return { section }
end

-- ---------------------------------------------------------------------------
-- tests
-- ---------------------------------------------------------------------------

local function normalize(text)
    text = text:gsub("[ \t]+\n", "\n"):gsub("[ \t]+$", "")
    return (text:gsub("\n+$", ""))
end

-- prints a failure and returns false when the texts differ
local function compare(recorded, fresh, what, name, link)
    if normalize(recorded) == normalize(fresh) then return true end
    red(link .. " " .. name .. " (" .. what .. " differs)")
    show_diff(normalize(recorded), normalize(fresh))
    return false
end

-- compares the program's run with the entry's "// stdout:" lines and "// expect:" exit code
local function check_run(code, result, name, link)
    local expected, has_stdout, exit_code = {}, false, nil
    for _, line in ipairs(lines(code)) do
        local text = line:match("^// stdout: ?(.*)$")
        if text then
            has_stdout = true
            expected[#expected + 1] = text
        end
        exit_code = exit_code or line:match("^// expect:.-(%d+)")
    end

    if has_stdout and not compare(table.concat(expected, "\n"), result.stdout, "stdout", name, link) then
        return false
    end
    if exit_code and exit_code ~= tostring(result.status) then
        red(link .. " " .. name .. " (exit " .. tostring(result.status) .. ", expected " .. exit_code .. ")")
        return false
    end
    return true
end

-- runs one entry and prints its result line; true when it passed:
-- the ucg, the LLVM IR and the output must match the recordings
local function test_entry(md_path, section, all)
    local code = entry_code(section)
    local name, link = section.description, entry_link(md_path, section)
    if not find_section(all.output, section.number) then
        red(link .. " " .. name .. " (not recorded: run record)")
        return false
    end

    local result = compile_case(code)
    local passed = true
    for _, kind in ipairs(RECORDINGS) do
        local recording = find_section(all[kind], section.number)
        local recorded = ""
        if recording then recorded = table.concat(recording.lines, "\n", 3) end
        local fresh = table.concat(recorded_lines(result, kind), "\n")
        passed = passed and compare(recorded, fresh, kind, name, link)
    end
    passed = passed and check_run(code, result, name, link)

    if passed then green(link .. " " .. name) end
    return passed
end

local function tests(args)
    build.build_if_changed() -- always test the current sources, never a stale bin/ura
    require_compiler()
    print()

    local files, number = select_tests(args[1], args[2])
    local passed, failed = 0, 0
    for _, md_path in ipairs(files) do
        local sections = wanted_sections(md_path, number)
        if #sections > 0 then
            print(label_of(md_path))
            local all = load_recordings(md_path)
            for _, section in ipairs(sections) do
                if test_entry(md_path, section, all) then passed = passed + 1 else failed = failed + 1 end
            end
        end
    end

    print()
    print(passed .. " passed, " .. failed .. " failed")
    if failed > 0 then os.exit(1) end
end

-- ---------------------------------------------------------------------------
-- record
-- ---------------------------------------------------------------------------

local function record(args)
    build.build({})
    require_compiler()
    print()

    local files, number = select_tests(args[1], args[2])
    local recorded = 0
    for _, md_path in ipairs(files) do
        local sections = parse_sections(md_path)
        local all = load_recordings(md_path)
        print(label_of(md_path))

        -- whatever the compile and the run give is recorded: check the output before committing
        for _, section in ipairs(sections) do
            if not number or section.number == number then
                local result = compile_case(entry_code(section))
                set_recording(all, section, result)
                local link = entry_link(md_path, section)
                if result.ok then
                    green(link .. " " .. section.description)
                else
                    red(link .. " " .. section.description .. " (doesn't compile, its error is recorded)")
                    print_failure(result.stderr)
                end
                recorded = recorded + 1
            end
        end

        save_recordings(md_path, all)
        write_tests(md_path, sections) -- rewrites the index
    end

    print()
    print(recorded .. " recorded")
end

-- ---------------------------------------------------------------------------
-- copy
-- ---------------------------------------------------------------------------

-- the code with blank lines at the top and bottom removed, as a list of lines
local function trimmed_lines(code)
    local list = lines(code)
    while list[1] == "" do table.remove(list, 1) end
    while list[#list] == "" do list[#list] = nil end
    return list
end

local function copy(args)
    local ura_file, level, description = args[1], args[2], args[3]
    if not (ura_file and level and description) then
        die("usage: copy <ura_file> <file> <description>")
    end
    local code = read_file(ura_file) or die("copy: no such file: " .. ura_file)
    -- level becomes a path below tests/: folders are fine, leaving tests/ isn't
    if starts_with(level, "/") or starts_with(level, ".") or level:find("/%.") then
        die("copy: give a path inside tests/, like m01")
    end

    require_compiler()

    -- a folder: the file that has this description, else its last file
    local md_path = TESTS_DIR .. "/" .. level .. ".md"
    if succeeds("test -d " .. quote(TESTS_DIR .. "/" .. level)) then
        for _, file in ipairs(md_files_in(TESTS_DIR .. "/" .. level)) do
            md_path = file
            if find_by_description(parse_sections(file), description) then break end
        end
    end
    os.execute("mkdir -p " .. quote(md_path:match("^(.*)/[^/]*$")))

    -- a failed compile is recorded too, as an error test
    local result = compile_case(code)

    local sections = parse_sections(md_path)
    local section = find_by_description(sections, description)
    local verb = "replaced"
    if not section then
        verb = "added"
        local last = sections[#sections]
        section = { number = string.format("%03d", last and tonumber(last.number) + 1 or 1), description = description }
        sections[#sections + 1] = section
    end

    section.lines = { "## " .. section.number .. " — " .. description, "", "```ura" }
    append(section.lines, trimmed_lines(code))
    section.lines[#section.lines + 1] = "```"

    local all = load_recordings(md_path)
    set_recording(all, section, result)
    save_recordings(md_path, all)
    write_tests(md_path, sections)

    green("copy: " .. verb .. " " .. section.number .. " — " .. description ..
        (verb == "added" and " to " or " in ") .. label_of(md_path) .. ".md")
end

-- ---------------------------------------------------------------------------
-- run
-- ---------------------------------------------------------------------------

-- compiles one entry and runs its program with the terminal as stdin and stdout
local function run(args)
    if not (args[1] and args[2]) then die("run: give a file and a number, like: run m13 41") end
    build.build({})
    local files, number = select_tests(args[1], args[2])
    local section = find_section(parse_sections(files[1]), number)

    local dir = trim((capture("mktemp -d")))
    write_file(dir .. "/test.ura", entry_code(section))

    if not succeeds("cd " .. quote(dir) .. " && " .. quote(BIN) .. " test.ura </dev/null") then
        os.execute("rm -rf " .. quote(dir))
        die("run: test " .. number .. " doesn't compile")
    end
    local _, code = base.run("cd " .. quote(dir) .. " && ./test")
    os.execute("rm -rf " .. quote(dir))
    print("exit code: " .. tostring(code))
end

return {
    tests = tests,
    record = record,
    copy = copy,
    run = run,
}