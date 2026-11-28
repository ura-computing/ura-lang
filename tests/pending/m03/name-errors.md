# m03

## index

- 061 — name error: a struct declared twice
- 062 — name error: unknown attribute
- 063 — name error: unknown method
- 064 — name error: self outside a method
- 065 — structs: missing name
- 066 — structs: missing colon
- 067 — structs: expression inside a struct
- 068 — structs: attribute without a type
- 069 — structs: attribute declared twice
- 070 — structs: no attributes
- 071 — structs: contains itself by value
- 072 — structs: unknown attribute
- 073 — structs: unknown method
- 074 — structs: method with wrong arguments
- 075 — structs: bare attribute inside a method
- 076 — structs: bare method call inside a method
- 077 — structs: method called on the type
- 078 — structs: self outside a method
- 079 — expressions: add a struct and a number
- 080 — expressions: compare structs
- 081 — types: struct into another struct
- 082 — struct: init a struct
- 083 — struct: modify struct attribute
- 084 — struct: method reads and writes self
- 085 — struct: overloaded method
- 086 — struct: linked list
- 087 — program: linked list walked through ref attributes
- 088 — method: a method reads self
- 089 — method: a method mutates through self, and the caller sees it
- 090 — method: a method with a return value
- 091 — method: a method calling a sibling method declared below it
- 092 — method: two structs may declare the same method name
- 094 — method: a method on a nested struct field
- 095 — method: calling a method the struct does not have
- 096 — method: calling a method on a non-struct
- 097 — method: wrong argument count to a method
- 098 — method: chaining a field access onto a call
- 099 — struct: declare a struct and a local: named type, alloca, zero-init
- 100 — struct: a field whose type is another struct
- 101 — struct: forward reference: a field type declared later in the file
- 102 — struct: a struct declared inside a function body
- 103 — struct: a struct declared inside an if block
- 104 — struct: a struct declared inside a loop body
- 105 — struct: an array-of-struct field is a fat pointer to the named type
- 106 — struct: a struct nested inside another struct's body
- 107 — struct: self reference through an array stays finite
- 108 — struct: an unused struct emits no type: LLVM drops unreferenced named types
- 109 — struct: the same struct name in three nested scopes: innermost wins
- 110 — struct: read and write a field
- 111 — struct: nested field access through a struct field
- 115 — struct: 'stack' and 'heap' are ordinary identifiers
- 116 — struct: a 'ref?' field may refer to its own struct
- 117 — struct: bind, read and write through a 'ref?' field
- 118 — struct: struct assignment copies by value
- 119 — struct: a struct as a by-value parameter
- 120 — struct: a 'ref' struct parameter mutates the caller's struct
- 121 — struct: returning a struct: the create() constructor convention
- 122 — struct: a field declared twice
- 123 — struct: a field whose type does not exist
- 124 — struct: a struct that contains itself by value
- 125 — struct: two structs that contain each other by value
- 126 — struct: a variable of a type that was never declared
- 127 — struct: reading a field the struct does not have
- 128 — struct: reading a field from a scalar
- 130 — struct: a struct declared in a block is not visible outside it
- 131 — struct: a struct declared twice in the same scope
- 132 — struct: a method and a field chain called on a ref? field
- 133 — struct: struct redefinition
- 134 — struct: accessing an undefined field
- 135 — struct: calling undefined method on struct
- 137 — struct: a struct as a parameter and as a return value
- 138 — struct: assigning a struct copies it
- 139 — struct: a struct declared inside a function
- 140 — struct: a nested struct as a parameter and a copy
- 141 — struct: a struct value {x: 10, y: 11}: missing attributes are zero, nested, returned
- 142 — struct: a struct value with an unknown attribute
- 143 — struct: a struct value given to an i32

---

## 061 — name error: a struct declared twice

```ura
// error: struct 'User' is already declared, on line 2
use "@/core"
struct User:
    a i32

struct User:
    b i32

fn main() i32:
    return 0
```

---

## 062 — name error: unknown attribute

```ura
// error: User has no attribute 'age'
// help: User has: name, id
use "@/core"
struct User:
    name i8[]
    id i32

fn main() i32:
    u User
    return u.age
```

---

## 063 — name error: unknown method

```ura
// error: User has no method 'run'
// help: User has: get
use "@/core"
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    return u.run()
```

---

## 064 — name error: self outside a method

```ura
// error: 'self' only exists inside a method
use "@/core"
fn get() i32:
    return self.a

fn main() i32:
    return 0
```

