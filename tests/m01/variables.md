# m01

## index

- 001 — variables: declare with a value
- 002 — variables: reassign
- 003 — variables: use another variable
- 004 — variables: compound assignment
- 005 — variables: declare variable
- 006 — array: declarations without a value start at zero
- 007 — bool: a bool variable
- 008 — bool: False skips the branch
- 009 — bool: a comparison stored in a bool, reassigned
- 010 — bool: bool parameters and return values
- 011 — bool: or between bool variables
- 012 — bool: a bool as a while condition
- 013 — bool: a bool default value is False
- 015 — char: print chars
- 016 — char: compare chars
- 017 — char: order of chars
- 018 — char: escaped chars
- 019 — char: reassign a char
- 020 — char: char parameter and return value
- 021 — char: a char decides a loop
- 022 — char: default value of a char is '\0'
- 023 — char: declaring char/i8 variable
- 024 — if: if / elif / else chain
- 025 — if: every comparison
- 026 — if: and / or
- 027 — if: body on the same line
- 028 — if: nested ifs
- 029 — if: no branch taken: code after the if runs
- 030 — if: recursion
- 031 — if: a variable declared in a block
- 032 — if: basic if
- 033 — if: basic if/elif/else chain
- 034 — if: ifs with comparision operators
- 035 — if: nested ifs
- 036 — if: logic operators and/or
- 037 — while: count to 10
- 038 — while: break
- 039 — while: continue
- 040 — while: nested loops
- 041 — while: break leaves only the inner loop
- 042 — while: continue in the inner loop
- 043 — while: return from inside a loop
- 044 — while: loop that never runs
- 045 — while: loop in a function, called from main
- 046 — while: basic while loop
- 047 — while: break
- 048 — while: continue
- 049 — while: nested whiles
- 050 — while: get crazy
- 051 — function: parameters and calls
- 052 — function: call before the definition
- 053 — function: nested calls
- 054 — function: a parameter is a copy
- 055 — function: call result in a variable
- 056 — function: void function and bare return
- 057 — function: function inside a function
- 058 — function: calling function before declaration
- 059 — function: math op on functions
- 060 — function: bare return in a void function
- 061 — proto: printf with a number
- 062 — proto: several extra arguments
- 063 — proto: exit from C
- 064 — proto: escapes
- 065 — proto: printf returns how many chars it wrote
- 066 — proto: printf in a loop
- 067 — proto: proto function
- 068 — proto: escaped characters
- 069 — global scope: a global variable changed by a function
- 070 — crazy: mutual recursion, used before it's declared
- 071 — crazy: gcd, printf with many arguments
- 072 — crazy: collatz: a parameter changed in a loop
- 073 — crazy: the 20th prime: continue, break, a bool function
- 074 — crazy: return from two nested loops
- 075 — crazy: operator precedence soup
- 076 — crazy: negative numbers: signed / and %
- 077 — crazy: ackermann: deep recursion, a call as an argument
- 078 — crazy: one name declared in sibling blocks
- 079 — crazy: script: a global counts recursive calls
- 080 — crazy: script: nested loops with continue and break at file level

---

## 001 — variables: declare with a value

```ura
// expect: 10
use "@/core"
fn main() i32:
    a i32 = 10
    return a
```

---

## 002 — variables: reassign

```ura
// expect: 20
use "@/core"
fn main() i32:
    a i32 = 10
    a = 20
    return a
```

---

## 003 — variables: use another variable

```ura
// expect: 30
use "@/core"
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    return a
```

---

## 004 — variables: compound assignment

```ura
// expect: 21
use "@/core"
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    a += 5
    a -= 3
    a *= 2
    a /= 4
    a %= 5
    return a + b
```

---

## 005 — variables: declare variable

```ura
use "@/core"
fn main() i32:
    a i32 = 10
    a = 20
    return 0
```

---

## 006 — array: declarations without a value start at zero

```ura
// expect: 0
use "@/core"
fn main() i32:
    x i32
    b bool
    c char
    if b or c != '\0':
        return 1
    return x
```

---

## 007 — bool: a bool variable

```ura
// expect: 1
use "@/core"
fn main() i32:
    flag bool = True
    if flag:
        return 1
    return 0
```

---

## 008 — bool: False skips the branch

