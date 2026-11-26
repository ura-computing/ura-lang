# ura plan

# 1 test cleanup (done)

# 2 language features (now)

done: unic, ura::line / ura::filename / ura::function, mod, os::argc / os::argv, array literals, for in, null / isZero

sized arrays                                  milestone 4, tests: tests/pending/sized-arrays.md

throw records where it happened (later)
    throw Error::create("no file")
    catch e Error:
        io::err($"{e.filename}:{e.line} in {e.function}\n")     // main.ura:12 in load
    // uncaught: error: no file (main.ura:12 in load)

variadic functions in ura
    fn sum(count i32, ...) i32:

tuples
    fn divmod(a i32, b i32) (i32, i32):
        return a / b, a % b
    q, r = divmod(7, 2)

function values                               tests: tests/pending/function-values.md
    fn apply(f fn(i32) i32, x i32) i32:
        return f(x)
    apply(square, 3)

** power
    x i32 = 2 ** 10
    x **= 2
    2 ** 3 ** 2 == 2 ** 9                     // tighter than *, right-associative

i128 / u128                                   ucg: LLVM i128, __divti3, printing
bigint (in ucg)                               n bigint = 2 ** 200
any (undecided)                               fn show(x any):      // works like a template

expand a mod (like C++ using namespace)
    expand time
    clock Clock = Clock::create()             // no time:: prefix
    sleep(250)
    // the mod's names are found without the prefix, the prefixed names still work

bug: wrong place, wrong message
    for x in Range::create(name.size()):
    error: main.ura:13:19 can't loop over '::'
    // want: point at the call, list the Range::create prototypes

# 3 fmt and io

done: $ strings (\{ \}, operator $), io::out / io::err / io::input in ura-core, WRITE / READ deleted

left
    io::out($"100%% done\n")                  // %% prints %
    name String = io::input("Your name: ")    // prompt
    buffered output                           // one write per line, not per call
    later: every literal is a fmt string, $ goes away
        s String = "hp: {hp}\n"

adjacent string literals join
    text String = "abc""efg"
                  "ijk"
                  "lmn"                       // "abcefgijklmn"
    io::out($"hello "
            "{ura::filename} "
            "{ura::line}\n")

colours in io
    io::err($"{io::red}failed{io::reset}\n")

Stream (files, sockets) + alias
    alias print = io::out
    f Stream = io::open("a.txt")
    f.write($"hi {n}\n")
    line String = f.getline()

scan (reverse of fmt)
    scan("12 3.5", &a, &b)

std repos like ura-core
    ura-posix/
    ├── posix.ura      use "./io", use "./net"
    ├── io/            Stream, files
    └── net/           Socket

# 4 memory and safety

done: free operators, both sides written, found by type
    operator =(s &String, text &i8[]):        // s = c_text
    operator =(text &i8[], s &String):        // c_text = s

bitmap arena allocator (default)
    1 bit per 16-byte block: 1001 = blocks 1, 4 used
    // behind alloc / free: Array, String unchanged

stack and heap arrays
    rooms Room[] = Room[2]                    // zeroed, stack, runtime size ok
    grid Room[][] = Room[2][3]
    a i32[] = new<i32>(5)                     // heap; no .len on raw arrays

memory builtins renamed
    memory_alloc(W * H) as char[]    ->   new<char[]>(W * H)
    memory_free(arr)                 ->   clean(arr)

new before a constructor: stack value copied to the heap
    p &Vec3 = new Vec3::create(1, 2, 3)
    new Vec3{x: 1}                            // error: only before Type::create(...)
    new 5                                     // error
    // operator drop cleans fields only, same for stack and heap values

@arena on a fn (like Zig, design later)
    @arena
    fn frame():                               // its heap allocations freed on exit

assignment the C++ way
    x = y                                     // operator = in place, move for temporaries
    // then drop never sees a zeroed value (today m08 033 guards if self.id != 0)

