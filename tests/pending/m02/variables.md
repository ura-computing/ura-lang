# m02

## index

- 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse
- 129 — use: a package that doesn't exist
- 130 — use: an error after a use points at the user's file and line
- 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates
- 132 — array: any integer type is an index, an unsigned one is zero-extended
- 134 — as: a struct can't be cast, operator = converts it
- 135 — ref: &T and T[] are the same pointer, each fits where the other is expected

---

## 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse

```ura
use "@/core"
fn main() i32:
    a Array<i32>
    a.append(1)
    a.append(2)
    a.append(3)
    a.insert(0, 0)
    a.insert(2, 7)
    a[0] = 10
    io::out($"{a[0]} {a[1]} {a[2]} {a[3]} {a[4]} size {a.size()}\n")
    x i32 = a.pop()
    y i32 = a.pop(0)
    z i32 = a.pop(1)
    a.last() = 9
    io::out($"{x} {y} {z} {a.first()} {a.last()} {a.len()} {a.is_empty()}\n")
    a.resize(4)
    a.reverse()
    io::out($"{a[0]} {a[1]} {a[2]} {a[3]} cap {a.capacity()}\n")
    a.clear()
    io::out($"{a.size()} {a.is_empty()}\n")
    a.release()
    c Array<i8>
    c.append('h')
    c.append('i')
    io::out($"{c[0]}{c[1]} {typeof(c)}\n")
    return 0
```

---

## 129 — use: a package that doesn't exist

```ura
use "@/core"
use "@/nothing"

fn main() i32:
    return 0
```

---

## 130 — use: an error after a use points at the user's file and line

```ura
use "@/core"
use "@/posix/string"

fn main() i32:
    a Array<i32>
    a.append("text")
    return 0
```

---

## 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates

```ura
// stdout: 2 7 5 16 -4
// stdout: 15
// stdout: 6 6 True
// stdout: 9 1
// expect: 0
use "@/core"

fn main() i32:
    a i32 = 6
    b i32 = 3
    io::out($"{a & b} {a | b} {a ^ b} {1 << 4} {-16 >> 2}\n")
    u u32 = 4294967280
    io::out($"{u >> 28}\n")
    c i32 = 1
    c <<= 3
    c |= 1
    c &= 13
    c ^= 4
    c >>= 1
    io::out($"{c} {1 + 2 << 1} {a & b == 2}\n")
    r &i32 = &a
    r = 9
    grid Array<Array<i32>>
    io::out($"{a} {grid.size() + 1}\n")
    return 0
```

---

## 132 — array: any integer type is an index, an unsigned one is zero-extended

```ura
// stdout: 30 20 10 40
// expect: 0
use "@/core"
fn main() i32:
    a i32[] = [10, 20, 30, 40]
    i u64 = 2
    j u8 = 1
    k i64 = 0
    big u8 = 3
    io::out($"{a[i]} {a[j]} {a[k]} {a[big]}\n")
    return 0
```

---

## 134 — as: a struct can't be cast, operator = converts it

```ura
// error: 'Point' has no operator 'as'
use "@/core"
struct Point:
    x i32

fn main() i32:
    p Point
    raw i8[] = p as i8[]
    return 0
```

---

## 135 — ref: &T and T[] are the same pointer, each fits where the other is expected

```ura
// stdin: hi
// stdout: hi
// stdout: 7
// expect: 0
use "@/core"

proto read(fd i32, text i8[], count i64) i64

fn first(values &i32) i32:
    return values

fn main() i32:
    c char
    line String
    while read(0, &c, 1) == 1 and c != '\n':
        line.append(c)
    io::out($"{line}\n")
    numbers i32[] = [7, 8]
    io::out($"{first(numbers)}\n")
    return 0
```
