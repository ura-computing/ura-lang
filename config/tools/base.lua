-- tools/base.lua - what build.lua and tests.lua share:
-- shell and string helpers, colors, paths, and the test config.

-- ---------------------------------------------------------------------------
-- shell and string helpers
-- ---------------------------------------------------------------------------

local function quote(text)
    return "'" .. text:gsub("'", "'\\''") .. "'"
end

local function quote_all(list)
    local out = {}
    for i, item in ipairs(list) do out[i] = quote(item) end
    return table.concat(out, " ")
end

-- runs a shell command, returns what it printed and its exit code
local function capture(command)
    local pipe = io.popen(command)
    local output = pipe:read("a")
    local _, _, code = pipe:close()
    return output, code or 1
end

local function succeeds(command)
    return os.execute(command .. " >/dev/null 2>&1") == true
end

local function read_file(path)
    local file = io.open(path, "rb")
    if not file then return nil end
    local text = file:read("a")
    file:close()
    return text
end

local function write_file(path, text)
    local file = assert(io.open(path, "wb"))
    file:write(text)
    file:close()
end

-- splits text into lines; one final newline doesn't add an empty line
local function lines(text)
    if text:sub(-1) == "\n" then text = text:sub(1, -2) end
    local list = {}
    for line in (text .. "\n"):gmatch("(.-)\n") do list[#list + 1] = line end
    return list
end

-- the non-empty lines a shell command prints
local function command_lines(command)
    local list = {}
    for _, line in ipairs(lines((capture(command)))) do
        if line ~= "" then list[#list + 1] = line end
    end
    return list
end

local function words(text)
    local list = {}
    for word in text:gmatch("%S+") do list[#list + 1] = word end
    return list
end

local function append(list, more)
    for _, item in ipairs(more) do list[#list + 1] = item end
end

local function trim(text)
    return (text:gsub("^%s+", ""):gsub("%s+$", ""))
end

local function starts_with(text, prefix)
    return text:sub(1, #prefix) == prefix
end

-- true when some line of text starts with prefix
local function has_line(text, prefix)
    return ("\n" .. text):find("\n" .. prefix, 1, true) ~= nil
end

-- ---------------------------------------------------------------------------
-- colors
-- ---------------------------------------------------------------------------

local function green(text) print("\27[0;32m" .. text .. "\27[0m") end
local function red(text) print("\27[0;31m" .. text .. "\27[0m") end

local function die(text)
    io.stderr:write("\27[0;31m" .. text .. "\27[0m\n")
    os.exit(1)
end

-- ---------------------------------------------------------------------------
-- paths: main.lua sits in <repo>/config, the C sources are in <repo>/src
-- ---------------------------------------------------------------------------

local CONFIG_DIR = trim(capture("cd " .. quote(arg[0]:match("^(.*)/[^/]*$") or ".") .. " && pwd"))
local REPO = trim(capture("cd " .. quote(CONFIG_DIR) .. "/.. && pwd"))

local SRC_DIR = REPO .. "/src"
local BIN_DIR = REPO .. "/bin"
local BIN = BIN_DIR .. "/ura"
local TESTS_DIR = REPO .. "/tests"
local PROJECTS_DIR = REPO .. "/ura-libs"

local have_timeout = succeeds("command -v timeout")

-- ---------------------------------------------------------------------------
-- test config
-- ---------------------------------------------------------------------------

-- The ura.conf every test compiles with: tests/ura.conf with each library
-- pointed at its git clone in projects/, so tests run against the code being edited.
local test_config_text
local function test_config()
    if test_config_text then return test_config_text end
    local conf = read_file(TESTS_DIR .. "/ura.conf") or die("no " .. TESTS_DIR .. "/ura.conf")
    local out = {}
    for _, line in ipairs(lines(conf)) do
        local url = line:match('^%s*@url "([^"]*)"')
        if starts_with(line, '@dep "') then
            out[#out + 1] = line
        elseif url then
            out[#out + 1] = '    @path "' .. PROJECTS_DIR .. "/" .. url:match("[^/]+$") .. '"'
        end
    end
    test_config_text = table.concat(out, "\n")
    return test_config_text
end

-- Each library in projects/ gets its own dependencies as links
-- deps/<name>@<version> to the other clones in projects/.
local function link_projects()
    for _, conf in ipairs(command_lines("ls " .. quote(PROJECTS_DIR) .. "/*/ura.conf 2>/dev/null")) do
        local dir = conf:match("^(.*)/[^/]*$")
        local name, repo
        for _, line in ipairs(lines(read_file(conf))) do
            name = line:match('^@dep "([^"]*)"') or name
            repo = line:match('^%s*@url ".*/([^/"]*)"') or repo
            local version = line:match('^%s*@version "([^"]*)"')
            if version then
                os.execute("mkdir -p " .. quote(dir .. "/deps") .. " && ln -sfn " ..
                    quote(PROJECTS_DIR .. "/" .. repo) .. " " .. quote(dir .. "/deps/" .. name .. "@" .. version))
            end
        end
    end
end

return {
    quote = quote, quote_all = quote_all, capture = capture, succeeds = succeeds,
    read_file = read_file, write_file = write_file,
    lines = lines, command_lines = command_lines, words = words, append = append,
    trim = trim, starts_with = starts_with, has_line = has_line,
    green = green, red = red, die = die,

    REPO = REPO, SRC_DIR = SRC_DIR, BIN_DIR = BIN_DIR, BIN = BIN, TESTS_DIR = TESTS_DIR,
    have_timeout = have_timeout,

    test_config = test_config, link_projects = link_projects,
}