# m13

## index

- 001 — builtin: default argument promotion in a user variadic call
- 002 — directive: @if/@elif/@else selects the matching platform branch
- 003 — directive: @if unix matches on macOS and linux
- 004 — directive: a non-matching branch is dropped before parsing
- 005 — directive: a platform-gated top-level proto
- 006 — directive: @if selects a value inside a function
- 014 — function: nested non-capturing helper function
- 016 — function: proto then definition, and definition then proto
- 018 — function: conflict between two of your own protos
- 020 — function: infinite recursion overflows the stack
- 025 — project: calc
- 027 — project: net server
- 028 — project: net client
- 029 — dungeon: floor04
- 030 — dungeon: floor09b
- 031 — dungeon: floor14a
- 032 — dungeon: boss
- 034 — libft: isalnum
- 035 — libft: isalpha
- 036 — libft: isdigit
- 041 — link: an SDL2 window drawn from ura
- 042 — link: a library that is not installed
- 043 — macros: @if / @elif / @else on os and installed()
- 044 — macros: @error stops the build with a message and a help
- 045 — macros: @if on something it can't check

---

## 001 — builtin: default argument promotion in a user variadic call

```ura
// stdout: 3.50 A 7 1 42
// stdout: 123456789
// stdout: 2.50 Z 9
// stdout: 7.00 43
// stdout: 1 0
// stdout: none
// expect: 0
use "@/core"

proto printf(format i8[], ...) i32

fn main() i32:
    f f32 = 3.5
    c char  = 'A'
    s i16 = 7 as i16
    b bool  = True
    l i64  = 123456789 as i64
    i i32   = 42

    printf("%.2f %c %d %d %d\n", f, c, s, b, i)
    printf("%ld\n", l)
    printf("%.2f %c %d\n", 2.5, 'Z', 9)
    printf("%.2f %d\n", f + f, i + 1)
    printf("%d %d\n", 1 < 2, 3 == 4)
    printf("none\n")
    return 0
```

---

## 002 — directive: @if/@elif/@else selects the matching platform branch

```ura
// stdout: unix
// expect: 0
use "@/core"
fn main() i32:
    @if os == "macos":
        io::out("unix\n")
    @elif os == "linux":
        io::out("unix\n")
    @else:
        io::out("other\n")
    return 0
```

---

## 003 — directive: @if unix matches on macOS and linux

```ura
// stdout: posix
// expect: 0
use "@/core"
fn main() i32:
    @if os == "macos" or os == "linux":
        io::out("posix\n")
    @else:
        io::out("windows\n")
    return 0
```

---

## 004 — directive: a non-matching branch is dropped before parsing

```ura
// stdout: kept
// expect: 0
use "@/core"
fn main() i32:
    @if os == "plan9":
        undefined_that_would_error()
    @else:
        io::out("kept\n")
    return 0
```

---

## 005 — directive: a platform-gated top-level proto

```ura
// stdout: True
// expect: 0
use "@/core"
@if os == "macos" or os == "linux":
    proto getpid() i32
@else:
    proto GetCurrentProcessId() u32

fn main() i32:
    io::out($"{getpid() > 0}\n")
    return 0
```

---

## 006 — directive: @if selects a value inside a function

```ura
// stdout: 1
// expect: 0
use "@/core"
fn tag() i32:
    @if os == "macos":
        return 1
    @elif os == "linux":
        return 1
    @else:
        return 0

fn main() i32:
    io::out($"{tag()}\n")
    return 0
```

---

## 014 — function: nested non-capturing helper function

```ura
// expect: 42
use "@/core"

fn main() i32:
    fn dbl(k i32) i32:
        return k + k
    return dbl(21)
```

---

## 016 — function: proto then definition, and definition then proto

```ura
// stdout: 42 42
// expect: 0
use "@/core"

proto tally(n i32) i32

fn tally(n i32) i32:
    return n + 1

fn double(n i32) i32:
    return n * 2

proto double(n i32) i32

fn main() i32:
    io::out($"{tally(41)} {double(21)}\n")
    return 0
```

---

## 018 — function: conflict between two of your own protos

```ura
// error: Conflicting declaration of 'scan': expected 2 parameter(s), found 3
use "@/core"

proto scan(a i32, b i8[]) i32

proto scan(a i32, b i8[], c i32) i32

fn main() i32:
    io::out("unreachable\n")
    return 0
```

---

## 020 — function: infinite recursion overflows the stack

```ura
// expect: 139
use "@/core"

fn recurse(n i32) i32:
    return recurse(n)

fn main() i32:
    return recurse(1)
```

