# m03

## index

- 001 — global scope: no main: the file is the program
- 002 — struct: declare: attributes start at zero
- 003 — struct: write an attribute, read it back
- 004 — struct: attributes don't overlap
- 005 — struct: compound assignment on an attribute
- 006 — struct: two variables of one struct are separate
- 007 — struct: attributes of different types
- 008 — struct: nested struct: r.b.x
- 009 — struct: attributes in a loop
- 010 — struct: array attribute
- 011 — struct: print attributes
- 012 — struct: a function returns a struct
- 013 — struct: a ref variable to a struct
- 014 — struct: write through a ref parameter
- 015 — struct: read through a ref parameter
- 016 — method: a method reads self
- 017 — method: a method writes self
- 018 — method: a method reads and writes self
- 019 — method: a method with arguments
- 020 — method: a method calls another method
- 021 — method: a method returns a bool
- 022 — method: a method in a loop
- 023 — method: two structs, one method name
- 024 — method: a method on a nested attribute
- 025 — method: overloaded method
- 026 — method: a method on a ref receiver
- 027 — method: a method through a ref changes the caller

---

## 001 — global scope: no main: the file is the program

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
use "@/core"
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

f Finder
f.data = [4, 8, 15, 16, 23, 42]
f.size = 6
return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

---

## 002 — struct: declare: attributes start at zero

```ura
// expect: 0
use "@/core"
struct User:
    a i32
    b i32

fn main() i32:
    u User
    return u.a + u.b
```

---

## 003 — struct: write an attribute, read it back

```ura
// expect: 7
use "@/core"
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.b = 7
    return u.b
```

---

## 004 — struct: attributes don't overlap

```ura
// expect: 34
use "@/core"
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.a = 3
    u.b = 4
    return u.a * 10 + u.b
```

---

## 005 — struct: compound assignment on an attribute

```ura
// expect: 21   ((5 + 2) * 3)
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    u.a = 5
    u.a += 2
    u.a *= 3
    return u.a
```

---

## 006 — struct: two variables of one struct are separate

```ura
// expect: 12
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    v User
    u.a = 1
    v.a = 2
    return u.a * 10 + v.a
```

---

## 007 — struct: attributes of different types

```ura
// expect: 4
use "@/core"
struct Item:
    id i32
    used bool
    tag char

fn main() i32:
    it Item
    it.id = 4
    it.used = True
    it.tag = 'x'
    if it.used and it.tag == 'x':
        return it.id
    return 0
```

---

## 008 — struct: nested struct: r.b.x

```ura
// expect: 6   (5 - 1 + 2)
use "@/core"
struct Point:
    x i32
    y i32

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.a.x = 1
    r.b.x = 5
    r.b.y = 2
    return r.b.x - r.a.x + r.b.y
```

---

## 009 — struct: attributes in a loop

```ura
// expect: 10   (1 + 2 + 3 + 4)
use "@/core"
struct Counter:
    n i32
    total i32

fn main() i32:
    c Counter
    while c.n < 4:
        c.n += 1
        c.total += c.n
    return c.total
```

---

## 010 — struct: array attribute

```ura
// expect: 12
use "@/core"
struct Row:
    cells i32[]

fn main() i32:
    r Row
    r.cells = [1, 2, 3]
    r.cells[0] = 9
    return r.cells[0] + r.cells[2]
```

---

## 011 — struct: print attributes

```ura
// stdout: ura 42
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

struct User:
    id i32
    name i8[]

fn main() i32:
    u User
    u.id = 42
    u.name = "ura"
    printf("%s %d\n", u.name, u.id)
    return 0
```

---

## 012 — struct: a function returns a struct

```ura
// expect: 5
use "@/core"
struct User:
    a i32

fn make(a i32) User:
    u User
    u.a = a
    return u

fn main() i32:
    u User = make(5)
    return u.a
```

---

## 013 — struct: a ref variable to a struct

```ura
// expect: 8
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    p &User = &u
    p.a = 8
    return u.a
```

---

## 014 — struct: write through a ref parameter

```ura
// expect: 3
use "@/core"
struct User:
    a i32

fn set(p &User):
    p.a = 3

fn main() i32:
    u User
    set(&u)
    return u.a
```

---

## 015 — struct: read through a ref parameter

```ura
// expect: 7
use "@/core"
struct User:
    a i32
    b i32

fn total(p &User) i32:
    return p.a + p.b

fn main() i32:
    u User
    u.a = 2
    u.b = 5
    return total(&u)
```

---

## 016 — method: a method reads self

```ura
// expect: 4
use "@/core"
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    u.a = 4
    return u.get()
```

---

## 017 — method: a method writes self

```ura
// expect: 9
use "@/core"
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn main() i32:
    u User
    u.set(9)
    return u.a
```

---

## 018 — method: a method reads and writes self

```ura
// expect: 7
use "@/core"
struct User:
    a i32

    fn bump() i32:
        self.a += 1
        return self.a

fn main() i32:
    u User
    u.a = 5
    u.bump()
    return u.bump()
```

---

## 019 — method: a method with arguments

```ura
// expect: 8
use "@/core"
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    u.a = 5
    return u.add(3)
```

---

## 020 — method: a method calls another method

```ura
// expect: 12
use "@/core"
struct User:
    a i32

    fn get() i32:
        return self.a

    fn twice() i32:
        return self.get() * 2

fn main() i32:
    u User
    u.a = 6
    return u.twice()
```

---

## 021 — method: a method returns a bool

```ura
// expect: 1
use "@/core"
struct User:
    age i32

    fn adult() bool:
        return self.age >= 18

fn main() i32:
    u User
    u.age = 20
    if u.adult():
        return 1
    return 0
```

---

## 022 — method: a method in a loop

```ura
// stdout: 3
// expect: 3
use "@/core"
proto printf(fmt i8[], ...) i32

struct Counter:
    n i32

    fn tick():
        self.n += 1

fn main() i32:
    c Counter
    i i32 = 0
    while i < 3:
        c.tick()
        i += 1
    printf("%d\n", c.n)
    return c.n
```

---

## 023 — method: two structs, one method name

```ura
// expect: 21   (1 + 2 * 10)
use "@/core"
struct A:
    v i32

    fn get() i32:
        return self.v

struct B:
    v i32

    fn get() i32:
        return self.v * 10

fn main() i32:
    a A
    b B
    a.v = 1
    b.v = 2
    return a.get() + b.get()
```

---

## 024 — method: a method on a nested attribute

```ura
// expect: 7
use "@/core"
struct Point:
    x i32

    fn get() i32:
        return self.x

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.b.x = 7
    return r.b.get()
```

---

## 025 — method: overloaded method

```ura
// expect: 6   (1 + 5)
use "@/core"
struct User:
    a i32

    fn set(n i32):
        self.a = n

    fn set():
        self.a = 1

fn main() i32:
    u User
    u.set()
    first i32 = u.a
    u.set(5)
    return first + u.a
```

---

## 026 — method: a method on a ref receiver

```ura
// expect: 4
use "@/core"
struct User:
    a i32

    fn get() i32:
        return self.a

fn via(p &User) i32:
    return p.get()

fn main() i32:
    u User
    u.a = 4
    return via(&u)
```

---

## 027 — method: a method through a ref changes the caller

```ura
// expect: 11
use "@/core"
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn reset(p &User):
    p.set(11)

fn main() i32:
    u User
    u.a = 3
    reset(&u)
    return u.a
```
