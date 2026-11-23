# m09

## index

- 001 — method: a pub fn is static: no self, called with ::
- 002 — method: calling a non-pub method with ::
- 003 — method: :: on a type that does not exist
- 004 — method: two structs declare the same pub fn, and statics compose
- 005 — operator: an operator inside a struct is an error
- 006 — operator: operator missing operator symbol
- 007 — operator: adding two structs without an operator + overload
- 008 — operator: operator + on two structs
- 009 — operator: two overloads of + with different right-hand types
- 010 — operator: operator == returning bool
- 011 — operator: no overload for that operator
- 012 — operator: assigning a different struct
- 013 — operator: indexing an array literal directly
- 014 — operator: operators and a method on the same struct
- 015 — operator: assigning a void result
- 016 — operator: a binary operator with mismatched scalar types
- 017 — operator: operator = copies on declaration and on reassignment
- 018 — operator: compound assignment operators
- 019 — operator: a compound operator with no overload
- 020 — operator: return unwinds every scope, and keeps the returned local
- 021 — operator: every operator on one struct, and = from an i32 overload
- 022 — operator: a temporary is destroyed at the end of its statement
- 023 — operator: every overloadable operator on one struct
- 024 — operator: bitwise operator overloads, and one returning a scalar
- 025 — operator: compound bitwise assignment overloads
- 026 — operator: bitwise and shift overloads taking a ref parameter

---

## 001 — method: a pub fn is static: no self, called with ::

```ura
// stdout: floor = 7
// expect: 0
use "@/core"

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

fn main() i32:
    r Room = Room::create(7)
    io::out($"floor = {r.floor}\n")
    return 0
```

---

## 002 — method: calling a non-pub method with ::

```ura
// error: 'Room::depth' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
use "@/core"

struct Room:
    floor i32

    fn depth() i32:
        return self.floor

fn main() i32:
    io::out($"{Room::depth()}\n")
    return 0
```

---

## 003 — method: :: on a type that does not exist

```ura
// error: Unknown type 'Nope'
use "@/core"

fn main() i32:
    io::out($"{Nope::create()}\n")
    return 0
```

---

## 004 — method: two structs declare the same pub fn, and statics compose

```ura
// stdout: room 3 tower 9
// stdout: zero 0
// stdout: level 40
// stdout: method 5
// stdout: mixed 3
// expect: 0
use "@/core"

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

    pub fn zero() Room:
        return Room::create(0)

    pub fn level(f i32) i32:
        return f * 10

    fn depth() i32:
        return self.floor

struct Tower:
    height i32

    pub fn create(h i32) Tower:
        t Tower
        t.height = h
        return t

fn main() i32:
    a Room  = Room::create(3)
    b Tower = Tower::create(9)
    io::out($"room {a.floor} tower {b.height}\n")
    io::out($"zero {Room::zero().floor}\n")
    io::out($"level {Room::level(4)}\n")
    io::out($"method {Room::create(5).depth()}\n")
    n i32 = Room::create(2).depth() + Tower::create(1).height
    io::out($"mixed {n}\n")
    return 0
```

---

## 005 — operator: an operator inside a struct is an error

```ura
// error: 'operator' must be declared outside the struct
use "@/core"

struct Vec:
    x i32
    operator +(other &Vec) Vec:
        return other

fn main() i32:
    return 0
```

---

## 006 — operator: operator missing operator symbol

```ura
// error: Expected an operator after 'operator' (one of + - * / % & | ^ << >> == != < > <= >= = += -= *= /= %= &= |= ^= <<= >>= drop)
use "@/core"
use "@/posix"

struct Vec:
    x i32


operator (left &Vec) Vec:
    return left

fn main() i32:
    v Vec
    v.x = 1
    return 0
```

---

## 007 — operator: adding two structs without an operator + overload

```ura
// error: Struct Vec has no 'operator +' taking Vec; declare one inside the struct
use "@/core"
use "@/posix"

struct Vec:
    x i32
    y i32

fn main() i32:
    a Vec
    b Vec
    c Vec = a + b
    io::out($"{c.x}\n")
    return 0
```

---

## 008 — operator: operator + on two structs

```ura
// stdout: 11 22
// expect: 0
use "@/core"

struct Vec:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v


operator +(left &Vec, other Vec) Vec:
    return Vec::create(left.x + other.x, left.y + other.y)

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)
    c Vec = a + b
    io::out($"{c.x} {c.y}\n")
    return 0
```

