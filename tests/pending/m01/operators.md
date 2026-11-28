# m01

## index

- 220 — operator: all arithmetic ops in dungeon combat context
- 221 — operator: arithmetic with precedence (2*3 + 20/4 - 1)
- 222 — operator: reassign an existing variable (2*10 + 3)
- 223 — operator: modulo
- 224 — operator: all comparison ops on hero/enemy stats
- 225 — operator: comparisons produce bools (with precedence)
- 226 — operator: compound assignment += -= *= /= %=
- 227 — operator: logical ops: &&, ||, !, and, or, not
- 228 — operator: and / or with comparisons and precedence
- 229 — operator: type mismatch between operands
- 230 — operator: a comparison result is a bool, not an i32
- 231 — variable: hero stats: declare all common types
- 232 — variable: reassignment: hp loss, xp gain, mp drain
- 233 — variable: all primitive types in one place
- 234 — variable: a use before a shadowing declaration binds to the outer one
- 235 — variable: redeclaring a variable in the same scope
- 236 — variable: using a name that was never declared
- 237 — variable: assigning to a name that was never declared
- 238 — variable: a variable does not outlive the block that declared it
- 239 — variable: redeclaring a global
- 240 — variable: a bool takes only True or False
- 241 — variable: assigning a string or a struct to an integer
- 242 — proto: a string passed to a fn with an i8[] param
- 243 — variables: unary minus on a variable and a parameter
- 244 — global scope: a top-level statement runs before main
- 245 — function: early bare return in a void function
- 246 — bool: i1 and bool are the same type
- 247 — tokenizer: a block comment over two lines, /*/ does not close it
- 248 — calls: arguments over several lines
- 249 — variables: a number literal takes the type of its i8 target
- 250 — variables: a variable first set inside an if still starts at zero
- 251 — variables: a variable with the name of a parameter
- 252 — unsigned: u8 wraps, u32 divides and compares unsigned, u64, casts
- 253 — unsigned: a number that doesn't fit in u8
- 254 — variables: the type written before the name
- 255 — bool: not, with and / or and in if / elif
- 256 — bool: not on an i32
- 257 — float: 2.5f is an f32
- 258 — float: a negative f64 literal into an f32

---

## 220 — operator: all arithmetic ops in dungeon combat context

```ura
// stdout: atk + def:       33
// stdout: atk - def:       17
// stdout: kills * 50:      150
// stdout: total / count:   25
// stdout: atk % xp_cap:    25
// stdout: 10 + 5:          15
// stdout: base_atk + atk:  35
// stdout: 2 + 3 * 4:       14
// stdout: (2 + 3) * 4:     20
// stdout: add:  1.800000
// stdout: sub:  1.200000
// stdout: mul:  0.450000
// stdout: div:  5.000000
// stdout: lt:   False
// stdout: gt:   True
// stdout: lte:  False
// stdout: gte:  True
// stdout: clamp(0.52): 0.520000
// stdout: clamp(0.01): 0.050000
// expect: 0
use "@/core"

fn base_atk() i32:
    return 10

fn clamp(t f32) f32:
    if t < 0.05:
        return 0.05
    return t

fn main() i32:
    atk     i32 = 25
    def     i32 = 8
    kills   i32 = 3
    total   i32 = 100
    count   i32 = 4
    xp_cap  i32 = 70

    // var + var
    io::out($"atk + def:       {atk + def}\n")
    // var - var
    io::out($"atk - def:       {atk - def}\n")
    // var * var
    io::out($"kills * 50:      {kills * 50}\n")
    // var / var
    io::out($"total / count:   {total / count}\n")
    // var % var
    io::out($"atk % xp_cap:    {atk % xp_cap}\n")
    // val + val
    io::out($"10 + 5:          {10 + 5}\n")
    // fn + var
    io::out($"base_atk + atk:  {base_atk() + atk}\n")
    // chained with precedence
    io::out($"2 + 3 * 4:       {2 + 3 * 4}\n")
    io::out($"(2 + 3) * 4:     {(2 + 3) * 4}\n")

    a f32 = 1.5
    b f32 = 0.3

    io::out($"add:  {a + b}\n")  // 1.8
    io::out($"sub:  {a - b}\n")  // 1.2
    io::out($"mul:  {a * b}\n")  // 0.45
    io::out($"div:  {a / b}\n")  // 5.0

    io::out($"lt:   {a < b}\n")  // 0 (false)
    io::out($"gt:   {a > b}\n")  // 1 (true)
    io::out($"lte:  {a <= b}\n")  // 0
    io::out($"gte:  {a >= b}\n")  // 1

    t f32 = 0.8 - 0.28
    io::out($"clamp(0.52): {clamp(t)}\n")  // 0.52
    io::out($"clamp(0.01): {clamp(0.01)}\n")  // 0.05
    return 0
```

---

## 221 — operator: arithmetic with precedence (2*3 + 20/4 - 1)

```ura
// expect: 10
use "@/core"

fn main() i32:
    return 2 * 3 + 20 / 4 - 1
```

---

## 222 — operator: reassign an existing variable (2*10 + 3)

```ura
// expect: 23
use "@/core"

fn main() i32:
    a i32 = 2
    a = a * 10 + 3
    return a
```

---

## 223 — operator: modulo

```ura
// expect: 2
use "@/core"

fn main() i32:
    return 17 % 5
```

---

## 224 — operator: all comparison ops on hero/enemy stats

```ura
// stdout: hero_hp == min_hp: True
// stdout: hero_hp != orc_hp: True
// stdout: orc_hp < hero_hp:  True
// stdout: orc_hp > hero_hp:  False
// stdout: orc_hp <= 60:      True
// stdout: hero_hp >= 100:    True
// stdout: strong enemy
// stdout: hero is at full health and winning
// expect: 0
use "@/core"

fn enemy_atk() i32:
    return 18

fn main() i32:
    hero_hp  i32 = 100
    orc_hp   i32 = 60
    min_hp   i32 = 100

    io::out($"hero_hp == min_hp: {hero_hp == min_hp}\n")   // True
    io::out($"hero_hp != orc_hp: {hero_hp != orc_hp}\n")   // True
    io::out($"orc_hp < hero_hp:  {orc_hp < hero_hp}\n")   // True
    io::out($"orc_hp > hero_hp:  {orc_hp > hero_hp}\n")   // False
    io::out($"orc_hp <= 60:      {orc_hp <= 60}\n")   // True
    io::out($"hero_hp >= 100:    {hero_hp >= 100}\n")   // True

    // comparison with fn call
    if enemy_atk() > 15:
        io::out("strong enemy\n")
    else:
        io::out("weak enemy\n")

    // chained via if
    if hero_hp == 100 and orc_hp < hero_hp:
        io::out("hero is at full health and winning\n")
    return 0
```

---

## 225 — operator: comparisons produce bools (with precedence)

```ura
// expect: 3
use "@/core"

fn main() i32:
    a i32 = 5
    b i32 = 9
    t bool = a < b
    u bool = a == 5
    v bool = b != a
    w bool = b >= 9
    x bool = a <= 4
    y bool = a > b
    z bool = a is 5
    m bool = a + 1 < b * 2
    return 3
```

---

## 226 — operator: compound assignment += -= *= /= %=

```ura
// stdout: 15
// stdout: 12
// stdout: 48
// stdout: 8
// stdout: 3
// expect: 0
use "@/core"

fn main() i32:
    x i32 = 10
    x += 5
    io::out($"{x}\n")
    x -= 3
    io::out($"{x}\n")
    x *= 4
    io::out($"{x}\n")
    x /= 6
    io::out($"{x}\n")
    x %= 5
    io::out($"{x}\n")
    return 0
```

---

## 227 — operator: logical ops: &&, ||, !, and, or, not

```ura
// stdout: can_proceed: True
// stdout: escape:      False
// stdout: blocked:     False
// stdout: hero fights
// stdout: hero advances
// stdout: !!alive: True
// expect: 0
use "@/core"

fn is_alive(hp i32) bool:
    return hp > 0

fn has_key(keys i32) bool:
    return keys > 0

fn main() i32:
    hp       i32  = 60
    keys     i32  = 1
    exhausted bool = False

    // &&
    can_proceed bool = is_alive(hp) && has_key(keys) && !exhausted
    io::out($"can_proceed: {can_proceed}\n")   // True

    // ||
    escape bool = hp < 10 || keys == 0
    io::out($"escape:      {escape}\n")   // False

    // !
    blocked bool = !can_proceed
    io::out($"blocked:     {blocked}\n")   // False

    // and / or / not keywords
    if is_alive(hp) and not exhausted:
        io::out("hero fights\n")

    if hp < 10 or keys == 0:
        io::out("hero retreats\n")
    else:
        io::out("hero advances\n")

    // f64 negation
    alive bool = is_alive(hp)
    io::out($"!!alive: {!(!alive)}\n")   // True
    return 0
```

---

## 228 — operator: and / or with comparisons and precedence

```ura
// stdout: True
// stdout: True
// stdout: False
// stdout: True
// expect: 0
use "@/core"

fn main() i32:
    a i32 = 5
    b i32 = 9
    io::out($"{a < b and b == 9}\n")
    io::out($"{a > b or b == 9}\n")
    io::out($"{a < b and a > b}\n")
    io::out($"{a < b or a > b}\n")
    return 0
```

---

## 229 — operator: type mismatch between operands

```ura
// error: Cannot use '+' with fn and i32
use "@/core"

fn twice(n i32) i32:
    return n + n
fn main() i32:
    return twice + 1
```

---

## 230 — operator: a comparison result is a bool, not an i32

```ura
// error: Cannot assign bool to i32
use "@/core"

fn main() i32:
    a i32 = 5
    n i32 = a < 9
    return 0
```

---

## 231 — variable: hero stats: declare all common types

```ura
// stdout: === Aldric enters the dungeon ===
// stdout: HP: 100 | MP: 50 | Speed: 1.500000
// stdout: Alive: True
// expect: 0
use "@/core"

fn main() i32:
    name  char[] = "Aldric"
    hp    i32   = 100
    mp    i32   = 50
    alive bool  = True
    speed f32 = 1.5
    io::out($"=== {name} enters the dungeon ===\n")
    io::out($"HP: {hp} | MP: {mp} | Speed: {speed}\n")
    io::out($"Alive: {alive}\n")
    return 0
```

---

## 232 — variable: reassignment: hp loss, xp gain, mp drain

```ura
// stdout: After combat:
// stdout: HP: 88
// stdout: MP: 40
// stdout: XP: 50
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 100
    mp i32 = 50
    xp i32 = 0

    hp = hp - 12
    mp -= 10
    xp += 50

    io::out("After combat:\n")
    io::out($"HP: {hp}\n")
    io::out($"MP: {mp}\n")
    io::out($"XP: {xp}\n")
    return 0
```

---

## 233 — variable: all primitive types in one place

```ura
// stdout: name:  Aldric
// stdout: hp:    100
// stdout: xp:    999999
// stdout: level: 12
// stdout: grade: S
// stdout: alive: True
// stdout: ratio: 0.850000
// expect: 0
use "@/core"

fn main() i32:
    name   char[] = "Aldric"
    hp     i32   = 100
    xp     i64  = 999999 as i64
    level  i16 = 12 as i16
    grade  char  = 'S'
    alive  bool  = True
    ratio  f32 = 0.85

    io::out($"name:  {name}\n")
    io::out($"hp:    {hp}\n")
    io::out($"xp:    {xp}\n")
    io::out($"level: {level}\n")
    io::out($"grade: {grade}\n")
    io::out($"alive: {alive}\n")
    io::out($"ratio: {ratio}\n")
    return 0
```

---

## 234 — variable: a use before a shadowing declaration binds to the outer one

```ura
// stdout: outer 1
// stdout: inner 2
// stdout: after 1
// stdout: loop y 6
// stdout: shadow y 0
// expect: 0
use "@/core"

fn pick(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 1
    if 1 < 2:
        // resolves to the OUTER x, even though an inner x follows
        io::out($"outer {x}\n")
        x i32 = 2
        io::out($"inner {x}\n")
    io::out($"after {x}\n")

    y i32 = pick(3)
    while y > 0:
        io::out($"loop y {y}\n")
        y i32 = 0
        io::out($"shadow y {y}\n")
        break
    return 0
```

---

## 235 — variable: redeclaring a variable in the same scope

```ura
// error: Redeclaration of variable 'a'
use "@/core"

fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

---

## 236 — variable: using a name that was never declared

```ura
// error: Undeclared variable 'y'
use "@/core"

fn main() i32:
    x i32 = 5
    io::out($"{y}\n")
    return 0
```

---

## 237 — variable: assigning to a name that was never declared

```ura
// error: Undeclared variable 'score'
use "@/core"

fn main() i32:
    score = 100
    io::out($"{score}\n")
    return 0
```

---

## 238 — variable: a variable does not outlive the block that declared it

```ura
// error: Undeclared variable 'y'
use "@/core"

fn main() i32:
    x i32 = 1
    if x > 0:
        y i32 = 5
    io::out($"{y}\n")
    return 0
```

---

## 239 — variable: redeclaring a global

```ura
// error: Redeclaration of variable 'score'
use "@/core"

score i32 = 0
score i32 = 10

fn main() i32:
    io::out($"{score}\n")
    return 0
```

---

## 240 — variable: a bool takes only True or False

```ura
// error: Cannot assign i32 to bool
use "@/core"

fn main() i32:
    b bool = 5
    return 0
```

---

## 241 — variable: assigning a string or a struct to an integer

```ura
// error: Cannot assign array to i32
use "@/core"

struct Point:
    x i32

fn main() i32:
    a i32 = "hello"
    p Point
    b i32 = p
    io::out($"{a}{b}\n")
    return 0
```

---

## 242 — proto: a string passed to a fn with an i8[] param

```ura
// stdout: hi ura
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn greet(name i8[]):
    printf("hi %s\n", name)

fn main() i32:
    greet("ura")
    return 0
```

---

## 243 — variables: unary minus on a variable and a parameter

```ura
// expect: 12   (neg(-5) + 7)
use "@/core"
fn neg(n i32) i32:
    return -n

fn main() i32:
    a i32 = 5
    b i32 = -a
    return neg(b) + -(-7)
```

---

## 244 — global scope: a top-level statement runs before main

```ura
// expect: 15
use "@/core"
count i32 = 10
count += 5

fn main() i32:
    return count
```

---

## 245 — function: early bare return in a void function

```ura
// expect: 3
use "@/core"
total i32 = 0

fn add_small(n i32):
    if n > 2:
        return
    total += n

fn main() i32:
    add_small(1)
    add_small(5)
    add_small(2)
    return total
```

---

## 246 — bool: i1 and bool are the same type

```ura
// expect: 2
use "@/core"
fn main() i32:
    a i1 = True
    b bool = a
    if b:
        return 2
    return 0
```

---

## 247 — tokenizer: a block comment over two lines, /*/ does not close it

