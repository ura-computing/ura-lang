# m02

## index

- 001 — array: literal and indexing
- 002 — array: write through an index
- 003 — array: index from a variable
- 004 — array: index from an expression
- 005 — array: literal made of variables
- 006 — array: compound assignment on an element
- 007 — array: loop over an array
- 008 — array: an array passed to a function
- 009 — array: a function writes into the caller's array
- 010 — array: a string is a writable i8[]
- 011 — array: an array of bools
- 012 — proto: a string in a variable
- 014 — proto: proto variadic function
- 015 — proto: chars type
- 016 — ref: write through a ref
- 017 — ref: read through a ref, then write
- 018 — ref: compound assignment through a ref
- 019 — ref: swap with ref parameters
- 020 — ref: a ref parameter changes the caller
- 021 — ref: p1 = p2 copies the value
- 022 — ref: p1 = &p2 rebinds
- 023 — ref: pass a ref parameter on to another function
- 024 — ref: a ref to a bool
- 025 — ref: a ref in a loop
- 026 — ref: take the address of a variable
- 027 — crazy: fibonacci with a memo array
- 028 — crazy: upper-case a string in place
- 029 — crazy: not and ! on bools, in a loop
- 030 — crazy: count trues with as, then a cast that wraps
- 031 — crazy: caesar cipher: char arithmetic with as
- 032 — crazy: print the alphabet with a char counter
- 033 — crazy: isNullable on array elements and chars
- 034 — crazy: transpose a 3x3 matrix stored flat
- 035 — crazy: a ref to an array element
- 036 — crazy: an array literal made of calls
- 037 — crazy: elif chain on one line each, with not / and / or
- 038 — crazy: reverse a string, palindromes, a while on one line
- 039 — crazy: sieve of eratosthenes on a bool array
- 040 — crazy: atoi: parse numbers from strings
- 041 — crazy: itoa: write a number's digits into a buffer
- 042 — crazy: print a number in hex with as
- 043 — crazy: a string builder on a global buffer
- 044 — crazy: filter with as bool, not and isNullable

---

## 001 — array: literal and indexing

```ura
// expect: 10
use "@/core"
fn main() i32:
    a i32[] = [4, 5, 6]
    return a[0] + a[2]
```

---

## 002 — array: write through an index

```ura
// expect: 9
use "@/core"
fn main() i32:
    a i32[] = [1, 2, 3]
    a[1] = 9
    return a[1]
```

---

## 003 — array: index from a variable

```ura
// expect: 9
use "@/core"
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i]
```

---

## 004 — array: index from an expression

```ura
// expect: 8
use "@/core"
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i - 1]
```

---

## 005 — array: literal made of variables

```ura
// expect: 16
use "@/core"
fn main() i32:
    x i32 = 10
    y i32 = 4
    a i32[] = [x, 2, y]
    return a[0] + a[1] + a[2]
```

---

## 006 — array: compound assignment on an element

```ura
// expect: 18   (5 + 2 + 9 + 2)
use "@/core"
fn main() i32:
    a i32[] = [1, 2, 3]
    a[0] += 4
    a[2] *= 3
    return a[0] + a[1] + a[2] + 2
```

---

## 007 — array: loop over an array

```ura
// stdout: 3 1 4 1 5
// expect: 14
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32[] = [3, 1, 4, 1, 5]
    sum i32 = 0
    i i32 = 0
    while i < 5:
        printf("%d", a[i])
        if i < 4:
            printf(" ")
        sum += a[i]
        i += 1
    printf("\n")
    return sum
```

---

## 008 — array: an array passed to a function

```ura
// expect: 6
use "@/core"
fn sum(a i32[], n i32) i32:
    total i32 = 0
    i i32 = 0
    while i < n:
        total += a[i]
        i += 1
    return total

fn main() i32:
    values i32[] = [1, 2, 3]
    return sum(values, 3)
```

---

## 009 — array: a function writes into the caller's array

```ura
// expect: 7
use "@/core"
fn set_first(a i32[], v i32):
    a[0] = v

fn main() i32:
    values i32[] = [1, 2, 3]
    set_first(values, 7)
    return values[0]
```

---

## 010 — array: a string is a writable i8[]