-O flags
    ura compile main.ura -O2                  // -O0..-O3, -Os, -Oz: ucg_start LLVM passes, row in compile_flags[]
    @flags "-O2 -san"                         // in ura.conf once both exist
    // ura init: build script -O2, run script -san; benchmark each -O level

-san
    ura compile main.ura -san                 // ASan in ucg_start; report the ura file:line (needs DWARF)
    // find memory leaks with it before the arena allocator exists

annotations (later)
    @cache                                    // memoize by arguments
    @vector  @gpu                             // SIMD / GPU math

# 5 tooling

done: ura.conf, ura compile / ura run, ura install with ~/.ura/packages, ura init starter, ura-only tests (llvm-ir.md, tests/pending)

imports and ura.conf: bugs + redesign (discuss first)
    @dependency "core" "0.0.1" "https://github.com/ura-computing/ura-core"
    use "@/core"
    - bugs importing files (list them when we discuss)
    - dependency = 3 positional strings; want named fields:
        @dependency core:
            version "0.0.1"
            url "https://github.com/ura-computing/ura-core"
    - a dependency's own ura.conf loaded recursively, files already loaded skipped
    - review the macros (@name, @version, @entry, @script, @dependency): hard-coded today, programmable?
    - flag for the dependencies location

dependency without a url
    @dependency "math" "0.0.1"                // github.com/ura-computing/ura-math
    @dependency "graphics/raylib" "0.0.1"     // group/name

core by default                               ura adds core to ura.conf; installing ura installs core
AI agents library                             @dependency "agent/claude" "0.0.1"
ura watch                                     rebuild + rerun on save
ura doc Array                                 docs in the terminal, read from the sources
ura compile main.ura -tree                    print the AST
testing library in ura                        design later
shell completion                              tab after ura, like clang

static and dynamic libraries
    ura build --lib                           libft.a
    ura build --shared                        libft.so / .dylib / .dll
    // no fn main = a library; others link it with link / @link

benchmark
    compile time    ura vs clang -O0 / -O2, tcc, Go, Rust, Zig
    run time        raytracer, donut, string-heavy test, memory
    2026-11-23 raytracer (805 lines): dev bin/ura 0.53s, -O2 bin/ura 0.21s (half is clang + ld)
    - link with ld / lld directly (crt1.o, crti.o, crtn.o, loader, -lc per OS)
    - cache compiled dependencies (one .o per version)

VS Code extension
    - highlight ura::argc, ura::argv, ura::line, ura::filename, ura::function
    - ctrl + click: definitions, use "file"
    - stale errors after ura install / build: re-check on dependencies/, ura.conf, compiler change
    - files pasted into a struct (x/methods.ura): check through the project's entry, not alone
    - an icon for ura.conf

# 6 projects

done: ura-donut, ura-chatroom (was ura-tcp-server), ura-libft from the subject

raytracer: use match, try, $ strings, io    // modern syntax + struct folders done
rewrite m13 projects (new<T> / free<T>, values own, refs borrow): calc, net server / client, dungeon boss, calculator
Pacman in ura

graphics
    ura-graphics
    ├── ura-sdl2  ura-raylib  ura-opengl  ura-wayland
    └── showcases: uraRaytracer, uraRasterizer (multi threading)

later: http lib + FastAPI-like framework, ura init SDL2, other architectures (emulator), ucg + ura-lang rewritten in ura

# 7 cleanup

- fix the TODOs in the code
- review try / catch / thrown (ucg_find_throwing, checks after calls)
- remove every implicit part
- every C print goes through format
- rename place
- ucg: line numbers as unsigned long
- format, remove unused files, move helpers to lib/header.h
- ALLOC / MEMCPY in ucg: how handled?
- every repo: description + README

# 8 release (only when you say)