---

## 025 — project: calc

```ura
// stdin: 2 * (3 + 4) - 10 / 5
// stdin: 8 / (3 - 3)
// stdout: ura-calc: type an expression, an empty line quits
// stdout: > calculate expr:  2 * (3 + 4) - 10 / 5
// stdout: ast:
// stdout: op: -
// stdout: ├─ op: *
// stdout: │  ├─ 2
// stdout: │  └─ op: +
// stdout: │     ├─ 3
// stdout: │     └─ 4
// stdout: └─ op: /
// stdout:    ├─ 10
// stdout:    └─ 5
// stdout: result: 12
// stdout: ========================================
// stdout: > calculate expr:  8 / (3 - 3)
// stdout: ast:
// stdout: op: /
// stdout: ├─ 8
// stdout: └─ op: -
// stdout:    ├─ 3
// stdout:    └─ 3
// stdout: error: division by zero
// stdout: ========================================
// stdout: >
// expect: 0
use "@/core"

enum Kind: END, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    kind Kind
    value i32

struct Node:
    token Token
    left i32
    right i32

fn is_digit(c char) bool:
    return c >= '0' and c <= '9'

fn lex(src String) Array<Token>:
    toks Array<Token>
    i i32 = 0
    while i < src.len():
        c char = src[i]
        t Token
        if c == ' ':
            i += 1
            continue
        if is_digit(c):
            t.kind = Kind::NUMBER
            while i < src.len() and is_digit(src[i]):
                t.value = t.value * 10 + ((src[i] as i32) - 48)
                i += 1
            toks.append(t)
            continue
        match c:
            case '+': t.kind = Kind::ADD
            case '-': t.kind = Kind::SUB
            case '*': t.kind = Kind::MUL
            case '/': t.kind = Kind::DIV
            case '(': t.kind = Kind::LPAREN
            case ')': t.kind = Kind::RPAREN
        toks.append(t)
        i += 1
    return toks

fn sign(kind Kind) String:
    match kind:
        case Kind::ADD: return "+"
        case Kind::SUB: return "-"
        case Kind::MUL: return "*"
        case Kind::DIV: return "/"
    return "?"


struct Parser:
    toks Array<Token>
    pos i32
    nodes Array<Node>

    fn peek() Kind:
        if self.pos < self.toks.len():
            return self.toks[self.pos].kind
        return Kind::END

    fn add(token Token, left i32, right i32) i32:
        n Node
        n.token = token
        n.left = left
        n.right = right
        self.nodes.append(n)
        return self.nodes.len() - 1

    fn factor() i32:
        t Token = self.toks[self.pos]
        self.pos += 1
        if t.kind == Kind::LPAREN:
            inner i32 = self.expr()
            self.pos += 1
            return inner
        return self.add(t, -1, -1)

    fn term() i32:
        left i32 = self.factor()
        while self.peek() == Kind::MUL or self.peek() == Kind::DIV:
            t Token = self.toks[self.pos]
            self.pos += 1
            right i32 = self.factor()
            left = self.add(t, left, right)
        return left

    fn expr() i32:
        left i32 = self.term()
        while self.peek() == Kind::ADD or self.peek() == Kind::SUB:
            t Token = self.toks[self.pos]
            self.pos += 1
            right i32 = self.term()
            left = self.add(t, left, right)
        return left

    fn label(i i32) String:
        n Node = self.nodes[i]
        if n.token.kind == Kind::NUMBER:
            return String::create(n.token.value)
        return $"op: {sign(n.token.kind)}"

    fn show(i i32, pre String, last bool):
        branch String = "├─ "
        more String = "│  "
        if last:
            branch = "└─ "
            more = "   "
        io::out($"{pre}{branch}{self.label(i)}\n")
        n Node = self.nodes[i]
        kid String = pre + more
        if n.left >= 0:
            self.show(n.left, kid, n.right < 0)
        if n.right >= 0:
            self.show(n.right, kid, True)

    fn eval(i i32) i32:
        n Node = self.nodes[i]
        match n.token.kind:
            case Kind::NUMBER: return n.token.value
            case Kind::ADD: return self.eval(n.left) + self.eval(n.right)
            case Kind::SUB: return self.eval(n.left) - self.eval(n.right)
            case Kind::MUL: return self.eval(n.left) * self.eval(n.right)
            case Kind::DIV:
                d i32 = self.eval(n.right)
                if d == 0:
                    throw Error::create("division by zero")
                return self.eval(n.left) / d
        return 0


fn calc(text String):
    io::out($"calculate expr:  {text}\nast:\n")
    p Parser
    p.toks = lex(text)
    root i32 = p.expr()
    io::out($"{p.label(root)}\n")
    n Node = p.nodes[root]
    if n.left >= 0:
        p.show(n.left, "", n.right < 0)
    if n.right >= 0:
        p.show(n.right, "", True)
    io::out($"result: {p.eval(root)}\n")

fn main() i32:
    io::out("ura-calc: type an expression, an empty line quits\n")
    io::out("> ")
    line String = io::input()
    while not line.is_empty():
        try:
            calc(line)
        catch e Error:
            io::out($"error: {e.message}\n")
        io::out("========================================\n")
        io::out("> ")
        line = io::input()
    return 0
```