```ura
// stdout: xbc
// expect: 1
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    s i8[] = "abc"
    s[0] = 'x'
    printf("%s\n", s)
    if s[1] == 'b':
        return 1
    return 0
```

---

## 011 — array: an array of bools

```ura
// expect: 2
use "@/core"
fn main() i32:
    flags bool[] = [True, False, True]
    n i32 = 0
    i i32 = 0
    while i < 3:
        if flags[i]:
            n += 1
        i += 1
    return n
```

---

## 012 — proto: a string in a variable

```ura
// stdout: <abcdef>
// expect: 0
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>\n", str)
    return 0
```

---

## 014 — proto: proto variadic function

```ura
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>", str)
```

---

## 015 — proto: chars type

```ura
use "@/core"
fn main() i32:
    str i8[] = "abcdef"
```

---

## 016 — ref: write through a ref

```ura
// stdout: a = 12
// expect: 12
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    printf("a = %d\n", a)
    return a
```

---

## 017 — ref: read through a ref, then write

```ura
// stdout: p = 10, a = 15
// expect: 15
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    printf("p = %d, ", p)
    p = p + 5
    printf("a = %d\n", a)
    return a
```

---

## 018 — ref: compound assignment through a ref

```ura
// expect: 24
use "@/core"
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p += 2
    p *= 2
    return a
```

---

## 019 — ref: swap with ref parameters

```ura
// stdout: a = 2, b = 1
// expect: 21
use "@/core"
proto printf(fmt i8[], ...) i32

fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    printf("a = %d, b = %d\n", a, b)
    return a * 10 + b
```

---

## 020 — ref: a ref parameter changes the caller

```ura
// expect: 3
use "@/core"
fn inc(c &i32):
    c += 1

fn main() i32:
    n i32 = 0
    inc(&n)
    inc(&n)
    inc(&n)
    return n
```

---

## 021 — ref: p1 = p2 copies the value

```ura
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    printf("a = %d, b = %d\n", a, b)
    return a
```

---

## 022 — ref: p1 = &p2 rebinds

```ura
// stdout: a = 1, b = 9
// expect: 9
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    printf("a = %d, b = %d\n", a, b)
    return b
```

---

## 023 — ref: pass a ref parameter on to another function

```ura
// expect: 2
use "@/core"
fn add_one(p &i32):
    p += 1

fn add_two(p &i32):
    add_one(&p)
    add_one(&p)

fn main() i32:
    a i32 = 0
    add_two(&a)
    return a
```

---

## 024 — ref: a ref to a bool

```ura
// expect: 1
use "@/core"
fn main() i32:
    b bool = False
    p &bool = &b
    p = True
    if b:
        return 1
    return 0
```

---

## 025 — ref: a ref in a loop

```ura
// stdout: 1 2 3 4 5
// expect: 15
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    total i32 = 0
    p &i32 = &total
    i i32 = 1
    while i <= 5:
        printf("%d ", i)
        p += i
        i += 1
    printf("\n")
    return total
```

---

## 026 — ref: take the address of a variable

```ura
use "@/core"
fn main() i32:
    a i32 = 10
    p &i32 = &a
    return 0
```

---

## 027 — crazy: fibonacci with a memo array

```ura
// expect: 109   (fib(20) = 6765, 6765 % 256)
use "@/core"
fn fib(n i32, memo i32[]) i32:
    if n < 2: return n
    if memo[n] != 0: return memo[n]
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo)
    return memo[n]

fn main() i32:
    memo i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    return fib(20, memo) % 256
```

---

## 028 — crazy: upper-case a string in place

```ura
// stdout: HELLO, URA
// expect: 10
use "@/core"
proto printf(fmt i8[], ...) i32

fn length(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0':
        n += 1
    return n

fn upper(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        if s[i] >= 'a' and s[i] <= 'z':
            s[i] -= 32 as char
        i += 1

fn main() i32:
    s i8[] = "hello, ura"
    upper(s)
    printf("%s\n", s)
    return length(s)
```

---

## 029 — crazy: not and ! on bools, in a loop

```ura
// expect: 12
use "@/core"
fn main() i32:
    a bool = True
    b bool = !a
    n i32 = 0
    i i32 = 0
    while i < 5:
        a = not a
        if a: n += 1
        i += 1
    if not b and !(n == 3): n += 10
    return n
```

