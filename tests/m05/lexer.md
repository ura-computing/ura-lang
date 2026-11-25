# m05

## index

- 007 — lexer: use missing opening quote
- 008 — lexer: use with unclosed quote
- 009 — lexer: link missing opening quote
- 010 — lexer: link with unclosed quote
- 011 — mod: a module function is called with ::
- 012 — mod: nested modules: a::b::f()
- 013 — mod: an enum inside a module, read as mod::Enum::VARIANT
- 014 — mod: a module may be referenced before it is declared
- 015 — mod: a module function calls a sibling unqualified
- 016 — mod: a struct declared in a module, used as mod::Type
- 017 — mod: a pub static method on a struct inside a module
- 018 — mod: calling a module member with . is an error
- 019 — mod: an unknown member of a known module is an error
- 020 — mod: a global inside a module, read and written with ::
- 021 — mod: a module call is arity-checked
- 022 — os: argc and argv from mod os
- 023 — mod: a struct attribute with the name of a module global
- 024 — ura::line, ura::filename, ura::function
- 025 — unic: utf-8 char literals, printed as utf-8
- 026 — multi-line string: a newline and the spaces after it become one space
- 027 — multi-line string: as a call argument, blank lines dropped
- 028 — multi-line string: escapes and quotes inside
- 029 — sizeof: takes a struct type name

---

## 007 — lexer: use missing opening quote

```ura
// error: Expected '"' after 'use'
use "@/core"
use @/header

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 008 — lexer: use with unclosed quote

```ura
// error: Unterminated 'use' path, expected closing '"'
use "@/core"
use "@/header

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 009 — lexer: link missing opening quote

```ura
// error: Expected '"' after 'link'
// errors/lexer/008 — `link` missing opening quote
use "@/core"
link header.h

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 010 — lexer: link with unclosed quote

```ura
// error: Unterminated 'link' path, expected closing '"'
// errors/lexer/009 — `link` with unclosed quote
use "@/core"
link "header.h

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 011 — mod: a module function is called with ::

```ura
// stdout: 1
// expect: 0
use "@/core"
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    io::out($"{lib::foo()}\n")
    return 0
```

---

## 012 — mod: nested modules: a::b::f()

```ura
// stdout: 42
// expect: 0
use "@/core"
mod a:
    mod b:
        fn f() i32:
            return 42

fn main() i32:
    io::out($"{a::b::f()}\n")
    return 0
```

---

## 013 — mod: an enum inside a module, read as mod::Enum::VARIANT

```ura
// stdout: 0 2
// expect: 0
use "@/core"
mod col:
    enum Color:
        RED, GREEN, BLUE

fn main() i32:
    io::out($"{col::Color::RED} {col::Color::BLUE}\n")
    return 0
```

---

## 014 — mod: a module may be referenced before it is declared

```ura
// stdout: 7
// expect: 0
use "@/core"
fn main() i32:
    io::out($"{later::hi()}\n")
    return 0

mod later:
    fn hi() i32:
        return 7
```

---

## 015 — mod: a module function calls a sibling unqualified

```ura
// stdout: 6
// expect: 0
use "@/core"
mod m:
    fn helper() i32:
        return 5
    fn use_it() i32:
        return helper() + 1

fn main() i32:
    io::out($"{m::use_it()}\n")
    return 0
```

---

## 016 — mod: a struct declared in a module, used as mod::Type

```ura
// stdout: 9
// expect: 0
use "@/core"
mod g:
    struct Point:
        x i32

fn main() i32:
    p g::Point
    p.x = 9
    io::out($"{p.x}\n")
    return 0
```

---

## 017 — mod: a pub static method on a struct inside a module

```ura
// stdout: 3
// expect: 0
use "@/core"
mod g:
    struct Point:
        x i32
        pub fn create(v i32) Point:
            p Point
            p.x = v
            return p

fn main() i32:
    p g::Point = g::Point::create(3)
    io::out($"{p.x}\n")
    return 0
```

