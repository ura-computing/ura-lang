# m03

## index

- 028 — crazy: a method called three times through a ref
- 029 — crazy: methods of a nested struct, called through self
- 030 — crazy: bubble sort: a struct with an array, methods calling methods
- 031 — crazy: bank account: methods, a ref parameter, an array of bills
- 032 — crazy: an array of structs is a copy
- 033 — crazy: vectors: struct parameters, struct returns, chained calls
- 034 — crazy: linked list: ref attributes, isNullable, rebinding a ref in a loop
- 035 — crazy: a recursive method that counts its calls
- 036 — crazy: a method takes a ref to its own struct type
- 037 — crazy: stack: push squares, pop until empty
- 038 — crazy: ring-buffer queue: full, wrap around, drain
- 039 — crazy: binary tree: recursion through ref attributes
- 040 — crazy: a linked list over array elements
- 041 — crazy: search a linked list, -1 when missing
- 042 — crazy: assigning a struct copies it
- 043 — crazy: a struct parameter is a copy
- 044 — crazy: compound assignment through a ref attribute
- 045 — crazy: a method on an array element
- 046 — crazy: builder: a method returns a ref to self, chained
- 047 — crazy: a struct holding an array of structs
- 048 — crazy: tree walk with a ref out-parameter
- 049 — crazy: a function returns a ref into a struct, the caller binds it
- 050 — crazy: fibonacci as a pair: recursion returning a struct, .a on a call
- 051 — crazy: method calls nested as arguments
- 052 — crazy: print a linked list backwards with recursion
- 053 — crazy: sort structs in an array by an attribute
- 054 — crazy: ping pong: two structs that name each other, before either is declared
- 055 — crazy: while True with break and return inside a method
- 056 — crazy: script: a global stack evaluates postfix 3 4 + 2 *
- 057 — crazy: structs used before declared, a struct local to a function
- 059 — crazy: binary search tree: recursive methods through ref attributes
- 060 — crazy: 2x2 matrix product with methods and ref parameters

---

## 028 — crazy: a method called three times through a ref

```ura
// expect: 21
use "@/core"
struct Pair:
    a i32
    b i32

    fn swap():
        t i32 = self.a
        self.a = self.b
        self.b = t

fn thrice(p &Pair):
    p.swap()
    p.swap()
    p.swap()

fn main() i32:
    x Pair
    x.a = 1
    x.b = 2
    thrice(&x)
    return x.a * 10 + x.b
```

---

## 029 — crazy: methods of a nested struct, called through self

```ura
// expect: 30   (area 4 * 5, plus a.x 10)
use "@/core"
struct Point:
    x i32
    y i32

    fn move(dx i32, dy i32):
        self.x += dx
        self.y += dy

struct Rect:
    a Point
    b Point

    fn shift(d i32):
        self.a.move(d, d)
        self.b.move(d, d)

    fn area() i32:
        return (self.b.x - self.a.x) * (self.b.y - self.a.y)

fn main() i32:
    r Rect
    r.b.move(4, 5)
    r.shift(10)
    return r.area() + r.a.x
```

---

## 030 — crazy: bubble sort: a struct with an array, methods calling methods

```ura
// stdout: 1 3 4 5 9
// expect: 19
use "@/core"
proto printf(fmt i8[], ...) i32

struct List:
    items i32[]
    size i32

    fn swap(i i32, j i32):
        t i32 = self.items[i]
        self.items[i] = self.items[j]
        self.items[j] = t

    fn sort():
        i i32 = 0
        while i < self.size:
            j i32 = 0
            while j < self.size - 1 - i:
                if self.items[j] > self.items[j + 1]:
                    self.swap(j, j + 1)
                j += 1
            i += 1

    fn show():
        i i32 = 0
        while i < self.size:
            if i > 0: printf(" ")
            printf("%d", self.items[i])
            i += 1
        printf("\n")

fn main() i32:
    l List
    l.items = [5, 3, 9, 1, 4]
    l.size = 5
    l.sort()
    l.show()
    return l.items[0] * 10 + l.items[4]
```

---

