# m12

## index

- 007 — try: return inside a try returns from the function
- 008 — try: break inside a try exits the enclosing loop
- 009 — try: an uncaught throw prints its message and exits
- 010 — try: any type can be thrown; a catch takes the type it names
- 011 — try: catch without a matching try is a parse error
- 012 — try: code after a throw is unreachable and dropped

---

## 007 — try: return inside a try returns from the function

```ura
// stdout: ret = 5
// expect: 0
use "@/core"

fn pick() i32:
    try:
        return 5
    catch e Error:
        return 0

fn main() i32:
    io::out($"ret = {pick()}\n")
    return 0
```

---

## 008 — try: break inside a try exits the enclosing loop

```ura
// stdout: i=0
// stdout: done
// expect: 0
use "@/core"

fn main() i32:
    i i32 = 0
    while i < 3:
        try:
            if i == 1:
                break
            io::out($"i={i}\n")
        catch e Error:
            io::out("x\n")
        i += 1
    io::out("done\n")
    return 0
```

---

## 009 — try: an uncaught throw prints its message and exits

```ura
// stderr: uncaught
// expect: 1
use "@/core"

fn main() i32:
    throw Error::create(String::create("uncaught\n"))
    io::out("unreachable\n")
    return 0
```

---

## 010 — try: any type can be thrown; a catch takes the type it names

```ura
// stdout: code 42
// stdout: after
// expect: 0
use "@/core"

fn fail():
    throw 42

fn main() i32:
    try:
        fail()
    catch e Error:
        io::out($"error {e.message}\n")
    catch code i32:
        io::out($"code {code}\n")
    io::out("after\n")
    return 0
```

---

## 011 — try: catch without a matching try is a parse error

```ura
// error: 'catch' without a matching 'try'
use "@/core"
fn main() i32:
    catch e Error:
        io::out("x\n")
    return 0
```

---

## 012 — try: code after a throw is unreachable and dropped

```ura
// stdout: caught: x
// expect: 0
use "@/core"

fn boom():
    throw Error::create(String::create("x"))
    io::out("dead\n")

fn main() i32:
    try:
        boom()
    catch e Error:
        io::out($"caught: {e.message}\n")
    return 0
```