```ura
// expect: 0
use "@/core"
fn main() i32:
    flag bool = False
    if flag:
        return 1
    return 0
```

---

## 009 — bool: a comparison stored in a bool, reassigned

```ura
// expect: 2
use "@/core"
fn main() i32:
    a i32 = 5
    big bool = a > 3
    n i32 = 0
    if big:
        n += 1
    big = a > 9
    if big:
        n += 10
    big = False
    if big:
        n += 100
    if big == False:
        n += 1
    return n
```

---

## 010 — bool: bool parameters and return values

```ura
// expect: 3
use "@/core"
fn is_even(n i32) bool:
    return n % 2 == 0

fn both(a bool, b bool) bool:
    return a and b

fn main() i32:
    n i32 = 0
    if is_even(4):
        n += 1
    if both(is_even(2), is_even(6)):
        n += 2
    if both(is_even(2), is_even(7)):
        n += 10
    return n
```

---

## 011 — bool: or between bool variables

```ura
// expect: 5
use "@/core"
fn main() i32:
    a bool = False
    b bool = True
    n i32 = 0
    if a or b:
        n += 5
    if a and b:
        n += 50
    return n
```

---

## 012 — bool: a bool as a while condition

```ura
// expect: 4
use "@/core"
fn main() i32:
    i i32 = 0
    going bool = True
    while going:
        i += 1
        going = i < 4
    return i
```

---

## 013 — bool: a bool default value is False

```ura
// expect: 7
use "@/core"
fn main() i32:
    flag bool
    if flag:
        return 1
    return 7
```

---

## 015 — char: print chars

```ura
// stdout: a b
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
    printf("%c %c\n", c1, c2)
    return 0
```

---

## 016 — char: compare chars

```ura
// expect: 2
use "@/core"
fn main() i32:
    c char = 'x'
    n i32 = 0
    if c == 'x':
        n += 1
    if c != 'y':
        n += 1
    if c == 'z':
        n += 10
    return n
```

---

## 017 — char: order of chars

```ura
// expect: 3
use "@/core"
fn main() i32:
    c char = 'm'
    n i32 = 0
    if c > 'a':
        n += 1
    if c < 'z':
        n += 1
    if c >= 'm' and c <= 'm':
        n += 1
    return n
```

---

## 018 — char: escaped chars

```ura
// stdout: [
// stdout: ]
// stdout: [	]
// stdout: ['] [\]
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    nl char = '\n'
    tab char = '\t'
    printf("[%c]\n", nl)
    printf("[%c]\n", tab)
    printf("[%c] [%c]\n", '\'', '\\')
    return 0
```

---

## 019 — char: reassign a char

```ura
// stdout: q
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'p'
    c = 'q'
    printf("%c\n", c)
    return 0
```

---

## 020 — char: char parameter and return value

```ura
// stdout: Y N
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn answer(yes bool) char:
    if yes:
        return 'Y'
    return 'N'

fn show(c char):
    printf("%c", c)

fn main() i32:
    show(answer(True))
    printf(" ")
    show(answer(False))
    printf("\n")
    return 0
```

---

## 021 — char: a char decides a loop

```ura
// expect: 4
use "@/core"
fn main() i32:
    c char = 'a'
    n i32 = 0
    while c != 'e':
        n += 1
        if n == 1:
            c = 'b'
        elif n == 2:
            c = 'c'
        elif n == 3:
            c = 'd'
        else:
            c = 'e'
    return n
```

---

## 022 — char: default value of a char is '\0'

```ura
// expect: 1
use "@/core"
fn main() i32:
    c char
    if c == '\0':
        return 1
    return 0
```

---

## 023 — char: declaring char/i8 variable

```ura
use "@/core"
proto exit(code i32)

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
```

---

## 024 — if: if / elif / else chain

```ura
// expect: 123
use "@/core"
fn grade(n i32) i32:
    if n < 10:
        return 1
    elif n < 20:
        return 2
    else:
        return 3

fn main() i32:
    return grade(5) * 100 + grade(15) * 10 + grade(25)
```

---

## 025 — if: every comparison

```ura
// expect: 6
use "@/core"
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a == 5:
        n += 1
    if a != 4:
        n += 1
    if a < 6:
        n += 1
    if a <= 5:
        n += 1
    if a > 4:
        n += 1
    if a >= 5:
        n += 1
    if a > 5:
        n += 100
    return n
```