## 031 — crazy: bank account: methods, a ref parameter, an array of bills

```ura
// stdout: balance 10, ops 4, failed 1
// expect: 10
use "@/core"
proto printf(fmt i8[], ...) i32

struct Account:
    balance i32
    ops i32

    fn deposit(n i32):
        self.balance += n
        self.ops += 1

    fn withdraw(n i32) bool:
        if n > self.balance:
            return False
        self.balance -= n
        self.ops += 1
        return True

fn pay_all(acc &Account, bills i32[], count i32) i32:
    failed i32 = 0
    i i32 = 0
    while i < count:
        if acc.withdraw(bills[i]) == False:
            failed += 1
        i += 1
    return failed

fn main() i32:
    a Account
    a.deposit(100)
    bills i32[] = [30, 50, 40, 10]
    failed i32 = pay_all(&a, bills, 4)
    printf("balance %d, ops %d, failed %d\n", a.balance, a.ops, failed)
    return a.balance
```

---

## 032 — crazy: an array of structs is a copy

```ura
// expect: 10   (1 + 7 + 2: q itself stays 2)
use "@/core"
struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    q Point
    p.x = 1
    q.x = 2
    ps Point[] = [p, q]
    ps[1].x = 7
    return ps[0].x + ps[1].x + q.x
```

---

## 033 — crazy: vectors: struct parameters, struct returns, chained calls

```ura
// expect: 27   (c = (7, 10), c . a = 7 + 20)
use "@/core"
struct Vec:
    x i32
    y i32

    fn add(o Vec) Vec:
        r Vec
        r.x = self.x + o.x
        r.y = self.y + o.y
        return r

    fn dot(o Vec) i32:
        return self.x * o.x + self.y * o.y

fn make(x i32, y i32) Vec:
    v Vec
    v.x = x
    v.y = y
    return v

fn main() i32:
    a Vec = make(1, 2)
    b Vec = make(3, 4)
    c Vec = a.add(b).add(b)
    return c.dot(a)
```

---

## 034 — crazy: linked list: ref attributes, isNullable, rebinding a ref in a loop

```ura
// expect: 36   (1 + 2 + 3, plus 3 * 10)
use "@/core"
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3 // c.next is never set: it starts null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while not isZero(p):
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```

---

## 035 — crazy: a recursive method that counts its calls

```ura
// expect: 125   (5! = 120, plus 5 calls)
use "@/core"
struct Math:
    calls i32

    fn fact(n i32) i32:
        self.calls += 1
        if n <= 1: return 1
        return n * self.fact(n - 1)

fn main() i32:
    m Math
    r i32 = m.fact(5)
    return r + m.calls
```

---

## 036 — crazy: a method takes a ref to its own struct type

```ura
// expect: 100   (a = 10, b = 0)
use "@/core"
struct Counter:
    n i32

    fn absorb(other &Counter):
        self.n += other.n
        other.n = 0

fn main() i32:
    a Counter
    b Counter
    a.n = 4
    b.n = 6
    a.absorb(&b)
    a.absorb(&b)
    return a.n * 10 + b.n
```

---

## 037 — crazy: stack: push squares, pop until empty

```ura
// expect: 173   (pops 25 16 9 4 1, last digits folded: ((((5*2+6)*2+9)*2+4)*2+1)
use "@/core"
struct Stack:
    items i32[]
    top i32

    fn push(v i32):
        self.items[self.top] = v
        self.top += 1

    fn pop() i32:
        self.top -= 1
        return self.items[self.top]

    fn empty() bool:
        return self.top == 0

fn main() i32:
    s Stack
    s.items = [0, 0, 0, 0, 0, 0, 0, 0]
    i i32 = 1
    while i <= 5:
        s.push(i * i)
        i += 1
    total i32 = 0
    while not s.empty():
        total = total * 2 + s.pop() % 10
    return total
```

---

## 038 — crazy: ring-buffer queue: full, wrap around, drain

