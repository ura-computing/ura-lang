# m06

## index

- 026 — output: output and errput print every scalar type in one call
- 027 — input: read two lines from stdin
- 028 — $ strings: values inside {} formatted into a String
- 029 — $ strings: \{ and \} are literal braces
- 030 — operator $: a struct formats itself inside {}
- 031 — operator $: a struct without it can't go inside {}
- 032 — io: io::out, io::err and io::input from ura-core

---

## 026 — output: output and errput print every scalar type in one call

```ura
// stdout: hp: 100 speed: 1.500000 alive: True grade: S 100%
// stderr: bad value: 7
// expect: 0
use "@/core"
fn main() i32:
    hp i32 = 100
    speed f32 = 1.5
    alive bool = True
    grade char = 'S'
    io::out($"hp: {hp} speed: {speed} alive: {alive} grade: {grade} 100%\n")
    io::err($"bad value: {7}\n")
    return 0
```

---

## 027 — input: read two lines from stdin

```ura
// stdin: Aldric
// stdin: 30
// stdout: name? hello Aldric
// stdout: age? you said 30
// expect: 0
use "@/core"
fn main() i32:
    io::out("name? ")
    name String = io::input()
    io::out($"hello {name}\n")
    io::out("age? ")
    age String = io::input()
    io::out($"you said {age}\n")
    return 0
```

---

## 028 — $ strings: values inside {} formatted into a String

```ura
// stdout: test.ura:8 hp 10 2.500000 True c
// stdout: name: ura, 浦
// expect: 0
use "@/core"
fn main() i32:
    hp i32 = 10
    name String = "ura"
    s String = $"{ura::filename}:{ura::line} hp {hp} {2.5f} {hp == 10} {'c'}"
    io::out($"{s}\n")
    bay unic = '浦'
    io::out($"name: {name}, {bay}\n")
    return 0
```

---

## 029 — $ strings: \{ and \} are literal braces

```ura
// stdout: set { x: 5 }
// stdout: {}
// expect: 0
use "@/core"
fn main() i32:
    x i32 = 5
    io::out($"set \{ x: {x} \}\n")
    io::out("\{\}\n")
    return 0
```

---

## 030 — operator $: a struct formats itself inside {}

```ura
// stdout: p = (1, 2)
// stdout: line = (1, 2) -> (3, 4)
// stdout: 0 (0, 0)
// stdout: 1 (1, 1)
// expect: 0
use "@/core"
struct Point:
    x i32
    y i32

operator $(left &Point) String:
    return $"({left.x}, {left.y})"

struct Line:
    a Point
    b Point

operator $(left &Line) String:
    return $"{left.a} -> {left.b}"

fn main() i32:
    p Point
    p.x = 1
    p.y = 2
    io::out($"p = {p}\n")
    l Line
    l.a = p
    l.b.x = 3
    l.b.y = 4
    io::out($"line = {l}\n")
    i i32 = 0
    while i < 2:
        q Point
        q.x = i
        q.y = i
        io::out($"{i} {q}\n")
        i += 1
    return 0
```

---

## 031 — operator $: a struct without it can't go inside {}

```ura
// error: 'Point' has no operator '$'
use "@/core"
struct Point:
    x i32

fn main() i32:
    p Point
    io::out($"p = {p}\n")
    return 0
```

---

## 032 — io: io::out, io::err and io::input from ura-core

```ura
// stdin: Aldric
// stdout: name? hello Aldric
// stdout: 3 letters? no, 6
// expect: 0
use "@/core"
fn main() i32:
    io::out("name? ")
    name String = io::input()
    io::out($"hello {name}\n")
    io::err("this goes to stderr\n")
    io::out($"3 letters? no, {name.len()}\n")
    return 0
```
