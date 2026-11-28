# ura plan

## now: the rewrite (src/main.c + src/ucg.c)

works, each tested (exit 42): ints / floats / uints / bool, `as` casts, ops with lossless widening,
literal fit checks, if / elif / else, while, break / continue, fns, params, calls, proto, strings as char arrays,
structs (p.x, by value), indexing, stack arrays `i32[2][3]` and `Point[3]`, array literals,
if as a value and inline `if a: x else: y`, local fns and closures, float exponents,
analyze decides every type, -tokens / -ast / -ucg / -ll printers (ucg printed like LLVM)

next, in order:
1. struct and array values: `make().x`, `[0, 10][1]`, `f()[0]` without a temp var, extractvalue,
   ucg_in_memory (a variable or a struct attribute), assigning to a value is an error, struct return types
2. tests: start testing the rewrite
   - move what the rewrite can't compile yet to tests/pending, delete `a i32[3]` declaration tests
   - shorthand-if tests (below), test compiling an empty file
   - edge cases to fix in analyze (each one is silent today):
     - assignment is a statement (the Python rule): `y = x = 3` is a chain, `=` groups right to left
     - an assignment where a value is needed: `y = if c: x = 5 else: 100` -> "an assignment has no value"
     - an assignment target that isn't a name, attribute or item: "can't assign to this"