```ura
// expect: 242   (first 1 * 100, sum 2 + 3 + 9 = 14 * 10, 2 dropped)
use "@/core"
struct Queue:
    items i32[]
    head i32
    tail i32
    size i32
    cap i32

    fn put(v i32) bool:
        if self.size == self.cap: return False
        self.items[self.tail] = v
        self.tail = (self.tail + 1) % self.cap
        self.size += 1
        return True

    fn take() i32:
        v i32 = self.items[self.head]
        self.head = (self.head + 1) % self.cap
        self.size -= 1
        return v

fn main() i32:
    q Queue
    q.items = [0, 0, 0]
    q.cap = 3
    dropped i32 = 0
    i i32 = 1
    while i <= 5:
        if !q.put(i): dropped += 1
        i += 1
    first i32 = q.take()
    q.put(9)
    sum i32 = 0
    while q.size > 0:
        sum += q.take()
    return first * 100 + sum * 10 + dropped
```

---

## 039 — crazy: binary tree: recursion through ref attributes

```ura
// expect: 103   (sum 10 * 10, depth 3)
use "@/core"
struct Tree:
    value i32
    left &Tree
    right &Tree

fn sum(t &Tree) i32:
    if isZero(t): return 0
    return t.value + sum(&t.left) + sum(&t.right)

fn depth(t &Tree) i32:
    if isZero(t): return 0
    l i32 = depth(&t.left)
    r i32 = depth(&t.right)
    if l > r: return l + 1
    return r + 1

fn main() i32:
    a Tree
    b Tree
    c Tree
    d Tree
    a.value = 1
    b.value = 2
    c.value = 3
    d.value = 4
    a.left = &b
    a.right = &c
    b.left = &d
    return sum(&a) * 10 + depth(&a)
```

---

## 040 — crazy: a linked list over array elements

```ura
// expect: 33   (length 3, plus the third value 30)
use "@/core"
struct Node:
    value i32
    next &Node

fn length(n &Node) i32:
    if isZero(n): return 0
    return 1 + length(&n.next)

fn main() i32:
    a Node
    b Node
    c Node
    nodes Node[] = [a, b, c]
    i i32 = 0
    while i < 3:
        nodes[i].value = (i + 1) * 10
        if i < 2: nodes[i].next = &nodes[i + 1]
        i += 1
    return length(&nodes[0]) + nodes[0].next.next.value
```

---

## 041 — crazy: search a linked list, -1 when missing

```ura
// expect: 121   (13 at 2 * 10, 8 at 1, 99 missing: 100)
use "@/core"
struct Node:
    value i32
    next &Node

fn find(head &Node, v i32) i32:
    i i32 = 0
    p &Node = &head
    while !isZero(p):
        if p.value == v: return i
        p = &p.next
        i += 1
    return -1

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 5
    b.value = 8
    c.value = 13
    a.next = &b
    b.next = &c
    r i32 = find(&a, 13) * 10 + find(&a, 8)
    if find(&a, 99) == -1: r += 100
    return r
```

---

## 042 — crazy: assigning a struct copies it

```ura
// expect: 63   (1 + 20 + 10 + 2 + 10 + 20)
use "@/core"
struct Point:
    x i32
    y i32

fn main() i32:
    a Point
    a.x = 1
    a.y = 2
    b Point = a
    b.x = 10
    c Point
    c = b
    c.y = 20
    return a.x + a.y * 10 + b.x + b.y + c.x + c.y
```

---

## 043 — crazy: a struct parameter is a copy

```ura
// expect: 61
use "@/core"
struct Point:
    x i32

fn bump(p Point) i32:
    p.x += 5
    return p.x

fn main() i32:
    a Point
    a.x = 1
    r i32 = bump(a)
    return r * 10 + a.x
```

---

## 044 — crazy: compound assignment through a ref attribute

```ura
// expect: 14   ((0 + 5) * 3 - 1)
use "@/core"
struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    b Node
    a.next = &b
    a.next.value += 5
    a.next.value *= 3
    b.value -= 1
    return b.value
```

---

## 045 — crazy: a method on an array element

