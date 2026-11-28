# m04

## index

- 001 — template: struct with a type parameter
- 002 — template: method that takes the type parameter
- 003 — template: two type parameters
- 004 — template: two instances of the same template
- 005 — template: a template inside itself
- 006 — template: proto inside a template keeps its symbol
- 007 — template: function called with type arguments
- 008 — template: sizeof of the type parameter
- 009 — templates: too many type arguments
- 010 — templates: used without type arguments
- 011 — templates: parameter is not a name
- 012 — templates: body holds something else
- 013 — templates: missing colon
- 014 — template: declared inside a function
- 015 — templates: used outside the scope it is declared in
- 016 — template: several functions in one template, each instance renames all of them; memory_alloc, memory_copy, memory_free
- 017 — template: an array type as the type argument, sizeof(T) with T = char[]

---

## 001 — template: struct with a type parameter

```ura
use "@/core"
template<T>:
    struct Box:
        value T

fn main() i32:
    b Box<i32>
    b.value = 7
    return b.value
```

---

## 002 — template: method that takes the type parameter

```ura
use "@/core"
template<T>:
    struct Box:
        value T
        fn set(v T):
            self.value = v

fn main() i32:
    b Box<i32>
    b.set(20)
    return b.value
```

---

## 003 — template: two type parameters

```ura
use "@/core"
template<K, V>:
    struct Pair:
        key K
        value V

fn main() i32:
    p Pair<i8, i32>
    p.key = 'z'
    p.value = 5
    return p.value
```

---

## 004 — template: two instances of the same template

```ura
use "@/core"
template<T>:
    struct Box:
        value T

fn main() i32:
    a Box<i32>
    b Box<i8>
    a.value = 3
    b.value = 'a'
    return a.value
```

---

## 005 — template: a template inside itself

```ura
use "@/core"
template<T>:
    struct Box:
        value T

fn main() i32:
    n Box<Box<i32>>
    n.value.value = 9
    return n.value.value
```

---

## 006 — template: proto inside a template keeps its symbol

```ura
use "@/core"
template<T>:
    proto malloc(size i32) T[]

    struct Vec:
        arr T[]
        fn init():
            self.arr = malloc(40)

fn main() i32:
    v Vec<i32>
    v.init()
    return 0
```

---

## 007 — template: function called with type arguments

```ura
use "@/core"
template<T>:
    fn twice(v T) T:
        return v + v

fn main() i32:
    a i32 = twice<i32>(5)
    b i8 = twice<i8>(3)
    c i32 = 0
    if b == 6:
        c = 1
    return a + c
```

---

## 008 — template: sizeof of the type parameter

```ura
use "@/core"
template<T>:
    proto malloc(size i32) T[]

    struct Vec:
        arr T[]
        fn init():
            self.arr = malloc(sizeof(T) * 10)

fn main() i32:
    x Vec<i32>
    x.init()
    x.arr[3] = 7
    return x.arr[3]
```

---

## 009 — templates: too many type arguments

```ura
use "@/core"
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box<i32, i8>
   return 0
```

---

## 010 — templates: used without type arguments

```ura
use "@/core"
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box
   return 0
```

---

## 011 — templates: parameter is not a name

```ura
use "@/core"
template<3>:
   struct Box:
      value i32

fn main() i32:
   return 0
```

---

## 012 — templates: body holds something else

```ura
use "@/core"
template<T>:
   value T

fn main() i32:
   return 0
```

---

## 013 — templates: missing colon

```ura
use "@/core"
template<T>
   struct Box:
      value T

fn main() i32:
   return 0
```

---

## 014 — template: declared inside a function

```ura
use "@/core"
fn main() i32:
    template<T>:
        struct Box:
            value T
    b Box<i32>
    b.value = 4
    return b.value
```

---

## 015 — templates: used outside the scope it is declared in

```ura
use "@/core"
fn f() i32:
    template<T>:
        struct Box:
            value T
    return 0

fn main() i32:
    b Box<i32>
    return 0
```

---

## 016 — template: several functions in one template, each instance renames all of them; memory_alloc, memory_copy, memory_free

```ura
// stdout: 10 20 30
// stdout: 0 7
// expect: 0
use "@/core"
template<T>:
    fn alloc(count i32) T[]:
        return memory_alloc(count * sizeof(T)) as T[]

    fn copy(dst T[], src T[], count i32):
        memory_copy(dst as i8[], src as i8[], count * sizeof(T))

    fn free(data T[]):
        memory_free(data as i8[])

struct P:
    x i32
    y i32

fn main() i32:
    a i32[] = alloc<i32>(3)
    b i32[] = alloc<i32>(3)
    a[0] = 10
    a[1] = 20
    a[2] = 30
    copy<i32>(b, a, 3)
    io::out($"{b[0]} {b[1]} {b[2]}\n")
    ps P[] = alloc<P>(2)
    ps[1].y = 7
    io::out($"{ps[0].x} {ps[1].y}\n")
    free<i32>(a)
    free<i32>(b)
    free<P>(ps)
    return 0
```

---

## 017 — template: an array type as the type argument, sizeof(T) with T = char[]

```ura
// stdout: a b
// expect: 0
use "@/core"

fn main() i32:
    rows char[][] = new<char[]>(2)
    rows[0] = "a"
    rows[1] = "b"
    io::out($"{rows[0]} {rows[1]}\n")
    free<char[]>(rows)
    return 0
```