---

## 009 — operator: two overloads of + with different right-hand types

```ura
// stdout: 11 6
// expect: 0
use "@/core"

struct Vec:
    x i32

    pub fn create(a i32) Vec:
        v Vec
        v.x = a
        return v


operator +(left &Vec, other Vec) Vec:
    return Vec::create(left.x + other.x)

operator +(left &Vec, n i32) Vec:
    return Vec::create(left.x + n)

fn main() i32:
    a Vec = Vec::create(1)
    b Vec = Vec::create(10)
    io::out($"{(a + b).x} {(a + 5).x}\n")
    return 0
```

---

## 010 — operator: operator == returning bool

```ura
// stdout: True False
// expect: 0
use "@/core"

struct Vec:
    x i32

    pub fn create(a i32) Vec:
        v Vec
        v.x = a
        return v


operator ==(left &Vec, other Vec) bool:
    return left.x == other.x

fn main() i32:
    a Vec = Vec::create(3)
    b Vec = Vec::create(3)
    c Vec = Vec::create(9)
    io::out($"{a == b} {a == c}\n")
    return 0
```

---

## 011 — operator: no overload for that operator

```ura
// error: Struct Vec has no 'operator -' taking Vec; declare one inside the struct
use "@/core"

struct Vec:
    x i32

fn main() i32:
    a Vec
    b Vec
    c Vec = a - b
    return 0
```

---

## 012 — operator: assigning a different struct

```ura
// error: Cannot assign Point to Vec; they are different structs
use "@/core"

struct Vec:
    x i32

struct Point:
    x i32

fn main() i32:
    v Vec
    p Point
    v = p
    return 0
```

---

## 013 — operator: indexing an array literal directly

```ura
// stdout: 20
// expect: 0
use "@/core"

fn main() i32:
    io::out($"{[10, 20, 30][1]}\n")
    return 0
```

---

## 014 — operator: operators and a method on the same struct

```ura
// stdout: (11, 22)
// stdout: a == a  True
// stdout: a == b  False
// stdout: (11, 22)
// expect: 0
use "@/core"

struct Vec:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    fn show():
        io::out($"({self.x}, {self.y})\n")


operator +(left &Vec, other Vec) Vec:
    return Vec::create(left.x + other.x, left.y + other.y)

operator ==(left &Vec, other Vec) bool:
    return left.x == other.x and left.y == other.y

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the same variable on both sides
    io::out($"a == a  {a == a}\n")
    io::out($"a == b  {a == b}\n")

    // an operator result used directly as a receiver
    (a + b).show()
    return 0
```

---

## 015 — operator: assigning a void result

```ura
// error: Cannot assign void to Vec
use "@/core"

struct Vec:
    x i32

    fn show():
        io::out($"{self.x}\n")

fn main() i32:
    a Vec
    e Vec = a.show()
    return 0
```

---

## 016 — operator: a binary operator with mismatched scalar types

```ura
// error: Cannot use '+' with i32 and array
use "@/core"

fn main() i32:
    n i32 = 1
    s char[] = "hi"
    io::out($"{n + s}\n")
    return 0
```

---

## 017 — operator: operator = copies on declaration and on reassignment

```ura
// stdout: decl   5 99
// stdout: assign 5 99
// expect: 0
use "@/core"

struct Vec:
    x i32
    tag i32


operator =(left &Vec, other &Vec) void:
    left.x   = other.x
    left.tag = 99

fn main() i32:
    a Vec
    a.x = 5

    // declaration: copies through operator =, tag becomes 99
    b Vec = a
    io::out($"decl   {b.x} {b.tag}\n")

    // reassignment: calls operator =, tag becomes 99
    c Vec
    c = a
    io::out($"assign {c.x} {c.tag}\n")
    return 0
```

---

## 018 — operator: compound assignment operators

```ura
// stdout: after += 14
// stdout: after -= 11
// expect: 0
use "@/core"

struct Vec:
    x i32


operator +=(left &Vec, other Vec) void:
    left.x = left.x + other.x

operator -=(left &Vec, n i32) void:
    left.x = left.x - n

fn main() i32:
    a Vec
    a.x = 10
    b Vec
    b.x = 4

    a += b
    io::out($"after += {a.x}\n")

    a -= 3
    io::out($"after -= {a.x}\n")
    return 0
```

