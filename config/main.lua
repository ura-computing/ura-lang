#!/usr/bin/env lua
-- main.lua - build and test workflow for ura-lang (needs Lua 5.3 or newer).
--
--   lua config/main.lua <command> [arguments]
--
-- commands:
--   build [-D DEBUG=true]      rebuild <repo>/bin/ura, plus src/includes/protos.h
--   tests [target] [number]    rebuild, compile and run the tests, compare their ucg, LLVM IR and
--                              output with ucg.md, llvm.md and output.md (and "// stdout:", "// expect:")
--   record [target] [number]   rebuild, then (re)write ucg.md, llvm.md and output.md for the tests
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
    ["set-llvm"] = build.set_llvm,
    extension = build.extension,
    tests = tests.tests,
    record = tests.record,
    copy = tests.copy,
    run = tests.run,
}

local command = commands[arg[1] or ""]
if not command then
    print("commands: build, tests, record, copy, run, extension, set-llvm")
    os.exit(1)
end
command({ (table.unpack or unpack)(arg, 2) })