---

## 018 — mod: calling a module member with . is an error

```ura
// error: 'lib' is a mod, not a value
use "@/core"
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    io::out($"{lib.foo()}\n")
    return 0
```

---

## 019 — mod: an unknown member of a known module is an error

```ura
// error: no function named 'lib::bar'
use "@/core"
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    io::out($"{lib::bar()}\n")
    return 0
```

---

## 020 — mod: a global inside a module, read and written with ::

```ura
// stdout: 5 6
// expect: 0
use "@/core"
mod lib:
    x i32 = 5
    fn bump():
        x = x + 1

fn main() i32:
    io::out($"{lib::x} ")
    lib::bump()
    io::out($"{lib::x}\n")
    return 0
```

---

## 021 — mod: a module call is arity-checked

```ura
// error: 'lib::foo' takes 1 argument, not 2
use "@/core"
mod lib:
    fn foo(a i32) i32:
        return a

fn main() i32:
    io::out($"{lib::foo(1, 2)}\n")
    return 0
```

---

## 022 — os: argc and argv from mod os

```ura
// stdout: argc: 1
// stdout: argv: 1
// expect: 0
use "@/core"

fn main() i32:
    io::out($"argc: {os::argc}\n")
    io::out($"argv: {os::argv.len()}\n")
    return 0
```

---

## 023 — mod: a struct attribute with the name of a module global

```ura
// stdout: 1 2
// expect: 0
use "@/core"
mod geo:
    x i32 = 1
    struct Point:
        x i32
        fn show():
            io::out($"{x} {self.x}\n")

fn main() i32:
    p geo::Point
    p.x = 2
    p.show()
    return 0
```

---

## 024 — ura::line, ura::filename, ura::function

```ura
// stdout: test.ura:7 in where
// stdout: show 12
// stdout: 19
// expect: 0
use "@/core"
fn where():
    io::out($"{ura::filename}:{ura::line} in {ura::function}\n")

struct Point:
    x i32
    fn show():
        io::out($"{ura::function} {ura::line}\n")

fn main() i32:
    where()
    p Point
    p.show()
    n i32 = ura::line + 1
    io::out($"{n}\n")
    return 0
```

---

## 025 — unic: utf-8 char literals, printed as utf-8

```ura
// stdout: a é 浦 🌊 🐚 🐙
// stdout: 233 28006 127754
// expect: 0
use "@/core"
fn main() i32:
    a unic = 'a'
    e unic = 'é'
    bay unic = '浦'
    wave unic = '🌊'
    shell unic = '🐚'
    octopus unic = '🐙'
    io::out($"{a} {e} {bay} {wave} {shell} {octopus}\n")
    io::out($"{e as i32} {bay as i32} {wave as i32}\n")
    return 0
```

---

## 026 — multi-line string: a newline and the spaces after it become one space

```ura
// stdout: hello world
// stdout: hello again
// stdout: line 14
// expect: 0
use "@/core"
fn main() i32:
    a String = "hello
        world\n"
    io::out($"{a}")
    b String = "
        hello again\n
        "
    io::out($"{b}")
    io::out($"line {ura::line}\n")
    return 0
```

---

## 027 — multi-line string: as a call argument, blank lines dropped

```ura
// stdout: one two
// stdout: three
// expect: 0
use "@/core"
fn show(text String):
    io::out($"{text}")

fn main() i32:
    show("one

        two\n
        three\n")
    return 0
```

---

## 028 — multi-line string: escapes and quotes inside

```ura
// stdout: name: "ura"
// stdout: 	tab
// expect: 0
use "@/core"
fn main() i32:
    io::out("
        name: \"ura\"\n
        \ttab\n
        ")
    return 0
```

## 029 — sizeof: takes a struct type name

```ura
// stdout: 8
// expect: 0
use "@/core"
struct Pair:
    a i32
    b i32

fn main() i32:
    io::out($"{sizeof(Pair)}\n")
    return 0
```
