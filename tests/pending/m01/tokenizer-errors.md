# m01

## index

- 081 — tokenizer error: unknown character
- 082 — tokenizer error: unclosed string
- 083 — tokenizer error: unclosed character
- 084 — tokenizer error: character with two letters
- 085 — tokenizer error: unknown escape
- 086 — tokenizer error: unclosed comment
- 087 — tokenizer error: number too big for i32
- 088 — tokenizer: indentation that is not a multiple of 4 works
- 089 — tokenizer error: two errors in one file: both are shown
- 090 — parser error: missing : after a function header
- 091 — parser error: missing ( after a function name
- 092 — parser error: missing ) after the parameters
- 093 — parser error: a function with no name
- 094 — parser error: a parameter with no type
- 095 — parser error: missing : after an if condition
- 096 — parser error: missing : after a while condition
- 097 — parser error: else with a condition
- 098 — parser error: elif without an if
- 099 — parser error: unclosed (
- 100 — parser error: nothing after =
- 101 — parser error: nothing after an operator
- 102 — parser error: unclosed ( in a call
- 103 — name error: undeclared variable
- 104 — name error: a typo in a variable name
- 105 — name error: a variable used before its declaration
- 106 — name error: a variable declared twice
- 107 — name error: unknown type
- 108 — name error: undeclared function
- 109 — name error: a typo in a function name
- 110 — name error: wrong number of arguments
- 111 — name error: a function declared twice
- 112 — name error: break outside a loop
- 113 — name error: continue outside a loop
- 114 — name error: calling a variable
- 115 — tokenizer: unexpected character
- 116 — tokenizer: unterminated character
- 117 — tokenizer: empty character
- 118 — tokenizer: character with two letters
- 119 — tokenizer: integer too big for i32
- 120 — functions: missing name
- 121 — functions: unclosed parameters
- 122 — functions: parameter without a type
- 123 — functions: missing comma between parameters
- 124 — functions: variadic not last
- 125 — functions: unknown return type
- 126 — functions: unknown parameter type
- 127 — functions: parameter declared twice
- 128 — functions: defined twice
- 129 — functions: overloaded on return type only
- 130 — functions: return at top level
- 131 — structs: dot on a non-struct
- 132 — variables: unknown type
- 133 — variables: declared twice
- 134 — variables: used before its declaration
- 135 — variables: used outside its block
- 136 — variables: assigned without a declaration
- 137 — expressions: missing operand
- 138 — expressions: unclosed parenthesis
- 139 — expressions: unexpected parenthesis
- 140 — expressions: assign to a literal
- 141 — expressions: assign to a call
- 142 — expressions: compare a number and a string
- 143 — expressions: add a number and a string
- 144 — expressions: and on numbers
- 145 — calls: unknown function
- 146 — calls: too many arguments
- 147 — calls: wrong argument type
- 148 — calls: missing comma between arguments
- 149 — calls: unclosed call
- 150 — calls: trailing comma
- 151 — calls: call a variable
- 152 — calls: use a void result
- 153 — types: string into i8
- 154 — types: bool into i32
- 155 — types: number into bool
- 156 — types: number too big for i8
- 157 — types: wrong return type
- 158 — types: value returned from a void fn
- 159 — types: bare return in a fn with a type
- 160 — types: number as an if condition
- 161 — types: number as a while condition
- 162 — control flow: continue outside a loop
- 163 — control flow: elif without if
- 164 — control flow: else without if
- 165 — control flow: missing colon after if
- 166 — control flow: missing colon after elif
- 167 — control flow: missing colon after else
- 168 — control flow: missing colon after while
- 169 — control flow: if without a condition
- 170 — control flow: else with a condition
- 171 — recovery: three analysis errors
- 172 — recovery: two syntax errors in two functions
- 173 — diagnostics: error past line 9
- 174 — function: bare return in a void function
- 175 — type: compound assignment/unary operator
- 176 — type: boolean type
- 177 — builtin: puts: simple messages, char comparison on hero grade
- 178 — builtin: proto printf: hello world with a format string
- 179 — builtin: custom putchar via write: print dungeon tiles
- 180 — if: basic if: first floor greeting
- 181 — if: if-else: alive or dead check
- 182 — if: if-elif-else: floor difficulty rating
- 183 — function: void fn: announce dungeon floor
- 184 — function: fn with return value + single-line fns
- 185 — function: multi-fn: clamp, damage, is_dead
- 186 — function: return an integer literal
- 187 — function: declare a variable and return it
- 188 — function: function parameters and a direct call
- 189 — function: forward reference: call a function declared later
- 190 — function: a call used as an argument to another call
- 191 — function: value parameters: take_damage
- 192 — function: a by-value parameter does not change the caller
- 193 — function: redeclaring a function
- 194 — function: calling a function that does not exist
- 195 — function: too many arguments
- 196 — function: an argument of the wrong type
- 197 — function: two parameters with the same name
- 198 — function: a variadic call missing its required arguments
- 199 — function: a bare return in a void function
- 200 — lexer: unterminated string literal
- 201 — lexer: unterminated character literal
- 202 — lexer: unterminated block comment
- 203 — lexer: an unexpected character
- 204 — lexer: an unknown escape sequence
- 205 — lexer: as with no type after it
- 206 — lexer: a call with no closing parenthesis
- 207 — lexer: a function declaration missing its colon
- 208 — lexer: a function declaration missing its name
- 209 — lexer: a parameter missing its type
- 210 — function: a fn without a return type returns nothing
- 211 — lexer: if and while missing their colon
- 212 — lexer: proto not followed by fn
- 213 — lexer: a struct declaration missing its name
- 214 — lexer: an unexpected token in an expression
- 215 — lexer: member access with no member name after .
- 216 — literal: bool literals: declare, zero-init, bool function, reassign
- 217 — literal: char literals: declare, print, escapes, in a function
- 218 — loop: basic while: count enemy kills

---

## 081 — tokenizer error: unknown character

```ura
// error: unknown character '`'
// help: remove it
use "@/core"
fn main() i32:
    x i32 = 5 ` 3
    return x
```

---

## 082 — tokenizer error: unclosed string

```ura
// error: unclosed string
// help: add " at the end
use "@/core"
fn main() i32:
    s i8[] = "abc
    return 0
```

---

## 083 — tokenizer error: unclosed character

```ura
// error: unclosed character
// help: add ' after the character
use "@/core"
fn main() i32:
    c char = 'a
    return 0
```

---

## 084 — tokenizer error: character with two letters

```ura
// error: a character holds one letter, not 2
// help: use "ab" for a string
use "@/core"
fn main() i32:
    c char = 'ab'
    return 0
```

---

## 085 — tokenizer error: unknown escape

```ura
// error: unknown escape '\q'
// help: the escapes are \n \t \r \0 \\ \" \'
use "@/core"
fn main() i32:
    s i8[] = "a\qb"
    return 0
```

---

## 086 — tokenizer error: unclosed comment

```ura
// error: unclosed comment
// help: add */ to close it
use "@/core"
fn main() i32:
    /* this never ends
    return 0
```

---

## 087 — tokenizer error: number too big for i32

```ura
// error: 99999999999 doesn't fit in i32
// help: i32 goes from -2147483648 to 2147483647
use "@/core"
fn main() i32:
    x i32 = 99999999999
    return 0
```

---

## 088 — tokenizer: indentation that is not a multiple of 4 works

```ura
// expect: 7
use "@/core"
fn main() i32:
   a i32 = 7
   if a == 7:
         return a
   return 0
```

---

## 089 — tokenizer error: two errors in one file: both are shown

```ura
// error: unknown character '`'
// error: unknown character '@'
use "@/core"
fn main() i32:
    x i32 = 5 ` 3
    y i32 = 2 @ 1
    return 0
```

---

## 090 — parser error: missing : after a function header

```ura
// error: expected ':' after the function header
// help: use fn main() i32:
use "@/core"
fn main() i32
    return 0
```

---

## 091 — parser error: missing ( after a function name

```ura
// error: expected '(' after the function name
// help: use fn main() i32:
use "@/core"
fn main i32:
    return 0
```

---

## 092 — parser error: missing ) after the parameters

```ura
// error: expected ')' after the parameters
// help: use fn add(a i32, b i32) i32:
use "@/core"
fn add(a i32, b i32 i32:
    return a + b

fn main() i32:
    return 0
```

---

## 093 — parser error: a function with no name

```ura
// error: expected a name after 'fn'
// help: use fn name() i32:
use "@/core"
fn () i32:
    return 0
```

---

## 094 — parser error: a parameter with no type

```ura
// error: parameter 'a' has no type
// help: use a i32
use "@/core"
fn twice(a) i32:
    return a * 2

fn main() i32:
    return 0
```

---

## 095 — parser error: missing : after an if condition

```ura
// error: expected ':' after the if condition
// help: use if x > 1:
use "@/core"
fn main() i32:
    x i32 = 2
    if x > 1
        return 1
    return 0
```

---

## 096 — parser error: missing : after a while condition

```ura
// error: expected ':' after the while condition
// help: use while i < 3:
use "@/core"
fn main() i32:
    i i32 = 0
    while i < 3
        i += 1
    return i
```

---

## 097 — parser error: else with a condition

```ura
// error: else has no condition
// help: use elif x > 1:
use "@/core"
fn main() i32:
    x i32 = 2
    if x == 0:
        return 0
    else x > 1:
        return 1
    return 2
```

---

## 098 — parser error: elif without an if

```ura
// error: elif without an if before it
// help: use if x == 1:
use "@/core"
fn main() i32:
    x i32 = 1
    elif x == 1:
        return 1
    return 0
```

---

## 099 — parser error: unclosed (

```ura
// error: expected ')' to close this '('
// help: use x i32 = (1 + 2)
use "@/core"
fn main() i32:
    x i32 = (1 + 2
    return x
```

---

## 100 — parser error: nothing after =

```ura
// error: expected a value after '='
use "@/core"
fn main() i32:
    x i32 =
    return 0
```

---

## 101 — parser error: nothing after an operator

```ura
// error: expected a value after '+'
use "@/core"
fn main() i32:
    x i32 = 1 +
    return x
```

---

## 102 — parser error: unclosed ( in a call

```ura
// error: expected ')' to close the call to 'add'
// help: use return add(1, 2)
use "@/core"
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1, 2
```

---

## 103 — name error: undeclared variable

```ura
// error: undeclared variable 'y'
// help: declare it first, like: y i32 = 0
use "@/core"
fn main() i32:
    x i32 = 1
    return y
```

---

## 104 — name error: a typo in a variable name

```ura
// error: undeclared variable 'cuont'
// help: did you mean 'count'?
use "@/core"
fn main() i32:
    count i32 = 3
    return cuont
```

---

## 105 — name error: a variable used before its declaration

```ura
// error: 'x' is used before its declaration, on line 5
// help: move the declaration above this line
use "@/core"
fn main() i32:
    y i32 = x + 1
    x i32 = 1
    return y
```

---

## 106 — name error: a variable declared twice

```ura
// error: 'x' is already declared, on line 4
// help: use x = 2
use "@/core"
fn main() i32:
    x i32 = 1
    x i32 = 2
    return x
```

---

## 107 — name error: unknown type

```ura
// error: unknown type 'Point'
// help: declare it: struct Point:
use "@/core"
fn main() i32:
    p Point
    return 0
```

---

## 108 — name error: undeclared function

```ura
// error: no function named 'foo'
use "@/core"
fn main() i32:
    return foo()
```

---

## 109 — name error: a typo in a function name

```ura
// error: no function named 'ad'
// help: did you mean 'add'?
use "@/core"
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return ad(1, 2)
```

---

## 110 — name error: wrong number of arguments

```ura
// error: 'add' takes 2 arguments, not 1
// help: fn add(a i32, b i32) i32
use "@/core"
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1)
```

---

## 111 — name error: a function declared twice

```ura
// error: function 'f' is already declared, on line 2
use "@/core"
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return f()
```

---

## 112 — name error: break outside a loop

```ura
// error: break outside a loop or match
// help: break only works inside a while or a match
use "@/core"
fn main() i32:
    break
    return 0
```

---

## 113 — name error: continue outside a loop

```ura
// error: continue outside a loop
// help: continue only works inside a while
use "@/core"
fn main() i32:
    if True:
        continue
    return 0
```

---

## 114 — name error: calling a variable

```ura
// error: 'x' is a variable, not a function
use "@/core"
fn main() i32:
    x i32 = 1
    return x()
```

---

## 115 — tokenizer: unexpected character

```ura
use "@/core"
fn main() i32:
    x i32 = 5 `
    return x
```

---

## 116 — tokenizer: unterminated character

```ura
use "@/core"
fn main() i32:
    c i8 = 'a
    return 0
```

---

## 117 — tokenizer: empty character

```ura
// expect: 0
use "@/core"
fn main() i32:
    c i8 = ''
    if c == '\0':
        return 0
    return 1
```

---

## 118 — tokenizer: character with two letters

```ura
use "@/core"
fn main() i32:
    c i8 = 'ab'
    return 0
```

---

## 119 — tokenizer: integer too big for i32

```ura
use "@/core"
fn main() i32:
    x i32 = 99999999999
    return x
```

---

## 120 — functions: missing name

```ura
use "@/core"
fn () i32:
    return 0

fn main() i32:
    return 0
```

---

## 121 — functions: unclosed parameters

```ura
use "@/core"
fn add(a i32:
    return a

fn main() i32:
    return 0
```

---

## 122 — functions: parameter without a type

```ura
use "@/core"
fn f(a) i32:
    return 0

fn main() i32:
    return 0
```

---

## 123 — functions: missing comma between parameters

```ura
use "@/core"
fn f(a i32 b i32) i32:
    return a

fn main() i32:
    return 0
```

---

## 124 — functions: variadic not last

```ura
use "@/core"
proto p(..., a i32) i32

fn main() i32:
    return 0
```

---

## 125 — functions: unknown return type

```ura
use "@/core"
fn f() Foo:
    return 0

fn main() i32:
    return 0
```

---

## 126 — functions: unknown parameter type

```ura
use "@/core"
fn f(a Foo) i32:
    return 0

fn main() i32:
    return 0
```

---

## 127 — functions: parameter declared twice

```ura
use "@/core"
fn f(a i32, a i32) i32:
    return a

fn main() i32:
    return 0
```

---

## 128 — functions: defined twice

```ura
use "@/core"
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return 0
```

---

## 129 — functions: overloaded on return type only

```ura
use "@/core"
fn f() i32:
    return 1

fn f() i8:
    return 'a'

fn main() i32:
    return 0
```

---

## 130 — functions: return at top level

```ura
use "@/core"
return 1

fn main() i32:
    return 0
```

---

## 131 — structs: dot on a non-struct

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    return a.b
```

---

## 132 — variables: unknown type

```ura
use "@/core"
fn main() i32:
    x Foo
    return 0
```

---

## 133 — variables: declared twice

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

---

## 134 — variables: used before its declaration

```ura
use "@/core"
fn main() i32:
    b i32 = a
    a i32 = 1
    return b
```

---

## 135 — variables: used outside its block

```ura
use "@/core"
fn main() i32:
    if True:
        x i32 = 1
    return x
```

---

## 136 — variables: assigned without a declaration

```ura
use "@/core"
fn main() i32:
    x = 5
    return 0
```

---

## 137 — expressions: missing operand

```ura
use "@/core"
fn main() i32:
    a i32 = 1 +
    return a
```

---

## 138 — expressions: unclosed parenthesis

```ura
use "@/core"
fn main() i32:
    a i32 = (1 + 2
    return a
```

---

## 139 — expressions: unexpected parenthesis

```ura
use "@/core"
fn main() i32:
    a i32 = 1 + 2)
    return a
```

---

## 140 — expressions: assign to a literal

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    1 = a
    return a
```

---

## 141 — expressions: assign to a call

```ura
use "@/core"
fn f() i32:
    return 1

fn main() i32:
    f() = 3
    return 0
```

---

## 142 — expressions: compare a number and a string

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    if a == "x":
        return 1
    return 0
```

---

## 143 — expressions: add a number and a string

```ura
use "@/core"
fn main() i32:
    return 1 + "a"
```

---

## 144 — expressions: and on numbers

```ura
use "@/core"
fn main() i32:
    if 1 and 2:
        return 1
    return 0
```

---

## 145 — calls: unknown function

```ura
use "@/core"
fn main() i32:
    return zz()
```

---

## 146 — calls: too many arguments

```ura
use "@/core"
fn f(a i32) i32:
    return a

fn main() i32:
    return f(1, 2)
```

---

## 147 — calls: wrong argument type

```ura
use "@/core"
fn f(a i32) i32:
    return a

fn main() i32:
    return f("a")
```

---

## 148 — calls: missing comma between arguments

```ura
use "@/core"
fn f(a i32, b i32) i32:
    return a

fn main() i32:
    return f(1 2)
```

---

## 149 — calls: unclosed call

```ura
use "@/core"
fn f(a i32, b i32) i32:
    return a

fn main() i32:
    x i32 = f(1, 2
    return x
```

---

## 150 — calls: trailing comma

```ura
use "@/core"
fn f(a i32) i32:
    return a

fn main() i32:
    return f(1,)
```

---

## 151 — calls: call a variable

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    return a()
```

---

## 152 — calls: use a void result

```ura
use "@/core"
fn v():
    return

fn main() i32:
    x i32 = v()
    return x
```

---

## 153 — types: string into i8

```ura
use "@/core"
fn main() i32:
    x i8 = "a"
    return 0
```

---

## 154 — types: bool into i32

```ura
use "@/core"
fn main() i32:
    a i32 = True
    return a
```

---

## 155 — types: number into bool

```ura
use "@/core"
fn main() i32:
    b bool = 1
    return 0
```

---

## 156 — types: number too big for i8

```ura
use "@/core"
fn main() i32:
    x i8 = 300
    return 0
```

---

## 157 — types: wrong return type

```ura
use "@/core"
fn f() i32:
    return "a"

fn main() i32:
    return 0
```

---

## 158 — types: value returned from a void fn

```ura
use "@/core"
fn f():
    return 1

fn main() i32:
    return 0
```

---

## 159 — types: bare return in a fn with a type

```ura
use "@/core"
fn f() i32:
    return

fn main() i32:
    return 0
```

---

## 160 — types: number as an if condition

```ura
use "@/core"
fn main() i32:
    if 5:
        return 1
    return 0
```

---

## 161 — types: number as a while condition

```ura
use "@/core"
fn main() i32:
    a i32 = 3
    while a:
        a -= 1
    return a
```

---

## 162 — control flow: continue outside a loop

```ura
use "@/core"
fn main() i32:
    continue
    return 0
```

---

## 163 — control flow: elif without if

```ura
use "@/core"
fn main() i32:
    elif True:
        return 1
    return 0
```

---

## 164 — control flow: else without if

```ura
use "@/core"
fn main() i32:
    else:
        return 1
    return 0
```

---

## 165 — control flow: missing colon after if

```ura
use "@/core"
fn main() i32:
    a i32 = 2
    if a > 1
        return 1
    return 0
```

---

## 166 — control flow: missing colon after elif

```ura
use "@/core"
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    elif a > 0
        return 2
    return 0
```

---

## 167 — control flow: missing colon after else

```ura
use "@/core"
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    else
        return 2
```

---

## 168 — control flow: missing colon after while

```ura
use "@/core"
fn main() i32:
    a i32 = 2
    while a > 0
        a -= 1
    return a
```

---

## 169 — control flow: if without a condition

```ura
use "@/core"
fn main() i32:
    if:
        return 1
    return 0
```

---

## 170 — control flow: else with a condition

```ura
use "@/core"
fn main() i32:
    a i32 = 2
    if a > 1:
        return 1
    else a > 0:
        return 2
    return 0
```

---

## 171 — recovery: three analysis errors

```ura
use "@/core"
fn main() i32:
    a i32 = zz
    b i32 = u.x
    return yy()
```

---

## 172 — recovery: two syntax errors in two functions

```ura
use "@/core"
fn f() i32
    return 1

fn g() i32:
    return (2

fn main() i32:
    return 0
```

---

## 173 — diagnostics: error past line 9

```ura
use "@/core"
fn main() i32:
    a i32 = 1
    b i32 = 2
    c i32 = 3
    d i32 = 4
    e i32 = 5
    f i32 = 6
    g i32 = 7
    h i32 = 8
    i i32 = 9
    return zz
```

---

## 174 — function: bare return in a void function

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

## 175 — type: compound assignment/unary operator

```ura
use "@/core"
fn main() i32:
    a i32 = -10
    a += +2
    a *= 4
    a /= 2
    a -= 2
    a %= 3
```

---

## 176 — type: boolean type

```ura
use "@/core"
fn main() i32:
   flag bool = True
   if flag:
      return 1
   return 0
```

---

## 177 — builtin: puts: simple messages, char comparison on hero grade

```ura
// stdout: S-rank hero — unstoppable
// expect: 0
use "@/core"

proto puts(str char[]) i32

fn is_s_rank(grade char) bool:
    return grade == 'S'

fn main() i32:
    grade char = 'S'
    if is_s_rank(grade):
        puts("S-rank hero — unstoppable")
    else:
        puts("keep training")
    return 0
```

---

## 178 — builtin: proto printf: hello world with a format string

```ura
// stdout: hello ura 42
// expect: 0
use "@/core"

proto printf(fmt char[], ...) i32

fn main() i32:
    s char[] = "ura"
    printf("hello %s %d\n", s, 42)
    return 0
```

---

## 179 — builtin: custom putchar via write: print dungeon tiles

```ura
// stdout: DUNGEON
// expect: 0
use "@/core"

fn putchar(c char) i32:
    buf char[] = [c, '\0']
    io::write(1, buf, 1)
    return 0

fn main() i32:
    putchar('D')
    putchar('U')
    putchar('N')
    putchar('G')
    putchar('E')
    putchar('O')
    putchar('N')
    putchar('\n')
    return 0
```

---

## 180 — if: basic if: first floor greeting

```ura
// stdout: Fresh start
// expect: 0
use "@/core"

fn main() i32:
    floor i32 = 1
    if floor == 1:
        io::out("Fresh start\n")
    return 0
```

---

## 181 — if: if-else: alive or dead check

```ura
// stdout: Hero has fallen
// expect: 0
use "@/core"

fn main() i32:
    hp i32 = 0
    if hp > 0:
        io::out("Hero is alive\n")
    else:
        io::out("Hero has fallen\n")
    return 0
```

---

## 182 — if: if-elif-else: floor difficulty rating

```ura
// stdout: Normal
// expect: 0
use "@/core"

fn main() i32:
    floor i32 = 6
    if floor < 3:
        io::out("Easy\n")
    elif floor < 7:
        io::out("Normal\n")
    else:
        io::out("Hard\n")
    return 0
```

---

## 183 — function: void fn: announce dungeon floor

```ura
// stdout: === Entering floor 1 ===
// stdout: === Entering floor 5 ===
// stdout: BOSS: The Lich King appears!
// expect: 0
use "@/core"

fn announce_floor(floor i32):
    io::out($"=== Entering floor {floor} ===\n")

fn announce_boss(name char[]):
    io::out($"BOSS: {name} appears!\n")

fn main() i32:
    announce_floor(1)
    announce_floor(5)
    announce_boss("The Lich King")
    return 0
```

---

## 184 — function: fn with return value + single-line fns

```ura
// stdout: Floor 3 bonus: 150
// stdout: Hero alive:    True
// stdout: Boss floor:    True
// expect: 0
use "@/core"

fn victory_bonus(floor i32) i32:
    return floor * 50

fn is_alive(hp i32) bool: return hp > 0
fn is_boss_floor(floor i32) bool: return floor == 10

fn main() i32:
    bonus i32 = victory_bonus(3)
    io::out($"Floor 3 bonus: {bonus}\n")
    io::out($"Hero alive:    {is_alive(80)}\n")
    io::out($"Boss floor:    {is_boss_floor(10)}\n")
    return 0
```

---

## 185 — function: multi-fn: clamp, damage, is_dead

```ura
// stdout: Orc takes 17 damage
// stdout: Orc dead: True
// expect: 0
use "@/core"

fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo:
        return lo
    if val > hi:
        return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn main() i32:
    d i32 = damage(25, 8)
    io::out($"Orc takes {d} damage\n")
    io::out($"Orc dead: {is_dead(d - 60)}\n")
    return 0
```

---

## 186 — function: return an integer literal

```ura
// expect: 42
use "@/core"

fn main() i32:
    return 42
```

---

## 187 — function: declare a variable and return it

```ura
// expect: 7
use "@/core"

fn main() i32:
    a i32 = 7
    return a
```

---

## 188 — function: function parameters and a direct call

```ura
// expect: 42
use "@/core"

fn add(a i32, b i32) i32:
    return a + b
fn main() i32:
    return add(40, 2)
```

---

## 189 — function: forward reference: call a function declared later

```ura
// expect: 42
use "@/core"

fn main() i32:
    return dbl(21)
fn dbl(n i32) i32:
    return n + n
```

---

## 190 — function: a call used as an argument to another call

```ura
// expect: 28
use "@/core"

fn square(n i32) i32:
    return n * n
fn add(a i32, b i32) i32:
    return a + b
fn main() i32:
    return add(square(5), 3)
```

---

## 191 — function: value parameters: take_damage

```ura
// stdout: <82>
// expect: 0
use "@/core"

fn take_damage(hp i32, atk i32) i32:
    return hp - atk

fn main() i32:
    result i32 = take_damage(100, 18)
    io::out($"<{result}>\n")
    return 0
```

---

## 192 — function: a by-value parameter does not change the caller

```ura
// expect: 5
use "@/core"

fn inc(n i32) i32:
    n = n + 1
    return n
fn main() i32:
    n i32 = 5
    inc(n)
    return n
```

---

## 193 — function: redeclaring a function

```ura
// error: Redeclaration of function 'dup'
use "@/core"

fn dup() i32:
    return 1

fn dup() i32:
    return 2

fn main() i32:
    return dup()
```

---

## 194 — function: calling a function that does not exist

```ura
// error: no function named 'bar'
use "@/core"

fn main() i32:
    return bar(3)
```

---

## 195 — function: too many arguments

```ura
// error: Wrong number of arguments to 'add'
use "@/core"

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    x i32 = add(1, 2, 3)
    io::out($"{x}\n")
    return 0
```

---

## 196 — function: an argument of the wrong type

```ura
// error: Argument 1 type mismatch in call to 'show'
use "@/core"

fn show(s char[]):
    io::out($"{s}\n")

fn main() i32:
    show(42)
    return 0
```

---

## 197 — function: two parameters with the same name

```ura
// error: Redeclaration of variable 'a'
use "@/core"

fn add(a i32, a i32) i32:
    return a

fn main() i32:
    io::out($"{add(1, 2)}\n")
    return 0
```

---

## 198 — function: a variadic call missing its required arguments

```ura
// error: Wrong number of arguments to 'printf'
use "@/core"

proto printf(fmt char[], ...) i32

fn main() i32:
    printf()
    return 0
```

---

## 199 — function: a bare return in a void function

```ura
// stdout: floor cleared
// stdout: down
// stdout: still up
// expect: 0
use "@/core"

fn announce():
    io::out("floor cleared\n")
    return

fn guard(hp i32):
    if hp <= 0:
        io::out("down\n")
        return
    io::out("still up\n")

fn main() i32:
    announce()
    guard(0)
    guard(5)
    return 0
```

---

## 200 — lexer: unterminated string literal

```ura
// error: unclosed string
use "@/core"

fn main() i32:
    output("hello)
    return 0
```

---

## 201 — lexer: unterminated character literal

```ura
// error: unclosed character
use "@/core"

fn main() i32:
    c char = 'a
    return 0
```

---

## 202 — lexer: unterminated block comment

```ura
// error: unclosed comment
use "@/core"

/* this comment never closes
fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 203 — lexer: an unexpected character

```ura
// error: unknown character '#'
use "@/core"

fn main() i32:
    a i32 = 5 # 1
    return 0
```

---

## 204 — lexer: an unknown escape sequence

```ura
// error: unknown escape '\q'
use "@/core"

fn main() i32:
    c char = '\q'
    io::out($"{c}\n")
    return 0
```

---

## 205 — lexer: as with no type after it

```ura
// error: Expected a type
use "@/core"

fn main() i32:
    a i32 = 5 as
    return 0
```

---

## 206 — lexer: a call with no closing parenthesis

```ura
// error: expected ')' to close the call to 'add'
use "@/core"

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    x i32 = add(1, 2
    return 0
```

---

## 207 — lexer: a function declaration missing its colon

```ura
// error: expected ':' after the function header
use "@/core"

fn answer() i32
    return 42

fn main() i32:
    io::out($"{answer()}\n")
    return 0
```

---

## 208 — lexer: a function declaration missing its name

```ura
// error: expected a name after 'fn'
use "@/core"

fn () i32:
    return 0

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 209 — lexer: a parameter missing its type

```ura
// error: Expected a type
use "@/core"

fn add(a, b i32) i32:
    return a + b

fn main() i32:
    io::out($"{add(1, 2)}\n")
    return 0
```

---

## 210 — function: a fn without a return type returns nothing

```ura
// stdout: hi
// expect: 0
use "@/core"

fn greet():
    io::out("hi\n")

fn main() i32:
    greet()
    return 0
```

---

## 211 — lexer: if and while missing their colon

```ura
// error: expected ':' after the if condition
use "@/core"

fn main() i32:
    x i32 = 5
    if x > 0
        io::out("positive\n")
    return 0
```

---

## 212 — lexer: proto not followed by fn

```ura
// error: Expected 'fn' after 'proto'
use "@/core"

proto x i32

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 213 — lexer: a struct declaration missing its name

```ura
// error: Expected a struct name after 'struct'
use "@/core"

struct:
    x i32

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 214 — lexer: an unexpected token in an expression

```ura
// error: unexpected '*'
use "@/core"

fn main() i32:
    x i32 = *
    return 0
```

---

## 215 — lexer: member access with no member name after .

```ura
// error: Expected a member name after '.'
use "@/core"
use "@/posix"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.
    return 0
```

---

## 216 — literal: bool literals: declare, zero-init, bool function, reassign

```ura
// expect: 9
use "@/core"

fn flag() bool:
    return True

fn main() i32:
    b bool = flag()
    c bool = False
    d bool
    c = True
    return 9
```

---

## 217 — literal: char literals: declare, print, escapes, in a function

```ura
// stdout: z
// stdout: !
// stdout: A
// expect: 0
use "@/core"

fn grade() char:
    return 'A'

fn main() i32:
    c char = 'z'
    io::out($"{c}\n")
    io::out("!\n")
    io::out($"{grade()}\n")
    io::out("\n")
    return 0
```

---

## 218 — loop: basic while: count enemy kills

```ura
// stdout: <5>
// expect: 0
use "@/core"

fn main() i32:
    kills i32 = 0
    while kills < 5:
        kills = kills + 1
    io::out($"<{kills}>\n")
    return 0
```