- merge refactor -> dev -> main, tag the std repos
- rewrite history: must not look AI generated, smaller (removed files gone, TODO.md gone), tags
- versions 0.x.y until production, then 1.0.0; MIT license
- ura-computing -> ura-ecosystem
- ura grammar in github-linguist (colours .ura on GitHub)
- VS Code extension in the Marketplace (vsce publish)

# at the end: Makefile, Windows

- Makefile replaces config.sh (build, tests, record, ucg), installs missing LLVM / clang / git / ctags
- tests / record / copy / run as a C program (src/test/runner.c)
- ucg standalone, tested on LLVM 12 / 15 / 21, writes main.ura.ir, emits objects without the .ll
- header.h on Windows: fork / waitpid, open_memstream, realpath, dirname; symlink2 -> junction without Developer Mode

# personal

    + 30 push dsrl
	+ what if new failed

+ llvm already support i128 ...., we should make use of that

+ for preproccesing macros:
	+ if it's for example awindows but the mahcine is linux
	+ what's inside should be lack dark background or similar to how vscode does it
+ when i hover over sturct attirbute i much see it's type you know inside {}
+ when {do a new line here it shold luk go one step back or somehting}
+ we should add a code formatter for ura-lang

+ no need for expand mode

+ all stucts should follow one rule:
	+ operator/methods/utils folders that's it

+ always: 
	use:	
		file File = {
			path: path,
			fd: cstd::open(path.chars(), flags, cstd::S_IRUSR | cstd::S_IWUSR)
		}
		or
		address Address = {
    	    raw: {
				sin_family: cstd::AF_INET as u16,
				sin_port: cstd::htons(port as u16),
				sin_addr: {
					s_addr: cstd::INADDR_ANY
				}
			},
    }
	rather than:
		file.path = path
		file.fd =  cstd::open(path.chars(), flags, cstd::S_IRUSR | cstd::S_IWUSR)
	
	use:
		// TODO: chouldn't we remove
		// sending adderr to bind ? 
		// and just access to it inside
		server.socket.reuse().bind(&server.address).listen(backlog)
	
	use:
	    wait = {
			tv_sec: ms / 1000,
    		tv_nsec: (ms % 1000) * 1000000
		} 
	rather than:
	    wait.tv_sec = ms / 1000
	    wait.tv_nsec = (ms % 1000) * 1000000
	
	+ there are lot of unciton that u don't check there returns ??:
		cstd::nanosleep(&wait, &left)
	
	+ i was thinking:
		+ instead of  hardpcing values like:
			fn read_all() String:
				text String
				part String = self.read(4096)
		+ they should be stoed in a varibale so i can edit them
	+ waaait:
		+ in read_all why reading line by line ??
		+ in fopen, fread, ....
		+ we can nkow the size of the file and read it fully ?????? tat's more optimal
		+ we shdoul review allll you implemntations one by one
		+ ooor since we are using State why not read the full file
	+ for the extenion it has loot of bugs:
		+ for example when ctrl+clique on struct method it does not take me to the file, same hting for inside mod etc....
		+ same thing on strl + clqieu on mattribute in struct
		+ when cliquing on struct type
		+ fn elapsed() i64:??? hwta does tis name even means
	+ other wise we can do the following:
		alias process = Process::process or osmehting close
		eumm process is a long name is there a better naming try getting inspired from zig or rust or something
	+ i was htiking about rorginizing all ural ibraries when we finish
	+ add a testing tool integrated in ura-lang:
		+ add ci for ura-lang compiler and all complex projects built using ura-lang
		+ should we keep mit licence or chnage it ?
	+ for varidic argument:
		+ use 2 approaches:
			+ ... (the existing one)
			+ ...args:
				+ special stucr thaht get splitted 2:
				fn foo(...args) -> fn foo(array_of_lements, array_of_there_size)
				should be supported in ucg
	+ I added some TODO in .ura file check them all
	+ waht is the tag thing in ura-lang github repo
	+ test with multiple oss and llvm versions