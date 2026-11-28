# m08

## index

- 004 — array: heap array: new<i32>(N), zero-init, write, free
- 005 — array: heap array with a runtime size, new<i32>(n) + free
- 014 — array: fill a heap char buffer by index, print it, free it
- 015 — enum: a variant as an array index and in arithmetic
- 017 — heap: new<T>(1) gives one zeroed struct, a ref points at it
- 021 — heap: operator drop frees the nodes a tree owns
- 022 — heap: a function fills a heap struct and returns a ref to it
- 025 — lexer: an index with nothing in it
- 026 — lexer: :: must be followed by a name
- 027 — operator: operator drop declares a destructor named Vec.drop
- 029 — operator: calling .drop() yourself is rejected
- 030 — operator: operator drop beside operator = does not warn
- 031 — operator: a destructor fires on break and on continue
- 032 — operator: drop on fall-through: an if block, each iteration, reverse order
- 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows
- 034 — operator: a heap-owning struct: create, deep-copy operator = and drop
- 035 — operator: a growable string struct: nullable buffer, grow, join and drop
- 036 — operator: a struct owning heap buffers: deep-copy operator = and drop
- 037 — ref: ref return: create a node on the heap and return a reference to it
- 038 — ref: ref? return: one branch returns a reference, the other returns null
- 039 — ref: declare a ref and bind it to a ref-returning call
- 040 — ref: a ref-returning call passed straight into a ref parameter
- 041 — ref: a reference function must return a reference, not a bare value
- 042 — ref: a field read chained directly on a ref-returning call
- 043 — struct: a heap array of structs: new<Room>(N), then free
- 045 — struct: an unknown type as a parameter, a return type, or an array element
- 046 — struct: destructor must take no parameters
- 047 — struct: destructor must return void
- 049 — variable: a global struct is initialised and dropped like a local
- 050 — variable: a global is dropped on an early return too

---

## 004 — array: heap array: new<i32>(N), zero-init, write, free

```ura
// stdout: a[0]=0 a[2]=42
// expect: 0
use "@/core"

fn main() i32:
    a i32[] = new<i32>(5)
    a[2] = 42
    io::out($"a[0]={a[0]} a[2]={a[2]}\n")
    free<i32>(a)
    return 0
```

---

## 005 — array: heap array with a runtime size, new<i32>(n) + free

```ura
// stdout: a[5]=7 a[0]=0
// expect: 0
use "@/core"

fn main() i32:
    n i32 = 3
    a i32[] = new<i32>(n * 2)
    a[5] = 7
    io::out($"a[5]={a[5]} a[0]={a[0]}\n")
    free<i32>(a)
    return 0
```

---

## 014 — array: fill a heap char buffer by index, print it, free it

```ura
// stdout: DUNGEON
// expect: 0
use "@/core"

fn main() i32:
    str char[] = new<char>(8)
    str[0] = 'D'
    str[1] = 'U'
    str[2] = 'N'
    str[3] = 'G'
    str[4] = 'E'
    str[5] = 'O'
    str[6] = 'N'
    str[7] = 0 as char
    io::out($"{str}\n")
    free<char>(str)
    return 0
```

---

## 015 — enum: a variant as an array index and in arithmetic

```ura
// stdout: 14 2
// expect: 0
use "@/core"
enum Slot: HEAD, BODY, LEGS

fn main() i32:
    gear i32[] = new<i32>(3)
    gear[Slot::HEAD] = 5
    gear[Slot::LEGS] = 9
    io::out($"{gear[Slot::HEAD] + gear[Slot::LEGS]} {Slot::LEGS - Slot::HEAD}\n")
    free<i32>(gear)
    return 0
```

---

## 017 — heap: new<T>(1) gives one zeroed struct, a ref points at it

```ura
// stdout: 0 0
// stdout: 7
// expect: 0
use "@/core"
struct P:
    a i32
    b i32

fn main() i32:
    ps P[] = new<P>(1)
    p &P = &ps[0]
    io::out($"{p.a} {p.b}\n")
    p.a = 7
    io::out($"{p.a}\n")
    free<P>(ps)
    return 0
```

---

## 021 — heap: operator drop frees the nodes a tree owns

```ura
// stdout: 1 2 3
// stdout: freed
// expect: 0
use "@/core"
struct Node:
    value i32
    left &Node
    right &Node

struct Tree:
    nodes Node[]

    pub fn create() Tree:
        t Tree
        t.nodes = new<Node>(3)
        return t


operator drop(value &Tree):
    free<Node>(value.nodes)
    io::out("freed\n")

fn main() i32:
    tree Tree = Tree::create()
    root &Node = &tree.nodes[0]
    root.value = 1
    root.left = &tree.nodes[1]
    root.left.value = 2
    root.right = &tree.nodes[2]
    root.right.value = 3
    io::out($"{root.value} {root.left.value} {root.right.value}\n")
    return 0
```

