# m10

## index

- 001 — loop: loop (infinite) with break + continue
- 002 — loop: for over Range::create in both directions
- 003 — loop: array-for iterates elements by value
- 004 — loop: change array elements through an index
- 005 — loop: for without a loop variable
- 006 — loop: for over a non-iterable
- 008 — loop: for over struct fields: an Array of rows and an Array of structs
- 009 — loop: Range::create with a step
- 012 — loop: break and continue inside a for
- 013 — loop: continue: skip wall tiles '#', count open spaces

---

## 001 — loop: loop (infinite) with break + continue

```ura
// stdout: 1 2 4 5
// expect: 0
use "@/core"

fn main() i32:
    i i32 = 0
    while True:
        i = i + 1
        if i == 3:
            continue
        if i == 6:
            break
        io::out($"{i} ")
    io::out("\n")
    return 0
```

---

## 002 — loop: for over Range::create in both directions

```ura
// stdout: 0 1 2 3 4 | 5 4 3 2 1
// expect: 0
use "@/core"

fn main() i32:
    for i in Range::create(0, 5):
        io::out($"{i} ")
    io::out("| ")
    for i in Range::create(5, 0):
        io::out($"{i} ")
    io::out("\n")
    return 0
```

---

## 003 — loop: array-for iterates elements by value

```ura
// stdout: sum = 60
// expect: 0
use "@/core"

fn main() i32:
    nums Array<i32> = [10, 20, 30]
    total i32 = 0
    for x in nums:
        total = total + x
    io::out($"sum = {total}\n")
    return 0
```

---

## 004 — loop: change array elements through an index

```ura
// stdout: 2 4 6 8
// expect: 0
use "@/core"

fn main() i32:
    nums Array<i32> = [1, 2, 3, 4]
    for i in Range::create(0, nums.len()):
        nums[i] = nums[i] * 2
    for x in nums:
        io::out($"{x} ")
    io::out("\n")
    return 0
```

---

## 005 — loop: for without a loop variable

```ura
// error: expected a name after 'for'
use "@/core"
fn main() i32:
    for in Range::create(0, 5):
        io::out($"{1}")
    return 0
```

---

## 006 — loop: for over a non-iterable

```ura
// error: can't loop over 'i32'
use "@/core"
fn main() i32:
    x i32 = 5
    for i in x:
        io::out($"{i}")
    return 0
```

---

## 008 — loop: for over struct fields: an Array of rows and an Array of structs

```ura
// stdout: 0 0 0
// stdout: 0 0 9
// stdout: 0 1 2
// stdout: 0 10 20
// expect: 0
use "@/core"

struct Room:
    floor i32

struct World:
    grid  Array<Array<i32>>
    rooms Array<Room>

    pub fn create() World:
        w World
        w.grid.append([0, 0, 0])
        w.grid.append([0, 0, 9])
        for i in Range::create(0, 3):
            r Room
            r.floor = i
            w.rooms.append(r)
        return w

    // nested for over a 2D field
    fn show_grid():
        for row in self.grid:
            for v in row:
                io::out($"{v} ")
            io::out("\n")

    // read the rooms, change them through an index, read them again
    fn bump():
        for r in self.rooms:
            io::out($"{r.floor} ")
        io::out("\n")
        for i in Range::create(0, self.rooms.len()):
            self.rooms[i].floor = self.rooms[i].floor * 10
        for r in self.rooms:
            io::out($"{r.floor} ")
        io::out("\n")

fn main() i32:
    w World = World::create()
    w.show_grid()
    w.bump()
    return 0
```

---

## 009 — loop: Range::create with a step

```ura
// stdout: 0 3 6 9
// stdout: 10 7 4 1
// stdout: 0 4 8 12 16
// stdout: 0
// expect: 0
use "@/core"

fn main() i32:
    for i in Range::create(0, 10, 3):
        io::out($"{i} ")
    io::out("\n")
    for i in Range::create(10, 0, -3):
        io::out($"{i} ")
    io::out("\n")
    n i32 = 4
    for i in Range::create(0, 20, n):
        io::out($"{i} ")
    io::out("\n")
    // a step wider than the span yields the start only
    for i in Range::create(0, 3, 10):
        io::out($"{i} ")
    io::out("\n")
    return 0
```

---

## 012 — loop: break and continue inside a for

```ura
// stdout: break:    0 1 2 3
// stdout: continue: 1 3 5 7
// stdout: nested:   00 01 10 11 20 21
// expect: 0
use "@/core"

fn main() i32:
    io::out("break:    ")
    for i in Range::create(0, 10):
        if i == 4:
            break
        io::out($"{i} ")

    io::out("\ncontinue: ")
    for i in Range::create(0, 8):
        if i % 2 == 0:
            continue
        io::out($"{i} ")

    // break leaves only the inner for
    io::out("\nnested:   ")
    for i in Range::create(0, 3):
        for j in Range::create(0, 3):
            if j == 2:
                break
            io::out($"{i}{j} ")
    io::out("\n")
    return 0
```

---

## 013 — loop: continue: skip wall tiles '#', count open spaces

```ura
// stdout: Open tiles: 6
// expect: 0
use "@/core"

fn main() i32:
    tiles Array<char> = ['#', '.', '#', '.', '.',
                         '#', '.', '@', '.', '#']
    open i32 = 0
    i    i32 = 0
    while i < tiles.len():
        if tiles[i] == '#':
            i = i + 1
            continue
        open = open + 1
        i = i + 1
    io::out($"Open tiles: {open}\n")
    return 0
```