---

## 065 — structs: missing name

```ura
use "@/core"
struct :
    a i32

fn main() i32:
    return 0
```

---

## 066 — structs: missing colon

```ura
use "@/core"
struct User
    a i32

fn main() i32:
    return 0
```

---

## 067 — structs: expression inside a struct

```ura
use "@/core"
struct User:
    a i32
    1 + 2

fn main() i32:
    return 0
```

---

## 068 — structs: attribute without a type

```ura
use "@/core"
struct User:
    a

fn main() i32:
    return 0
```

---

## 069 — structs: attribute declared twice

```ura
use "@/core"
struct User:
    a i32
    a i32

fn main() i32:
    return 0
```

---

## 070 — structs: no attributes

```ura
use "@/core"
struct E:

fn main() i32:
    return 0
```

---

## 071 — structs: contains itself by value

```ura
use "@/core"
struct N:
    next N

fn main() i32:
    return 0
```

---

## 072 — structs: unknown attribute

```ura
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    return u.zz
```

---

## 073 — structs: unknown method

```ura
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    return u.zz()
```

---

## 074 — structs: method with wrong arguments

```ura
use "@/core"
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    return u.add(1, 2)
```

---

## 075 — structs: bare attribute inside a method

```ura
use "@/core"
struct User:
    value i32

    fn get() i32:
        return value

fn main() i32:
    return 0
```

---

## 076 — structs: bare method call inside a method

```ura
use "@/core"
struct User:
    value i32

    fn get() i32:
        return self.value

    fn twice() i32:
        return get() * 2

fn main() i32:
    return 0
```

---

## 077 — structs: method called on the type

```ura
use "@/core"
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    return User.get()
```

---

## 078 — structs: self outside a method

```ura
use "@/core"
fn main() i32:
    return self.a
```

---

## 079 — expressions: add a struct and a number

```ura
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    return u + 1
```

---

## 080 — expressions: compare structs

```ura
use "@/core"
struct User:
    a i32

fn main() i32:
    u User
    v User
    if u == v:
        return 1
    return 0
```

---

## 081 — types: struct into another struct

```ura
use "@/core"
struct User:
    a i32

struct Addr:
    b i32

fn main() i32:
    v Addr
    u User = v
    return 0
```

---

## 082 — struct: init a struct

```ura
use "@/core"
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
```

---

## 083 — struct: modify struct attribute

```ura
use "@/core"
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
   u.b = 10
```

---

## 084 — struct: method reads and writes self

```ura
use "@/core"
struct User:
   a i32

   fn method() i32:
      self.a = 10
      return self.a

fn main() i32:
   return 0
```

---

## 085 — struct: overloaded method

```ura
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
   return u.a
```

---

## 086 — struct: linked list

```ura
use "@/core"
struct Node:
   value i32
   next &Node

fn create(value i32) Node:
   res Node
   res.value = value
   res.next = null
   return res

fn main() i32:
   a Node = create(1)
   b Node = create(2)

   a.next = &b

   p &Node = &a
   sum i32 = 0
   while &p != null:
      sum += p.value
      p = &p.next

   // output("sum: ", sum, "\n")
```

---

## 087 — program: linked list walked through ref attributes

```ura
use "@/core"
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3
    c.next = null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while &p != null:
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```

---

## 088 — method: a method reads self

```ura
// stdout: room hall on floor 2
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

    fn describe():
        io::out($"room {self.name} on floor {self.floor}\n")

fn main() i32:
    r Room
    r.name  = "hall"
    r.floor = 2
    r.describe()
    return 0
```

---

## 089 — method: a method mutates through self, and the caller sees it

```ura
// stdout: floor = 5
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn climb(n i32):
        self.floor = self.floor + n

fn main() i32:
    r Room
    r.floor = 2
    r.climb(3)
    io::out($"floor = {r.floor}\n")
    return 0
```

---

## 090 — method: a method with a return value

```ura
// stdout: depth = 40
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn depth() i32:
        return self.floor * 10

fn main() i32:
    r Room
    r.floor = 4
    io::out($"depth = {r.depth()}\n")
    return 0
```

---

## 091 — method: a method calling a sibling method declared below it

```ura
// stdout: floor 10
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn show():
        io::out($"floor {self.twice()}\n")

    fn twice() i32:
        return self.floor * 2

fn main() i32:
    r Room
    r.floor = 5
    r.show()
    return 0
```

---

## 092 — method: two structs may declare the same method name

