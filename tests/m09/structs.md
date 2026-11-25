# m09

## index

- 028 — struct: pub inside struct must be followed by fn
- 029 — struct: instance method called via :: instead of .
- 030 — struct: pub fn called via . instead of ::
- 031 — struct: pub may only be used inside a struct
- 032 — operator: [] returns &i32, so b[0] reads and writes through
- 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local
- 034 — operator: copying a struct with drop needs operator =
- 035 — operator: passing a struct with drop by value
- 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win
- 037 — struct: from a struct that doesn't exist
- 038 — struct: String from Array<char> in "@/posix/string", temporaries are dropped
- 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

---

## 028 — struct: pub inside struct must be followed by fn

```ura
// error: Expected 'fn' after 'pub'
use "@/core"
use "@/posix"

struct Counter:
    n i32

    pub n i32

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 029 — struct: instance method called via :: instead of .

```ura
// error: 'Counter::bump' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
use "@/core"
use "@/posix"

struct Counter:
    n i32

    fn bump():
        self.n += 1

fn main() i32:
    c Counter
    c.n = 0
    Counter::bump()
    return 0
```

---

## 030 — struct: pub fn called via . instead of ::

```ura
// error: 'm.make' is a 'pub fn', so it is static and takes no receiver; call it as 'Maker::create()'
use "@/core"
use "@/posix"

struct Maker:
    n i32

    pub fn create() Maker:
        m Maker
        m.n = 1
        return m

fn main() i32:
    m Maker
    m.n = 0
    m.make()
    return 0
```

---

## 031 — struct: pub may only be used inside a struct

```ura
// error: 'pub' marks a static method; it may only be used inside a struct
use "@/core"
use "@/posix"

pub fn helper():
    return

fn main() i32:
    helper()
    return 0
```

---

## 032 — operator: [] returns &i32, so b[0] reads and writes through

```ura
use "@/core"
proto malloc(size i32) i32[]

struct Bag:
    data i32[]


operator [](left &Bag, index i32) &i32:
    return &left.data[index]

fn main() i32:
    b Bag
    b.data = malloc(40)
    b[0] = 5
    b[1] = b[0] + 2
    b[1] += 1
    io::out($"{b[0]} {b[1]}\n")
    return b[1]
```

---

## 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local

```ura
use "@/core"
struct Noisy:
    id i32


operator drop(value &Noisy):
    io::out($"drop {value.id}\n")

fn make(id i32) Noisy:
    n Noisy
    n.id = id
    return n

fn main() i32:
    a Noisy = make(1)
    i i32 = 0
    while i < 3:
        b Noisy
        b.id = 10 + i
        i += 1
        if i == 2:
            continue
        io::out($"loop {i}\n")
    if a.id == 1:
        c Noisy
        c.id = 5
    x Array<i32>
    x.append(4)
    y Array<i32> = x
    y.append(5)
    io::out($"{x.size()} {y.size()} {y[1]}\n")
    return 0
```

---

## 034 — operator: copying a struct with drop needs operator =

```ura
use "@/core"
struct Buffer:
    size i32


operator drop(value &Buffer):
    io::out("drop\n")

fn main() i32:
    a Buffer
    b Buffer = a
    return 0
```

---

## 035 — operator: passing a struct with drop by value

```ura
use "@/core"
struct Buffer:
    size i32


operator drop(value &Buffer):
    io::out("drop\n")

fn take(b Buffer) i32:
    return b.size

fn main() i32:
    a Buffer
    return take(a)
```

---

## 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win

```ura
use "@/core"
struct Named:
    label i8[]

    fn name() i8[]:
        return self.label

struct Text from Array<char> and Named:
    words i32

    fn push_back(c char):
        if self.count == self.cap:
            self.reserve(self.cap * 2 + 4)
        self.data[self.count] = c
        self.count += 1
        if c == ' ':
            self.words += 1

    fn shout() Text:
        res Text
        i i32 = 0
        while i < self.size():
            c char = self[i]
            if c >= 'a' and c <= 'z':
                c = (c as i32 - 32) as char
            res.append(c)
            i += 1
        return res

struct Title from Text:
    fn first() char:
        return self[0]

fn main() i32:
    t Text
    t.label = "greeting"
    t.append('h')
    t.append('i')
    t.append(' ')
    t.append('u')
    loud Text = t.shout()
    title Title
    title.append('z')
    io::out($"{t.size()} {t.words} {loud[0]}{loud[1]}{loud[3]} {t.name()} {title.first()} {typeof(loud)}\n")
    return 0
```

---

## 037 — struct: from a struct that doesn't exist

```ura
use "@/core"
struct Text from Nothing:
    size i32

fn main() i32:
    return 0
```

---

## 038 — struct: String from Array<char> in "@/posix/string", temporaries are dropped

```ura
use "@/core"
use "@/posix"

fn main() i32:
    s String = "  Hello World  "
    t String = s.trim()
    io::out($"[{t.chars()}] {t.size()}\n")
    io::out($"{t.upper().chars()} {t.lower().chars()} {t.reverse().chars()}\n")
    io::out($"{t.find("World")} {t.contains("lo W")} {t.starts_with("Hell")} {t.ends_with("ld")} {t.find("xyz")}\n")
    r String = t.replace("World", "ura")
    io::out($"{r.chars()} {r.substr(1, 3).chars()} {r.repeat(2).chars()}\n")
    a String = String::create("abc")
    b String = "abd"
    c String = a + "-" + b
    c += "!"
    io::out($"{c.chars()} {a < b} {a == "abc"} {a != b} {c.size()}\n")
    n String = "42"
    io::out($"{n.to_int() + 1} {typeof(n)}\n")
    return 0
```

---

## 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

```ura
use "@/core"
struct Plain:
    id i32

    fn drop():
        io::out($"plain drop {self.id}\n")

struct Owned:
    id i32


operator drop(value &Owned):
    io::out($"owned drop {value.id}\n")

fn main() i32:
    p Plain
    p.id = 1
    o Owned
    o.id = 2
    p.drop()
    return 0
```
