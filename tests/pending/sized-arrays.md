# pending

## index

- 001 — (was milestone02 061) arrays: size in the type
- 002 — (was milestone02 084) array: sized constructor i32[N] with zero-init
- 003 — (was milestone02 127) variable: an array type carries no size
- 004 — (was milestone03 058) crazy: a linked list built with chained methods that return refs
- 005 — (was milestone03 093) method: a method on an array element
- 006 — (was milestone03 112) struct: a stack array of structs: Room[N]
- 007 — (was milestone03 113) struct: a 2D array of structs: Room[N][M]
- 008 — (was milestone03 114) struct: '.len' on arrays of structs, including 2D
- 009 — (was milestone03 129) struct: '.len' on a scalar
- 010 — (was milestone08 001) array: 2D array literal and nested indexing
- 011 — (was milestone08 002) array: sized 2D matrix i32[N][M], zero-init and writes
- 012 — (was milestone08 003) array: 3D sized array with runtime size
- 013 — (was milestone08 006) array: multi-dim heap: new<i32[]>(N), a row each, free each
- 014 — (was milestone08 008) array: arr.len on a literal and on multi-dim
- 015 — (was milestone08 012) array: .len on a non-array
- 016 — (was milestone08 013) array: array literals: one element, expressions, and strings
- 017 — (was milestone08 044) struct: a 2D heap array of structs frees every inner slice
- 018 — (was milestone08 048) variable: global variables: scalars, arrays, forward reference, shadowing
- 019 — (was m02 085) array: VLA: runtime-sized stack array

---

## 001 — (was milestone02 061) arrays: size in the type

```ura
use "@/core"
fn main() i32:
    x i32[3]
    return 0
```

---

## 002 — (was milestone02 084) array: sized constructor i32[N] with zero-init

```ura
// stdout: zero-init: 0 0
// stdout: a[2]=42
// expect: 0
use "@/core"

fn main() i32:
    a i32[] = i32[5]
    io::out($"zero-init: {a[0]} {a[4]}\n")
    a[2] = 42
    io::out($"a[2]={a[2]}\n")
    return 0
```

---

## 003 — (was milestone02 127) variable: an array type carries no size

```ura
// error: An array type carries no size; ex. 'arr int[] = int[3]' to make one
use "@/core"

fn main() i32:
    xs i32[3]
    xs[0] = 7
    return 0
```

---

## 004 — (was milestone03 058) crazy: a linked list built with chained methods that return refs

```ura
// expect: 44   (length 4 * 10, last value 4)
use "@/core"
struct Node:
    value i32
    next &Node

    fn attach(n &Node) &Node:
        self.next = &n
        return &n

    fn length() i32:
        n i32 = 1
        p &Node = &self
        while not isZero(p.next):
            n += 1
            p = &p.next
        return n

a Node
b Node
c Node
d Node
d.value = 4
a.attach(&b).attach(&c).attach(&d)
return a.length() * 10 + a.next.next.next.value
```

---

## 005 — (was milestone03 093) method: a method on an array element

```ura
// stdout: 8 1
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn bump():
        self.floor = self.floor + 1

fn main() i32:
    rooms Room[] = Room[2]
    rooms[0].floor = 7
    rooms[0].bump()
    rooms[1].bump()
    io::out($"{rooms[0].floor} {rooms[1].floor}\n")
    return 0
```

---

## 006 — (was milestone03 112) struct: a stack array of structs: Room[N]

```ura
// stdout: lobby@1 cellar@0
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn main() i32:
    s Room[] = Room[2]
    s[0].name  = "lobby"
    s[0].floor = 1
    s[1].name  = "cellar"
    io::out($"{s[0].name}@{s[0].floor} {s[1].name}@{s[1].floor}\n")
    return 0
```

---

## 007 — (was milestone03 113) struct: a 2D array of structs: Room[N][M]

```ura
// stdout: grid-a@3 grid-b@4
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn main() i32:
    g Room[][] = Room[2][1]
    g[0][0].name  = "grid-a"
    g[0][0].floor = 3
    g[1][0].name  = "grid-b"
    g[1][0].floor = 4
    io::out($"{g[0][0].name}@{g[0][0].floor} {g[1][0].name}@{g[1][0].floor}\n")
    return 0
```

---

## 008 — (was milestone03 114) struct: '.len' on arrays of structs, including 2D

```ura
// stdout: 2 4
// expect: 0
use "@/core"

struct Room:
    floor i32

fn main() i32:
    s Room[]   = Room[2]
    g Room[][] = Room[4][1]
    io::out($"{s.len} {g.len}\n")
    return 0
```

---

## 009 — (was milestone03 129) struct: '.len' on a scalar

```ura
// error: '.len' is only valid on an array, not i32
use "@/core"

fn main() i32:
    x i32 = 3
    io::out($"{x.len}")
    return 0
```

---

## 010 — (was milestone08 001) array: 2D array literal and nested indexing

