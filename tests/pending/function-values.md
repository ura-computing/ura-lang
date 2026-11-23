# pending

## index

- 019 — (was milestone13 010) function: fn-typed value: assign top-level fn to a local, call indirectly
- 020 — (was milestone13 011) function: fn-typed parameter: callback passed to higher-order fn
- 021 — (was milestone13 012) function: store a function in a fn-typed variable, then call it
- 022 — (was milestone13 013) function: pass a function as an argument (higher-order)
- 023 — (was milestone13 015) function: a function-typed parameter given a value
- 024 — (was milestone13 019) function: calling a null (zero-init) function value traps with file:line

---

## 019 — (was milestone13 010) function: fn-typed value: assign top-level fn to a local, call indirectly

```ura
// stdout: 5
// stdout: 6
// expect: 0
use "@/core"

fn add(a i32, b i32) i32:
    return a + b

fn sub(a i32, b i32) i32:
    return a - b

fn main() i32:
    op fn(i32, i32) i32 = add
    io::out($"{op(2, 3)}\n")

    op = sub
    io::out($"{op(10, 4)}\n")
    return 0
```

---

## 020 — (was milestone13 011) function: fn-typed parameter: callback passed to higher-order fn

```ura
// stdout: 42
// stdout: 21
// expect: 0
use "@/core"

fn double_it(n i32) i32:
    return n * 2

fn triple_it(n i32) i32:
    return n * 3

fn apply(cb fn(i32) i32, x i32) i32:
    return cb(x)

fn main() i32:
    io::out($"{apply(double_it, 21)}\n")
    io::out($"{apply(triple_it, 7)}\n")
    return 0
```

---

## 021 — (was milestone13 012) function: store a function in a fn-typed variable, then call it

```ura
// expect: 42
use "@/core"

fn twice(n i32) i32:
    return n + n
fn main() i32:
    f fn(i32) i32 = twice
    return f(21)
```

---

## 022 — (was milestone13 013) function: pass a function as an argument (higher-order)

```ura
// expect: 42
use "@/core"

fn twice(n i32) i32:
    return n + n
fn apply(f fn(i32) i32, x i32) i32:
    return f(x)
fn main() i32:
    return apply(twice, 21)
```

---

## 023 — (was milestone13 015) function: a function-typed parameter given a value

```ura
// error: Argument 1 type mismatch in call to 'apply'
use "@/core"

fn twice(n i32) i32:
    return n + n

fn apply(f fn(i32) i32, x i32) i32:
    return f(x)

fn main() i32:
    return apply(5, 21)
```

---

## 024 — (was milestone13 019) function: calling a null (zero-init) function value traps with file:line

```ura
// stderr: runtime error: Call to a null function value
// expect: 1
use "@/core"

fn main() i32:
    f fn(i32) i32
    return f(21)
```
