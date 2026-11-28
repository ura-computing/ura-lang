# m06

## index

- 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse
- 020 — string: operators: + += == != < > against String and against a literal
- 021 — string: to_int, compare and chained transformations

---

## 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse

```ura
// stdout: HELLO WORLD
// stdout: hello world
// stdout: World
// stdout: Hello
// stdout: Hello World
// stdout: 0
// stdout: hello
// stdout: hello
// stdout: 0
// stdout: qux bar qux baz
// stdout: foo bar foo baz
// stdout: foo bar foo baz
// stdout: ababab
// stdout: 0
// stdout: edcba
// stdout: a
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s String = String::create("Hello World")

    u String = s.upper()
    io::out($"{u}\n")

    l String = s.lower()
    io::out($"{l}\n")

    sub String = s.substr(6, 5)
    io::out($"{sub}\n")

    sub2 String = s.substr(0, 5)
    io::out($"{sub2}\n")

    sub3 String = s.substr(0, 100)
    io::out($"{sub3}\n")

    sub4 String = s.substr(-1, 5)
    io::out($"{sub4.size()}\n")

    t1 String = String::create("   hello   ")
    t2 String = t1.trim()
    io::out($"{t2}\n")

    t3 String = String::create("hello")
    t4 String = t3.trim()
    io::out($"{t4}\n")

    t5 String = String::create("   ")
    t6 String = t5.trim()
    io::out($"{t6.size()}\n")

    r1 String = String::create("foo bar foo baz")
    r2 String = r1.replace("foo", "qux")
    io::out($"{r2}\n")

    r3 String = r1.replace("xyz", "qux")
    io::out($"{r3}\n")

    r4 String = r1.replace("", "qux")
    io::out($"{r4}\n")

    rep String = String::create("ab")
    rep2 String = rep.repeat(3)
    io::out($"{rep2}\n")

    rep3 String = rep.repeat(0)
    io::out($"{rep3.size()}\n")

    rev String = String::create("abcde")
    rev2 String = rev.reverse()
    io::out($"{rev2}\n")

    rev3 String = String::create("a")
    rev4 String = rev3.reverse()
    io::out($"{rev4}\n")
    return 0
```

---

## 020 — string: operators: + += == != < > against String and against a literal

```ura
// stdout: hello world
// stdout: hello there
// stdout: hello world
// stdout: testing
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: abc True
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    a String = String::create("hello")
    b String = String::create(" world")
    c String = a + b
    io::out($"{c}\n")

    d String = a + " there"
    io::out($"{d}\n")

    a += b
    io::out($"{a}\n")

    e String = String::create("test")
    e += "ing"
    io::out($"{e}\n")

    x String = String::create("abc")
    y String = String::create("abc")
    z String = String::create("xyz")

    io::out($"{x == y}\n")
    io::out($"{x == z}\n")
    io::out($"{x != z}\n")
    io::out($"{x != y}\n")
    io::out($"{x < z}\n")
    io::out($"{z < x}\n")
    io::out($"{z > x}\n")
    io::out($"{x > z}\n")

    io::out($"{x == "abc"}\n")
    io::out($"{x == "xyz"}\n")
    io::out($"{x != "xyz"}\n")
    io::out($"{x != "abc"}\n")

    f String = String::create("")
    g String = String::create("")
    f = y
    io::out($"{f} {f == y}\n")
    return 0
```

---

## 021 — string: to_int, compare and chained transformations

```ura
// stdout: 12345
// stdout: -99
// stdout: 0
// stdout: -1
// stdout: 1
// stdout: 0
// stdout: DLROW OLLEH
// stdout: 24
// stdout: 1000000
// stdout: -2000000
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s1 String = String::create("12345")
    io::out($"{s1.to_int()}\n")

    s2 String = String::create("-99")
    io::out($"{s2.to_int()}\n")

    s3 String = String::create("0")
    io::out($"{s3.to_int()}\n")

    a String = String::create("apple")
    b String = String::create("banana")
    io::out($"{a.compare(&b)}\n")
    io::out($"{b.compare(&a)}\n")
    io::out($"{a.compare(&a)}\n")

    s String = String::create("Hello World")
    r String = s.upper()
    r2 String = r.reverse()
    io::out($"{r2}\n")

    big String = String::create("aaaa")
    i i32 = 0
    while i < 5:
        big.join("bbbb")
        i += 1
    io::out($"{big.size()}\n")

    n1 String = String::create(1000000)
    io::out($"{n1}\n")

    n2 String = String::create(-2000000)
    io::out($"{n2}\n")
    return 0
```