---

## 027 — project: net server

```ura
// a TCP chat server on one poll() loop: stdin, the listener and every client.
// A line typed here goes to every client, a line from a client is printed.
// It needs port 8080, so run it by hand: the test only compiles it.
use "@/core"
use "@/posix/net"
use "@/posix/unistd"

PORT i32 = 8080

fn address(port i32) Array<i8>:
    data Array<i8>
    @if os == "macos":
        data.append(16)
        data.append(2)
    @else:
        data.append(2)
        data.append(0)
    data.append((port >> 8) as i8)
    data.append((port & 255) as i8)
    for i in Range::create(0, 12):
        data.append(0)
    return data

fn listener(port i32) i32:
    fd i32 = socket(2, 1, 0)
    if fd < 0:
        return -1
    one Array<i32>
    one.append(1)
    @if os == "macos":
        setsockopt(fd, 65535, 4, one.data as i8[], 4)
    @else:
        setsockopt(fd, 1, 2, one.data as i8[], 4)
    addr Array<i8> = address(port)
    is_bound bool = bind(fd, addr.data, 16) == 0
    if not is_bound or listen(fd, 16) < 0:
        close(fd)
        return -1
    return fd

fn watch(fds &Array<Pollfd>, fd i32):
    p Pollfd
    p.fd = fd
    p.events = POLLIN as i16
    fds.append(p)

fn main() i32:
    server i32 = listener(PORT)
    if server < 0:
        io::err("[server] can't listen on port 8080\n")
        return 1
    io::out("[server] listening on port 8080\n")

    // [0] stdin, [1] the listener, [2..] the clients
    fds Array<Pollfd>
    watch(&fds,0)
    watch(&fds,server)
    buf Array<i8>
    buf.resize(1024)

    while True:
        poll(fds.data as i8[], fds.len(), -1)

        // a line typed here goes to every client
        if ((fds[0].revents as i32) & POLLIN) != 0:
            r i64 = read(0, buf.data, 1023)
            if r <= 0:
                break
            i i32 = 2
            while i < fds.len():
                write(fds[i].fd, buf.data, r)
                i += 1

        // a new client joins the poll set
        if ((fds[1].revents as i32) & POLLIN) != 0:
            client i32 = accept(server, null, null)
            if client >= 0:
                watch(&fds,client)
                io::out("[server] client connected\n")

        // a client's line is printed; a closed client leaves the set
        j i32 = 2
        while j < fds.len():
            if ((fds[j].revents as i32) & POLLIN) == 0:
                j += 1
                continue
            r i64 = read(fds[j].fd, buf.data, 1023)
            if r <= 0:
                close(fds[j].fd)
                fds[j] = fds[fds.len() - 1]
                fds.pop()
                io::out("[server] client disconnected\n")
                continue
            io::out("[client] ")
            write(1, buf.data, r)
            j += 1

    close(server)
    return 0
```

---

## 028 — project: net client