```ura
// stdout: room on floor 1
// stdout: tower of height 9
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn describe():
        io::out($"room on floor {self.floor}\n")

struct Tower:
    height i32

    fn describe():
        io::out($"tower of height {self.height}\n")

fn main() i32:
    r Room
    r.floor = 1
    t Tower
    t.height = 9
    r.describe()
    t.describe()
    return 0
```

---

## 094 — method: a method on a nested struct field

```ura
// stdout: entry depth 3
// expect: 0
use "@/core"

struct Room:
    floor i32

    fn depth() i32:
        return self.floor

struct Dungeon:
    entry Room

fn main() i32:
    d Dungeon
    d.entry.floor = 3
    io::out($"entry depth {d.entry.depth()}\n")
    return 0
```

---

## 095 — method: calling a method the struct does not have

```ura
// error: Struct Room has no method 'nope'; check the spelling or declare it in the struct
use "@/core"

struct Room:
    floor i32

fn main() i32:
    r Room
    r.nope()
    return 0
```

---

## 096 — method: calling a method on a non-struct

```ura
// error: Cannot call '.nope()' on i32; only a struct has methods
use "@/core"

fn main() i32:
    x i32 = 3
    x.nope()
    return 0
```

---

## 097 — method: wrong argument count to a method

```ura
// error: Wrong number of arguments to 'climb'
use "@/core"

struct Room:
    floor i32

    fn climb(n i32):
        self.floor = self.floor + n

fn main() i32:
    r Room
    r.climb()
    return 0
```

---

## 098 — method: chaining a field access onto a call

```ura
// stdout: static 7
// stdout: plain  3
// expect: 0
use "@/core"

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

fn make(f i32) Room:
    r Room
    r.floor = f
    return r

fn main() i32:
    io::out($"static {Room::create(7).floor}\n")
    io::out($"plain  {make(3).floor}\n")
    return 0
```

---

## 099 — struct: declare a struct and a local: named type, alloca, zero-init

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Player:
    name char[]
    hp   i32
    mp   i32

fn main() i32:
    p Player
    io::out("ok\n")
    return 0
```

---

## 100 — struct: a field whose type is another struct

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

fn main() i32:
    d Dungeon
    io::out("ok\n")
    return 0
```

---

## 101 — struct: forward reference: a field type declared later in the file

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Dungeon:
    name  char[]
    entry Room

struct Room:
    name  char[]
    floor i32

fn main() i32:
    d Dungeon
    io::out("ok\n")
    return 0
```

---

## 102 — struct: a struct declared inside a function body

```ura
// stdout: ok
// expect: 0
use "@/core"

fn main() i32:
    struct Local:
        x i32
        y i32

    l Local
    io::out("ok\n")
    return 0
```

---

## 103 — struct: a struct declared inside an if block

```ura
// stdout: ok
// expect: 0
use "@/core"

fn main() i32:
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

        i Inner
        io::out("ok\n")
    return 0
```

---

## 104 — struct: a struct declared inside a loop body

```ura
// stdout: ok
// expect: 0
use "@/core"

fn main() i32:
    for i in Range::create(0, 2):
        struct Tick:
            n i32

        t Tick
    io::out("ok\n")
    return 0
```

---

## 105 — struct: an array-of-struct field is a fat pointer to the named type

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Room:
    floor i32

struct Tree:
    label char[]
    kids  Room[]

fn main() i32:
    t Tree
    io::out("ok\n")
    return 0
```

---

## 106 — struct: a struct nested inside another struct's body

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Outer:
    name char[]
    struct Config:
        debug bool

    hp i32

fn main() i32:
    o Outer
    io::out("ok\n")
    return 0
```

---

## 107 — struct: self reference through an array stays finite

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Tree:
    label char[]
    kids  Tree[]

fn main() i32:
    t Tree
    io::out("ok\n")
    return 0
```

---

## 108 — struct: an unused struct emits no type: LLVM drops unreferenced named types

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Unused:
    x i32
    y i32

fn main() i32:
    io::out("ok\n")
    return 0
```

---

## 109 — struct: the same struct name in three nested scopes: innermost wins

```ura
// stdout: ok
// expect: 0
use "@/core"

struct Local:
    a char
    b char

fn main() i32:
    l0 Local

    struct Local:
        x i32
        y i32

    n i32 = 1
    if n == 1:
        struct Local:
            z char[]

        l3 Local

    l Local
    io::out("ok\n")
    return 0
