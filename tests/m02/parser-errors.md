# m02

## index

- 045 — parser error: unclosed [ in an array literal
- 046 — refs: ref to a ref
- 047 — refs: missing type after &
- 048 — refs: initialized with a value
- 049 — refs: address of a literal
- 050 — refs: address of an expression
- 051 — refs: address of a call
- 052 — refs: address of an address
- 053 — refs: ref to a different type
- 054 — null: assigned to a value
- 055 — null: compared with a value
- 056 — null: address of null
- 057 — expressions: compound assign on chars
- 058 — expressions: negate a string
- 059 — calls: missing & on a ref argument
- 060 — arrays: unclosed type
- 062 — arrays: index a non-array
- 063 — arrays: index with a string
- 064 — arrays: mixed element types
- 065 — arrays: array into a scalar
- 066 — arrays: unclosed literal
- 067 — arrays: missing comma between elements
- 068 — array: string as i8[] is writable
- 069 — array: array passed to a function
- 070 — ref: write through a ref
- 071 — ref: ref as parameter
- 072 — ref: read through a ref
- 073 — ref: math through a ref
- 074 — ref: copy the pointed value between refs
- 075 — ref: rebind a ref
- 076 — function: fn returns ref
- 077 — function: swap
- 078 — function: overloaded function
- 079 — program: null compared to null
- 081 — array: 1D i32 array: literal, index read, indexed write
- 082 — array: variable index and element arithmetic
- 083 — array: arrays of bool, char, f32
- 085 — array: VLA: runtime-sized stack array
- 086 — array: indexing a non-array
- 087 — array: non-integer index
- 088 — array: array literal with mixed element types
- 089 — array: array size is not an integer
- 090 — array: missing closing bracket on index
- 092 — array: unknown member
- 093 — function: single ref param: buff hero hp
- 094 — function: swap two weapon damage values via refs
- 095 — function: refs used in expression, return result
- 096 — function: compound ref assign through in a loop
- 097 — function: ref-to-value: ref passing a where value is expected
- 098 — function: ref parameter mutates the caller (counter)
- 099 — function: a void function with a ref parameter
- 100 — ref: basic ref: bind to hp, ref write through
- 101 — ref: aliasing: two refs to same hp, both see mutations
- 102 — ref: compound ref assignment through in a loop
- 103 — ref: separate refs to different vars: hp ref ref and mp
- 104 — ref: multiply and subtract through refs: f64 shield, pay mana
- 105 — ref: chain of compound mutations ref through a
- 106 — ref: reference in conditional
- 107 — ref: a ref reads correctly through every operator + as a by-value arg
- 108 — ref: ref? nullable: bound at declaration, guarded reads pass, write-through works
- 109 — ref: nested fn shares an enclosing var through a ref parameter (the sanctioned path)
- 110 — ref: a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops
- 111 — ref: a ref declared without a value starts as null
- 112 — ref: binding a reference to the wrong type
- 113 — ref: a ref parameter given a value
- 114 — ref: taking a reference to a literal
- 115 — ref: a by-value parameter given a reference
- 116 — ref: a reference to a name that does not exist
- 117 — ref: a nested function may not capture an enclosing local
- 118 — ref: linked list: build with ref, walk it with cur = ref cur.next
- 119 — ref: a ref parameter rebinds a struct's ref field in place (add-front)
- 120 — ref: ref? compared to null with == and !=, bound and unbound
- 121 — ref: chained rebind removes the head: lst.head = ref lst.head.next
- 122 — ref: = ref rebinds the pointer, = value writes through the referent
- 123 — ref: ref a == ref a is identity true, ref a == ref b is false
- 124 — ref: a value function cannot return a reference
- 125 — ref: a ref declaration cannot be bound to a plain value
- 126 — ref: a non-nullable value cannot be compared to null

---

## 045 — parser error: unclosed [ in an array literal

```ura
// error: expected ']' to close this '['
// help: use a i32[] = [1, 2]
use "@/core"
fn main() i32:
    a i32[] = [1, 2
    return a[0]
```

---

## 046 — refs: ref to a ref

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    p &&i32 = &a
    return 0
```

---

## 047 — refs: missing type after &

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    p & = &a
    return 0
```

---

## 048 — refs: initialized with a value

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    p &i32 = a
    return 0
```

---

## 049 — refs: address of a literal

```ura
use "@/core"
fn main() i32:
    p &i32 = &5
    return 0
```

---

## 050 — refs: address of an expression

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    p &i32 = &(a + 1)
    return 0
```

---

## 051 — refs: address of a call

```ura
use "@/core"
fn f() i32:
    return 1

fn main() i32:
    p &i32 = &f()
    return 0
```

---

## 052 — refs: address of an address

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    p &i32 = &&a
    return 0
```

---

## 053 — refs: ref to a different type

```ura
use "@/core"
fn main() i32:
    c i8 = 'a'
    p &i32 = &c
    return 0
```

---

## 054 — null: assigned to a value

```ura
use "@/core"
fn main() i32:
    x i32 = null
    return x
```

---

## 055 — null: compared with a value

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    b bool = a == null
    return 0
```

---

## 056 — null: address of null

```ura
use "@/core"
fn main() i32:
    p &i32 = &null
    return 0
```

---

## 057 — expressions: compound assign on chars

```ura
use "@/core"
fn main() i32:
    s i8[] = "a"
    s += 1
    return 0
```

---

## 058 — expressions: negate a string

```ura
use "@/core"
fn main() i32:
    s i8[] = -"a"
    return 0
```

---

## 059 — calls: missing & on a ref argument

```ura
use "@/core"
fn f(p &i32):
    p = 1

fn main() i32:
    a i32 = 0
    f(a)
    return a
```

---

## 060 — arrays: unclosed type

```ura
use "@/core"
fn main() i32:
    x i32[
    return 0
```

---

## 062 — arrays: index a non-array

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    return a[0]
```

---

## 063 — arrays: index with a string

```ura
use "@/core"
fn main() i32:
    x i32[] = [1, 2]
    return x["a"]
```

---

## 064 — arrays: mixed element types

```ura
use "@/core"
fn main() i32:
    x i32[] = [1, "a"]
    return 0
```

---

## 065 — arrays: array into a scalar

```ura
use "@/core"
fn main() i32:
    a i32 = [1, 2]
    return a
```

---

## 066 — arrays: unclosed literal

```ura
use "@/core"
fn main() i32:
    x i32[] = [1, 2
    return 0
```

---

## 067 — arrays: missing comma between elements

```ura
use "@/core"
fn main() i32:
    x i32[] = [1 2]
    return 0
```

---

## 068 — array: string as i8[] is writable

```ura
use "@/core"
fn main() i32:
   s i8[] = "abc"
   s[0] = 'x'
   c i8 = s[1]
   if c == 'b':
      return 1
   return 0
```

---

## 069 — array: array passed to a function

```ura
use "@/core"
fn sum(a i32[]) i32:
   total i32 = 0
   i i32 = 0
   while i < 3:
      total += a[i]
      i += 1
   return total

fn main() i32:
   values i32[] = [1, 2, 3]
   return sum(values)
```

---

## 070 — ref: write through a ref

```ura
use "@/core"
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    return a
```

---

## 071 — ref: ref as parameter

```ura
use "@/core"
fn foo(a &i32):
    a = 2

fn main() i32:
    a i32 = 10
    foo(&a)
    return a
```

---

## 072 — ref: read through a ref

```ura
use "@/core"
fn main() i32:
    a i32 = 10
    p &i32 = &a
    b i32 = p
    return b
```

---

## 073 — ref: math through a ref

```ura
use "@/core"
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = p + 5
    return a
```

---

## 074 — ref: copy the pointed value between refs

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    return a
```

---

## 075 — ref: rebind a ref

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    return b
```

---

## 076 — function: fn returns ref

```ura
use "@/core"
fn same(p &i32) &i32:
    return &p

fn main() i32:
    a i32 = 6
    q &i32 = same(&a)
    return q
```

---

## 077 — function: swap

```ura
use "@/core"
fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    return a * 10 + b
```

---

## 078 — function: overloaded function

```ura
use "@/core"
proto printf(s i8[], ...)

fn show(n i32):
   printf("int %d\n", n)

fn show(s i8[]):
   printf("str %s\n", s)

fn main() i32:
   show(42)
   show("hi")
   return 0
```

---

## 079 — program: null compared to null

```ura
use "@/core"
fn main() i32:
    if null == null:
        return 7
    return 0
```

---

## 081 — array: 1D i32 array: literal, index read, indexed write

```ura
// stdout: a[0]=10 a[1]=20 a[2]=30
// stdout: after a[1]=99: 99
// expect: 0
use "@/core"

fn main() i32:
    a i32[] = [10, 20, 30]
    io::out($"a[0]={a[0]} a[1]={a[1]} a[2]={a[2]}\n")
    a[1] = 99
    io::out($"after a[1]=99: {a[1]}\n")
    return 0
```

---

## 082 — array: variable index and element arithmetic

```ura
// stdout: a[i]=20 a[i-1]=15
// stdout: a[0]+a[3]=25
// expect: 0
use "@/core"

fn main() i32:
    a i32[] = [5, 10, 15, 20]
    i i32 = 3
    io::out($"a[i]={a[i]} a[i-1]={a[i - 1]}\n")
    io::out($"a[0]+a[3]={a[0] + a[3]}\n")
    return 0
```

---

## 083 — array: arrays of bool, char, f32

```ura
// stdout: b[0]=True c[1]=b f[2]=3.000000
// expect: 0
use "@/core"

fn main() i32:
    b bool[]  = [True, False, True]
    c char[]  = ['a', 'b', 'c']
    f f32[] = [1.5, 2.5, 3.0]
    io::out($"b[0]={b[0]} c[1]={c[1]} f[2]={f[2]}\n")
    return 0
```

---

## 085 — array: VLA: runtime-sized stack array

```ura
// stdout: a[0]=0 a[7]=100
// expect: 0
use "@/core"

fn main() i32:
    n i32 = 4
    a i32[] = i32[n * 2]
    a[7] = 100
    io::out($"a[0]={a[0]} a[7]={a[7]}\n")
    return 0
```

---

## 086 — array: indexing a non-array

```ura
// error: Cannot index 'i32', it is not an array
use "@/core"
fn main() i32:
    a i32 = 5
    io::out($"{a[0]}")
    return 0
```

---

## 087 — array: non-integer index

```ura
// error: Array index must be an integer, got f32
use "@/core"
fn main() i32:
    a i32[] = [1, 2, 3]
    io::out($"{a[1.5]}")
    return 0
```

---

## 088 — array: array literal with mixed element types

```ura
// error: Array elements must all be the same type
use "@/core"
fn main() i32:
    a i32[] = [1, True, 3]
    return 0
```

---

## 089 — array: array size is not an integer

```ura
// error: Array size must be an integer
use "@/core"
fn main() i32:
    a i32[] = i32[1.5]
    return 0
```

---

## 090 — array: missing closing bracket on index

```ura
// error: Expected ']' after array index
use "@/core"
fn main() i32:
    a i32[] = [1, 2, 3]
    output(a[0)
    return 0
```

---

## 092 — array: unknown member

```ura
// error: Unknown member '.size'
use "@/core"
fn main() i32:
    a i32[] = [1, 2, 3]
    io::out($"{a.size}")
    return 0
```

---

## 093 — function: single ref param: buff hero hp

```ura
// stdout: <100>
// expect: 0
use "@/core"

fn buff(hp &i32):
    hp = hp + 20

fn main() i32:
    hero_hp i32 = 80
    buff(&hero_hp)
    io::out($"<{hero_hp}>\n")
    return 0
```

---

## 094 — function: swap two weapon damage values via refs

```ura
// stdout: <15 30>
// expect: 0
use "@/core"

fn swap_weapons(a &i32, b &i32):
    temp i32 = a
    a = b
    b = temp

fn main() i32:
    sword  i32 = 30
    dagger i32 = 15
    swap_weapons(&sword, &dagger)
    io::out($"<{sword} {dagger}>\n")
    return 0
```

---

## 095 — function: refs used in expression, return result

```ura
// stdout: <50>
// expect: 0
use "@/core"

fn add_xp(a &i32, b &i32) i32:
    result i32 = a + b
    return result

fn main() i32:
    kill_xp  i32 = 30
    bonus_xp i32 = 20
    z        i32
    z = add_xp(&kill_xp, &bonus_xp)
    io::out($"<{z}>\n")
    return 0
```

---

## 096 — function: compound ref assign through in a loop

```ura
// stdout: <5>
// expect: 0
use "@/core"

fn tick(kills &i32):
    kills += 1

fn main() i32:
    kills i32 = 0
    i     i32 = 0
    while i < 5:
        tick(&kills)
        i = i + 1
    io::out($"<{kills}>\n")
    return 0
```

---

## 097 — function: ref-to-value: ref passing a where value is expected

```ura
// stdout: HP: 75
// expect: 0
use "@/core"

fn show_hp(hp i32):
    io::out($"HP: {hp}\n")

fn main() i32:
    hero_hp i32 = 75
    r &i32 = &hero_hp
    show_hp(r)
    return 0
```

---

## 098 — function: ref parameter mutates the caller (counter)

```ura
// expect: 2
use "@/core"

fn tick(n &i32) i32:
    n = n + 1
    return n
fn main() i32:
    n i32 = 0
    tick(&n)
    tick(&n)
    return n
```

---

## 099 — function: a void function with a ref parameter

```ura
// expect: 6
use "@/core"

fn bump(n &i32):
    n = n + 1
fn main() i32:
    n i32 = 5
    bump(&n)
    return n
```

---

## 100 — ref: basic ref: bind to hp, ref write through

```ura
// stdout: <0>
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 100
    r &i32 = &hp
    r = 0
    io::out($"<{hp}>\n")
    return 0
```

---

## 101 — ref: aliasing: two refs to same hp, both see mutations

```ura
// stdout: <70>
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 100
    a &i32 = &hp
    b &i32 = &hp
    a = 80
    b = b - 10
    io::out($"<{hp}>\n")
    return 0
```

---

## 102 — ref: compound ref assignment through in a loop

```ura
// stdout: <200>
// expect: 0
use "@/core"

fn main() i32:
    xp      i32 = 0
    xp_gain i32 = 50
    r &i32 = &xp
    i       i32 = 0
    while i < 4:
        r += xp_gain
        i = i + 1
    io::out($"<{xp}>\n")
    return 0
```

---

## 103 — ref: separate refs to different vars: hp ref ref and mp

```ura
// stdout: HP after hit:   70
// stdout: MP after spell: 30
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 100
    mp i32 = 50

    shield &i32 = &hp
    shield = shield - 30
    io::out($"HP after hit:   {hp}\n")

    mana &i32 = &mp
    mana = mana - 20
    io::out($"MP after spell: {mp}\n")
    return 0
```

---

## 104 — ref: multiply and subtract through refs: f64 shield, pay mana

```ura
// stdout: <20 55>
// expect: 0
use "@/core"

fn main() i32:
    shield i32 = 10
    mp     i32 = 80

    s &i32 = &shield
    m &i32 = &mp
    s *= 2
    m -= 25

    io::out($"<{shield} {mp}>\n")
    return 0
```

---

## 105 — ref: chain of compound mutations ref through a

```ura
// stdout: <270>
// expect: 0
use "@/core"

fn main() i32:
    score i32 = 0
    r &i32 = &score
    r += 100
    r += 50
    r *= 2
    r -= 30
    io::out($"<{score}>\n")
    return 0
```

---

## 106 — ref: reference in conditional

```ura
// stdout: healthy
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 75
    r &i32 = &hp
    if r > 50:
        io::out("healthy\n")
    else:
        io::out("critical\n")
    return 0
```

---

## 107 — ref: a ref reads correctly through every operator + as a by-value arg

```ura
// stdout: 15
// stdout: 7
// stdout: 24
// stdout: True
// stdout: True
// stdout: 8
// stdout: 24
// stdout: -12
// stdout: -13
// stdout: 24
// expect: 0
use "@/core"

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 12
    r &i32 = &x
    io::out($"{r + 3}\n")
    io::out($"{r - 5}\n")
    io::out($"{r * 2}\n")
    io::out($"{r > 10}\n")
    io::out($"{r == 12}\n")
    io::out($"{r & 10}\n")
    io::out($"{r << 1}\n")
    io::out($"{-r}\n")
    io::out($"{~r}\n")
    io::out($"{twice(r)}\n")
    return 0
```

---

## 108 — ref: ref? nullable: bound at declaration, guarded reads pass, write-through works

```ura
// stdout: 42
// stdout: 7
// expect: 0
use "@/core"

fn main() i32:
    x i32 = 42
    m &i32 = &x
    io::out($"{m}\n")
    m = 7
    io::out($"{x}\n")
    return 0
```

---

## 109 — ref: nested fn shares an enclosing var through a ref parameter (the sanctioned path)

```ura
// stdout: 6
// expect: 0
use "@/core"

fn main() i32:
    x i32 = 5
    fn bump(n &i32):
        n = n + 1
    bump(&x)
    io::out($"{x}\n")
    return 0
```

---

## 110 — ref: a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

```ura
// stdout: 3
// stdout: 2
// stdout: False
// stdout: True
// stdout: True
// stdout: False
// stdout: 23
// stdout: 16
// stdout: 5
// stdout: 20
// stdout: False
// stdout: True
// stdout: False
// expect: 0
use "@/core"

fn main() i32:
    a i32 = 20
    r &i32 = &a
    io::out($"{r / 6}\n")
    io::out($"{r % 6}\n")
    io::out($"{r != 20}\n")
    io::out($"{r < 25}\n")
    io::out($"{r <= 20}\n")
    io::out($"{r >= 21}\n")
    io::out($"{r | 3}\n")
    io::out($"{r ^ 4}\n")
    io::out($"{r >> 2}\n")
    io::out($"{r as i64}\n")
    flag bool = True
    b &bool = &flag
    io::out($"{b and False}\n")
    io::out($"{b or False}\n")
    io::out($"{not b}\n")
    return 0
```

---

## 111 — ref: a ref declared without a value starts as null

```ura
// stdout: null
// stdout: bound 4
// stdout: null again
// expect: 0
use "@/core"

fn main() i32:
    r &i32
    if r == null:
        io::out("null\n")
    n i32 = 4
    r = &n
    if r != null:
        io::out($"bound {r}\n")
    r = null
    if isZero(r):
        io::out("null again\n")
    return 0
```

---

## 112 — ref: binding a reference to the wrong type

```ura
// error: Reference type mismatch: expected array, got i32
use "@/core"

fn main() i32:
    x i32 = 5
    y &char[] = &x
    return 0
```

---

## 113 — ref: a ref parameter given a value

```ura
// error: Argument 1 to 'tick' must be passed by reference (ref x)
use "@/core"

fn tick(n &i32) i32:
    n = n + 1
    return n

fn main() i32:
    return tick(5)
```

---

## 114 — ref: taking a reference to a literal

```ura
// error: Expected a variable after 'ref'
use "@/core"

fn main() i32:
    r &i32 = &5
    return 0
```

---

## 115 — ref: a by-value parameter given a reference

```ura
// error: Argument 1 to 'twice' does not take a reference
use "@/core"

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 5
    return twice(&x)
```

---

## 116 — ref: a reference to a name that does not exist

```ura
// error: Undeclared variable 'ghost'
use "@/core"

fn main() i32:
    r &i32 = &ghost
    return 0
```

---

## 117 — ref: a nested function may not capture an enclosing local

```ura
// error: Cannot use 'r' from an enclosing function - pass it as a parameter
use "@/core"

fn main() i32:
    x i32 = 5
    r &i32 = &x
    fn inner() i32:
        return r
    return inner()
```

---

## 118 — ref: linked list: build with ref, walk it with cur = ref cur.next

```ura
// stdout: sum=60
// expect: 0
use "@/core"
struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    a.value = 10
    b Node
    b.value = 20
    c Node
    c.value = 30
    a.next = &b
    b.next = &c
    cur &Node = &a
    sum i32 = 0
    while cur != null:
        sum += cur.value
        cur = &cur.next
    io::out($"sum={sum}\n")
    return 0
```

---

## 119 — ref: a ref parameter rebinds a struct's ref field in place (add-front)

```ura
// stdout: 2 1
// expect: 0
use "@/core"
struct Node:
    content i32
    next &Node

struct Lst:
    head &Node

fn push(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn main() i32:
    lst Lst
    a Node
    a.content = 1
    b Node
    b.content = 2
    push(&lst, &a)
    push(&lst, &b)
    cur &Node = &lst.head
    while cur != null:
        io::out($"{cur.content} ")
        cur = &cur.next
    io::out("\n")
    return 0
```

---

## 120 — ref: ref? compared to null with == and !=, bound and unbound

```ura
// stdout: False True True
// expect: 0
use "@/core"
struct N:
    v i32
    nx &N

fn main() i32:
    a N
    a.v = 1
    p &N = &a
    q &N = null
    io::out($"{p == null} {p != null} {q == null}\n")
    return 0
```

---

## 121 — ref: chained rebind removes the head: lst.head = ref lst.head.next

```ura
// stdout: 2
// expect: 0
use "@/core"
struct Node:
    v i32
    next &Node

struct Lst:
    head &Node

fn main() i32:
    a Node
    a.v = 1
    b Node
    b.v = 2
    a.next = &b
    lst Lst
    lst.head = &a
    lst.head = &lst.head.next
    io::out($"{lst.head.v}\n")
    return 0
```

---

## 122 — ref: = ref rebinds the pointer, = value writes through the referent

```ura
// stdout: 99 99 88
// expect: 0
use "@/core"
fn main() i32:
    x i32 = 10
    y i32 = 20
    cur &i32 = &x
    cur = 99
    io::out($"{x} ")
    cur = &y
    cur = 88
    io::out($"{x} {y}\n")
    return 0
```

---

## 123 — ref: ref a == ref a is identity true, ref a == ref b is false

```ura
// stdout: True False
// expect: 0
use "@/core"
struct N:
    v i32

fn main() i32:
    a N
    b N
    io::out($"{&a == &a} {&a == &b}\n")
    return 0
```

---

## 124 — ref: a value function cannot return a reference

```ura
// error: 'f' returns a value, not a reference; drop the `ref`
use "@/core"
fn f() i32:
    x i32 = 5
    return &x

fn main() i32:
    io::out($"{f()}\n")
    return 0
```

---

## 125 — ref: a ref declaration cannot be bound to a plain value

```ura
// error: A reference must be bound to a variable (ref x)
use "@/core"
fn main() i32:
    m &i32 = 5
    io::out($"{m}\n")
    return 0
```

---

## 126 — ref: a non-nullable value cannot be compared to null

```ura
// error: Only arrays and pointers can be null, not i32
use "@/core"
fn main() i32:
    x i32 = 5
    io::out($"{x == null}\n")
    return 0
```