```ura
// a TCP chat client on one poll() loop: stdin and the server socket.
// A line typed here goes to the server, a line from the server is printed.
// It needs the net server on port 8080, so run it by hand: the test only
// compiles it.
use "@/core"
use "@/posix/net"
use "@/posix/unistd"

PORT i32 = 8080

fn address(port i32) Array<i8>:
    data Array<i8>
    @if os == "macos":
        data.append(16)
        data.append(2)
    @else:
        data.append(2)
        data.append(0)
    data.append((port >> 8) as i8)
    data.append((port & 255) as i8)
    data.append(127)
    data.append(0)
    data.append(0)
    data.append(1)
    for i in Range::create(0, 8):
        data.append(0)
    return data

fn watch(fds &Array<Pollfd>, fd i32):
    p Pollfd
    p.fd = fd
    p.events = POLLIN as i16
    fds.append(p)

fn main() i32:
    fd i32 = socket(2, 1, 0)
    addr Array<i8> = address(PORT)
    if fd < 0 or connect(fd, addr.data, 16) != 0:
        io::err("[client] can't reach 127.0.0.1:8080\n")
        return 1
    io::out("[client] connected to 127.0.0.1:8080\n")

    // [0] stdin, [1] the server
    fds Array<Pollfd>
    watch(&fds,0)
    watch(&fds,fd)
    buf Array<i8>
    buf.resize(1024)

    while True:
        poll(fds.data as i8[], fds.len(), -1)

        // a line typed here goes to the server
        if ((fds[0].revents as i32) & POLLIN) != 0:
            r i64 = read(0, buf.data, 1023)
            if r <= 0:
                break
            write(fd, buf.data, r)

        // a line from the server is printed; a closed server ends the loop
        if ((fds[1].revents as i32) & POLLIN) != 0:
            r i64 = read(fd, buf.data, 1023)
            if r <= 0:
                io::out("[client] the server closed the connection\n")
                break
            io::out("[server] ")
            write(1, buf.data, r)

    close(fd)
    return 0
```

---

## 029 — dungeon: floor04

```ura
// stdout: Orc takes 17 damage — dead: True
// stdout: 4 squared: 16
// expect: 0
// dungeon/floor04.ura
use "@/core"
fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn square(n i32) i32:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

fn main() i32:
    d i32 = damage(25, 8)
    io::out($"Orc takes {d} damage — dead: {is_dead(d - 60)}\n")
    io::out($"4 squared: {square(4)}\n")
    return 0
```

---

## 030 — dungeon: floor09b

```ura
// stdout: 100
// expect: 0
// dungeon/floor09b.ura
use "@/core"
fn main() i32:
    hp  i32     = 80
    r &i32 = &hp
    r   = 100
    io::out($"{hp}\n")
    return 0
```

---

## 031 — dungeon: floor14a

```ura
// stdout: hi
// stdout: length: 7
// expect: 0
// dungeon/floor14a.ura
use "@/core"
proto strlen(s i8[]) i64
proto write(fd i32, ptr i8[], len i64) i64
proto printf(fmt i8[], ...) i32

fn main() i32:
    n i32 = strlen("dungeon") as i32
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
    return 0
```

---

## 032 — dungeon: boss

```ura
// stdout: == Dungeon ==
// stdout: Party:
// stdout:   Aldric atk=18
// stdout:   Borin atk=25
// stdout:   Cass atk=22
// stdout: Champion: Borin (alive=True)
// stdout: Foe hp=20 loot=none
// stdout: Foe hp=45 loot=none
// stdout: Foe hp=120 loot=Dragonfang
// stdout: Trap! Aldric hp=90
// stdout: sizeof=24 typeof=Hero
// expect: 0
use "@/core"

enum EnemyType: GOBLIN, ORC, DRAGON

struct Stats:
    hp  i32
    atk i32

struct Hero:
    name  String
    stats Stats

    pub fn create(name String, hp i32, atk i32) Hero:
        h Hero
        h.name      = name
        h.stats.hp  = hp
        h.stats.atk = atk
        return h

    fn alive() bool:
        return self.stats.hp > 0

operator =(left &Hero, right &Hero):
    left.name = right.name
    left.stats = right.stats

operator $(hero &Hero) String:
    return hero.name

fn enemy_hp(kind EnemyType) i32:
    match kind:
        case EnemyType::GOBLIN: return 20
        case EnemyType::ORC:    return 45
        case EnemyType::DRAGON: return 120
    return 0

fn loot_for(kind EnemyType) String:
    if kind == EnemyType::DRAGON:
        return "Dragonfang"
    return "none"

fn strongest(party &Array<Hero>) i32:
    best i32 = 0
    for i in Range::create(1, party.len()):
        if party[i].stats.atk > party[best].stats.atk:
            best = i
    return best

fn main() i32:
    io::out("== Dungeon ==\n")
    party Array<Hero>
    party.append(Hero::create("Aldric", 100, 18))
    party.append(Hero::create("Borin", 90, 25))
    party.append(Hero::create("Cass", 80, 22))

    io::out("Party:\n")
    for h in party:
        io::out($"  {h} atk={h.stats.atk}\n")

    champ Hero = party[strongest(&party)]
    io::out($"Champion: {champ} (alive={champ.alive()})\n")

    foes Array<EnemyType> = [EnemyType::GOBLIN, EnemyType::ORC, EnemyType::DRAGON]
    for kind in foes:
        io::out($"Foe hp={enemy_hp(kind)} loot={loot_for(kind)}\n")

    for i in Range::create(0, party.len()):
        party[i].stats.hp -= 10
    io::out($"Trap! {party[0]} hp={party[0].stats.hp}\n")
    io::out($"sizeof={sizeof(champ)} typeof={typeof(champ)}\n")
    return 0
```