```

---

## 110 — struct: read and write a field

```ura
// stdout: armory on floor 3
// stdout: floor now 13
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn main() i32:
    p Room
    p.name  = "armory"
    p.floor = 3
    io::out($"{p.name} on floor {p.floor}\n")
    p.floor = p.floor + 10
    io::out($"floor now {p.floor}\n")
    return 0
```

---

## 111 — struct: nested field access through a struct field

```ura
// stdout: keep entry gate floor 1
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

fn main() i32:
    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    io::out($"{d.name} entry {d.entry.name} floor {d.entry.floor}\n")
    return 0
```

---

## 115 — struct: 'stack' and 'heap' are ordinary identifiers

```ura
// stdout: 5 7
// expect: 0
use "@/core"

fn main() i32:
    stack i32 = 5
    heap  i32 = 7
    io::out($"{stack} {heap}\n")
    return 0
```

---

## 116 — struct: a 'ref?' field may refer to its own struct

```ura
// stdout: 1
// expect: 0
use "@/core"

struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    a.value = 1
    io::out($"{a.value}\n")
    return 0
```

---

## 117 — struct: bind, read and write through a 'ref?' field

```ura
// stdout: 20 3 20
// expect: 0
use "@/core"

struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    a.next.value = 20
    io::out($"{a.next.value} {a.next.next.value} {b.value}\n")
    return 0
```

---

## 118 — struct: struct assignment copies by value

```ura
// stdout: hall 9 / hall 2
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn main() i32:
    a Room
    a.name  = "hall"
    a.floor = 2
    b Room
    b = a
    b.floor = 9
    io::out($"{b.name} {b.floor} / {a.name} {a.floor}\n")
    return 0
```

---

## 119 — struct: a struct as a by-value parameter

```ura
// stdout: room hall@2
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn show(r Room):
    io::out($"room {r.name}@{r.floor}\n")

fn main() i32:
    a Room
    a.name  = "hall"
    a.floor = 2
    show(a)
    return 0
```

---

## 120 — struct: a 'ref' struct parameter mutates the caller's struct

```ura
// stdout: after 3
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn bump(r &Room):
    r.floor = r.floor + 1

fn main() i32:
    a Room
    a.floor = 2
    bump(&a)
    io::out($"after {a.floor}\n")
    return 0
```

---

## 121 — struct: returning a struct: the create() constructor convention

```ura
// stdout: made 4
// expect: 0
use "@/core"

struct Room:
    name  char[]
    floor i32

fn create(f i32) Room:
    r Room
    r.name  = "made"
    r.floor = f
    return r

fn describe(r Room) char[]:
    return r.name

fn main() i32:
    a Room = create(4)
    io::out($"{describe(a)} {a.floor}\n")
    return 0
```

---

## 122 — struct: a field declared twice

```ura
// error: Field 'hp' is already declared in this struct; rename it or remove the duplicate
use "@/core"

struct Player:
    name char[]
    hp   i32
    hp   i32

fn main() i32:
    p Player
    return 0
```

---

## 123 — struct: a field whose type does not exist

```ura
// error: Unknown type 'Room'
use "@/core"

struct Dungeon:
    entry Room

fn main() i32:
    d Dungeon
    return 0
```

---

## 124 — struct: a struct that contains itself by value

```ura
// error: Struct Node cannot contain itself by value; use an array or a 'ref?' so the size stays finite
use "@/core"

struct Node:
    value i32
    next  Node

fn main() i32:
    n Node
    return 0
```

---

## 125 — struct: two structs that contain each other by value

```ura
// error: Struct A cannot contain itself by value; use an array or a 'ref?' so the size stays finite
use "@/core"

struct A:
    b B

struct B:
    a A

fn main() i32:
    x A
    return 0
```

---

## 126 — struct: a variable of a type that was never declared

```ura
// error: Unknown type 'Player'
use "@/core"

fn main() i32:
    p Player
    return 0
```

---

## 127 — struct: reading a field the struct does not have

```ura
// error: Struct Player has no field 'nope'; check the spelling or declare it in the struct
use "@/core"

struct Player:
    name char[]
    hp   i32

fn main() i32:
    p Player
    io::out($"{p.nope}")
    return 0
```

---

## 128 — struct: reading a field from a scalar

```ura
// error: Cannot read '.foo' from i32; only a struct has fields
use "@/core"

fn main() i32:
    x i32 = 3
    io::out($"{x.foo}")
    return 0
```

---

## 130 — struct: a struct declared in a block is not visible outside it

```ura
// error: Unknown type 'Inner'
use "@/core"