---

## 026 — if: and / or

```ura
// expect: 11
use "@/core"
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 1 and a < 10:
        n += 1
    if a < 1 or a == 5:
        n += 10
    if a < 1 and a == 5:
        n += 100
    return n
```

---

## 027 — if: body on the same line

```ura
// expect: 3
use "@/core"
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n += 1
    if a > 9: n += 10
    if a == 5: n += 2
    return n
```

---

## 028 — if: nested ifs

```ura
// expect: 1
use "@/core"
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7:
            return 1
        return 2
    return 3
```

---

## 029 — if: no branch taken: code after the if runs

```ura
// expect: 9
use "@/core"
fn main() i32:
    a i32 = 1
    if a == 2:
        a = 100
    elif a == 3:
        a = 200
    return a + 8
```

---

## 030 — if: recursion

```ura
use "@/core"
fn fact(n i32) i32:
    if n <= 1:
        return 1
    return n * fact(n - 1)

fn main() i32:
    return fact(5)
```

---

## 031 — if: a variable declared in a block

```ura
// expect: 3
use "@/core"
fn main() i32:
    a i32 = 1
    if a == 1:
        b i32 = 2
        a = a + b
    return a
```

---

## 032 — if: basic if

```ura
use "@/core"
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    return 2
```

---

## 033 — if: basic if/elif/else chain

```ura
use "@/core"
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    elif a < 2:
        return 2
    else:
        return 3
    return a
```

---

## 034 — if: ifs with comparision operators

```ura
use "@/core"
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n = n + 1
    if a > 5: n = n + 10
    if a < 6: n = n + 1
    if a < 5: n = n + 10
    if a >= 5: n = n + 1
    if a >= 6: n = n + 10
    if a <= 5: n = n + 1
    if a <= 4: n = n + 10
    if a == 5: n = n + 1
    if a == 4: n = n + 10
    if a != 4: n = n + 1
    if a != 5: n = n + 10
    return n
```

---

## 035 — if: nested ifs

```ura
use "@/core"
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7: return 1
        return 2
    return 3
```

---

## 036 — if: logic operators and/or

```ura
use "@/core"
fn main() i32:
    a i32 = 5
    if a > 1 and a < 10:
        return 1
    return 0
```

---

## 037 — while: count to 10

```ura
// expect: 10
use "@/core"
fn main() i32:
    a i32 = 0
    while a < 10:
        a += 1
    return a
```

---

## 038 — while: break

```ura
// expect: 3
use "@/core"
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a += 1
    return a
```

---

## 039 — while: continue

```ura
// expect: 25   (1 + 3 + 5 + 7 + 9)
use "@/core"
fn main() i32:
    i i32 = 0
    sum i32 = 0
    while i < 10:
        i += 1
        if i % 2 == 0:
            continue
        sum += i
    return sum
```

---

## 040 — while: nested loops

```ura
// expect: 12   (3 x 4)
use "@/core"
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c += 1
            j += 1
        i += 1
    return c
```

---

## 041 — while: break leaves only the inner loop

```ura
// expect: 3
use "@/core"
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while c < 100:
            c += 1
            break
        i += 1
    return c
```

---

## 042 — while: continue in the inner loop

```ura
// expect: 6   (j = 1 and j = 3, three times)
use "@/core"
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            j += 1
            if j % 2 == 0:
                continue
            c += 1
        i += 1
    return c
```

---

## 043 — while: return from inside a loop

```ura
// expect: 7
use "@/core"
fn main() i32:
    i i32 = 0
    while i < 100:
        if i == 7:
            return i
        i += 1
    return 0
```

---

## 044 — while: loop that never runs

```ura
// expect: 5
use "@/core"
fn main() i32:
    a i32 = 5
    while a < 0:
        a = 100
    return a
```

---

## 045 — while: loop in a function, called from main

```ura
// expect: 55   (1 + 2 + ... + 10)
use "@/core"
fn sum_to(n i32) i32:
    total i32 = 0
    i i32 = 1
    while i <= n:
        total += i
        i += 1
    return total

fn main() i32:
    return sum_to(10)
```

---

## 046 — while: basic while loop