---

## 019 — operator: a compound operator with no overload

```ura
// error: Struct Vec has no 'operator *=' taking Vec; declare one inside the struct
use "@/core"

struct Vec:
    x i32

fn main() i32:
    a Vec
    b Vec
    a *= b
    return 0
```

---

## 020 — operator: return unwinds every scope, and keeps the returned local

```ura
// stdout: drop 10
// stdout: drop 1
// stdout: early 7
// stdout: g.id 99
// stdout: drop 99
// expect: 0
use "@/core"

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t


operator =(left &Tag, other &Tag) void:
    left.id = other.id

operator drop(value &Tag):
    io::out($"drop {value.id}\n")

fn early() i32:
    a Tag = Tag::create(1)
    if 1 < 2:
        b Tag = Tag::create(10)
        return 7
    return 0

fn giveback() Tag:
    t Tag = Tag::create(99)
    return t

fn main() i32:
    r i32 = early()
    io::out($"early {r}\n")
    g Tag = giveback()
    io::out($"g.id {g.id}\n")
    return 0
```

---

## 021 — operator: every operator on one struct, and = from an i32 overload

```ura
// stdout: (11, 22) tag 0
// stdout: (6, 7) tag 0
// stdout: a == a  True
// stdout: a == b  False
// stdout: (11, 22) tag 0
// stdout: (1, 2) tag 99
// stdout: (1, 2) tag 99
// stdout: (20, 30) tag 0
// stdout: (17, 27) tag 0
// expect: 0
use "@/core"

struct Vec:
    x i32
    y i32
    tag i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    fn show():
        io::out($"({self.x}, {self.y}) tag {self.tag}\n")


operator +(left &Vec, other Vec) Vec:
    return Vec::create(left.x + other.x, left.y + other.y)

operator +(left &Vec, n i32) Vec:
    return Vec::create(left.x + n, left.y + n)

operator ==(left &Vec, other Vec) bool:
    return left.x == other.x and left.y == other.y

operator =(left &Vec, other &Vec) void:
    left.x   = other.x
    left.y   = other.y
    left.tag = 99

operator +=(left &Vec, other Vec) void:
    left.x = left.x + other.x
    left.y = left.y + other.y

operator -=(left &Vec, n i32) void:
    left.x = left.x - n
    left.y = left.y - n

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the i32 overload's result as a declaration initialiser
    d Vec = a + 5
    d.show()

    io::out($"a == a  {a == a}\n")
    io::out($"a == b  {a == b}\n")

    (a + b).show()

    // a declaration copies through operator =, tag becomes 99
    e Vec = a
    e.show()

    // reassignment does call operator =, so tag becomes 99
    f Vec
    f = a
    f.show()

    g Vec = Vec::create(10, 10)
    g += b
    g.show()
    g -= 3
    g.show()
    return 0
```

---

## 022 — operator: a temporary is destroyed at the end of its statement

```ura
// stdout: show 3
// stdout: DROP 3
// stdout: --
// stdout: 3
// stdout: DROP 3
// stdout: --
// stdout: show 4
// stdout: DROP 4
// stdout: DROP 3
// stdout: --
// stdout: show 3
// stdout: DROP 3
// stdout: show 3
// stdout: DROP 3
// stdout: --
// stdout: DROP 3
// stdout: 3
// stdout: --
// stdout: 9
// stdout: DROP 9
// stdout: --
// stdout: 7
// stdout: end
// stdout: DROP 2
// stdout: DROP 1
// expect: 0
use "@/core"

struct Plain:
    x i32

    pub fn create(n i32) Plain:
        p Plain
        p.x = n
        return p

struct R:
    id i32

    pub fn create(n i32) R:
        r R
        r.id = n
        return r

    fn show():
        io::out($"show {self.id}\n")


operator =(left &R, other &R) void:
    left.id = other.id

operator +(left &R, other R) R:
    return R::create(left.id + other.id)

operator drop(value &R):
    io::out($"DROP {value.id}\n")

fn via_return(a R, b R) i32:
    return (a + b).id

fn main() i32:
    a R = R::create(1)
    b R = R::create(2)
    (a + b).show()
    io::out("--\n")
    io::out($"{(a + b).id}\n")
    io::out("--\n")
    // a chain makes two temporaries; both die, in reverse
    (a + b + a).show()
    io::out("--\n")
    i i32 = 0
    while i < 2:
        (a + b).show()
        i += 1
    io::out("--\n")
    // dropped before the ret, not after it
    io::out($"{via_return(a, b)}\n")
    io::out("--\n")
    io::out($"{R::create(9).id}\n")
    io::out("--\n")
    // a struct with no destructor is never recorded as a temporary
    io::out($"{Plain::create(7).x}\n")
    io::out("end\n")
    return 0
```

