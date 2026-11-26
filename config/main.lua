#!/usr/bin/env lua
-- config.lua - build and test workflow for ura-lang (needs Lua 5.3 or newer).
--
--   lua config.lua <command> [arguments]
--
-- commands:
--   build [-D DEBUG=true]      rebuild <repo>/bin/ura, plus proto.h and compile_commands.json
--   ucg [-D DEBUG=true] [n]    build ucg_test.c into <repo>/bin/ucg and run its test n (all when none)
--   tests [target] [number]    rebuild, compile tests, compare the LLVM IR with the recorded
--                              one, run them and compare "// stdout:" and "// expect:"
--   record [target] [number]   rebuild, then (re)write the recorded IR of the tests
--   copy <ura_file> <file> <description>
--                              add <ura_file> to tests/<file>/ as a new entry
--                              (an entry with the same description is replaced)
--   run <target> <number>      compile one entry and run its program in the terminal
--   extension                  package the VS Code extension and install it
--   set-llvm [version]         list the LLVM versions, or pick the one build uses
--
-- The code lives in tools/: base.lua (helpers, paths), build.lua, tests.lua.

package.path = (arg[0]:match("^(.*)/[^/]*$") or ".") .. "/tools/?.lua;" .. package.path

local build = require("build")
local tests = require("tests")

local commands = {
    build = build.build,
    ucg = build.ucg,
    ["set-llvm"] = build.set_llvm,
    extension = build.extension,
    tests = tests.tests,
    record = tests.record,
    copy = tests.copy,
    run = tests.run,
}

local command = commands[arg[1] or ""]
if not command then
    print("commands: build, ucg, tests, record, copy, run, extension, set-llvm")
    os.exit(1)
end
command({ table.unpack(arg, 2) })