---

## 034 — libft: isalnum

```ura
// stdout: is alnum
// expect: 0
// libft/isalnum.ura
use "@/core"
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'
fn isdigit(c char) bool: return c >= '1' and c <= '9'
fn isalnum(c char) bool: return isalpha(c) or isdigit(c)

fn main() i32:
    if isalnum('1'):
        io::out("is alnum\n")
    else:
        io::out("is not alnum\n")
    return 0
```

---

## 035 — libft: isalpha

```ura
// stdout: is not alpha
// expect: 0
// libft/isalpha.ura
use "@/core"
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'

fn main() i32:
    if isalpha('1'):
        io::out("is alpha\n")
    else:
        io::out("is not alpha\n")
    return 0
```

---

## 036 — libft: isdigit

```ura
// stdout: is alnum
// expect: 0
// libft/isdigit.ura
use "@/core"
fn isdigit(c char) bool: return c >= '1' and c <= '9'

fn main() i32:
    if isdigit('1'):
        io::out("is alnum\n")
    else:
        io::out("is not alnum\n")
    return 0
```

---

## 041 — link: an SDL2 window drawn from ura

```ura
// stdout: window closed after 181 frames
// expect: 0
// opens a 400x300 SDL2 window with a color gradient for 3 seconds
use "@/core"
link "SDL2"
proto SDL_Init(flags i32) i32
proto SDL_CreateWindow(title i8[], x i32, y i32, w i32, h i32, flags i32) i8[]
proto SDL_CreateRenderer(window i8[], index i32, flags i32) i8[]
proto SDL_SetRenderDrawColor(renderer i8[], r i8, g i8, b i8, a i8) i32
proto SDL_RenderDrawPoint(renderer i8[], x i32, y i32) i32
proto SDL_RenderPresent(renderer i8[])
proto SDL_PollEvent(event &Event) i32
proto SDL_Delay(ms i32)
proto SDL_Quit()

struct Event:
    type i32
    a1 i32
    a2 i32
    a3 i32
    a4 i32
    a5 i32
    a6 i32
    a7 i32
    a8 i32
    a9 i32
    a10 i32
    a11 i32
    a12 i32
    a13 i32

fn main() i32:
    width i32 = 400
    height i32 = 300
    SDL_Init(32)
    window i8[] = SDL_CreateWindow("ura + SDL2", 805240832, 805240832, width, height, 0)
    renderer i8[] = SDL_CreateRenderer(window, -1, 0)

    y i32 = 0
    while y < height:
        x i32 = 0
        while x < width:
            red i8 = (x * 255 / width) as i8
            green i8 = (y * 255 / height) as i8
            SDL_SetRenderDrawColor(renderer, red, green, 128 as i8, 255 as i8)
            SDL_RenderDrawPoint(renderer, x, y)
            x += 1
        y += 1
    SDL_RenderPresent(renderer)

    event Event
    running bool = True
    frames i32 = 0
    while running:
        while SDL_PollEvent(&event) != 0:
            if event.type == 256:
                running = False
        SDL_Delay(16)
        frames += 1
        if frames > 180:
            running = False
    SDL_Quit()
    io::out($"window closed after {frames} frames\n")
    return 0
```

---

## 042 — link: a library that is not installed

```ura
// error: library 'NoSuchLib' not found
use "@/core"
link "NoSuchLib"
fn main() i32:
    return 0
```

---

## 043 — macros: @if / @elif / @else on os and installed()

```ura
use "@/core"
@if os == "linux" or os == "macos":
    name i8[] = "unix"
@elif os == "windows":
    name i8[] = "windows"
@else:
    @error "unsupported platform"

fn main() i32:
    @if installed("SDL2") and not installed("raylib"):
        io::out($"{name} sdl only\n")
    @else:
        io::out("other\n")
    return 0
```

---

## 044 — macros: @error stops the build with a message and a help

```ura
use "@/core"
@if not installed("nothing_like_this"):
    @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"

fn main() i32:
    return 0
```

---

## 045 — macros: @if on something it can't check

```ura
use "@/core"
@if version == 2:
    x i32 = 1

fn main() i32:
    return 0
```
