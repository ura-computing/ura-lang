# ura plan

# milestone 1: test cleanup (done)

# milestone 2: language features (now)

    sized arrays
        postponed to milestone 4 (tests use Array<T> meanwhile)
        - tests waiting for it: tests/pending/sized-arrays.md

    done: unic: 32-bit Unicode code point (like Rust's char)
        c unic = '🦀'
        output(c)                  // prints its UTF-8 bytes
        for u in s.unics():        // a String's code points (String stays UTF-8 bytes)
            output(u)
        - nothing existing changes: char stays i8, char[], String, libc protos stay

    done: ura::line, ura::filename, ura::function (like __LINE__, __FILE__, __func__)
        io::err(ura::filename, ":", ura::line, " in ", ura::function, "\n")

    later: throw records where it happened
        throw Error::create("no file")     // fills line, filename, function by itself
        catch e Error:
            output(e.filename, ":", e.line, " in ", e.function, "\n")   // main.ura:12 in load
        - uncaught: error: no file (main.ura:12 in load)

    variadic functions written in ura
        fn sum(count i32, ...) i32:        // read the extra arguments inside

    tuples
        fn divmod(a i32, b i32) (i32, i32):
            return a / b, a % b
        q, r = divmod(7, 2)

    function values (callbacks)
        fn apply(f fn(i32) i32, x i32) i32:
            return f(x)
        apply(square, 3)
        - tests waiting for it: tests/pending/function-values.md (fn values, params, a zeroed fn value traps)

    ** power operator (like Python)
        x i32 = 2 ** 10
        f f64 = 2.0 ** 0.5
        x **= 2
        2 ** 3 ** 2 == 2 ** 9      // tighter than *, right-associative

    i128 / u128
        big i128 = 170141183460469231731687303715884105727
        - ucg: LLVM i128, / and % through __divti3, printing
        - ura: i128 and u128 keywords

    big integers, provided by ucg (any compiler built on it gets them)
        n bigint = 2 ** 200
        - ucg: limbs, + - * / % compare, printing

    any (not decided)
        fn show(x any): ...        // works like a template

    bugs
        for x in Range::create(name.size()):
        error: main.ura:13:19 can't loop over '::'           <- wrong place, wrong message
           |
        13 |     for x in Range::create(name.size()):
           |                   ^^
        help: add operator in(value &T, i &i32, x &E) bool
        - should point at the call and print the prototypes of Range::create

# milestone 3: fmt and io

    done: $ strings
        s String = $"hp: {hp}, mp: {mp}\n"    // tokenizer: $("hp: ", hp, ", mp: ", mp, "\n")
        throw Error::create($"{ura::filename}:{ura::line} no file")
        - hidden builtin "$" (FORMAT): snprintf twice into a String that owns its buffer
        - done: \{ and \} for a literal brace; a struct inside {} uses its operator $
        - left: handle %% in io::out
        - later: every string literal is a fmt string, $ goes away
            s String = "hp: {hp}\n"

    adjacent string literals join (like C)
        text String = "hello 1"
                      "hello 2"
                      "hello 3"             // one literal, joined by the tokenizer
        - instead of one literal across lines:
            "
                hello 1
                hello 2
            "
        - this must work:
            wave unic = '👋'
            io::out($"hello "
                    "{ura::filename} "
                    "{ura::line}\n")

    done: output / errput move to ura-core, the compiler loses WRITE
        fn output(text String):
            write(1, text.chars(), text.len())
        - WRITE, READ, codegen_write, codegen_write_struct deleted (ucg_write stays for ucg's own tests)

    done: operator $ (a struct formats itself)
        operator $() String:
            return $"({self.x}, {self.y})"
        output($"p = {p}\n")                 // calls p.operator $()

    colours as globals in io
        io::out(io::red, "failed", io::reset, "\n")

    done: io in core, output / errput builtins removed
        io::out(...)
        io::err(...)
        name String = io::input()
        - left: io::input("Your name: ") with a prompt
        - left: buffered output instead of one write per call

    alias + Stream (files, sockets)
        alias print = io::out
        f Stream = io::open("a.txt")
        f.write("hi ", n, "\n")
        line String = f.getline()

    scan (reverse of fmt)
        scan("12 3.5", &a, &b)

    organize the std repos (ura-posix, ura-math, ura-graphics) like ura-core
        ura-posix
        ├── posix.ura      uses ./io, ./net
        ├── io/            Stream, files
        └── net/           Socket
        - one folder per area, one struct with methods each

# milestone 4: memory and safety

    bitmap arena allocator, default for every ura program
        1 bit per 16-byte block:  1001 = blocks 1, 4 used, 2, 3 free
        - sits behind alloc / free, so Array, String work unchanged

    stack arrays and heap arrays (later, maybe Array instances made at compile time)
        rooms Room[] = Room[2]        // zeroed, on the stack; size can be a runtime value
        grid Room[][] = Room[2][3]
        a i32[] = new<i32>(5)         // heap
        - no .len on raw arrays; Array<T> has size()

    memory builtins renamed
        memory_alloc(W * H) as char[]   ->   new<char[]>(W * H)
        memory_free                     ->   clean(arr)

    new before a constructor: copies the stack value to the heap
        p &Vec3 = new Vec3::create(1, 2, 3)
        new Vec3{x: 1}                // error: only before Type::create(...)
        new 5                         // error
        - operator drop cleans fields only, same for stack and heap values

    arena decorator on a fn (like Zig's arena allocator, design later)
        @arena
        fn frame():                   // every heap allocation in it is freed when the fn exits

    done: operators with both sides written (design fix, free operators found by type)
        // today: left side is always self, so only "into String" exists
        operator =(text i8[]):                 // s = c_text   (inside String)

        // right: both sides named, works in either direction
        operator =(s &String, text &i8[]):     // s = c_text
            s.assign(text)
        operator =(text &i8[], s &String):     // c_text = s
            text = s.data
        - the missing direction caused the String/Array -> C detour (implicit .data, proto lowering, operator as)
        - same form for every operator: operator +(a &Vec, b &Vec) Vec, operator as, operator $
        - never implicit: runs only where = (or the operator) is written

    assignment the C++ way
        x = y                         // operator = in place; a move operator for temporaries
        - then operator drop never sees a zeroed value
        - today: drop must be safe on zeros (m08 033 guards with if self.id != 0)

    optimization flags
        ura compile main.ura -O2      -O0 .. -O3, -Os, -Oz
        - ucg_start: LLVM passes for -O
        - add each flag to compile_flags[] in project.c (ura -help and ura init comments show it)
        - ura init: build script uses -O2, run script uses -san
        - benchmark compile and run time for each -O level (milestone 5)

    -san
        ura compile main.ura -san     // the run script uses it, ura ships with it
        - ucg_start: ASan setup
        - test memory leaks with it before the arena allocator exists
        - report the ura file:line of the error (needs DWARF debug info from ucg)

    later: function annotations, like Python decorators
        @cache                        // remembers results by arguments (@lru_cache)
        fn fib(n i32) i64:

        @vector                       // runs its math as SIMD vectors (graphics)
        @gpu                          // or on the GPU
        fn shade(...):

# milestone 5: tooling

    done: ura.conf (ura syntax, @if works)
        @name "raytracer"
        @version "0.0.1"
        @script "run" "ura compile src/main.ura -o build/raytracer && ./build/raytracer"
        @dependency "math" "0.0.1" "https://github.com/ura-computing/ura-math"   // or "./folder"

        ura install                    clones go to <project>/dependencies/
        ura install <name> <version> <where>
        ura remove / list / <script>
        - done: ura-lib split into ura-core, ura-posix, ura-math, ura-graphics (tag 0.0.1)
        - left: @flags "-O2 -san" once -O and -san exist; a ura.conf icon in the extension

    done: ura compile / ura run
        ura compile file.ura           one file, the others come in with use
        ura run                        a @script of ura.conf

    done: ura init starter (hello + user name per OS), the donut moved to ura-donut

    done: tests hold only ura code
        tests/m01/variables.md                 ## 017 — title + the ura code (// stdout: // expect: // error:)
        tests/m01/llvm-ir.md                   ## 017 ```llvm (IR without ModuleID / datalayout / triple)
                                               ## 042 ```stderr (an error test, last line = exit status)
        tests/pending/                         tests of features not built yet (tests pending runs them)

    dependency without a url: found in the ura-computing organization
        @dependency "ura-math" "0.0.1"                              // github.com/ura-computing/ura-math
        @dependency "geo" "0.2.0" "https://github.com/user/geo"     // the url given
        @dependency "graphics/raylib" "0.0.1"                       // group/name

    AI agent support
        @dependency "agent/claude" "0.0.1"
        - a ura library to call AI agents (Claude, ...) from ura programs

    ura-core by default
        - ura adds ura-core to ura.conf by itself
        - installing ura also installs ura-core

    ura install: a global package cache, copied into each project
        ura install
        └── each @dependency "ura-core" "0.0.1" "<url>"
            ├── ~/.ura/packages/ura-core@0.0.1 missing?
            │   └── git clone --branch 0.0.1 <url> ~/.ura/packages/ura-core@0.0.1
            └── copy it -> <project>/dependencies/ura-core@0.0.1
        - Windows: %USERPROFILE%\.ura\packages\
        - versions are a.b.c only: "latest", "main", "1.0", "^1.2" are errors
        - no tag for that version: biggest version in ~/.ura/packages/, else the repo's biggest tag
        - ./folder dependencies: copied straight, never cached
        - already in dependencies/: skipped

    ura watch (rebuild and rerun on every save, like nodemon)
        ura watch

    ura doc: docs in the terminal (design later)
        ura doc Array          // docs of a type, fn or library, read from the sources
        ura doc SDL2

    print the AST as a tree
        ura compile main.ura -tree

    testing lib written in ura (design later)

    Makefile instead of config.sh
        make ucg               builds only ucg
        - ucg in its own repo, or a standalone build, so it can be reused
        - test ucg with LLVM 12, 15, 21
        - ucg writes its own IR file: main.ura.ir
        - emit assembly / objects straight from LLVM, without writing the .ll file
        - tests / record / copy / run move to a C program (src/test/runner.c, uses header.h)
          no bash/zsh quirks, works on Windows; then config.sh is deleted

    build static and dynamic libraries
        ura build --lib         libft.a        (objects + ar)
        ura build --shared      libft.so / .dylib / .dll
        - a project with no fn main builds as a library; others link it with link / @link
        - example: ura-libft builds libft.a, a C or ura program links it

    benchmark ura-lang against other compilers and tools
        compile time   same program in ura, C (clang -O0 / -O2, tcc), Go, Rust, Zig
        run time       raytracer, donut, a JSON / string heavy test, + memory
        per stage      frontend (tokenize, parse, check) | LLVM codegen | link

        first numbers (2026-11-23, raytracer 805 lines + dependencies, LLVM 12)
        ├── dev bin/ura (ASan, no -O)         0.53s
        └── bin/ura -O2, no ASan              0.21s
            └── clang driver + ld link        ~0.1s  (half)
        - always measure with an optimized bin/ura, not the dev build
        - link with ld (or lld) directly, not through the clang driver
            + needs crt1.o, crti.o, crtn.o, the dynamic loader path and -lc per OS (clang finds them today)
            + lld links Linux, macOS and Windows objects: one linker everywhere
        - cache compiled dependencies (one .o per dependency version)
        - compare each -O level once the flags exist (milestone 4)
        - a bench command in config.sh (later the C runner) that prints a table

    VS Code extension
        - highlight ura::argc, ura::argv, ura::line, ura::filename, ura::function
          (rows of builtins[] in main.c, plain names today)
        - ctrl + click goes to the definition: variables, use "filename"
        - stale errors: after ura install (or build) the errors stay until the window reloads
            + fix: re-check open files when dependencies/, ura.conf or the compiler binary changes

# milestone 6: projects

    raytracer: rewrite with the new features (match, try, fmt, io)
        - done so far: modern syntax, one folder per struct, own repo ura-computing/ura-raytracer

    done: ura-donut, the spinning donut in its own repo

    done: ura-tcp-server became ura-chatroom (rooms, emojis, dice, 8-ball, rainbow, donut)
        - ura-libft rewritten from the subject

    rewrite m13's projects on today's model (new<T>/free<T>, values own, refs borrow)
        025 calc, 026 libft, 027 net server, 028 net client, 032 dungeon boss,
        033 calculator, 037 038 040 libft lists

    Pacman written in ura

    graphics
        ura-graphics
        ├── backends
        │   ├── ura-sdl2
        │   ├── ura-raylib
        │   ├── ura-opengl
        │   └── ura-wayland
        └── showcase projects (custom, show ura's power; later reusable tools)
            ├── uraRaytracer
            └── uraRasterizer
        - multi threading

    later
        - http lib, backend framework (syntax close to FastAPI), ura init SDL2
        - test the compiler on other architectures with an emulator
        - write the LLVM ABI in ura, then rewrite ucg and ura-lang in ura

# milestone 7: cleanup

    - fix all remaining TODOs in the codebase
    - review the try / catch / thrown code (ucg_find_throwing, the thrown checks after calls)
    - review every implicit part of the codebase and remove it
    - rename check_* to assert_*
    - every printing function in the C code goes through format
    - ucg: line numbers as unsigned long
    - rename place to something readable
    - format the codebase
    - remove unnecessary files
    - move any code that can be moved to lib/header.h
    - how are ALLOC and MEMCPY handled in ucg?
    - every github repo has a proper description and README

# milestone 8: release (only when you say)

    - merge refactor -> dev -> main, tag the std repos
    - rewrite git history: must not look AI generated, add tags, delete removed files (smaller repo)
    - remove TODO.md from git history
    - versions
        0.x.y until production ready, bumped by how big the change is
        1.0.0 once it's well tested and production ready
    - MIT License
    - rename ura-computing -> ura-ecosystem (all repos live in the GitHub organization)

    ura syntax highlighting on GitHub
        - push the ura grammar to github.com/github-linguist/linguist
          GitHub colours .ura files and counts ura in the language bar

    VS Code extension in the store
        - publish _extension to the VS Code Marketplace (vsce publish)

# at the end: Makefile replaces config.sh, and Windows

    - remove src/config.sh, a Makefile does build / tests / record / ucg
    - the Makefile checks the build dependencies and installs the missing ones (LLVM, clang, git, ctags)
    - src/lib/header.h on Windows: no path yet for
        fork / waitpid (run_command), open_memstream (format), realpath, dirname
    - symlink2 has the Windows path (CreateSymbolicLinkA) but needs Developer Mode:
      fall back to a directory junction

# personal

    + 30 push dsrl

	+ i macros like @dependecy etc...:
		+ instead of hardocding them in the language
		+ make the m programable
	+ should support cliquein tab after wrting ura
	like how clang works etc....
	+ add a flag depedecies location