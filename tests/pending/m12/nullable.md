# m12

## index

- 001 — nullable: comparing a never-null value to null is rejected
- 002 — nullable: a char[] (C string) can be null, an i32 can't
- 003 — try: a throw is caught in the same function
- 004 — try: a throw propagates across calls to a caller's catch
- 005 — try: a rethrow inside a catch propagates outward
- 006 — try: a nested try: the inner catch handles it

---

## 001 — nullable: comparing a never-null value to null is rejected

```ura
// error: 's1' is never null, so this comparison is always the same
use "@/core"

fn main() i32:
    s1 String = String::create("hello")
    if s1 != null:
        io::out("never\n")
    return 0
```

---

## 002 — nullable: a char[] (C string) can be null, an i32 can't

```ura
// error: 'i32' can't be null
use "@/core"

fn main() i32:
    x char[] = null
    if x == null:
        io::out("x is null\n")
    n i32 = null
    return 0
```

---

## 003 — try: a throw is caught in the same function

```ura
// stdout: caught: boom
// expect: 0
use "@/core"

fn main() i32:
    try:
        throw Error::create(String::create("boom"))
    catch e Error:
        io::out($"caught: {e.message}\n")
    return 0
```

---

## 004 — try: a throw propagates across calls to a caller's catch

```ura
// stdout: caught: from deep
// stdout: after
// expect: 0
use "@/core"

fn deep():
    throw Error::create(String::create("from deep"))

fn middle():
    deep()

fn main() i32:
    try:
        middle()
    catch e Error:
        io::out($"caught: {e.message}\n")
    io::out("after\n")
    return 0
```

---

## 005 — try: a rethrow inside a catch propagates outward

```ura
// stdout: mid: first
// stdout: main: second
// expect: 0
use "@/core"

fn middle():
    try:
        throw Error::create(String::create("first"))
    catch e Error:
        io::out($"mid: {e.message}\n")
        throw Error::create(String::create("second"))

fn main() i32:
    try:
        middle()
    catch e Error:
        io::out($"main: {e.message}\n")
    return 0
```

---

## 006 — try: a nested try: the inner catch handles it

```ura
// stdout: inner: inner
// expect: 0
use "@/core"

fn main() i32:
    try:
        try:
            throw Error::create(String::create("inner"))
        catch e Error:
            io::out($"inner: {e.message}\n")
    catch e Error:
        io::out($"outer: {e.message}\n")
    return 0
```
