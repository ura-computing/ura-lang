# m07

## index

- 001 — enum: auto values start at 0 and count up
- 002 — enum: variants on one line, comma-separated
- 003 — enum: an explicit value resets the running counter
- 004 — enum: an enum-typed variable compared to a variant
- 005 — enum: match over an enum inside a function
- 006 — enum: an enum used as a struct field type
- 007 — enum: a variant used before the enum is declared
- 008 — enum: an enum must declare at least one variant
- 009 — enum: a variant name cannot be declared twice
- 010 — enum: an explicit variant value must be an integer
- 011 — match: a single-value case matches
- 012 — match: a case with several values
- 013 — match: default runs when nothing matches
- 014 — match: break leaves the match early
- 015 — match: break exits the match, not the enclosing loop
- 016 — match: continue applies to the loop, skipping the rest of the iteration
- 017 — match: a case needs at least one value
- 018 — match: case value type must match the subject
- 019 — match: a case with no enclosing match
- 020 — match: a default with no enclosing match
- 022 — match: a String subject compares through operator ==, break and continue drop it once

---

## 001 — enum: auto values start at 0 and count up

```ura
// stdout: 0 1 2
// expect: 0
use "@/core"
enum Color:
    RED,
    GREEN,
    BLUE,

fn main() i32:
    io::out($"{Color::RED} {Color::GREEN} {Color::BLUE}\n")
    return 0
```

---

## 002 — enum: variants on one line, comma-separated

```ura
// stdout: 012
// expect: 0
use "@/core"
enum Color: RED, GREEN, BLUE

fn main() i32:
    io::out($"{Color::RED}{Color::GREEN}{Color::BLUE}\n")
    return 0
```

---

## 003 — enum: an explicit value resets the running counter

```ura
// stdout: 1234
// expect: 0
use "@/core"
enum Dir:
    NORTH = 1,
    EAST,
    SOUTH,
    WEST,

fn main() i32:
    io::out($"{Dir::NORTH}{Dir::EAST}{Dir::SOUTH}{Dir::WEST}\n")
    return 0
```

---

## 004 — enum: an enum-typed variable compared to a variant

```ura
// stdout: green
// expect: 0
use "@/core"
enum Color: RED, GREEN, BLUE

fn main() i32:
    c Color = Color::GREEN
    if c == Color::GREEN:
        io::out("green\n")
    else:
        io::out("other\n")
    return 0
```

---

## 005 — enum: match over an enum inside a function

```ura
// stdout: 1 -1
// expect: 0
use "@/core"
enum Signal: LOW, HIGH, FLOAT

fn describe(s Signal) i32:
    match s:
        case Signal::LOW:
            return 0
        case Signal::HIGH:
            return 1
        default:
            return -1
    return -2

fn main() i32:
    io::out($"{describe(Signal::HIGH)} {describe(Signal::FLOAT)}\n")
    return 0
```

---

## 006 — enum: an enum used as a struct field type

```ura
// stdout: 1 30
// expect: 0
use "@/core"
enum Kind: WARRIOR, MAGE, ROGUE

struct Hero:
    kind Kind
    hp i32

fn main() i32:
    h Hero
    h.kind = Kind::MAGE
    h.hp = 30
    io::out($"{h.kind} {h.hp}\n")
    return 0
```

---

## 007 — enum: a variant used before the enum is declared

```ura
// stdout: 0
// expect: 0
use "@/core"
fn first() Color:
    return Color::RED

enum Color: RED, GREEN, BLUE

fn main() i32:
    io::out($"{first()}\n")
    return 0
```

---

## 008 — enum: an enum must declare at least one variant

```ura
// error: enum 'Nothing' has no variants
use "@/core"
enum Nothing:

fn main() i32:
    io::out($"{0}\n")
    return 0
```

---

## 009 — enum: a variant name cannot be declared twice

```ura
// error: 'RED' is already declared
use "@/core"
enum C:
    RED, GREEN, RED

fn main() i32:
    io::out($"{0}\n")
    return 0
```

---

## 010 — enum: an explicit variant value must be an integer

```ura
// error: the value of 'A' must be an integer
use "@/core"
enum C:
    A = hello

fn main() i32:
    io::out($"{0}\n")
    return 0
```

---

## 011 — match: a single-value case matches

```ura
// stdout: one
// expect: 0
use "@/core"
fn main() i32:
    a i32 = 1
    match a:
        case 1:
            io::out("one\n")
        default:
            io::out("other\n")
    return 0
```

---

## 012 — match: a case with several values

```ura
// stdout: small
// expect: 0
use "@/core"
fn main() i32:
    a i32 = 3
    match a:
        case 1, 2, 3:
            io::out("small\n")
        default:
            io::out("big\n")
    return 0
```

---

## 013 — match: default runs when nothing matches

```ura
// stdout: fallback
// expect: 0
use "@/core"
fn main() i32:
    a i32 = 99
    match a:
        case 1:
            io::out("one\n")
        default:
            io::out("fallback\n")
    return 0
```

---

## 014 — match: break leaves the match early

```ura
// stdout: before
// expect: 0
use "@/core"
fn main() i32:
    a i32 = 1
    match a:
        case 1:
            io::out("before\n")
            break
            io::out("after\n")
    return 0
```

---

## 015 — match: break exits the match, not the enclosing loop

```ura
// stdout: 1
// stdout: 2
// stdout: 3
// expect: 0
use "@/core"
fn main() i32:
    i i32 = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                break
        io::out($"{i}\n")
    return 0
```

---

## 016 — match: continue applies to the loop, skipping the rest of the iteration

```ura
// stdout: 1
// stdout: 3
// expect: 0
use "@/core"
fn main() i32:
    i i32 = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                continue
        io::out($"{i}\n")
    return 0
```

---

## 017 — match: a case needs at least one value

```ura
// error: expected a value after 'case'
// errors/match/001 — a case needs at least one value
use "@/core"
fn main() i32:
    a i32 = 1
    match a:
        case:
            io::out("x\n")
    return 0
```

---

## 018 — match: case value type must match the subject

```ura
// error: expected 'i32', found 'i8[]'
// errors/match/002 — case value type must match the subject
use "@/core"
fn main() i32:
    a i32 = 1
    match a:
        case "x":
            io::out("x\n")
    return 0
```

---

## 019 — match: a case with no enclosing match

```ura
// error: 'case' without a matching 'match'
// errors/match/003 — a case with no enclosing match
use "@/core"
fn main() i32:
    case 1:
        io::out("x\n")
    return 0
```

---

## 020 — match: a default with no enclosing match

```ura
// error: 'default' without a matching 'match'
// errors/match/004 — a default with no enclosing match
use "@/core"
fn main() i32:
    default:
        io::out("x\n")
    return 0
```

---

## 022 — match: a String subject compares through operator ==, break and continue drop it once

```ura
// stdout: 1120
// stdout: test
// expect: 0
use "@/core"
use "@/posix/string"

fn kind(s String) i32:
    match s:
        case "hello", "test":
            return 1
        case "stop":
            break
        default:
            return 0
    return 2

fn main() i32:
    name String = String::create("test")
    io::out($"{kind(String::create("hello"))}{kind(name)}{kind(String::create("stop"))}{kind(String::create("x"))}\n")
    i i32 = 0
    while i < 3:
        i += 1
        match String::create("abc"):
            case "abc":
                continue
        io::out("never\n")
    io::out($"{name}\n")
    return 0
```