---

## 030 — crazy: count trues with as, then a cast that wraps

```ura
// expect: 6   (3 * 10, plus 1000 as i8 = -24)
use "@/core"
fn main() i32:
    flags bool[] = [True, False, True, True]
    n i32 = 0
    i i32 = 0
    while i < 4:
        n += flags[i] as i32
        i += 1
    big i32 = 1000
    return n * 10 + (big as i8) as i32
```

---

## 031 — crazy: caesar cipher: char arithmetic with as

```ura
// stdout: khoor, zruog | hello, world
// expect: 104
use "@/core"
proto printf(fmt i8[], ...) i32

fn shift(c char, k i32) char:
    if c < 'a' or c > 'z': return c
    n i32 = (c as i32) - ('a' as i32)
    n = (n + k) % 26
    return (n + ('a' as i32)) as char

fn encode(s i8[], k i32):
    i i32 = 0
    while s[i] != '\0':
        s[i] = shift(s[i], k)
        i += 1

fn main() i32:
    s i8[] = "hello, world"
    encode(s, 3)
    printf("%s | ", s)
    encode(s, 23)
    printf("%s\n", s)
    return s[0] as i32
```

---

## 032 — crazy: print the alphabet with a char counter

```ura
// stdout: abcde
// expect: 102
use "@/core"
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'a'
    while c <= 'e':
        printf("%c", c)
        c = ((c as i32) + 1) as char
    printf("\n")
    return c as i32
```

---

## 033 — crazy: isNullable on array elements and chars

```ura
// expect: 33   (3 zeros * 10, plus 1, plus 2)
use "@/core"
fn count_zero(a i32[], n i32) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        if isZero(a[i]): c += 1
        i += 1
    return c

fn main() i32:
    a i32[] = [0, 5, 0, 0, 7]
    s i8[] = "ab"
    r i32 = count_zero(a, 5) * 10
    if isZero(s[2]): r += 1
    if !isZero(s[0]): r += 2
    return r
```

---

## 034 — crazy: transpose a 3x3 matrix stored flat

```ura
// expect: 154   (trace 15 * 10, plus t[1] = 4)
use "@/core"
fn get(m i32[], w i32, r i32, c i32) i32:
    return m[r * w + c]

fn main() i32:
    m i32[] = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    t i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    r i32 = 0
    while r < 3:
        c i32 = 0
        while c < 3:
            t[c * 3 + r] = get(m, 3, r, c)
            c += 1
        r += 1
    trace i32 = 0
    i i32 = 0
    while i < 3:
        trace += t[i * 3 + i]
        i += 1
    return trace * 10 + t[1]
```

---

## 035 — crazy: a ref to an array element

```ura
// expect: 92   (10 + 22 + 60)
use "@/core"
fn inc(p &i32):
    p += 1

fn main() i32:
    a i32[] = [10, 20, 30]
    inc(&a[1])
    inc(&a[1])
    p &i32 = &a[2]
    p *= 2
    return a[0] + a[1] + a[2]
```

---

## 036 — crazy: an array literal made of calls

```ura
// expect: 50   (1 + 4 + 9 + 20 + 16)
use "@/core"
fn sq(x i32) i32:
    return x * x

fn main() i32:
    k i32 = 2
    a i32[] = [sq(1), sq(k), sq(k + 1), k * 10, sq(sq(k))]
    return a[0] + a[1] + a[2] + a[3] + a[4]
```

---

## 037 — crazy: elif chain on one line each, with not / and / or

```ura
// expect: 31
use "@/core"
fn grade(n i32) char:
    if n >= 90: return 'A'
    elif n >= 80 and not (n == 85): return 'B'
    elif n == 85 or n >= 70: return 'C'
    else: return 'F'

fn main() i32:
    r i32 = 0
    if grade(95) == 'A': r += 1
    if grade(82) == 'B': r += 2
    if grade(85) == 'C': r += 4
    if grade(72) == 'C': r += 8
    if grade(50) == 'F': r += 16
    return r
```

---

## 038 — crazy: reverse a string, palindromes, a while on one line