```ura
// expect: 45   (4 * 10 + 5)
use "@/core"
struct Counter:
    n i32

    fn add(k i32):
        self.n += k

fn main() i32:
    a Counter
    b Counter
    cs Counter[] = [a, b]
    i i32 = 0
    while i < 2:
        cs[i].add(i + 3)
        cs[i].add(1)
        i += 1
    return cs[0].n * 10 + cs[1].n
```

---

## 046 — crazy: builder: a method returns a ref to self, chained

```ura
// expect: 6
use "@/core"
struct Builder:
    total i32

    fn add(v i32) &Builder:
        self.total += v
        return &self

fn main() i32:
    b Builder
    b.add(1).add(2).add(3)
    return b.total
```

---

## 047 — crazy: a struct holding an array of structs

```ura
// expect: 12   (9 - (-3))
use "@/core"
struct Point:
    x i32
    y i32

struct Shape:
    points Point[]
    count i32

    fn width() i32:
        lo i32 = self.points[0].x
        hi i32 = lo
        i i32 = 1
        while i < self.count:
            x i32 = self.points[i].x
            if x < lo: lo = x
            if x > hi: hi = x
            i += 1
        return hi - lo

fn main() i32:
    a Point
    b Point
    c Point
    a.x = 4
    b.x = -3
    c.x = 9
    s Shape
    s.points = [a, b, c]
    s.count = 3
    return s.width()
```

---

## 048 — crazy: tree walk with a ref out-parameter

```ura
// expect: 183   (sum 18 * 10, 3 nodes)
use "@/core"
struct Tree:
    value i32
    left &Tree
    right &Tree

fn walk(t &Tree, count &i32) i32:
    if isZero(t): return 0
    count += 1
    return t.value + walk(&t.left, &count) + walk(&t.right, &count)

fn main() i32:
    a Tree
    b Tree
    c Tree
    a.value = 5
    b.value = 6
    c.value = 7
    a.left = &b
    b.right = &c
    n i32 = 0
    s i32 = walk(&a, &n)
    return s * 10 + n
```

---

## 049 — crazy: a function returns a ref into a struct, the caller binds it

```ura
// expect: 103   (r binds to x.b: 3 + 100)
use "@/core"
struct Pair:
    a i32
    b i32

fn bigger(p &Pair) &i32:
    if p.a > p.b: return &p.a
    return &p.b

fn main() i32:
    x Pair
    x.a = 3
    x.b = 7
    r &i32 = bigger(&x)
    r = 100
    return x.a + x.b
```

---

## 050 — crazy: fibonacci as a pair: recursion returning a struct, .a on a call

```ura
// expect: 55
use "@/core"
struct Pair:
    a i32
    b i32

fn fib(n i32) Pair:
    r Pair
    if n == 0:
        r.a = 0
        r.b = 1
        return r
    p Pair = fib(n - 1)
    r.a = p.b
    r.b = p.a + p.b
    return r

fn main() i32:
    return fib(10).a
```

---

## 051 — crazy: method calls nested as arguments

```ura
// expect: 160   (a 15 * 10, b 10)
use "@/core"
struct Acc:
    v i32

    fn get() i32:
        return self.v

    fn add(k i32) i32:
        self.v += k
        return self.v

fn main() i32:
    a Acc
    b Acc
    b.v = 5
    a.add(b.add(a.add(b.get())))
    return a.v * 10 + b.v
```

---

## 052 — crazy: print a linked list backwards with recursion

```ura
// stdout: 321
// expect: 6
use "@/core"
proto printf(fmt i8[], ...) i32

struct Node:
    value i32
    next &Node

fn print_rev(n &Node):
    if isZero(n): return
    print_rev(&n.next)
    printf("%d", n.value)

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    print_rev(&a)
    printf("\n")
    return a.value + b.value + c.value
```

---

## 053 — crazy: sort structs in an array by an attribute

```ura
// stdout: abc
// expect: 123
use "@/core"
proto printf(fmt i8[], ...) i32

struct Item:
    key i32
    tag char

fn main() i32:
    a Item
    b Item
    c Item
    a.key = 3
    a.tag = 'c'
    b.key = 1
    b.tag = 'a'
    c.key = 2
    c.tag = 'b'
    items Item[] = [a, b, c]
    i i32 = 0
    while i < 3:
        j i32 = i + 1
        while j < 3:
            if items[j].key < items[i].key:
                t Item = items[i]
                items[i] = items[j]
                items[j] = t
            j += 1
        i += 1
    printf("%c%c%c\n", items[0].tag, items[1].tag, items[2].tag)
    return items[0].key * 100 + items[1].key * 10 + items[2].key
```