```ura
use "@/core"
fn main() i32:
    a i32 = 0
    while a < 10:
        a = a + 1
    return a
```

---

## 047 — while: break

```ura
use "@/core"
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a = a + 1
    return a
```

---

## 048 — while: continue

```ura
use "@/core"
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            a = a + 2
            continue
        a = a + 1
    return a
```

---

## 049 — while: nested whiles

```ura
// break leaves only the innermost loop
// expect: 3
use "@/core"
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while True: c = c + 1 break
        i = i + 1
    return c
```

---

## 050 — while: get crazy

```ura
use "@/core"
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c = c + 1
            j = j + 1
        i = i + 1
    return c
```

---

## 051 — function: parameters and calls

```ura
// expect: 25
use "@/core"
fn square(x i32) i32:
    return x * x

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(square(3), square(4))
```

---

## 052 — function: call before the definition

```ura
// expect: 18
use "@/core"
fn main() i32:
    return later(2)

fn later(n i32) i32:
    return n * 9
```

---

## 053 — function: nested calls

```ura
// expect: 3
use "@/core"
fn inc(x i32) i32:
    return x + 1

fn main() i32:
    return inc(inc(inc(0)))
```

---

## 054 — function: a parameter is a copy

```ura
// expect: 56
use "@/core"
fn bump(x i32) i32:
    x += 1
    return x

fn main() i32:
    a i32 = 5
    b i32 = bump(a)
    return a * 10 + b
```

---

## 055 — function: call result in a variable

```ura
// expect: 14
use "@/core"
fn twice(n i32) i32:
    return n * 2

fn main() i32:
    a i32 = twice(3)
    b i32 = twice(a) + 2
    return b
```

---

## 056 — function: void function and bare return

```ura
// expect: 4
use "@/core"
fn nothing():
    return

fn main() i32:
    nothing()
    return 4
```

---

## 057 — function: function inside a function

```ura
// expect: 7
use "@/core"
fn main() i32:
    fn help() i32:
        return 7
    return help()
```

---

## 058 — function: calling function before declaration

```ura
use "@/core"
fn main() i32:
    return later()

fn later() i32:
    return 9
```

---

## 059 — function: math op on functions

```ura
use "@/core"
fn square(x i32) i32:
    return x * x

fn main() i32:
    return square(3) + square(4)
```

---

## 060 — function: bare return in a void function

```ura
use "@/core"
fn foo(a i32):
    return

fn foo(a bool):
    return

fn main() i32:
    foo(1)
    foo(True)
    return 7
```

---

## 061 — proto: printf with a number

```ura
// stdout: hello 42
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("hello %d\n", 42)
    return 0
```

---

## 062 — proto: several extra arguments

```ura
// stdout: 1 + 2 = 3
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    printf("%d + %d = %d\n", a, b, a + b)
    return 0
```

---

## 063 — proto: exit from C

```ura
// expect: 10
use "@/core"
proto exit(code i32)

fn main() i32:
    exit(10)
    return 0
```

---

## 064 — proto: escapes

```ura
// stdout: a	b
// stdout: "quoted" \ done
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("a\tb\n")
    printf("\"quoted\" \\ done\n")
    return 0
```

---

## 065 — proto: printf returns how many chars it wrote

```ura
// stdout: abc
// expect: 3
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    return printf("abc")
```

---

## 066 — proto: printf in a loop

```ura
// stdout: 0 1 2
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    i i32 = 0
    while i < 3:
        printf("%d", i)
        if i < 2:
            printf(" ")
        i += 1
    printf("\n")
    return 0
```

---

## 067 — proto: proto function

```ura
use "@/core"
proto exit(code i32)

fn main() i32:
    exit(10)
```

---

## 068 — proto: escaped characters

```ura
use "@/core"
proto printf(s i8[], ...)

fn main() i32:
    printf("tab\there\n")
    return 0
```

---

## 069 — global scope: a global variable changed by a function

```ura
// expect: 4
use "@/core"
count i32 = 0

fn tick():
    count += 1

fn main() i32:
    i i32 = 0
    while i < 4:
        tick()
        i += 1
    return count
```

---

## 070 — crazy: mutual recursion, used before it's declared

