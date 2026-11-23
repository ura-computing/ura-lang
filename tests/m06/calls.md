# m06

## index

- 002 — multi-line string: a block string, its first and last newline add nothing
- 003 — builtin: output picks the spec from each argument
- 004 — builtin: output with literals and escapes
- 012 — output: basic output: strings, ints, floats, bools
- 013 — output: output with expressions: arithmetic inline
- 014 — output: output all supported types in one block
- 015 — output: output() auto-formats each argument by its type
- 016 — string: String constructors and accessors
- 017 — string: assign, push, pop, join and clear
- 018 — string: find, contains, starts_with and ends_with

---

## 002 — multi-line string: a block string, its first and last newline add nothing

```ura
// stdout: hello world 12
// expect: 0
use "@/core"
fn main() i32:
    a i32 = 12
    text String = "
                hello
                world
              "
    io::out($"{text} {a}\n")
    return 0
```

---

## 003 — builtin: output picks the spec from each argument

```ura
use "@/core"
fn main() i32:
   n i32 = 42
   c i8 = 'z'
   s i8[] = "hi"
   flag bool = n > 1
   io::out($"{n} {c} {s} {flag}\n")
   return 0
```

---

## 004 — builtin: output with literals and escapes

```ura
use "@/core"
fn main() i32:
   n i32 = 3
   io::out($"n={n}\ttab\n")
   io::out("no newline")
   io::out("\n")
   return 0
```

---

## 012 — output: basic output: strings, ints, floats, bools

```ura
// stdout: === Aldric enters the dungeon ===
// stdout: Floor: 1
// stdout: Alive: True
// stdout: Speed: 1.500000
// stdout: Ready to fight!
// expect: 0
use "@/core"

fn main() i32:
    io::out("=== Aldric enters the dungeon ===\n")
    io::out($"Floor: {1}\n")
    io::out($"Alive: {True}\n")
    io::out($"Speed: {1.5}\n")
    io::out("Ready to fight!\n")
    return 0
```

---

## 013 — output: output with expressions: arithmetic inline

```ura
// stdout: Damage dealt: 17
// stdout: XP gained:    150
// stdout: Gold split:   30
// stdout: Bonus:        37
// expect: 0
use "@/core"

fn main() i32:
    io::out($"Damage dealt: {25 - 8}\n")
    io::out($"XP gained:    {3 * 50}\n")
    io::out($"Gold split:   {120 / 4}\n")
    io::out($"Bonus:        {7 + 3 * 10}\n")
    return 0
```

---

## 014 — output: output all supported types in one block

```ura
// stdout: Name:  Aldric
// stdout: HP:    100
// stdout: Grade: S
// stdout: Alive: True
// stdout: Speed: 1.750000
// expect: 0
use "@/core"

fn main() i32:
    name  char[] = "Aldric"
    hp    i32   = 100
    grade char  = 'S'
    alive bool  = True
    speed f32 = 1.75

    io::out($"Name:  {name}\n")
    io::out($"HP:    {hp}\n")
    io::out($"Grade: {grade}\n")
    io::out($"Alive: {alive}\n")
    io::out($"Speed: {speed}\n")
    return 0
```

---

## 015 — output: output() auto-formats each argument by its type

```ura
// stdout: hello ura 42 True
// expect: 0
use "@/core"

fn main() i32:
    name char[] = "ura"
    n i32 = 42
    ok bool = True
    io::out($"hello {name} {n} {ok}\n")
    return 0
```

---

## 016 — string: String constructors and accessors

```ura
// stdout: 0 True
// stdout: hello 5 False
// stdout: 0 True
// stdout: 42
// stdout: -123
// stdout: 0
// stdout: h o 0 0
// stdout: hello
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s1 String = String::create("")
    io::out($"{s1.size()} {s1.is_empty()}\n")

    s2 String = String::create("hello")
    io::out($"{s2} {s2.size()} {s2.is_empty()}\n")

    s3 String = String::create("")
    io::out($"{s3.size()} {s3.is_empty()}\n")

    s4 String = String::create(42)
    io::out($"{s4}\n")

    s5 String = String::create(-123)
    io::out($"{s5}\n")

    s6 String = String::create(0)
    io::out($"{s6}\n")

    io::out($"{s2.at(0)} {s2.at(4)} {s2.at(-1) as i32} {s2.at(99) as i32}\n")
    io::out($"{s2.chars()}\n")
    return 0
```

---

## 017 — string: assign, push, pop, join and clear

```ura
// stdout: abc 3
// stdout: hello world this is a longer string
// stdout: 0 True
// stdout: XYZ 3
// stdout: Z XY 2
// stdout: 0 0
// stdout: hello world
// stdout: 0 True
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s String = String::create("")
    s = "abc"
    io::out($"{s} {s.size()}\n")

    s = "hello world this is a longer string"
    io::out($"{s}\n")

    s = ""
    io::out($"{s.size()} {s.is_empty()}\n")

    s2 String = String::create("")
    s2.append('X')
    s2.append('Y')
    s2.append('Z')
    io::out($"{s2} {s2.size()}\n")

    c char = s2.pop()
    io::out($"{c} {s2} {s2.size()}\n")

    s2.pop()
    s2.pop()
    c2 char = s2.pop()
    io::out($"{c2 as i32} {s2.size()}\n")

    s3 String = String::create("hello")
    s3.join(" world")
    io::out($"{s3}\n")

    s3.clear()
    io::out($"{s3.size()} {s3.is_empty()}\n")
    return 0
```

---

## 018 — string: find, contains, starts_with and ends_with

```ura
// stdout: 6
// stdout: 0
// stdout: -1
// stdout: 0
// stdout: 10
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: True
// stdout: False
// stdout: True
// stdout: -1
// stdout: False
// stdout: True
// stdout: True
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s String = String::create("hello world")

    io::out($"{s.find("world")}\n")
    io::out($"{s.find("hello")}\n")
    io::out($"{s.find("xyz")}\n")
    io::out($"{s.find("")}\n")
    io::out($"{s.find("d")}\n")

    io::out($"{s.contains("world")}\n")
    io::out($"{s.contains("xyz")}\n")

    io::out($"{s.starts_with("hello")}\n")
    io::out($"{s.starts_with("world")}\n")
    io::out($"{s.starts_with("")}\n")

    io::out($"{s.ends_with("world")}\n")
    io::out($"{s.ends_with("hello")}\n")
    io::out($"{s.ends_with("d")}\n")

    empty String = String::create("")
    io::out($"{empty.find("x")}\n")
    io::out($"{empty.contains("x")}\n")
    io::out($"{empty.starts_with("")}\n")
    io::out($"{empty.ends_with("")}\n")
    return 0
```