```ura
// expect: 5
use "@/core"
/*/ this comment opens with slash star slash
   and goes on until here */
fn main() i32:
    return 5
```

---

## 248 — calls: arguments over several lines

```ura
// expect: 6
use "@/core"
fn add(a i32, b i32, c i32) i32:
    return a + b + c

fn main() i32:
    return add(
        1,
        2,
    3)
```

---

## 249 — variables: a number literal takes the type of its i8 target

```ura
// expect: 1
use "@/core"
fn main() i32:
    c i8 = 5
    if c == 5:
        return 1
    return 0
```

---

## 250 — variables: a variable first set inside an if still starts at zero

```ura
// expect: 13
use "@/core"
fn main() i32:
    a i32 = 10
    b i32
    c i32
    if a > 50:
        c = 7
    if a > 5:
        b = 3
    return a + b + c
```

---

## 251 — variables: a variable with the name of a parameter

```ura
use "@/core"
fn f(r i32) i32:
    r i32 = 2
    return r

fn main() i32:
    return f(1)
```

---

## 252 — unsigned: u8 wraps, u32 divides and compares unsigned, u64, casts

```ura
use "@/core"
fn main() i32:
    small u8 = 250
    small += 10
    big u32 = 4000000000
    half u32 = big / 2
    wide u64 = 18000000000
    io::out($"{typeof(small)} {small} {big} {half} {big > 100} {wide}\n")
    io::out($"{sizeof(u16)} {big as f64} {-1 as u8}\n")
    return 0
```

