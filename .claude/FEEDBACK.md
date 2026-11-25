# The user's feedback (2026-11-25)

Read this before writing or reviewing any .ura code. It comes from TODO.md (the user's own notes) and from the user's answers on 2026-11-25. It lives in the repo so every machine's Claude sees it.

## How to write .ura code (every library and project)

- Build a struct with a literal, nested literals included, never field by field:
    + file File = { path: path, fd: cstd::open(path.chars(), flags, cstd::S_IRUSR | cstd::S_IWUSR) }
    + address Address = { raw: { sin_family: cstd::AF_INET as u16, sin_port: cstd::htons(port as u16), sin_addr: { s_addr: cstd::INADDR_ANY } } }
    + wait = { tv_sec: ms / 1000, tv_nsec: (ms % 1000) * 1000000 }
    + not: file.path = path, then file.fd = ...; not: wait.tv_sec = ..., then wait.tv_nsec = ...
- Chain calls when the API allows it:
    + server.socket.reuse().bind(&server.address).listen(backlog)
    + open question (ura-posix net/server/methods.ura): the socket could read the server's address itself, so bind takes no argument
- Check every return value of a C call, and throw on failure. Example of a missed one: cstd::nanosleep(&wait, &left).
- No magic numbers: a size like the 4096 in read_all goes in a named variable the user can edit.
- Read a file in one go: get its size from Stat (ura-posix fs/stat.ura), then one read. Not chunk by chunk like read_all in fs/file/methods.ura. Review every fopen, fread and similar implementation one by one.
- Every struct has the same layout: the struct file, then a folder with methods, operators and utils files, nothing else. Example: core/array.ura, core/array/methods.ura, core/array/operators.ura.
- Few comments in .ura code: no explanation blocks above structs or functions, the code says it.
- Names say what they mean:
    + fn elapsed() i64 in ura-posix time/clock/methods.ura returns milliseconds since reset(); the name does not say that
    + process::Process (mod process, struct Process) is long; find a shorter name, looking at Zig or Rust, or use an alias like alias process = ...

## ura-libs/

- ura-libs/core, posix, math and graphics are symlinks to ~/.ura/deps/<name>/0.0.1, the clones made by ura deps install. Keep them tracked.
- On a new machine: run ura deps install in a project that lists the four libraries (tests/ura.conf does), and the links resolve.
- The libraries are being rewritten to follow this guide. Reorganize all ura libraries when the rewrite ends.

## TODO comments the user left in the .ura files

- ura-posix net/server/methods.ura: should bind stop taking the address (see above).
- ura-posix process/utils.ura, run(): (status / 256) % 256, "is this valid". It is WEXITSTATUS on Linux and macOS. On Windows, system() returns the exit code itself, so that path needs @if os == "windows".
- ura-posix time/clock.ura: use "../cstd" failed. A relative use that goes up one folder does not resolve: check the compiler.

## Language and compiler

- Use LLVM's native i128 (and u128) instead of building them by hand.
- Variadic functions: keep ... (the C style), and add ...args: fn foo(...args) becomes fn foo(elements, sizes), an array of the values and an array of their sizes, supported in ucg.
- Cancelled: expand a mod (the C++ using namespace idea, TODO.md "expand a mod"). Do not build it.

## VS Code extension (_extension/)

- ctrl+click does not jump to: a struct method, a name inside a mod, a struct attribute, a struct type.
- Hovering a struct attribute shows its type, also inside a struct literal { name: value }.
- Enter after { indents the next line one level deeper, and the closing } lines up with the line that opened it.
- An @if block for another OS (windows code on a linux machine) is shown dimmed, like VS Code's inactive regions.

## Tooling

- A code formatter for ura.
- A test tool built into ura-lang, and CI for the compiler and for the big projects written in ura.

## Answered on 2026-11-25

- The tag in the ura-lang GitHub repo is legacy: it points to 6039042 "remove the vscode extension (snapshot before cleanup)", the repo as it was before the cleanup.

## Still open (ask the user)

- Keep the MIT license, or change it.