---

## 022 — heap: a function fills a heap struct and returns a ref to it

```ura
// stdout: 9
// expect: 0
use "@/core"
struct P:
    v i32

fn make(x i32, ps P[]) &P:
    ps[0].v = x
    return &ps[0]

fn main() i32:
    ps P[] = new<P>(1)
    q &P = make(9, ps)
    io::out($"{q.v}\n")
    free<P>(ps)
    return 0
```

---

## 025 — lexer: an index with nothing in it

```ura
// error: Unexpected token
use "@/core"

fn main() i32:
    arr i32[] = new<i32>(3)
    x i32 = arr[]
    return 0
```

---

## 026 — lexer: :: must be followed by a name

```ura
// error: Expected a name after '::'
use "@/core"
use "@/posix"

fn main() i32:
    s String = String::
    return 0
```

---

## 027 — operator: operator drop declares a destructor named Vec.drop

```ura
// stdout: done
// stdout: dropping 7
// expect: 0
use "@/core"

struct Vec:
    x i32


operator drop(value &Vec):
    io::out($"dropping {value.x}\n")

fn main() i32:
    v Vec
    v.x = 7
    io::out("done\n")
    return 0
```

---

## 029 — operator: calling .drop() yourself is rejected

```ura
// error: operator drop runs by itself; write drop(v) to drop early
use "@/core"

struct Vec:
    x i32


operator drop(value &Vec):
    io::out("dropping\n")

fn main() i32:
    v Vec
    v.drop()
    return 0
```

---

## 030 — operator: operator drop beside operator = does not warn

```ura
// stdout: done
// stdout: drop 5
// expect: 0
use "@/core"

struct Vec:
    x i32


operator =(left &Vec, other &Vec) void:
    left.x = other.x

operator drop(value &Vec):
    io::out($"drop {value.x}\n")

fn main() i32:
    a Vec
    a.x = 5
    io::out("done\n")
    return 0
```

---

## 031 — operator: a destructor fires on break and on continue

```ura
// stdout: drop 100
// stdout: drop 101
// stdout: after break
// stdout: drop 200
// stdout: drop 201
// stdout: after continue
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

fn main() i32:
    i i32 = 0
    while i < 3:
        b Tag = Tag::create(100 + i)
        if i == 1:
            break
        i = i + 1
    io::out("after break\n")

    k i32 = 0
    while k < 2:
        c Tag = Tag::create(200 + k)
        k = k + 1
        continue
    io::out("after continue\n")
    return 0
```

---

## 032 — operator: drop on fall-through: an if block, each iteration, reverse order

```ura
// stdout: inside
// stdout: drop 1
// stdout: after if
// stdout: drop 10
// stdout: drop 11
// stdout: drop 12
// stdout: after loop
// stdout: end of main
// stdout: drop 8
// stdout: drop 7
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

fn main() i32:
    if 1 < 2:
        x Tag = Tag::create(1)
        io::out("inside\n")
    io::out("after if\n")

    i i32 = 0
    while i < 3:
        y Tag = Tag::create(10 + i)
        i += 1
    io::out("after loop\n")

    p Tag = Tag::create(7)
    q Tag = Tag::create(8)
    io::out("end of main\n")
    return 0
```

---

## 033 — operator: a struct drops the structs it owns, at any depth, but not what it borrows

```ura
// stdout: built
// stdout: drop 1
// stdout: drop 3
// stdout: drop 9
// expect: 0
use "@/core"

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r


operator =(left &Res, other &Res) void:
    left.id = other.id

operator drop(value &Res):
    if value.id != 0:
        io::out($"drop {value.id}\n")

struct L3:
    res Res
struct L2:
    inner L3

// Owner declares no destructor of its own — everything below still drops
struct Owner:
    mid  L2
    also Res
    borrowed &Res

    pub fn create() Owner:
        o Owner
        o.mid.inner.res = Res::create(3)
        o.also          = Res::create(1)
        return o

fn main() i32:
    lent Res = Res::create(9)
    o Owner = Owner::create()
    o.borrowed = &lent
    io::out("built\n")
    return 0
```

---

## 034 — operator: a heap-owning struct: create, deep-copy operator = and drop