---

## 054 — crazy: ping pong: two structs that name each other, before either is declared

```ura
// expect: 33
use "@/core"
struct Ping:
    hits i32

    fn hit(o &Pong, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

struct Pong:
    hits i32

    fn hit(o &Ping, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

fn main() i32:
    a Ping
    b Pong
    a.hit(&b, 5)
    return a.hits * 10 + b.hits
```

---

## 055 — crazy: while True with break and return inside a method

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

fn main() i32:
    f Finder
    f.data = [4, 8, 15, 16, 23, 42]
    f.size = 6
    return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

---

## 056 — crazy: script: a global stack evaluates postfix 3 4 + 2 *

```ura
// expect: 14
use "@/core"
struct Stack:
    items i32[]
    top i32

stack Stack
stack.items = [0, 0, 0, 0, 0, 0, 0, 0]

fn push(v i32):
    stack.items[stack.top] = v
    stack.top += 1

fn pop() i32:
    stack.top -= 1
    return stack.items[stack.top]

push(3)
push(4)
push(pop() + pop())
push(2)
push(pop() * pop())
return pop()
```

---

## 057 — crazy: structs used before declared, a struct local to a function

```ura
// expect: 203   (area 5 * 4 = 20 * 10, plus 3)
use "@/core"
origin Point
origin.x = 2

fn area(r &Rect) i32:
    return (r.b.x - r.a.x) * (r.b.y - r.a.y)

fn make() i32:
    struct Local:
        v i32
    l Local
    l.v = 3
    return l.v

struct Rect:
    a Point
    b Point

struct Point:
    x i32
    y i32

r Rect
r.a = origin
r.b.x = 7
r.b.y = 4
return area(&r) * 10 + make()
```

---

## 059 — crazy: binary search tree: recursive methods through ref attributes

```ura
// expect: 213   (sum 210, depth 3)
use "@/core"
struct Tree:
    value i32
    left &Tree
    right &Tree

    fn insert(t &Tree):
        if t.value < self.value:
            if isZero(self.left): self.left = &t
            else: self.left.insert(&t)
        else:
            if isZero(self.right): self.right = &t
            else: self.right.insert(&t)

    fn sum() i32:
        s i32 = self.value
        if not isZero(self.left): s += self.left.sum()
        if not isZero(self.right): s += self.right.sum()
        return s

    fn depth() i32:
        l i32 = 0
        r i32 = 0
        if not isZero(self.left): l = self.left.depth()
        if not isZero(self.right): r = self.right.depth()
        if l > r: return l + 1
        return r + 1

a Tree
b Tree
c Tree
d Tree
e Tree
a.value = 50
b.value = 30
c.value = 70
d.value = 20
e.value = 40
a.insert(&b)
a.insert(&c)
a.insert(&d)
a.insert(&e)
return a.sum() + a.depth()
```

---

## 060 — crazy: 2x2 matrix product with methods and ref parameters

```ura
// expect: 134   (19 + 22 + 43 + 50)
use "@/core"
struct Mat:
    v i32[]

    fn at(r i32, c i32) i32:
        return self.v[r * 2 + c]

    fn mul(o &Mat, out &Mat):
        r i32 = 0
        while r < 2:
            c i32 = 0
            while c < 2:
                s i32 = 0
                k i32 = 0
                while k < 2:
                    s += self.at(r, k) * o.at(k, c)
                    k += 1
                out.v[r * 2 + c] = s
                c += 1
            r += 1

a Mat
b Mat
c Mat
a.v = [1, 2, 3, 4]
b.v = [5, 6, 7, 8]
c.v = [0, 0, 0, 0]
a.mul(&b, &c)
return c.v[0] + c.v[1] + c.v[2] + c.v[3]
```