```ura
// stdout: gnal aru
// expect: 83   (1 + 2, plus length 8 * 10)
use "@/core"
fn len(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0': n += 1
    return n

fn is_palindrome(s i8[]) bool:
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        if s[i] != s[j]: return False
        i += 1
        j -= 1
    return True

fn reverse(s i8[]):
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        t char = s[i]
        s[i] = s[j]
        s[j] = t
        i += 1
        j -= 1

proto printf(fmt i8[], ...) i32

fn main() i32:
    a i8[] = "racecar"
    b i8[] = "ura lang"
    reverse(b)
    printf("%s\n", b)
    r i32 = 0
    if is_palindrome(a): r += 1
    if not is_palindrome(b): r += 2
    return r + len(b) * 10
```

---

## 039 — crazy: sieve of eratosthenes on a bool array

```ura
// expect: 77   (2 + 3 + 5 + 7 + 11 + 13 + 17 + 19)
use "@/core"
fn main() i32:
    composite bool[] = [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False]
    sum i32 = 0
    i i32 = 2
    while i < 20:
        if not composite[i]:
            sum += i
            j i32 = i * i
            while j < 20:
                composite[j] = True
                j += i
        i += 1
    return sum
```

---

## 040 — crazy: atoi: parse numbers from strings

```ura
// expect: 85   (123 - 45 + 7)
use "@/core"
fn atoi(s i8[]) i32:
    n i32 = 0
    i i32 = 0
    neg bool = False
    if s[0] == '-':
        neg = True
        i = 1
    while s[i] >= '0' and s[i] <= '9':
        n = n * 10 + (s[i] as i32) - ('0' as i32)
        i += 1
    if neg: return -n
    return n

fn main() i32:
    return atoi("123") + atoi("-45") + atoi("7x")
```

---

## 041 — crazy: itoa: write a number's digits into a buffer

```ura
// stdout: 9071
// expect: 57   ('9')
use "@/core"
proto printf(fmt i8[], ...) i32

fn itoa(n i32, out i8[]):
    digits i8[] = "0000000000"
    k i32 = 0
    if n == 0:
        out[0] = '0'
        out[1] = '\0'
        return
    while n > 0:
        digits[k] = ((n % 10) + ('0' as i32)) as char
        n /= 10
        k += 1
    i i32 = 0
    while i < k:
        out[i] = digits[k - 1 - i]
        i += 1
    out[k] = '\0'

fn main() i32:
    buf i8[] = "............"
    itoa(9071, buf)
    printf("%s\n", buf)
    return buf[0] as i32
```

---

## 042 — crazy: print a number in hex with as

```ura
// stdout: 0000beef
// expect: 98   ('b')
use "@/core"
proto printf(fmt i8[], ...) i32

fn hex_digit(n i32) char:
    if n < 10: return (('0' as i32) + n) as char
    return (('a' as i32) + n - 10) as char

fn to_hex(n i32, out i8[]):
    i i32 = 7
    while i >= 0:
        out[i] = hex_digit(n % 16)
        n /= 16
        i -= 1
    out[8] = '\0'

buf i8[] = "........."
to_hex(48879, buf)
printf("%s\n", buf)
return buf[4] as i32
```

---

## 043 — crazy: a string builder on a global buffer

```ura
// stdout: x=1234, y=7
// expect: 12
use "@/core"
proto printf(fmt i8[], ...) i32

out i8[] = "                                "
len i32 = 0

fn put(c char):
    out[len] = c
    len += 1

fn put_num(n i32):
    if n >= 10: put_num(n / 10)
    put((('0' as i32) + n % 10) as char)

fn put_str(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        put(s[i])
        i += 1

put_str("x=")
put_num(1234)
put_str(", y=")
put_num(7)
put('\0')
printf("%s\n", out)
return len
```

---

## 044 — crazy: filter with as bool, not and isNullable

```ura
// expect: 42   (4 odd, 2 even, zeros skipped)
use "@/core"
fn count_if(a i32[], n i32, odd bool) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        is_odd bool = (a[i] % 2) as bool
        if is_odd == odd and not isZero(a[i]): c += 1
        i += 1
    return c

a i32[] = [0, 1, 2, 3, 4, 5, -3, 0]
return count_if(a, 8, True) * 10 + count_if(a, 8, False)
```