```ura
// stdout: b1: hello
// stdout: b2: world
// expect: 0
use "@/core"

use "@/posix"

struct Buffer:
    value char[]

    pub fn create() Buffer:
        b Buffer
        b.value = new<char>(64)
        return b


operator drop(value &Buffer):
    free<char>(value.value)

operator = (left &Buffer, s char[]) void:
    cstd::strcpy(left.value, s)

operator = (left &Buffer, b &Buffer) void:
    free<char>(left.value)
    left.value = new<char>(64)
    cstd::strcpy(left.value, b.value)

fn main() i32:
    b1 Buffer = Buffer::create()
    b1 = "hello"

    b2 Buffer = Buffer::create()
    b2 = b1

    cstd::strcpy(b2.value, "world")

    io::out($"b1: {b1.value}\n")
    io::out($"b2: {b2.value}\n")
    return 0
```

---

## 035 — operator: a growable string struct: nullable buffer, grow, join and drop

```ura
// stdout: s0:    1x2345 (6)
// stdout: s1:    6x789 (5)
// stdout: s0+s1: 1x23456x789 (11)
// expect: 0
use "@/core"

use "@/posix"

struct String2:
    value char[]
    count i32
    size  i32

    pub fn create() String2:
        s String2
        s.size  = 10
        s.value = new<char>(10)
        return s

    fn grow(want i32):
        if self.size >= want: return
        if self.size == 0: self.size = 10
        while self.size < want:
            self.size *= 2
        bigger char[] = new<char>(self.size)
        if self.count > 0: cstd::strcpy(bigger, self.value)
        free<char>(self.value)
        self.value = bigger

    fn assign(str char[]):
        len i32 = cstd::strlen(str) as i32
        self.grow(len + 1)
        cstd::bzero(self.value, self.size as i64)
        cstd::strcpy(self.value, str)
        self.count = len

    fn join(str char[]):
        len i32 = cstd::strlen(str) as i32
        self.grow(self.count + len + 1)
        cstd::strcat(self.value, str)
        self.count += len


operator drop(value &String2):
    free<char>(value.value)

operator = (left &String2, str char[]) void:
    left.assign(str)

operator = (left &String2, v &String2) void:
    left.assign(v.value)

fn add(v &String2, w &String2) String2:
    res String2 = String2::create()
    res = v
    res.join(w.value)
    return res

fn main() i32:
    s0 String2 = String2::create()
    s0 = "1x2345"

    s1 String2 = String2::create()
    s1 = "6x789"

    s2 String2 = String2::create()
    s2 = add(&s0, &s1)

    io::out($"s0:    {s0.value} ({s0.count})\n")
    io::out($"s1:    {s1.value} ({s1.count})\n")
    io::out($"s0+s1: {s2.value} ({s2.count})\n")
    return 0
```

---

## 036 — operator: a struct owning heap buffers: deep-copy operator = and drop

```ura
// stdout: The Bold Aldric enters!
// stdout: The Grim Borin enters!
// stdout: The Grim Borin enters!
// stdout: The Quick Cass enters!
// expect: 0
use "@/core"
use "@/posix"

struct Hero:
    name  char[]
    title char[]

    pub fn create() Hero:
        h Hero
        h.name  = new<char>(32)
        h.title = new<char>(32)
        return h

    fn setup(name char[], title char[]):
        cstd::strcpy(self.name,  name)
        cstd::strcpy(self.title, title)

    fn announce():
        io::out($"{self.title} {self.name} enters!\n")


operator = (left &Hero, other &Hero) void:
    free<char>(left.name)
    free<char>(left.title)
    left.name  = new<char>(32)
    left.title = new<char>(32)
    cstd::strcpy(left.name,  other.name)
    cstd::strcpy(left.title, other.title)

operator drop(value &Hero):
    free<char>(value.name)
    free<char>(value.title)

fn main() i32:
    hero Hero = Hero::create()
    hero.setup("Aldric", "The Bold")
    hero.announce()

    other Hero = Hero::create()
    other.setup("Borin", "The Grim")
    hero = other
    hero.announce()

    // the copy owns its own buffers, so mutating the source leaves it alone
    other.setup("Cass", "The Quick")
    hero.announce()
    other.announce()
    return 0
```

---

## 037 — ref: ref return: create a node on the heap and return a reference to it

```ura
// stdout: 42
// expect: 0
use "@/core"
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new<Node>(1)
    a &Node = make(42, h)
    io::out($"{a.value}\n")
    return 0
```

---

## 038 — ref: ref? return: one branch returns a reference, the other returns null