---

## 023 — operator: every overloadable operator on one struct

```ura
// stdout: a + b   (11, 15)
// stdout: a - b   (7, 9)
// stdout: a * 3   (27, 36)
// stdout: a / 2   (4, 6)
// stdout: a mod 4 (1, 0)
// stdout: == False  != True
// stdout: <  False  >  True
// stdout: <= False  >= True
// stdout: c = a    (9, 12)
// stdout: c += b   (11, 15)
// stdout: c -= b   (9, 12)
// stdout: c *= 2   (18, 24)
// stdout: c /= 3   (6, 8)
// stdout: c %= 5   (1, 3)
// stdout: a is longer
// stdout: steps to pass a: 4
// expect: 0
use "@/core"

struct Vec2:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec2:
        v Vec2
        v.x = a
        v.y = b
        return v

    fn taxicab() i32:
        return self.abs(self.x) + self.abs(self.y)

    fn abs(n i32) i32:
        if n < 0:
            return 0 - n
        return n

    fn show(label char[]):
        io::out($"{label}({self.x}, {self.y})\n")


operator +(left &Vec2, other Vec2) Vec2:
    return Vec2::create(left.x + other.x, left.y + other.y)

operator -(left &Vec2, other Vec2) Vec2:
    return Vec2::create(left.x - other.x, left.y - other.y)

operator *(left &Vec2, n i32) Vec2:
    return Vec2::create(left.x * n, left.y * n)

operator /(left &Vec2, n i32) Vec2:
    return Vec2::create(left.x / n, left.y / n)

operator %(left &Vec2, n i32) Vec2:
    return Vec2::create(left.x % n, left.y % n)

operator ==(left &Vec2, other Vec2) bool:
    return left.x == other.x and left.y == other.y

operator !=(left &Vec2, other Vec2) bool:
    return left.x != other.x or left.y != other.y

operator <(left &Vec2, other Vec2) bool:
    return left.taxicab() < other.taxicab()

operator >(left &Vec2, other Vec2) bool:
    return left.taxicab() > other.taxicab()

operator <=(left &Vec2, other Vec2) bool:
    return left.taxicab() <= other.taxicab()

operator >=(left &Vec2, other Vec2) bool:
    return left.taxicab() >= other.taxicab()

operator =(left &Vec2, other &Vec2) void:
    left.x = other.x
    left.y = other.y

operator +=(left &Vec2, other Vec2) void:
    left.x = left.x + other.x
    left.y = left.y + other.y

operator -=(left &Vec2, other Vec2) void:
    left.x = left.x - other.x
    left.y = left.y - other.y

operator *=(left &Vec2, n i32) void:
    left.x = left.x * n
    left.y = left.y * n

operator /=(left &Vec2, n i32) void:
    left.x = left.x / n
    left.y = left.y / n

operator %=(left &Vec2, n i32) void:
    left.x = left.x % n
    left.y = left.y % n

fn main() i32:
    a Vec2 = Vec2::create(9, 12)
    b Vec2 = Vec2::create(2, 3)

    (a + b).show("a + b   ")
    (a - b).show("a - b   ")
    (a * 3).show("a * 3   ")
    (a / 2).show("a / 2   ")
    (a % 4).show("a mod 4 ")

    io::out($"== {a == b}  != {a != b}\n")
    io::out($"<  {a < b}  >  {a > b}\n")
    io::out($"<= {a <= b}  >= {a >= b}\n")

    c Vec2 = Vec2::create(0, 0)
    c = a
    c.show("c = a    ")
    c += b
    c.show("c += b   ")
    c -= b
    c.show("c -= b   ")
    c *= 2
    c.show("c *= 2   ")
    c /= 3
    c.show("c /= 3   ")
    c %= 5
    c.show("c %= 5   ")

    // an overloaded comparison driving a branch and a loop
    if a > b:
        io::out("a is longer\n")
    n i32 = 0
    d Vec2 = Vec2::create(1, 1)
    while d < a:
        d += b
        n += 1
    io::out($"steps to pass a: {n}\n")
    return 0
```