fn main() i32:
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

    i Inner
    return 0
```

---

## 131 — struct: a struct declared twice in the same scope

```ura
// error: Redeclaration of struct 'Player'
use "@/core"

struct Player:
    hp i32

struct Player:
    mp i32

fn main() i32:
    p Player
    return 0
```

---

## 132 — struct: a method and a field chain called on a ref? field

```ura
// stdout: worth 5
// stdout: worth 7
// stdout: worth 9
// stdout: chained field 9
// expect: 0
use "@/core"

struct Item:
    worth i32
    next &Item

    pub fn create(w i32) Item:
        it Item
        it.worth = w
        return it

    fn show():
        io::out($"worth {self.worth}\n")

fn main() i32:
    head Item = Item::create(5)
    mid  Item = Item::create(7)
    tail Item = Item::create(9)
    head.next = &mid
    mid.next  = &tail
    head.show()
    head.next.show()
    head.next.next.show()
    io::out($"chained field {head.next.next.worth}\n")
    return 0
```

---

## 133 — struct: struct redefinition

```ura
// error: Redeclaration of struct 'Point'
use "@/core"
use "@/posix"

struct Point:
    x i32
    y i32

struct Point:
    a i32
    b i32

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 134 — struct: accessing an undefined field

```ura
// error: Struct Point has no field 'z'; check the spelling or declare it in the struct
use "@/core"
use "@/posix"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 1
    p.z = 5
    return 0
```

---

## 135 — struct: calling undefined method on struct

```ura
// error: Struct Point has no method 'normalize'; check the spelling or declare it in the struct
use "@/core"
use "@/posix"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 1
    p.normalize()
    return 0
```

---

## 137 — struct: a struct as a parameter and as a return value

```ura
// expect: 21   (3 + 4 + 14)
use "@/core"
struct Point:
    x i32
    y i32

fn sum(p Point) i32:
    return p.x + p.y

fn scale(p Point, k i32) Point:
    p.x *= k
    p.y *= k
    return p

fn main() i32:
    a Point
    a.x = 3
    a.y = 4
    b Point = scale(a, 2)
    return sum(a) + sum(b)
```

---

## 138 — struct: assigning a struct copies it

```ura
// expect: 15   (a stays 1 + 2, b is 5 + 7)
use "@/core"
struct Point:
    x i32
    y i32

fn main() i32:
    a Point
    a.x = 1
    a.y = 2
    b Point
    b = a
    b.x = 5
    b.y += 5
    return a.x + a.y + b.x + b.y
```

---

## 139 — struct: a struct declared inside a function

```ura
// expect: 9
use "@/core"
fn main() i32:
    struct Pair:
        a i32
        b i32
    p Pair
    p.a = 4
    p.b = 5
    return p.a + p.b
```

---

## 140 — struct: a nested struct as a parameter and a copy

```ura
// expect: 10   (width 4 + height 6)
use "@/core"
struct Point:
    x i32
    y i32

struct Rect:
    min Point
    max Point

fn size(r Rect) i32:
    return (r.max.x - r.min.x) + (r.max.y - r.min.y)

fn main() i32:
    r Rect
    r.min.x = 1
    r.min.y = 2
    r.max.x = 5
    r.max.y = 8
    c Rect = r
    return size(c)
```

---

## 141 — struct: a struct value {x: 10, y: 11}: missing attributes are zero, nested, returned

```ura
// expect: 39   (10 + 11 + 3 + 0 + 1 + 2 + 5 + 7)
use "@/core"
struct Point:
    x i32
    y i32

struct Rect:
    min Point
    max Point

fn origin() Point:
    return {x: 0, y: 0}

fn main() i32:
    p Point = {x: 10, y: 11}
    q Point = {x: 3}
    r Rect = {
        min: {x: 1, y: 2},
        max: {x: 5, y: 7},
    }
    o Point = origin()
    return p.x + p.y + q.x + q.y + r.min.x + r.min.y + r.max.x + r.max.y + o.x
```

---

## 142 — struct: a struct value with an unknown attribute

```ura
// error: 'Point' has no attribute 'z'
use "@/core"
struct Point:
    x i32
    y i32

fn main() i32:
    p Point = {x: 1, z: 2}
    return p.x
```

---

## 143 — struct: a struct value given to an i32

```ura
// error: expected 'i32', found a struct value
use "@/core"
fn main() i32:
    n i32 = {x: 1}
    return n
```