```ura
// stdout: 7 True
// expect: 0
use "@/core"
struct N:
    v i32
    nx &N

fn pick(h N[], want bool) &N:
    if want:
        return &h[0]
    return null

fn main() i32:
    h N[] = new<N>(1)
    h[0].v = 7
    r &N = pick(h, True)
    e &N = pick(h, False)
    io::out($"{r.v} {e == null}\n")
    return 0
```

---

## 039 — ref: declare a ref and bind it to a ref-returning call

```ura
// stdout: 9
// expect: 0
use "@/core"
struct Node:
    v i32
    next &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

fn main() i32:
    h Node[] = new<Node>(1)
    n &Node = make(9, h)
    io::out($"{n.v}\n")
    return 0
```

---

## 040 — ref: a ref-returning call passed straight into a ref parameter

```ura
// stdout: 5
// expect: 0
use "@/core"
struct Node:
    v i32
    next &Node

struct Lst:
    head &Node

fn make(v i32, h Node[]) &Node:
    h[0].v = v
    return &h[0]

fn sethead(lst &Lst, node &Node):
    lst.head = &node

fn main() i32:
    h Node[] = new<Node>(1)
    lst Lst
    sethead(&lst, make(5, h))
    io::out($"{lst.head.v}\n")
    return 0
```

---

## 041 — ref: a reference function must return a reference, not a bare value

```ura
// error: 'g' returns a reference; return `ref x` (or null), not a value
use "@/core"
struct N:
    v i32

fn g(h N[]) &N:
    return h[0]

fn main() i32:
    h N[] = new<N>(1)
    io::out($"{g(h).v}\n")
    return 0
```

---

## 042 — ref: a field read chained directly on a ref-returning call

```ura
// stdout: 42
// expect: 0
use "@/core"
struct Node:
    value i32
    next &Node

fn make(v i32, heap Node[]) &Node:
    heap[0].value = v
    return &heap[0]

fn main() i32:
    h Node[] = new<Node>(1)
    io::out($"{make(42, h).value}\n")
    return 0
```

---

## 043 — struct: a heap array of structs: new<Room>(N), then free

```ura
// stdout: attic@9 2
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn main() i32:
    h Room[] = new<Room>(2)
    h[0].name  = "attic"
    h[0].floor = 9
    h[1].floor = 2
    io::out($"{h[0].name}@{h[0].floor} {h[1].floor}\n")
    free<Room>(h)
    return 0
```

---

## 045 — struct: an unknown type as a parameter, a return type, or an array element

```ura
// error: Unknown type 'Widget'
use "@/core"

fn process(p Widget):
    return

fn make() Widget:
    w Widget
    return w

fn main() i32:
    arr Widget[] = new<Widget>(3)
    io::out("hi\n")
    return 0
```

---

## 046 — struct: destructor must take no parameters

```ura
// error: Destructor 'drop' takes no parameters; write 'operator drop():'
use "@/core"
use "@/posix"

struct Box:
    value i32


operator drop(value &Box, extra i32) void:
    return

fn main() i32:
    b Box
    b.value = 1
    return 0
```

---

## 047 — struct: destructor must return void

```ura
// error: Destructor 'drop' returns void; drop the return type
use "@/core"
use "@/posix"

struct Box:
    value i32


operator drop(value &Box) i32:
    return 0

fn main() i32:
    b Box
    b.value = 1
    return 0
```

---

## 049 — variable: a global struct is initialised and dropped like a local

```ura
// stdout: start 10
// stdout: after 12
// stdout: DROP at 12
// expect: 0
use "@/core"

struct Counter:
    hits i32

    pub fn create(n i32) Counter:
        c Counter
        c.hits = n
        return c

    fn bump():
        self.hits += 1


operator =(left &Counter, other &Counter) void:
    left.hits = other.hits

operator drop(value &Counter):
    io::out($"DROP at {value.hits}\n")

tally Counter = Counter::create(10)

fn touch():
    tally.bump()

fn main() i32:
    io::out($"start {tally.hits}\n")
    touch()
    touch()
    io::out($"after {tally.hits}\n")
    return 0
```

---

## 050 — variable: a global is dropped on an early return too

```ura
// stdout: body
// stdout: early
// stdout: DROP 2
// stdout: DROP 1
// expect: 0
use "@/core"

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r


operator =(left &Res, other &Res) void:
    left.id = other.id

operator drop(value &Res):
    io::out($"DROP {value.id}\n")

a Res = Res::create(1)
b Res = Res::create(2)

fn main() i32:
    io::out("body\n")
    if 1 < 2:
        io::out("early\n")
        return 0
    io::out("unreachable\n")
    return 0
```
