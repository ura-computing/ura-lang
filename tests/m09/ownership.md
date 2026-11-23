# m09

## index

- 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies
- 041 — ownership: operator = taking the other side by value
- 042 — ownership: a method called before its definition still copies a by-value String

---

## 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies

```ura
use "@/core"
struct Bag:
    items Array<i32>
    label i32

    fn first_items() Array<i32>:
        return self.items

fn make(n i32) Array<i32>:
    a Array<i32>
    i i32 = 0
    while i < n:
        a.append(i)
        i += 1
    return a

fn total(a Array<i32>) i32:
    sum i32 = 0
    i i32 = 0
    while i < a.size():
        sum += a[i]
        i += 1
    return sum

fn main() i32:
    grid Array<Array<i32>>
    row Array<i32> = make(3)
    grid.append(row)
    grid.append(make(2))
    grid[0].append(9)
    copy Array<i32> = grid[0]
    copy.append(5)
    io::out($"{grid.size()} {grid[0].size()} {grid[1].size()} {copy.size()} {row.size()}\n")
    bag Bag
    bag.items = make(4)
    bag.items = make(2)
    items Array<i32> = bag.first_items()
    items.append(7)
    io::out($"{bag.items.size()} {items.size()} {total(items)} {total(grid[0])}\n")
    popped Array<i32> = grid.pop()
    drop(row)
    io::out($"{popped.size()} {row.size()} {grid.size()}\n")
    return 0
```

---

## 041 — ownership: operator = taking the other side by value

```ura
use "@/core"
struct Vec:
    x i32


operator =(left &Vec, other Vec):
    left.x = other.x

fn main() i32:
    return 0
```

---

## 042 — ownership: a method called before its definition still copies a by-value String

```ura
// stdout: hi one
// stdout: hi two
// stdout: kept one
// expect: 0
use "@/core"
struct Greeter:
    count i32

    fn run(name String):
        self.greet(name)
        self.greet(String::create("two"))
        io::out($"kept {name}\n")

    fn greet(name String):
        io::out($"hi {name}\n")

fn main() i32:
    g Greeter
    g.run(String::create("one"))
    return 0
```