3. bring back every feature of the old compiler (list below), each with its tests;
   error checks go in src/assert.c as assert_* functions (move analyze's inline add_error calls there too)
4. anonymous structs, then heap `new` (language / memory below)
5. write the ucg IR to a .ucg text file (like LLVM's .ll)
6. when the rewrite is finished: document every function in main.c and ucg.c
   - very basic English, nothing abstract, visuals (small tables, before/after code, ASCII diagrams)
   - explain the -ucg text format, so a ucg parser can be written from it:
     a name is a slot (declare, params, fns), tN is a value numbered from t0 in each fn,
     every name is defined once, each fn printed on its own like LLVM (proto only for fns without a body),
     one instruction per line: `[tN =] op args`

shorthand-if tests
    if x == 30: y = 0 else: y = 100           // inline statement, else on the same line
    return if b == 10: 12 else: 15             // if as a value
    x i32 = if a: 1 elif b: 2 else: 3          // value with elif
    x i32 = if a: 1                            // error: an if used as a value needs an else
    while n > 0: n -= 1                        // inline loop body

## bring back from the old compiler (src/old-code/old.c)

language
- operators: `not` / `!`, `~`, `& | ^ << >>` and their `&= |= ^= <<= >>=`, `is` (means ==)
- globals (top-level variables), top-level code in a file without main
- structs: methods with self, `pub fn` statics (`Type::create`), `pub` only inside a struct,
  nested and local structs, `struct A from B` (overrides win), a struct can't contain itself
- fn overloading by parameters (not by return type), protos with `...` (C varargs, last parameter),
  a fn replaces a proto with the same parameters
- refs `&T`: `&x`, `null` (fits only `&T` or `T[]`), `isZero(x)`, `&ref_attr`, a ref must start with an address,
  comparing a never-null value to null is an error
- `Array<T>`, `String` (a literal is a core String), templates `template<T, V>:` on struct / fn / operator
- `for x in v:` (`operator in(value &T, i &i32, x &E) bool`), enum (`Color::RED`, integer values), match / case / default (break works in a match)
- try / catch with a typed `catch e Type:`, throw any value, uncaught errors in main exit 1 through core's `uncaught`
- operators written outside the struct, left side first: `operator +(left &T, right &T)`;
  conversions `operator =(left &To, right &From)`; `operator $` for $ strings
- ownership: `operator =`, temporaries moved, a type with a drop and no `operator =` can't be copied, `drop(x)`,
  `operator drop(value &T)` at block ends in reverse order, globals dropped at the end of main
- `use` (packages `@/core` and pasted files, circular use is an error), `mod` (top level only, nested, used before declared)
- builtins: sizeof, typeof, drop, isZero, memory_alloc / memory_copy / memory_free
- `ura::argc`, `ura::argv`, `ura::line`, `ura::filename`, `ura::function`, renamed with @ and shorter names
  (`@argc`, `@argv`, `@line`, `@file`, `@fn`: decide the names)
- unic, chars `'a'` (one letter), escapes `\n \t \r \0 \\ \" \e`, adjacent string literals on new lines join
- errors: already declared (with the line), used before its declaration, unknown type / symbol
- return checks: a value returned from a void fn, a bare return in a fn with a type
- error recovery: report every error in one run (several syntax errors, several analysis errors)
- an error inside a used file points at the user's file and line; the gutter fits line numbers past 9
- sizeof / typeof with and without parentheses, sizeof folds to a constant and follows the ABI padding
- C varargs calls promote their arguments (f32 -> f64, small ints -> i32)
- a top-level statement runs before main; @if inside a fn body, `@if unix` (linux and macOS)

compiler and projects
- `link "m"`: link a C library, find it (URA_LIBC), "library 'x' not found"
- macros: `@if os == "linux"` / `arch` / `installed("lib")`, `@elif`, `@else`, `@error "msg" "help"`
- unused fns and unused package globals not emitted (ucg_optimize)
- CLI: `ura compile main.ura` with `-o name`, `-ll [path]`, `-S [path]` (asm), `-c [path]` (object); `ura run`
- ura init, ura.conf and dependencies: rewritten with the new design (tooling: projects and dependencies)

## language

anonymous structs (replace tuples)
    fn divmod(a i32, b i32) {q i32, r i32}:   // type: same form as a struct body (name type)
        return {q: a / b, r: a % b}           // literal: name: value
    d {q i32, r i32} = divmod(7, 2)           // keep the whole value: d.q, d.r
    {q i32, r i32} = divmod(7, 2)             // destructure by field name, order doesn't matter
    {q i32} = divmod(7, 2)                    // take only some fields
    p Point = {x: 1, y: 2}                    // same literal for named structs, typed by the target
    // a colon means a value, a space means a type
    // structural equality: same field names and types in the same order = same type
    // needs: struct values (step 1), ':' inside { } must not bump ura.space

function values and callbacks                 tests: tests/pending/function-values.md
    fn apply(f fn(i32) i32, x i32) i32:
        return f(x)
    apply(square, 3)

variadic functions in ura, two forms
    fn sum(count i32, ...) i32:               // the existing one
    fn foo(...args)                           // split in 2: fn foo(items, sizes), supported in ucg

** power
    x i32 = 2 ** 10
    x **= 2
    2 ** 3 ** 2 == 2 ** 9                     // tighter than *, right-associative

alias (instead of expanding a mod)
    alias print = io::out
    alias process = Process::process          // and find a shorter name than process (look at Zig / Rust)

i128 / u128                                   LLVM supports them: __divti3, printing
bigint (in ucg)                               n bigint = 2 ** 200
any (undecided)                               fn show(x any):      // works like a template
const                                         on every variable that is never modified

throw records where it happened
    throw Error::create("no file")
    catch e Error:
        io::err($"{e.filename}:{e.line} in {e.function}\n")     // main.ura:12 in load
    // uncaught: error: no file (main.ura:12 in load)
    // review try / catch / thrown: which fns can throw, checks only after those calls

bug: wrong place, wrong message
    for x in Range::create(name.size()):
    error: main.ura:13:19 can't loop over '::'
    // want: point at the call, list the Range::create prototypes

## fmt and io

done (old compiler): $ strings (\{ \}, operator $), io::out / io::err / io::input in ura-core

    io::out($"100%% done\n")                  // %% prints %
    name String = io::input("Your name: ")    // prompt
    $"{&y}"                                   // a ref prints its address (printf %p)
    io::err($"{io::red}failed{io::reset}\n")  // colours
    buffered output                           // one write per line, not per call
    later: every literal is a fmt string, $ goes away: s String = "hp: {hp}\n"

adjacent string literals join
    text String = "abc""efg"
                  "ijk"                       // "abcefgijk"

Stream (files, sockets)
    f Stream = io::open("a.txt")
    f.write($"hi {n}\n")
    line String = f.getline()

scan (reverse of fmt)
    scan("12 3.5", &a, &b)

## memory

heap allocation: new copies any value from the stack to the heap
    a i32[] = new [1, 2, 3]
    a i32[] = new i32[n]                      // zeroed
    p &Point = new {x: 10, y: 10}
    p &Point = new make_point()               // a call that returns something
    // easiest approach: build the value, then copy it to the heap
    // if new fails it throws
    // no automatic freeing: containers free through their drop operator, drop(x) frees heap memory
    // never a size in a type: a i32[3] is an error

bitmap arena allocator (default)
    1 bit per 16-byte block: 1001 = blocks 1, 4 used, behind alloc / free

@arena on a fn (like Zig, design later)       its heap allocations freed on exit

assignment the C++ way
    x = y                                     // operator = in place, move for temporaries
    // then drop never sees a zeroed value

-O flags                                      -O0..-O3, -Os, -Oz: LLVM passes in ucg_start, in @dev / @release or on the command line
-san                                          ASan in ucg_start, report the ura file:line (needs DWARF), find leaks
annotations (later)                           @cache (memoize), @vector / @gpu, LLVM SIMD / vector and matrix types

## tooling

projects and dependencies (decided 2026-11-28): Cargo's model, in ura syntax
    main.ura      use "@/core"                          names only: no urls, no versions in the code
    ura.conf      @name "game"                          written by ura init, short, optional
                  @version "0.1.0"
                  @dev "-san -g"                        flags of ura build
                  @release "-O2"                        flags of ura build -release
                  @dep "core" "0.2"                     written by ura install core
                  @dep "json" "0.3" "github.com/bob/json"
    ura.lock      core 0.2.1 3f2a9c1 ...                written by ura only: exact commits, deps of deps too
    @/core -> ~/.ura/deps/core@3f2a9c1                  symlinks so the editor sees the sources, gitignored
    commands
    - ura main.ura [flags]         one file, no project needed: flags on the command line
    - ura build [-release] [flags] the project: the profile's flags, plus the ones given
    - ura init <name>              creates ura.conf and main.ura
    - ura install <name> [url]     no url: github.com/ura-computing/ura-<name>, newest tag; writes the @dep line
    - ura update [name]            newest compatible tag; ura why <name>: prints the chain (main -> http 0.2 -> core 0.2)
    rules
    - compatible versions (same major; for 0.x the same minor) share one copy, the highest:
      B wants A 1.1, C wants A 1.4 -> A 1.4 for both
    - incompatible versions get one copy each, each lib compiles with its own:
      B wants A 0.0.1, C wants A 0.2.0 -> two copies; passing a value between the two is a compile error
      (String from a 0.0.1 is not String from a 0.2.0)
    - deps of deps come from each lib's own ura.conf; the lock records the result, so a clone builds the same
    - an @dep nothing uses: warning "json is never used", nothing downloaded
    - a use inside an @if branch that's off still counts as used (its sources are fetched only where the branch is on)
    - a build keeps the locked commit even when the tag moved; ura update takes the new one
    - no tag on the repo: error "no version tag like 0.0.1"; a lib depending on a local folder can't be used by others
    - downloads go to a temp folder, then rename (interrupted downloads, two builds at once); offline works from the cache
    - C libraries (SDL2, ...) are installed by the user: ura checks with @if installed("sdl2") and errors with a hint
      (install SDL2: apt install libsdl2-dev); no install scripts ever, only .ura sources
    - a lib's name comes from its repo (ura-core -> core), @script dropped (Makefile)

CLI
    - -tokens / -ast show which source file each part comes from (several files can be compiled)
    - -help with colours
    - ./main.ura runs ura by itself (shebang)
    - ura watch (rebuild + rerun on save), ura doc Array (docs in the terminal), shell completion
    - a code formatter for ura
    - static and dynamic libraries: ura build --lib (libft.a), --shared (.so / .dylib / .dll); no fn main = a library

testing and platforms
    - a testing library / tool in ura, CI for ura-lang and every big ura project
    - tests / record / copy / run as a C program (src/test/runner.c)
    - test on every OS, LLVM 12 / 15 / 21 and other architectures (an emulator), wasm
    - ucg standalone, emits objects without the .ll
    - ucg backend without LLVM, hand-written assembly: asm_* functions next to llvm_*, x86-64 first,
      picked with a flag (ura main.ura -backend asm); then mem2reg (SSA), constant folding, dead code in ucg
    - header.h on Windows: fork / waitpid, open_memstream, realpath, dirname, symlink2 -> junction

benchmark
    compile time    ura vs clang -O0 / -O2, tcc, Go, Rust, Zig
    run time        raytracer, donut, string-heavy test, memory
    2026-11-23 raytracer (805 lines): dev bin/ura 0.53s, -O2 bin/ura 0.21s (half is clang + ld)
    - link with ld / lld directly (crt1.o, crti.o, crtn.o, loader, -lc per OS)
    - cache compiled dependencies (one .o per version)

VS Code extension
    - highlight @argc, @argv, @line, @filename, @function (after the rename)
    - ctrl + click: definitions, use "file", struct methods, struct attributes, struct types, names inside a mod
    - hover on a struct attribute shows its type
    - a new line after { indents one step
    - code inside an @if for another OS (windows on linux) shown dimmed
    - stale errors after ura install / build: re-check when @/ or the compiler changes
    - files pasted into a struct (x/methods.ura): check through the project's entry, not alone

## libraries and projects

ura code style for the libraries
    - every struct: x.ura with the attributes, then operators / methods / utils folders, nothing else
    - struct literals instead of one assignment per field:
        file File = { path: path, fd: cstd::open(path.chars(), flags, cstd::S_IRUSR | cstd::S_IWUSR) }
        wait = { tv_sec: ms / 1000, tv_nsec: (ms % 1000) * 1000000 }
    - chained calls: server.socket.reuse().bind(&server.address).listen(backlog)
      (can bind take the address from the socket itself?)
    - check every return value (cstd::nanosleep(&wait, &left))
    - no hard-coded sizes like self.read(4096): a named value
    - read a whole file at once (its size is known) instead of line by line
    - clear names (fn elapsed() i64?)
    - review every implementation one by one, fix the TODOs left in the .ura files
    - organize all the libraries when the rewrite is done

std repos like ura-core
    ura-posix/
    ├── posix.ura      use "./io", use "./net"
    ├── io/            Stream, files
    └── net/           Socket

projects
    done: ura-donut, ura-chatroom, ura-libft
    - raytracer: use match, try, $ strings, io
    - rewrite the m13 projects (new / drop, values own, refs borrow): calc, net server / client, dungeon boss
    - Pacman in ura
    - ura-graphics: ura-sdl2, ura-raylib, ura-opengl, ura-wayland; showcases uraRaytracer, uraRasterizer (threads)
    - AI agents library: @dependency "agent/claude" "0.0.1"
    - later: http lib + backend framework (FastAPI-like, existing libs for ssl), ura init SDL2,
      ucg + ura-lang rewritten in ura

## cleanup

- C code: every for loop over an int uses int i, j, k; no size_t assigned to int or the other way round
- fix the TODOs in the code, remove every implicit part, every C print goes through format
- ucg: line numbers as unsigned long
- remove unused files, move helpers to lib/header.h
- every repo: description + README
- ura-lang README: closures (nested fns capture outer locals by reference, hidden pointer params; globals not captured)
- ura-lang README: a History section, one snapshot per era (Claude drafts it), from git:
    2023-12-13  first commit, file.hr, x86 assembly:   main:  num a = 10  output(a)
    2024-01     int / char instead of num
    2024-02     .w, func void ft_ft(ref int nbr):, main():   (the 42 pool in it)
    2024-07     IR/ (the ancestor of ucg)
    2025-04     struct User: chars name, int age
    2025-08     if / else
    2025-10-23  .ura, protoFunc int putchar(int str), chars
    2026-02     inline if, and / or
    2026-04     type after the name, fn, use "@/raylib", enum (Tetris in ura)
    2026-08     fn main() i32:, i32, &T
    2026-11-26  the rewrite (analyze, ucg printed like LLVM)
- explain the compiler architecture on the website

## release (only when you say)

- merge refactor -> dev -> main, tag the std repos
- rewrite history: must not look AI generated, smaller (removed files gone, TODO.md gone), tags
  today (2026-11-28): 45.5 MB packed, 732 commits; biggest: 0.old-code/build/ura (50 MB binary, gone),
  build/ura, src/build/ura, src/ura, 42 subject pdfs, assets/ura-init.gif, old tests: all gone from the tree
  1. git clone --mirror as a backup
  2. git filter-repo (install it, one python script): keep only the files that exist at the end, no binaries;
     commits that only touched removed files become empty and disappear
  3. split the fat commits into small topic commits, grouped in topic branches with descriptive names,
     merged as #N on GitHub; code written on disk and committed late in one dump gets its dates back:
     each dump's window = from the previous real commit to the dump's date; the parts, ordered by what
     depends on what, are dated inside that window in proportion to their size, at usual working hours;
     never a date after the dump or outside its window
  4. force push + tags, everyone re-clones (confirm in full sentences right before)
  before that: bin/ and src/main in .gitignore + git rm --cached (each build adds ~2 MB to history)
- versions 0.x.y until production, then 1.0.0; license: MIT (or change it?)
- ura-computing -> ura-ecosystem
- ura grammar in github-linguist (colours .ura on GitHub)
- VS Code extension in the Marketplace (vsce publish)

## personal

- 30 push dsrl