```ura
// stdout: m[0][0]=1 m[1][1]=4
// stdout: m[0][1]=99
// expect: 0
use "@/core"

fn main() i32:
    m i32[][] = [[1, 2], [3, 4]]
    io::out($"m[0][0]={m[0][0]} m[1][1]={m[1][1]}\n")
    m[0][1] = 99
    io::out($"m[0][1]={m[0][1]}\n")
    return 0
```

---

## 011 — (was milestone08 002) array: sized 2D matrix i32[N][M], zero-init and writes

```ura
// stdout: g[0][0]=1 g[2][3]=99 g[1][1]=0
// expect: 0
use "@/core"

fn main() i32:
    g i32[][] = i32[3][4]
    g[0][0] = 1
    g[2][3] = 99
    io::out($"g[0][0]={g[0][0]} g[2][3]={g[2][3]} g[1][1]={g[1][1]}\n")
    return 0
```

---

## 012 — (was milestone08 003) array: 3D sized array with runtime size

```ura
// stdout: v[1][1][1]=7 v[0][0][0]=0
// expect: 0
use "@/core"

fn main() i32:
    n i32 = 2
    v i32[][][] = i32[n][n][n]
    v[1][1][1] = 7
    io::out($"v[1][1][1]={v[1][1][1]} v[0][0][0]={v[0][0][0]}\n")
    return 0
```

---

## 013 — (was milestone08 006) array: multi-dim heap: new<i32[]>(N), a row each, free each

```ura
// stdout: m[0][0]=1 m[1][2]=9 m[0][1]=0
// expect: 0
use "@/core"

fn main() i32:
    m i32[][] = new<i32[]>(2)
    m[0] = new<i32>(3)
    m[1] = new<i32>(3)
    m[0][0] = 1
    m[1][2] = 9
    io::out($"m[0][0]={m[0][0]} m[1][2]={m[1][2]} m[0][1]={m[0][1]}\n")
    free<i32>(m[0])
    free<i32>(m[1])
    free<i32[]>(m)
    return 0
```

---

## 014 — (was milestone08 008) array: arr.len on a literal and on multi-dim

```ura
// stdout: len = 4
// stdout: rows = 3 cols = 4
// expect: 0
use "@/core"

fn main() i32:
    nums i32[] = [10, 20, 30, 40]
    io::out($"len = {nums.len}\n")
    grid i32[][] = i32[3][4]
    io::out($"rows = {grid.len} cols = {grid[0].len}\n")
    return 0
```

---

## 015 — (was milestone08 012) array: .len on a non-array

```ura
// error: '.len' is only valid on an array, not i32
use "@/core"
fn main() i32:
    x i32 = 5
    io::out($"{x.len}")
    return 0
```

---

## 016 — (was milestone08 013) array: array literals: one element, expressions, and strings

```ura
// stdout: 42 len 1
// stdout: 5 6 10 10
// stdout: ada grace alan len 3
// expect: 0
use "@/core"

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    // a single-element literal
    one i32[] = [42]
    io::out($"{one[0]} len {one.len}\n")

    // elements that are not compile-time constants
    x i32 = 5
    ex i32[] = [x, x + 1, x * 2, twice(x)]
    for v in ex:
        io::out($"{v} ")
    io::out("\n")

    // an array of strings
    names char[][] = ["ada", "grace", "alan"]
    for n in names:
        io::out($"{n} ")
    io::out($"len {names.len}\n")
    return 0
```

---

## 017 — (was milestone08 044) struct: a 2D heap array of structs frees every inner slice

```ura
// stdout: 1 4
// expect: 0
use "@/core"

struct Room:
    floor i32

fn main() i32:
    g Room[][] = new<Room[]>(2)
    g[0] = new<Room>(2)
    g[1] = new<Room>(2)
    g[0][0].floor = 1
    g[1][1].floor = 4
    io::out($"{g[0][0].floor} {g[1][1].floor}\n")
    free<Room>(g[0])
    free<Room>(g[1])
    free<Room[]>(g)
    return 0
```

---

## 018 — (was milestone08 048) variable: global variables: scalars, arrays, forward reference, shadowing

```ura
// stdout: ura 1.500000 True
// stdout: total starts 7
// stdout: total now 9
// stdout: xs 7 len 3  heap 9 len 2
// stdout: local total 99
// expect: 0
use "@/core"

name  char[] = "ura"
ratio f32 = 1.5
flag  bool  = True
xs    i32[] = i32[3]
heap  i32[] = new<i32>(2)

fn touch():
    total += 1

fn main() i32:
    io::out($"{name} {ratio} {flag}\n")
    // forward reference: total is declared below main
    io::out($"total starts {total}\n")
    touch()
    touch()
    io::out($"total now {total}\n")

    xs[0] = 7
    heap[1] = 9
    io::out($"xs {xs[0]} len {xs.len}")
    io::out($"  heap {heap[1]} len {heap.len}\n")
    free<i32>(heap)

    // a local shadows the global for the rest of this scope
    total i32 = 99
    io::out($"local total {total}\n")
    return 0

total i32 = 7
```

---

## 019 — (was m02 085) array: VLA: runtime-sized stack array

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