---

## 253 — unsigned: a number that doesn't fit in u8

```ura
use "@/core"
fn main() i32:
    x u8 = 256
    return 0
```

---

## 254 — variables: the type written before the name

```ura
use "@/core"
struct Point:
    x i32

fn main() i32:
    i32 x = 1
    i8[] s = "ab"
    Point p
    p.x = x
    return p.x
```

---

## 255 — bool: not, with and / or and in if / elif

```ura
use "@/core"
fn main() i32:
    x i32 = 5
    big bool = not (x < 3)
    if not big or x == 0:
        io::out("no\n")
    elif not (x > 10) and big:
        io::out($"yes {not big}\n")
    return 0
```

---

## 256 — bool: not on an i32

```ura
use "@/core"
fn main() i32:
    x i32 = 5
    return not x
```

---

## 257 — float: 2.5f is an f32

```ura
// stdout: -2.500000 -1.000000 0.500000
use "@/core"
fn main() i32:
    a f32 = -2.5f
    b f32 = 1.5f + a
    c f32 = 0.5
    io::out($"{a} {b} {c}\n")
    return 0
```

---

## 258 — float: a negative f64 literal into an f32

```ura
// error: expected 'f32', found 'f64'
use "@/core"
fn main() i32:
    a f32 = -2.5
    return 0
```