---

## 024 — operator: bitwise operator overloads, and one returning a scalar

```ura
// stdout: and 8  or 14  xor 6
// stdout: shl 48  shr 3
// stdout: dot 23
// expect: 0
use "@/core"

struct Flags:
    bits i32

    pub fn create(n i32) Flags:
        f Flags
        f.bits = n
        return f


operator &(left &Flags, other Flags) Flags:
    return Flags::create(left.bits & other.bits)

operator |(left &Flags, other Flags) Flags:
    return Flags::create(left.bits | other.bits)

operator ^(left &Flags, other Flags) Flags:
    return Flags::create(left.bits ^ other.bits)

operator <<(left &Flags, n i32) Flags:
    return Flags::create(left.bits << n)

operator >>(left &Flags, n i32) Flags:
    return Flags::create(left.bits >> n)

struct Vec2:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec2:
        v Vec2
        v.x = a
        v.y = b
        return v

    // an overload may return something other than the struct

operator *(left &Vec2, other Vec2) i32:
    return left.x * other.x + left.y * other.y

fn main() i32:
    a Flags = Flags::create(12)
    b Flags = Flags::create(10)
    io::out($"and {(a & b).bits}  or {(a | b).bits}")
    io::out($"  xor {(a ^ b).bits}\n")
    io::out($"shl {(a << 2).bits}  shr {(a >> 2).bits}\n")

    p Vec2 = Vec2::create(2, 3)
    q Vec2 = Vec2::create(4, 5)
    io::out($"dot {p * q}\n")
    return 0
```

---

## 025 — operator: compound bitwise assignment overloads

```ura
// stdout: 8 11 14 56 28
// expect: 0
use "@/core"

struct Flags:
    bits i32


operator &=(left &Flags, n i32) void:
    left.bits = left.bits & n

operator |=(left &Flags, n i32) void:
    left.bits = left.bits | n

operator ^=(left &Flags, n i32) void:
    left.bits = left.bits ^ n

operator <<=(left &Flags, n i32) void:
    left.bits = left.bits << n

operator >>=(left &Flags, n i32) void:
    left.bits = left.bits >> n

fn main() i32:
    f Flags
    f.bits = 12
    f &= 10
    io::out($"{f.bits} ")
    f |= 3
    io::out($"{f.bits} ")
    f ^= 5
    io::out($"{f.bits} ")
    f <<= 2
    io::out($"{f.bits} ")
    f >>= 1
    io::out($"{f.bits}\n")
    return 0
```

---

## 026 — operator: bitwise and shift overloads taking a ref parameter

```ura
// stdout: a & b:  8
// stdout: a | b:  14
// stdout: a ^ b:  6
// stdout: a << 2: 48
// stdout: a >> 1: 6
// expect: 0
use "@/core"

struct Flags:
    bits i32


operator = (left &Flags, n i32) void:
    left.bits = n

operator = (left &Flags, f &Flags) void:
    left.bits = f.bits

operator & (left &Flags, f &Flags) Flags:
    res Flags
    res.bits = left.bits & f.bits
    return res

operator | (left &Flags, f &Flags) Flags:
    res Flags
    res.bits = left.bits | f.bits
    return res

operator ^ (left &Flags, f &Flags) Flags:
    res Flags
    res.bits = left.bits ^ f.bits
    return res

operator << (left &Flags, n i32) Flags:
    res Flags
    res.bits = left.bits << n
    return res

operator >> (left &Flags, n i32) Flags:
    res Flags
    res.bits = left.bits >> n
    return res

fn main() i32:
    a Flags
    a = 12

    b Flags
    b = 10

    band Flags = a & b
    io::out($"a & b:  {band.bits}\n")

    bor Flags = a | b
    io::out($"a | b:  {bor.bits}\n")

    bxor Flags = a ^ b
    io::out($"a ^ b:  {bxor.bits}\n")

    lsh Flags = a << 2
    io::out($"a << 2: {lsh.bits}\n")

    rsh Flags = a >> 1
    io::out($"a >> 1: {rsh.bits}\n")
    return 0
```