```ura
// expect: 3
use "@/core"
fn is_even(n i32) bool:
    if n == 0: return True
    return is_odd(n - 1)

fn is_odd(n i32) bool:
    if n == 0: return False
    return is_even(n - 1)

fn main() i32:
    r i32 = 0
    if is_even(10): r += 1
    if is_odd(7): r += 2
    if is_even(3): r += 4
    return r
```

---

## 071 — crazy: gcd, printf with many arguments

```ura
// stdout: 12 1 4
// expect: 13
use "@/core"
proto printf(fmt i8[], ...) i32

fn gcd(a i32, b i32) i32:
    if b == 0: return a
    return gcd(b, a % b)

fn main() i32:
    x i32 = gcd(84, 36)
    y i32 = gcd(17, 5)
    printf("%d %d %d\n", x, y, gcd(x, 4))
    return x + y
```

---

## 072 — crazy: collatz: a parameter changed in a loop

```ura
// expect: 111
use "@/core"
fn steps(n i32) i32:
    c i32 = 0
    while n != 1:
        if n % 2 == 0:
            n /= 2
        else:
            n = n * 3 + 1
        c += 1
    return c

fn main() i32:
    return steps(27)
```

---

## 073 — crazy: the 20th prime: continue, break, a bool function

```ura
// expect: 71
use "@/core"
fn is_prime(n i32) bool:
    if n < 2: return False
    d i32 = 2
    while d * d <= n:
        if n % d == 0: return False
        d += 1
    return True

fn main() i32:
    count i32 = 0
    n i32 = 0
    while n < 100:
        n += 1
        if is_prime(n) == False: continue
        count += 1
        if count == 20: break
    return n
```

---

## 074 — crazy: return from two nested loops

```ura
// expect: 67
use "@/core"
fn main() i32:
    i i32 = 1
    while i < 20:
        j i32 = i
        while j < 20:
            if i * j == 42 and i + j == 13:
                return i * 10 + j
            j += 1
        i += 1
    return 0
```

---

## 075 — crazy: operator precedence soup

```ura
// expect: 217   (a 5, b 0, c 5, d 2: 5*40 + 0*7 + 5*3 + 2)
use "@/core"
fn main() i32:
    a i32 = 1 + 2 * 3 - 4 / 2 % 3
    b i32 = (1 + 2) * (3 - 4 / 2) % 3
    c i32 = 10 - 3 - 2
    d i32 = 100 / 10 / 5
    return a * 40 + b * 7 + c * 3 + d
```

---

## 076 — crazy: negative numbers: signed / and %

```ura
// expect: 98   (-3 - 1 - 3 + 5, plus 100)
use "@/core"
fn main() i32:
    a i32 = -7 / 2
    b i32 = -7 % 3
    c i32 = 0 - a * b
    d i32 = -(-5)
    return a + b + c + d + 100
```

---

## 077 — crazy: ackermann: deep recursion, a call as an argument

```ura
// expect: 61
use "@/core"
fn ack(m i32, n i32) i32:
    if m == 0: return n + 1
    if n == 0: return ack(m - 1, 1)
    return ack(m - 1, ack(m, n - 1))

fn main() i32:
    return ack(3, 3)
```

---

## 078 — crazy: one name declared in sibling blocks

```ura
// expect: 130   (10 + 20 + 0 + 100)
use "@/core"
fn main() i32:
    x i32 = 1
    r i32 = 0
    if x == 1:
        y i32 = 10
        r += y
    if x == 1:
        y i32 = 20
        r += y
    i i32 = 0
    while i < 2:
        z i32 = i * 100
        r += z
        i += 1
    return r
```

---

## 079 — crazy: script: a global counts recursive calls

```ura
// stdout: 55 177
// expect: 177
use "@/core"
proto printf(fmt i8[], ...) i32

calls i32 = 0

fn fib(n i32) i32:
    calls += 1
    if n < 2: return n
    return fib(n - 1) + fib(n - 2)

r i32 = fib(10)
printf("%d %d\n", r, calls)
return calls
```

---

## 080 — crazy: script: nested loops with continue and break at file level

```ura
// expect: 37
use "@/core"
found i32 = -1
i i32 = 0
while i < 10:
    i += 1
    if i % 2 == 0: continue
    j i32 = 0
    while j < 10:
        if i * j == 21:
            found = i * 10 + j
            break
        j += 1
    if found != -1: break
return found
```
