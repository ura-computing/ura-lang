# m01 llvm ir

## 001 — variables: declare with a value

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 002 — variables: reassign

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 003 — variables: use another variable

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 2
  store i32 %MUL, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

## 004 — variables: compound assignment

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 2
  store i32 %MUL, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  %ADD3 = add i32 %"$a2", 5
  store i32 %ADD3, i32* %a, align 4
  %"$a4" = load i32, i32* %a, align 4
  %SUB = sub i32 %"$a4", 3
  store i32 %SUB, i32* %a, align 4
  %"$a5" = load i32, i32* %a, align 4
  %MUL6 = mul i32 %"$a5", 2
  store i32 %MUL6, i32* %a, align 4
  %"$a7" = load i32, i32* %a, align 4
  %DIV = sdiv i32 %"$a7", 4
  store i32 %DIV, i32* %a, align 4
  %"$a8" = load i32, i32* %a, align 4
  %MOD = srem i32 %"$a8", 5
  store i32 %MOD, i32* %a, align 4
  %"$a9" = load i32, i32* %a, align 4
  %"$b10" = load i32, i32* %b, align 4
  %ADD11 = add i32 %"$a9", %"$b10"
  ret i32 %ADD11
}
```

## 005 — variables: declare variable

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  ret i32 0
}
```

## 006 — array: declarations without a value start at zero

```llvm
define i32 @main() {
entry:
  %x = alloca i32, align 4
  %b = alloca i1, align 1
  %c = alloca i8, align 1
  store i32 0, i32* %x, align 4
  store i1 false, i1* %b, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 0
  %"$b" = load i1, i1* %b, align 1
  %OR = or i1 %"$b", %NQ
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"$x" = load i32, i32* %x, align 4
  ret i32 %"$x"
}
```

## 007 — bool: a bool variable

```llvm
define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 008 — bool: False skips the branch

```llvm
define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 009 — bool: a comparison stored in a bool, reassigned

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %big = alloca i1, align 1
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  store i1 %GT, i1* %big, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$big" = load i1, i1* %big, align 1
  br i1 %"$big", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a4" = load i32, i32* %a, align 4
  %GT5 = icmp sgt i32 %"$a4", 9
  store i1 %GT5, i1* %big, align 1
  br label %if1

if1:                                              ; preds = %endif
  %"$big6" = load i1, i1* %big, align 1
  br i1 %"$big6", label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n7" = load i32, i32* %n, align 4
  %ADD8 = add i32 %"$n7", 10
  store i32 %ADD8, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  store i1 false, i1* %big, align 1
  br label %if2

if2:                                              ; preds = %endif1
  %"$big9" = load i1, i1* %big, align 1
  br i1 %"$big9", label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 100
  store i32 %ADD11, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$big12" = load i1, i1* %big, align 1
  %EQ = icmp eq i1 %"$big12", false
  br i1 %EQ, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n13" = load i32, i32* %n, align 4
  %ADD14 = add i32 %"$n13", 1
  store i32 %ADD14, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  %"$n15" = load i32, i32* %n, align 4
  ret i32 %"$n15"
}
```

## 010 — bool: bool parameters and return values

```llvm
define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n", 2
  %EQ = icmp eq i32 %MOD, 0
  ret i1 %EQ
}

define i1 @both(i1 %0, i1 %1) {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  store i1 %1, i1* %b, align 1
  %"$a" = load i1, i1* %a, align 1
  %"$b" = load i1, i1* %b, align 1
  %AND = and i1 %"$a", %"$b"
  ret i1 %AND
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %call.is_even = call i1 @is_even(i32 4)
  br i1 %call.is_even, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %call.is_even3 = call i1 @is_even(i32 2)
  %call.is_even4 = call i1 @is_even(i32 6)
  %call.both = call i1 @both(i1 %call.is_even3, i1 %call.is_even4)
  br i1 %call.both, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n5" = load i32, i32* %n, align 4
  %ADD6 = add i32 %"$n5", 2
  store i32 %ADD6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %call.is_even7 = call i1 @is_even(i32 2)
  %call.is_even8 = call i1 @is_even(i32 7)
  %call.both9 = call i1 @both(i1 %call.is_even7, i1 %call.is_even8)
  br i1 %call.both9, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 10
  store i32 %ADD11, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n12" = load i32, i32* %n, align 4
  ret i32 %"$n12"
}
```

## 011 — bool: or between bool variables

```llvm
define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  store i1 false, i1* %a, align 1
  store i1 true, i1* %b, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i1, i1* %a, align 1
  %"$b" = load i1, i1* %b, align 1
  %OR = or i1 %"$a", %"$b"
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 5
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a2" = load i1, i1* %a, align 1
  %"$b3" = load i1, i1* %b, align 1
  %AND = and i1 %"$a2", %"$b3"
  br i1 %AND, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 50
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  %"$n6" = load i32, i32* %n, align 4
  ret i32 %"$n6"
}
```

## 012 — bool: a bool as a while condition

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %going = alloca i1, align 1
  store i32 0, i32* %i, align 4
  store i1 true, i1* %going, align 1
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$going" = load i1, i1* %going, align 1
  br i1 %"$going", label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i", 1
  store i32 %ADD, i32* %i, align 4
  %"$i1" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i1", 4
  store i1 %LT, i1* %going, align 1
  br label %while0

endloop:                                          ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  ret i32 %"$i2"
}
```

## 013 — bool: a bool default value is False

```llvm
define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 7
}
```

## 015 — char: print chars

```llvm
@str = private unnamed_addr constant [7 x i8] c"%c %c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  %"$c1" = load i8, i8* %c1, align 1
  %"$c2" = load i8, i8* %c2, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8 %"$c1", i8 %"$c2")
  ret i32 0
}
```

## 016 — char: compare chars

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 120, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 120
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$c3" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c3", 121
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 1
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$c6" = load i8, i8* %c, align 1
  %EQ7 = icmp eq i8 %"$c6", 122
  br i1 %EQ7, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 10
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

## 017 — char: order of chars

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 109, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %"$c", 97
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$c3" = load i8, i8* %c, align 1
  %LT = icmp slt i8 %"$c3", 122
  br i1 %LT, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 1
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$c6" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c6", 109
  %"$c7" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c7", 109
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 1
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

## 018 — char: escaped chars

```llvm
@str = private unnamed_addr constant [6 x i8] c"[%c]\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"[%c]\0A\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"[%c] [%c]\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %nl = alloca i8, align 1
  %tab = alloca i8, align 1
  store i8 10, i8* %nl, align 1
  store i8 9, i8* %tab, align 1
  %"$nl" = load i8, i8* %nl, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8 %"$nl")
  %"$tab" = load i8, i8* %tab, align 1
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8 %"$tab")
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i8 39, i8 92)
  ret i32 0
}
```

## 019 — char: reassign a char

```llvm
@str = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 112, i8* %c, align 1
  store i8 113, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8 %"$c")
  ret i32 0
}
```

## 020 — char: char parameter and return value

```llvm
@str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @answer(i1 %0) {
entry:
  %yes = alloca i1, align 1
  store i1 %0, i1* %yes, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$yes" = load i1, i1* %yes, align 1
  br i1 %"$yes", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i8 89

endif:                                            ; preds = %if0
  ret i8 78
}

define void @show(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8 %"$c")
  ret void
}

define i32 @main() {
entry:
  %call.answer = call i8 @answer(i1 true)
  call void @show(i8 %call.answer)
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call.answer1 = call i8 @answer(i1 false)
  call void @show(i8 %call.answer1)
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}
```

## 021 — char: a char decides a loop

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 97, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 101
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$n1" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n1", 1
  br i1 %EQ, label %if1.then, label %elif2

if1.then:                                         ; preds = %if1
  store i8 98, i8* %c, align 1
  br label %endif

elif2:                                            ; preds = %if1
  %"$n2" = load i32, i32* %n, align 4
  %EQ3 = icmp eq i32 %"$n2", 2
  br i1 %EQ3, label %elif2.then, label %elif3

elif2.then:                                       ; preds = %elif2
  store i8 99, i8* %c, align 1
  br label %endif

elif3:                                            ; preds = %elif2
  %"$n4" = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %"$n4", 3
  br i1 %EQ5, label %elif3.then, label %else4

elif3.then:                                       ; preds = %elif3
  store i8 100, i8* %c, align 1
  br label %endif

else4:                                            ; preds = %elif3
  store i8 101, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %else4, %elif3.then, %elif2.then, %if1.then
  br label %while0

endloop:                                          ; preds = %while0
  %"$n6" = load i32, i32* %n, align 4
  ret i32 %"$n6"
}
```

## 022 — char: default value of a char is '\0'

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 023 — char: declaring char/i8 variable

```llvm
define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  ret i32 0
}
```

## 024 — if: if / elif / else chain

```llvm
define i32 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 10
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i32 1

elif1:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %LT2 = icmp slt i32 %"$n1", 20
  br i1 %LT2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  ret i32 2

else2:                                            ; preds = %elif1
  ret i32 3

endif:                                            ; No predecessors!
  ret i32 0
}

define i32 @main() {
entry:
  %call.grade = call i32 @grade(i32 5)
  %MUL = mul i32 %call.grade, 100
  %call.grade1 = call i32 @grade(i32 15)
  %MUL2 = mul i32 %call.grade1, 10
  %ADD = add i32 %MUL, %MUL2
  %call.grade3 = call i32 @grade(i32 25)
  %ADD4 = add i32 %ADD, %call.grade3
  ret i32 %ADD4
}
```

## 025 — if: every comparison

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a7" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$a7", 4
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 1
  store i32 %ADD9, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a10" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a10", 6
  br i1 %LT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n11" = load i32, i32* %n, align 4
  %ADD12 = add i32 %"$n11", 1
  store i32 %ADD12, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$a13" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a13", 5
  br i1 %LE, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 1
  store i32 %ADD15, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif3
  %"$a16" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a16", 4
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"$n17" = load i32, i32* %n, align 4
  %ADD18 = add i32 %"$n17", 1
  store i32 %ADD18, i32* %n, align 4
  br label %endif4

endif4:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif4
  %"$a19" = load i32, i32* %a, align 4
  %GE = icmp sge i32 %"$a19", 5
  br i1 %GE, label %if5.then, label %endif5

if5.then:                                         ; preds = %if5
  %"$n20" = load i32, i32* %n, align 4
  %ADD21 = add i32 %"$n20", 1
  store i32 %ADD21, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif5
  %"$a22" = load i32, i32* %a, align 4
  %GT23 = icmp sgt i32 %"$a22", 5
  br i1 %GT23, label %if6.then, label %endif6

if6.then:                                         ; preds = %if6
  %"$n24" = load i32, i32* %n, align 4
  %ADD25 = add i32 %"$n24", 100
  store i32 %ADD25, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %if6.then, %if6
  %"$n26" = load i32, i32* %n, align 4
  ret i32 %"$n26"
}
```

## 026 — if: and / or

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 1
  %"$a3" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a3", 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a4" = load i32, i32* %a, align 4
  %LT5 = icmp slt i32 %"$a4", 1
  %"$a6" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a6", 5
  %OR = or i1 %LT5, %EQ
  br i1 %OR, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n7" = load i32, i32* %n, align 4
  %ADD8 = add i32 %"$n7", 10
  store i32 %ADD8, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a9" = load i32, i32* %a, align 4
  %LT10 = icmp slt i32 %"$a9", 1
  %"$a11" = load i32, i32* %a, align 4
  %EQ12 = icmp eq i32 %"$a11", 5
  %AND13 = and i1 %LT10, %EQ12
  br i1 %AND13, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 100
  store i32 %ADD15, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n16" = load i32, i32* %n, align 4
  ret i32 %"$n16"
}
```

## 027 — if: body on the same line

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a3" = load i32, i32* %a, align 4
  %GT4 = icmp sgt i32 %"$a3", 9
  br i1 %GT4, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n5" = load i32, i32* %n, align 4
  %ADD6 = add i32 %"$n5", 10
  store i32 %ADD6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a7" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a7", 5
  br i1 %EQ, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 2
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

## 028 — if: nested ifs

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"$b" = load i32, i32* %b, align 4
  %EQ2 = icmp eq i32 %"$b", 7
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 1

endif:                                            ; preds = %if1
  ret i32 2

endif1:                                           ; preds = %if0
  ret i32 3
}
```

## 029 — if: no branch taken: code after the if runs

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 2
  br i1 %EQ, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  store i32 100, i32* %a, align 4
  br label %endif

elif1:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %EQ2 = icmp eq i32 %"$a1", 3
  br i1 %EQ2, label %elif1.then, label %endif

elif1.then:                                       ; preds = %elif1
  store i32 200, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %elif1.then, %elif1, %if0.then
  %"$a3" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a3", 8
  ret i32 %ADD
}
```

## 030 — if: recursion

```llvm
define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$n", 1
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.fact = call i32 @fact(i32 %SUB)
  %"$n2" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n2", %call.fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %call.fact = call i32 @fact(i32 5)
  ret i32 %call.fact
}
```

## 031 — if: a variable declared in a block

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 2, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

## 032 — if: basic if

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 1
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 2
}
```

## 033 — if: basic if/elif/else chain

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 1
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i32 1

elif1:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %LT2 = icmp slt i32 %"$a1", 2
  br i1 %LT2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  ret i32 2

else2:                                            ; preds = %elif1
  ret i32 3

endif:                                            ; No predecessors!
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

## 034 — if: ifs with comparision operators

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a12" = load i32, i32* %a, align 4
  %GT13 = icmp sgt i32 %"$a12", 5
  br i1 %GT13, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 10
  store i32 %ADD15, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a16" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a16", 6
  br i1 %LT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n17" = load i32, i32* %n, align 4
  %ADD18 = add i32 %"$n17", 1
  store i32 %ADD18, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$a19" = load i32, i32* %a, align 4
  %LT20 = icmp slt i32 %"$a19", 5
  br i1 %LT20, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n21" = load i32, i32* %n, align 4
  %ADD22 = add i32 %"$n21", 10
  store i32 %ADD22, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif3
  %"$a23" = load i32, i32* %a, align 4
  %GE = icmp sge i32 %"$a23", 5
  br i1 %GE, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"$n24" = load i32, i32* %n, align 4
  %ADD25 = add i32 %"$n24", 1
  store i32 %ADD25, i32* %n, align 4
  br label %endif4

endif4:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif4
  %"$a26" = load i32, i32* %a, align 4
  %GE27 = icmp sge i32 %"$a26", 6
  br i1 %GE27, label %if5.then, label %endif5

if5.then:                                         ; preds = %if5
  %"$n28" = load i32, i32* %n, align 4
  %ADD29 = add i32 %"$n28", 10
  store i32 %ADD29, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif5
  %"$a30" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a30", 5
  br i1 %LE, label %if6.then, label %endif6

if6.then:                                         ; preds = %if6
  %"$n31" = load i32, i32* %n, align 4
  %ADD32 = add i32 %"$n31", 1
  store i32 %ADD32, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif6
  %"$a33" = load i32, i32* %a, align 4
  %LE34 = icmp sle i32 %"$a33", 4
  br i1 %LE34, label %if7.then, label %endif7

if7.then:                                         ; preds = %if7
  %"$n35" = load i32, i32* %n, align 4
  %ADD36 = add i32 %"$n35", 10
  store i32 %ADD36, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif7
  %"$a37" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a37", 5
  br i1 %EQ, label %if8.then, label %endif8

if8.then:                                         ; preds = %if8
  %"$n38" = load i32, i32* %n, align 4
  %ADD39 = add i32 %"$n38", 1
  store i32 %ADD39, i32* %n, align 4
  br label %endif8

endif8:                                           ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif8
  %"$a40" = load i32, i32* %a, align 4
  %EQ41 = icmp eq i32 %"$a40", 4
  br i1 %EQ41, label %if9.then, label %endif9

if9.then:                                         ; preds = %if9
  %"$n42" = load i32, i32* %n, align 4
  %ADD43 = add i32 %"$n42", 10
  store i32 %ADD43, i32* %n, align 4
  br label %endif9

endif9:                                           ; preds = %if9.then, %if9
  br label %if10

if10:                                             ; preds = %endif9
  %"$a44" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$a44", 4
  br i1 %NQ, label %if10.then, label %endif10

if10.then:                                        ; preds = %if10
  %"$n45" = load i32, i32* %n, align 4
  %ADD46 = add i32 %"$n45", 1
  store i32 %ADD46, i32* %n, align 4
  br label %endif10

endif10:                                          ; preds = %if10.then, %if10
  br label %if11

if11:                                             ; preds = %endif10
  %"$a47" = load i32, i32* %a, align 4
  %NQ48 = icmp ne i32 %"$a47", 5
  br i1 %NQ48, label %if11.then, label %endif11

if11.then:                                        ; preds = %if11
  %"$n49" = load i32, i32* %n, align 4
  %ADD50 = add i32 %"$n49", 10
  store i32 %ADD50, i32* %n, align 4
  br label %endif11

endif11:                                          ; preds = %if11.then, %if11
  %"$n51" = load i32, i32* %n, align 4
  ret i32 %"$n51"
}
```

## 035 — if: nested ifs

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"$b" = load i32, i32* %b, align 4
  %EQ2 = icmp eq i32 %"$b", 7
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 1

endif:                                            ; preds = %if1
  ret i32 2

endif1:                                           ; preds = %if0
  ret i32 3
}
```

## 036 — if: logic operators and/or

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 1
  %"$a1" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a1", 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 037 — while: count to 10

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a1", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

## 038 — while: break

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

## 039 — while: continue

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %sum, align 4
  br label %while0

while0:                                           ; preds = %endif, %if1.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %MOD = srem i32 %"$i2", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD4 = add i32 %"$sum", %"$i3"
  store i32 %ADD4, i32* %sum, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$sum5" = load i32, i32* %sum, align 4
  ret i32 %"$sum5"
}
```

## 040 — while: nested loops

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  %"$j3" = load i32, i32* %j, align 4
  %ADD4 = add i32 %"$j3", 1
  store i32 %ADD4, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c7" = load i32, i32* %c, align 4
  ret i32 %"$c7"
}
```

## 041 — while: break leaves only the inner loop

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %while1

while1:                                           ; preds = %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT2 = icmp slt i32 %"$c", 100
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c3" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c3", 1
  store i32 %ADD, i32* %c, align 4
  br label %endloop

endloop:                                          ; preds = %while1.then, %while1
  %"$i4" = load i32, i32* %i, align 4
  %ADD5 = add i32 %"$i4", 1
  store i32 %ADD5, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c6" = load i32, i32* %c, align 4
  ret i32 %"$c6"
}
```

## 042 — while: continue in the inner loop

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %if2.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$j3" = load i32, i32* %j, align 4
  %ADD = add i32 %"$j3", 1
  store i32 %ADD, i32* %j, align 4
  br label %if2

if2:                                              ; preds = %while1.then
  %"$j4" = load i32, i32* %j, align 4
  %MOD = srem i32 %"$j4", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  br label %while1

endif:                                            ; preds = %if2
  %"$c" = load i32, i32* %c, align 4
  %ADD5 = add i32 %"$c", 1
  store i32 %ADD5, i32* %c, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i6" = load i32, i32* %i, align 4
  %ADD7 = add i32 %"$i6", 1
  store i32 %ADD7, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c8" = load i32, i32* %c, align 4
  ret i32 %"$c8"
}
```

## 043 — while: return from inside a loop

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 100
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i1" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i1", 7
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i2" = load i32, i32* %i, align 4
  ret i32 %"$i2"

endif:                                            ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 0
}
```

## 044 — while: loop that never runs

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 0
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  store i32 100, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

## 045 — while: loop in a function, called from main

```llvm
define i32 @sum_to(i32 %0) {
entry:
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$i", %"$n"
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$i1"
  store i32 %ADD, i32* %total, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$total4" = load i32, i32* %total, align 4
  ret i32 %"$total4"
}

define i32 @main() {
entry:
  %call.sum_to = call i32 @sum_to(i32 10)
  ret i32 %call.sum_to
}
```

## 046 — while: basic while loop

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a1", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

## 047 — while: break

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

## 048 — while: continue

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %if1.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 2
  store i32 %ADD, i32* %a, align 4
  br label %while0

endif:                                            ; preds = %if1
  %"$a3" = load i32, i32* %a, align 4
  %ADD4 = add i32 %"$a3", 1
  store i32 %ADD4, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a5" = load i32, i32* %a, align 4
  ret i32 %"$a5"
}
```

## 049 — while: nested whiles

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %while1

while1:                                           ; preds = %while0.then
  br i1 true, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endloop

endloop:                                          ; preds = %while1.then, %while1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c4" = load i32, i32* %c, align 4
  ret i32 %"$c4"
}
```

## 050 — while: get crazy

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  %"$j3" = load i32, i32* %j, align 4
  %ADD4 = add i32 %"$j3", 1
  store i32 %ADD4, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c7" = load i32, i32* %c, align 4
  ret i32 %"$c7"
}
```

## 051 — function: parameters and calls

```llvm
define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x", %"$x1"
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 3)
  %call.square1 = call i32 @square(i32 4)
  %call.add = call i32 @add(i32 %call.square, i32 %call.square1)
  ret i32 %call.add
}
```

## 052 — function: call before the definition

```llvm
define i32 @main() {
entry:
  %call.later = call i32 @later(i32 2)
  ret i32 %call.later
}

define i32 @later(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 9
  ret i32 %MUL
}
```

## 053 — function: nested calls

```llvm
define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.inc = call i32 @inc(i32 0)
  %call.inc1 = call i32 @inc(i32 %call.inc)
  %call.inc2 = call i32 @inc(i32 %call.inc1)
  ret i32 %call.inc2
}
```

## 054 — function: a parameter is a copy

```llvm
define i32 @bump(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 1
  store i32 %ADD, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  ret i32 %"$x1"
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %call.bump = call i32 @bump(i32 %"$a")
  store i32 %call.bump, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 10
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b"
  ret i32 %ADD
}
```

## 055 — function: call result in a variable

```llvm
define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %call.twice = call i32 @twice(i32 3)
  store i32 %call.twice, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %call.twice1 = call i32 @twice(i32 %"$a")
  %ADD = add i32 %call.twice1, 2
  store i32 %ADD, i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

## 056 — function: void function and bare return

```llvm
define void @nothing() {
entry:
  ret void
}

define i32 @main() {
entry:
  call void @nothing()
  ret i32 4
}
```

## 057 — function: function inside a function

```llvm
define i32 @main() {
entry:
  %call.help = call i32 @help()
  ret i32 %call.help
}

define i32 @help() {
entry:
  ret i32 7
}
```

## 058 — function: calling function before declaration

```llvm
define i32 @main() {
entry:
  %call.later = call i32 @later()
  ret i32 %call.later
}

define i32 @later() {
entry:
  ret i32 9
}
```

## 059 — function: math op on functions

```llvm
define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x", %"$x1"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 3)
  %call.square1 = call i32 @square(i32 4)
  %ADD = add i32 %call.square, %call.square1
  ret i32 %ADD
}
```

## 060 — function: bare return in a void function

```llvm
define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

## 061 — proto: printf with a number

```llvm
@str = private unnamed_addr constant [10 x i8] c"hello %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 42)
  ret i32 0
}
```

## 062 — proto: several extra arguments

```llvm
@str = private unnamed_addr constant [14 x i8] c"%d + %d = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$a1", i32 %"$b2", i32 %ADD)
  ret i32 0
}
```

## 063 — proto: exit from C

```llvm
declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

## 064 — proto: escapes

```llvm
@str = private unnamed_addr constant [5 x i8] c"a\09b\0A\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"\22quoted\22 \\ done\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

## 065 — proto: printf returns how many chars it wrote

```llvm
@str = private unnamed_addr constant [4 x i8] c"abc\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 %call.printf
}
```

## 066 — proto: printf in a loop

```llvm
@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$i1")
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %LT3 = icmp slt i32 %"$i2", 2
  br i1 %LT3, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}
```

## 067 — proto: proto function

```llvm
declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

## 068 — proto: escaped characters

```llvm
@str = private unnamed_addr constant [10 x i8] c"tab\09here\0A\00", align 1

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  ret i32 0
}
```

## 069 — global scope: a global variable changed by a function

```llvm
@count = global i32 0

define void @tick() {
entry:
  %"$count" = load i32, i32* @count, align 4
  %ADD = add i32 %"$count", 1
  store i32 %ADD, i32* @count, align 4
  ret void
}

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* @count, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @tick()
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$count" = load i32, i32* @count, align 4
  ret i32 %"$count"
}
```

## 070 — crazy: mutual recursion, used before it's declared

```llvm
define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 true

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.is_odd = call i1 @is_odd(i32 %SUB)
  ret i1 %call.is_odd
}

define i1 @is_odd(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if1

if1:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.is_even = call i1 @is_even(i32 %SUB)
  ret i1 %call.is_even
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %call.is_even = call i1 @is_even(i32 10)
  br i1 %call.is_even, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %call.is_odd = call i1 @is_odd(i32 7)
  br i1 %call.is_odd, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$r3" = load i32, i32* %r, align 4
  %ADD4 = add i32 %"$r3", 2
  store i32 %ADD4, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif1
  %call.is_even5 = call i1 @is_even(i32 3)
  br i1 %call.is_even5, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 4
  store i32 %ADD7, i32* %r, align 4
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  %"$r8" = load i32, i32* %r, align 4
  ret i32 %"$r8"
}
```

## 071 — crazy: gcd, printf with many arguments

```llvm
@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @gcd(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %"$b", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"

endif:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %MOD = srem i32 %"$a1", %"$b2"
  %"$b3" = load i32, i32* %b, align 4
  %call.gcd = call i32 @gcd(i32 %"$b3", i32 %MOD)
  ret i32 %call.gcd
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %call.gcd = call i32 @gcd(i32 84, i32 36)
  store i32 %call.gcd, i32* %x, align 4
  %call.gcd1 = call i32 @gcd(i32 17, i32 5)
  store i32 %call.gcd1, i32* %y, align 4
  %"$x" = load i32, i32* %x, align 4
  %call.gcd2 = call i32 @gcd(i32 %"$x", i32 4)
  %"$x3" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x3", i32 %"$y", i32 %call.gcd2)
  %"$x4" = load i32, i32* %x, align 4
  %"$y5" = load i32, i32* %y, align 4
  %ADD = add i32 %"$x4", %"$y5"
  ret i32 %ADD
}
```

## 072 — crazy: collatz: a parameter changed in a loop

```llvm
define i32 @steps(i32 %0) {
entry:
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$n" = load i32, i32* %n, align 4
  %NQ = icmp ne i32 %"$n", 1
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$n1" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n1", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  %"$n2" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n2", 2
  store i32 %DIV, i32* %n, align 4
  br label %endif

else2:                                            ; preds = %if1
  %"$n3" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n3", 3
  %ADD = add i32 %MUL, 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  %"$c" = load i32, i32* %c, align 4
  %ADD4 = add i32 %"$c", 1
  store i32 %ADD4, i32* %c, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c5" = load i32, i32* %c, align 4
  ret i32 %"$c5"
}

define i32 @main() {
entry:
  %call.steps = call i32 @steps(i32 27)
  ret i32 %call.steps
}
```

## 073 — crazy: the 20th prime: continue, break, a bool function

```llvm
define i1 @is_prime(i32 %0) {
entry:
  %n = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  store i32 2, i32* %d, align 4
  br label %while1

while1:                                           ; preds = %endif1, %endif
  %"$d" = load i32, i32* %d, align 4
  %"$d2" = load i32, i32* %d, align 4
  %MUL = mul i32 %"$d", %"$d2"
  %"$n3" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %MUL, %"$n3"
  br i1 %LE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$n4" = load i32, i32* %n, align 4
  %"$d5" = load i32, i32* %d, align 4
  %MOD = srem i32 %"$n4", %"$d5"
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  ret i1 false

endif1:                                           ; preds = %if2
  %"$d6" = load i32, i32* %d, align 4
  %ADD = add i32 %"$d6", 1
  store i32 %ADD, i32* %d, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret i1 true
}

define i32 @main() {
entry:
  %count = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 0, i32* %count, align 4
  store i32 0, i32* %n, align 4
  br label %while3

while3:                                           ; preds = %endif1, %if4.then, %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 100
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"$n2" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n2", 1
  store i32 %ADD, i32* %n, align 4
  br label %if4

if4:                                              ; preds = %while3.then
  %"$n3" = load i32, i32* %n, align 4
  %call.is_prime = call i1 @is_prime(i32 %"$n3")
  %EQ = icmp eq i1 %call.is_prime, false
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  br label %while3

endif:                                            ; preds = %if4
  %"$count" = load i32, i32* %count, align 4
  %ADD4 = add i32 %"$count", 1
  store i32 %ADD4, i32* %count, align 4
  br label %if5

if5:                                              ; preds = %endif
  %"$count5" = load i32, i32* %count, align 4
  %EQ6 = icmp eq i32 %"$count5", 20
  br i1 %EQ6, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  br label %endloop

endif1:                                           ; preds = %if5
  br label %while3

endloop:                                          ; preds = %if5.then, %while3
  %"$n7" = load i32, i32* %n, align 4
  ret i32 %"$n7"
}
```

## 074 — crazy: return from two nested loops

```llvm
define i32 @main() {
entry:
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 20
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  store i32 %"$i2", i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT3 = icmp slt i32 %"$j", 20
  br i1 %LT3, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i4" = load i32, i32* %i, align 4
  %"$j5" = load i32, i32* %j, align 4
  %MUL = mul i32 %"$i4", %"$j5"
  %EQ = icmp eq i32 %MUL, 42
  %"$i6" = load i32, i32* %i, align 4
  %"$j7" = load i32, i32* %j, align 4
  %ADD = add i32 %"$i6", %"$j7"
  %EQ8 = icmp eq i32 %ADD, 13
  %AND = and i1 %EQ, %EQ8
  br i1 %AND, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$i9" = load i32, i32* %i, align 4
  %MUL10 = mul i32 %"$i9", 10
  %"$j11" = load i32, i32* %j, align 4
  %ADD12 = add i32 %MUL10, %"$j11"
  ret i32 %ADD12

endif:                                            ; preds = %if2
  %"$j13" = load i32, i32* %j, align 4
  %ADD14 = add i32 %"$j13", 1
  store i32 %ADD14, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i15" = load i32, i32* %i, align 4
  %ADD16 = add i32 %"$i15", 1
  store i32 %ADD16, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  ret i32 0
}
```

## 075 — crazy: operator precedence soup

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 5, i32* %c, align 4
  store i32 2, i32* %d, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 40
  %"$b" = load i32, i32* %b, align 4
  %MUL1 = mul i32 %"$b", 7
  %ADD = add i32 %MUL, %MUL1
  %"$c" = load i32, i32* %c, align 4
  %MUL2 = mul i32 %"$c", 3
  %ADD3 = add i32 %ADD, %MUL2
  %"$d" = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %"$d"
  ret i32 %ADD4
}
```

## 076 — crazy: negative numbers: signed / and %

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 -3, i32* %a, align 4
  store i32 -1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %MUL = mul i32 %"$a", %"$b"
  %SUB = sub i32 0, %MUL
  store i32 %SUB, i32* %c, align 4
  store i32 5, i32* %d, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b2"
  %"$c" = load i32, i32* %c, align 4
  %ADD3 = add i32 %ADD, %"$c"
  %"$d" = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %"$d"
  %ADD5 = add i32 %ADD4, 100
  ret i32 %ADD5
}
```

## 077 — crazy: ackermann: deep recursion, a call as an argument

```llvm
define i32 @ack(i32 %0, i32 %1) {
entry:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %m, align 4
  store i32 %1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$m" = load i32, i32* %m, align 4
  %EQ = icmp eq i32 %"$m", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  ret i32 %ADD

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$n2" = load i32, i32* %n, align 4
  %EQ3 = icmp eq i32 %"$n2", 0
  br i1 %EQ3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$m4" = load i32, i32* %m, align 4
  %SUB = sub i32 %"$m4", 1
  %call.ack = call i32 @ack(i32 %SUB, i32 1)
  ret i32 %call.ack

endif1:                                           ; preds = %if1
  %"$m5" = load i32, i32* %m, align 4
  %SUB6 = sub i32 %"$m5", 1
  %"$n7" = load i32, i32* %n, align 4
  %SUB8 = sub i32 %"$n7", 1
  %"$m9" = load i32, i32* %m, align 4
  %call.ack10 = call i32 @ack(i32 %"$m9", i32 %SUB8)
  %call.ack11 = call i32 @ack(i32 %SUB6, i32 %call.ack10)
  ret i32 %call.ack11
}

define i32 @main() {
entry:
  %call.ack = call i32 @ack(i32 3, i32 3)
  ret i32 %call.ack
}
```

## 078 — crazy: one name declared in sibling blocks

```llvm
define i32 @main() {
entry:
  %x = alloca i32, align 4
  %r = alloca i32, align 4
  %y = alloca i32, align 4
  %y4 = alloca i32, align 4
  %i = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 1, i32* %x, align 4
  store i32 0, i32* %r, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$x" = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %"$x", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 10, i32* %y, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", %"$y"
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$x2" = load i32, i32* %x, align 4
  %EQ3 = icmp eq i32 %"$x2", 1
  br i1 %EQ3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store i32 20, i32* %y4, align 4
  %"$y5" = load i32, i32* %y4, align 4
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", %"$y5"
  store i32 %ADD7, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"$i8" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i8", 100
  store i32 %MUL, i32* %z, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$r9" = load i32, i32* %r, align 4
  %ADD10 = add i32 %"$r9", %"$z"
  store i32 %ADD10, i32* %r, align 4
  %"$i11" = load i32, i32* %i, align 4
  %ADD12 = add i32 %"$i11", 1
  store i32 %ADD12, i32* %i, align 4
  br label %while2

endloop:                                          ; preds = %while2
  %"$r13" = load i32, i32* %r, align 4
  ret i32 %"$r13"
}
```

## 079 — crazy: script: a global counts recursive calls

```llvm
@calls = global i32 0
@r = global i32 0
@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$calls" = load i32, i32* @calls, align 4
  %ADD = add i32 %"$calls", 1
  store i32 %ADD, i32* @calls, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"

endif:                                            ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  %call.fib = call i32 @fib(i32 %SUB)
  %"$n3" = load i32, i32* %n, align 4
  %SUB4 = sub i32 %"$n3", 2
  %call.fib5 = call i32 @fib(i32 %SUB4)
  %ADD6 = add i32 %call.fib, %call.fib5
  ret i32 %ADD6
}

define i32 @main() {
entry:
  store i32 0, i32* @calls, align 4
  %call.fib = call i32 @fib(i32 10)
  store i32 %call.fib, i32* @r, align 4
  %"$r" = load i32, i32* @r, align 4
  %"$calls" = load i32, i32* @calls, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$r", i32 %"$calls")
  %"$calls1" = load i32, i32* @calls, align 4
  ret i32 %"$calls1"
}
```

## 080 — crazy: script: nested loops with continue and break at file level

```llvm
@found = global i32 0
@i = global i32 0

define i32 @main() {
entry:
  %j = alloca i32, align 4
  store i32 -1, i32* @found, align 4
  store i32 0, i32* @i, align 4
  br label %while0

while0:                                           ; preds = %endif2, %if1.then, %entry
  %"$i" = load i32, i32* @i, align 4
  %LT = icmp slt i32 %"$i", 10
  br i1 %LT, label %while0.then, label %endloop3

while0.then:                                      ; preds = %while0
  %"$i4" = load i32, i32* @i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* @i, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i5" = load i32, i32* @i, align 4
  %MOD = srem i32 %"$i5", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  store i32 0, i32* %j, align 4
  br label %while2

while2:                                           ; preds = %endif1, %endif
  %"$j" = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %"$j", 10
  br i1 %LT6, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  br label %if3

if3:                                              ; preds = %while2.then
  %"$i7" = load i32, i32* @i, align 4
  %"$j8" = load i32, i32* %j, align 4
  %MUL = mul i32 %"$i7", %"$j8"
  %EQ9 = icmp eq i32 %MUL, 21
  br i1 %EQ9, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$i10" = load i32, i32* @i, align 4
  %MUL11 = mul i32 %"$i10", 10
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %MUL11, %"$j12"
  store i32 %ADD13, i32* @found, align 4
  br label %endloop

endif1:                                           ; preds = %if3
  %"$j14" = load i32, i32* %j, align 4
  %ADD15 = add i32 %"$j14", 1
  store i32 %ADD15, i32* %j, align 4
  br label %while2

endloop:                                          ; preds = %if3.then, %while2
  br label %if4

if4:                                              ; preds = %endloop
  %"$found" = load i32, i32* @found, align 4
  %NQ = icmp ne i32 %"$found", -1
  br i1 %NQ, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  br label %endloop3

endif2:                                           ; preds = %if4
  br label %while0

endloop3:                                         ; preds = %if4.then, %while0
  %"$found16" = load i32, i32* @found, align 4
  ret i32 %"$found16"
}
```

## 081 — tokenizer error: unknown character

```stderr
error: test.ura:5:15 unknown character '`'
  |
5 |     x i32 = 5 ` 3
  |               ^
help: remove it
error: aborting due to 1 error
1
```

## 082 — tokenizer error: unclosed string

```stderr
error: test.ura:5:14 unclosed string
  |
5 |     s i8[] = "abc
  |              ^^^^
help: use s i8[] = "abc"
error: aborting due to 1 error
1
```

## 083 — tokenizer error: unclosed character

```stderr
error: test.ura:5:14 unclosed character
  |
5 |     c char = 'a
  |              ^^
help: use c char = 'a'
error: aborting due to 1 error
1
```

## 084 — tokenizer error: character with two letters

```stderr
error: test.ura:5:14 a character holds one letter, not 2
  |
5 |     c char = 'ab'
  |              ^^^^
help: use "ab" for a string
error: aborting due to 1 error
1
```

## 085 — tokenizer error: unknown escape

```stderr
error: test.ura:5:16 unknown escape '\q'
  |
5 |     s i8[] = "a\qb"
  |                ^^
help: the escapes are \n \t \r \0 \\ \" \'
error: aborting due to 1 error
1
```

## 086 — tokenizer error: unclosed comment

```stderr
error: test.ura:5:5 unclosed comment
  |
5 |     /* this never ends
  |     ^^
help: add */ to close it
error: aborting due to 1 error
1
```

## 087 — tokenizer error: number too big for i32

```stderr
error: test.ura:5:13 99999999999 doesn't fit in i32
  |
5 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
help: i32 goes from -2147483648 to 2147483647
error: aborting due to 1 error
1
```

## 088 — tokenizer: indentation that is not a multiple of 4 works

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 7
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 089 — tokenizer error: two errors in one file: both are shown

```stderr
error: test.ura:5:15 unknown character '`'
  |
5 |     x i32 = 5 ` 3
  |               ^
help: remove it
error: test.ura:6:15 unknown character '@'
  |
6 |     y i32 = 2 @ 1
  |               ^
help: remove it
error: aborting due to 2 errors
1
```

## 090 — parser error: missing : after a function header

```stderr
error: test.ura:4:11 expected ':' after the function header
  |
4 | fn main() i32
  |           ^^^
help: use fn main() i32:
error: aborting due to 1 error
1
```

## 091 — parser error: missing ( after a function name

```stderr
error: test.ura:4:9 expected '(' after the function name
  |
4 | fn main i32:
  |         ^^^
help: use fn main() i32:
error: aborting due to 1 error
1
```

## 092 — parser error: missing ) after the parameters

```stderr
error: test.ura:4:21 expected ')' after the parameters
  |
4 | fn add(a i32, b i32 i32:
  |                     ^^^
help: use fn add(a i32, b i32) i32:
error: aborting due to 1 error
1
```

## 093 — parser error: a function with no name

```stderr
error: test.ura:4:4 expected a name after 'fn'
  |
4 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
1
```

## 094 — parser error: a parameter with no type

```stderr
error: test.ura:4:10 parameter 'a' has no type
  |
4 | fn twice(a) i32:
  |          ^
help: like: a i32
error: aborting due to 1 error
1
```

## 095 — parser error: missing : after an if condition

```stderr
error: test.ura:6:12 expected ':' after the if condition
  |
6 |     if x > 1
  |            ^
help: use if x > 1:
error: aborting due to 1 error
1
```

## 096 — parser error: missing : after a while condition

```stderr
error: test.ura:6:15 expected ':' after the while condition
  |
6 |     while i < 3
  |               ^
help: use while i < 3:
error: aborting due to 1 error
1
```

## 097 — parser error: else with a condition

```stderr
error: test.ura:8:5 else has no condition
  |
8 |     else x > 1:
  |     ^^^^
help: use elif x > 1:
error: aborting due to 1 error
1
```

## 098 — parser error: elif without an if

```stderr
error: test.ura:6:5 elif without an if before it
  |
6 |     elif x == 1:
  |     ^^^^
help: use if x == 1:
error: aborting due to 1 error
1
```

## 099 — parser error: unclosed (

```stderr
error: test.ura:5:18 expected ')' to close this '('
  |
5 |     x i32 = (1 + 2
  |                  ^
help: use x i32 = (1 + 2)
error: aborting due to 1 error
1
```

## 100 — parser error: nothing after =

```stderr
error: test.ura:4:11 expected a value after '='
  |
4 |     x i32 =
  |           ^
error: aborting due to 1 error
1
```

## 101 — parser error: nothing after an operator

```stderr
error: test.ura:4:15 expected a value after '+'
  |
4 |     x i32 = 1 +
  |               ^
error: aborting due to 1 error
1
```

## 102 — parser error: unclosed ( in a call

```stderr
error: test.ura:8:19 expected ')' to close the call to 'add'
  |
8 |     return add(1, 2
  |                   ^
help: use return add(1, 2)
error: aborting due to 1 error
1
```

## 103 — name error: undeclared variable

```stderr
error: test.ura:6:12 undeclared variable 'y'
  |
6 |     return y
  |            ^
help: did you mean 'x'?
error: aborting due to 1 error
1
```

## 104 — name error: a typo in a variable name

```stderr
error: test.ura:6:12 undeclared variable 'cuont'
  |
6 |     return cuont
  |            ^^^^^
help: did you mean 'count'?
error: aborting due to 1 error
1
```

## 105 — name error: a variable used before its declaration

```stderr
error: test.ura:5:13 'x' is used before its declaration, on line 6
  |
5 |     y i32 = x + 1
  |             ^
help: move the declaration above this line
error: aborting due to 1 error
1
```

## 106 — name error: a variable declared twice

```stderr
error: test.ura:6:5 'x' is already declared, on line 5
  |
6 |     x i32 = 2
  |     ^
help: use x = 2
error: aborting due to 1 error
1
```

## 107 — name error: unknown type

```stderr
error: test.ura:5:7 unknown type 'Point'
  |
5 |     p Point
  |       ^^^^^
help: declare it: struct Point:
error: aborting due to 1 error
1
```

## 108 — name error: undeclared function

```stderr
error: test.ura:4:12 no function named 'foo'
  |
4 |     return foo()
  |            ^^^
error: aborting due to 1 error
1
```

## 109 — name error: a typo in a function name

```stderr
error: test.ura:8:12 no function named 'ad'
  |
8 |     return ad(1, 2)
  |            ^^
help: did you mean 'add'?
error: aborting due to 1 error
1
```

## 110 — name error: wrong number of arguments

```stderr
error: test.ura:8:12 'add' takes 2 arguments, not 1
  |
8 |     return add(1)
  |            ^^^
help: fn add(a i32, b i32) i32
error: aborting due to 1 error
1
```

## 111 — name error: a function declared twice

```stderr
error: test.ura:6:4 'f' is already defined with these parameters
  |
6 | fn f() i32:
  |    ^
error: aborting due to 1 error
1
```

## 112 — name error: break outside a loop

```stderr
error: test.ura:5:5 break outside a loop or match
  |
5 |     break
  |     ^^^^^
help: break only works inside a while or a match
error: aborting due to 1 error
1
```

## 113 — name error: continue outside a loop

```stderr
error: test.ura:6:9 continue outside a loop
  |
6 |         continue
  |         ^^^^^^^^
help: continue only works inside a while
error: aborting due to 1 error
1
```

## 114 — name error: calling a variable

```stderr
error: test.ura:5:12 'x' is a variable, not a function
  |
5 |     return x()
  |            ^
error: aborting due to 1 error
1
```

## 115 — tokenizer: unexpected character

```stderr
error: test.ura:3:15 unknown character '`'
  |
3 |     x i32 = 5 `
  |               ^
help: remove it
error: aborting due to 1 error
1
```

## 116 — tokenizer: unterminated character

```stderr
error: test.ura:3:12 unclosed character
  |
3 |     c i8 = 'a
  |            ^^
help: use c i8 = 'a'
error: aborting due to 1 error
1
```

## 117 — tokenizer: empty character

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  ret i32 1
}
```

## 118 — tokenizer: character with two letters

```stderr
error: test.ura:3:12 a character holds one letter, not 2
  |
3 |     c i8 = 'ab'
  |            ^^^^
help: use "ab" for a string
error: aborting due to 1 error
1
```

## 119 — tokenizer: integer too big for i32

```stderr
error: test.ura:3:13 99999999999 doesn't fit in i32
  |
3 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
help: i32 goes from -2147483648 to 2147483647
error: aborting due to 1 error
1
```

## 120 — functions: missing name

```stderr
error: test.ura:2:4 expected a name after 'fn'
  |
2 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
1
```

## 121 — functions: unclosed parameters

```stderr
error: test.ura:2:13 expected ')' after the parameters
  |
2 | fn add(a i32:
  |             ^
help: use fn add(a i32):
error: aborting due to 1 error
1
```

## 122 — functions: parameter without a type

```stderr
error: test.ura:2:6 parameter 'a' has no type
  |
2 | fn f(a) i32:
  |      ^
help: like: a i32
error: aborting due to 1 error
1
```

## 123 — functions: missing comma between parameters

```stderr
error: test.ura:2:12 expected ',' between parameters
  |
2 | fn f(a i32 b i32) i32:
  |            ^
error: aborting due to 1 error
1
```

## 124 — functions: variadic not last

```stderr
error: test.ura:2:9 '...' must be the last parameter
  |
2 | proto p(..., a i32) i32
  |         ^^^
error: aborting due to 1 error
1
```

## 125 — functions: unknown return type

```stderr
error: test.ura:2:8 unknown type 'Foo'
  |
2 | fn f() Foo:
  |        ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
1
```

## 126 — functions: unknown parameter type

```stderr
error: test.ura:2:8 unknown type 'Foo'
  |
2 | fn f(a Foo) i32:
  |        ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
1
```

## 127 — functions: parameter declared twice

```stderr
error: test.ura:2:13 'a' is already declared
  |
2 | fn f(a i32, a i32) i32:
  |             ^
error: aborting due to 1 error
1
```

## 128 — functions: defined twice

```stderr
error: test.ura:5:4 'f' is already defined with these parameters
  |
5 | fn f() i32:
  |    ^
error: aborting due to 1 error
1
```

## 129 — functions: overloaded on return type only

```stderr
error: test.ura:5:4 'f' can't be overloaded on its return type
  |
5 | fn f() i8:
  |    ^
error: aborting due to 1 error
1
```

## 130 — functions: return at top level

```stderr
error: test.ura:2:1 'return' outside of a function
  |
2 | return 1
  | ^^^^^^
help: top-level code returns only in a file without main
error: aborting due to 1 error
1
```

## 131 — structs: dot on a non-struct

```stderr
error: test.ura:4:12 'a' is 'i32', not a struct
  |
4 |     return a.b
  |            ^
error: aborting due to 1 error
1
```

## 132 — variables: unknown type

```stderr
error: test.ura:3:7 unknown type 'Foo'
  |
3 |     x Foo
  |       ^^^
help: declare it: struct Foo:
error: aborting due to 1 error
1
```

## 133 — variables: declared twice

```stderr
error: test.ura:4:5 'a' is already declared, on line 3
  |
4 |     a i32 = 2
  |     ^
help: use a = 2
error: aborting due to 1 error
1
```

## 134 — variables: used before its declaration

```stderr
error: test.ura:3:13 'a' is used before its declaration, on line 4
  |
3 |     b i32 = a
  |             ^
help: move the declaration above this line
error: aborting due to 1 error
1
```

## 135 — variables: used outside its block

```stderr
error: test.ura:5:12 undeclared variable 'x'
  |
5 |     return x
  |            ^
help: declare it first, like: x i32 = 0
error: aborting due to 1 error
1
```

## 136 — variables: assigned without a declaration

```stderr
error: test.ura:3:5 undeclared variable 'x'
  |
3 |     x = 5
  |     ^
help: declare it first, like: x i32 = 0
error: aborting due to 1 error
1
```

## 137 — expressions: missing operand

```stderr
error: test.ura:3:15 expected a value after '+'
  |
3 |     a i32 = 1 +
  |               ^
error: aborting due to 1 error
1
```

## 138 — expressions: unclosed parenthesis

```stderr
error: test.ura:3:18 expected ')' to close this '('
  |
3 |     a i32 = (1 + 2
  |                  ^
help: use a i32 = (1 + 2)
error: aborting due to 1 error
1
```

## 139 — expressions: unexpected parenthesis

```stderr
error: test.ura:3:18 unexpected ')'
  |
3 |     a i32 = 1 + 2)
  |                  ^
error: aborting due to 1 error
1
```

## 140 — expressions: assign to a literal

```stderr
error: test.ura:4:5 can't assign to a value
  |
4 |     1 = a
  |     ^
error: aborting due to 1 error
1
```

## 141 — expressions: assign to a call

```stderr
error: test.ura:6:5 can't assign to a value
  |
6 |     f() = 3
  |     ^
error: aborting due to 1 error
1
```

## 142 — expressions: compare a number and a string

```stderr
error: test.ura:4:13 expected 'i32', found 'String'
  |
4 |     if a == "x":
  |             ^^^
error: aborting due to 1 error
1
```

## 143 — expressions: add a number and a string

```stderr
error: test.ura:3:14 can't apply '+' to 'i32' and 'String'
  |
3 |     return 1 + "a"
  |              ^
error: aborting due to 1 error
1
```

## 144 — expressions: and on numbers

```stderr
error: test.ura:3:8 expected 'bool', found 'i32'
  |
3 |     if 1 and 2:
  |        ^
error: aborting due to 1 error
1
```

## 145 — calls: unknown function

```stderr
error: test.ura:3:12 no function named 'zz'
  |
3 |     return zz()
  |            ^^
error: aborting due to 1 error
1
```

## 146 — calls: too many arguments

```stderr
error: test.ura:6:12 'f' takes 1 argument, not 2
  |
6 |     return f(1, 2)
  |            ^
help: fn f(a i32) i32
error: aborting due to 1 error
1
```

## 147 — calls: wrong argument type

```stderr
error: test.ura:6:14 expected 'i32', found 'String'
  |
6 |     return f("a")
  |              ^^^
error: aborting due to 1 error
1
```

## 148 — calls: missing comma between arguments

```stderr
error: test.ura:6:16 expected ',' between arguments
  |
6 |     return f(1 2)
  |                ^
error: aborting due to 1 error
1
```

## 149 — calls: unclosed call

```stderr
error: test.ura:6:18 expected ')' to close the call to 'f'
  |
6 |     x i32 = f(1, 2
  |                  ^
help: use x i32 = f(1, 2)
error: aborting due to 1 error
1
```

## 150 — calls: trailing comma

```stderr
error: test.ura:6:16 expected an argument after ','
  |
6 |     return f(1,)
  |                ^
error: aborting due to 1 error
1
```

## 151 — calls: call a variable

```stderr
error: test.ura:4:12 'a' is a variable, not a function
  |
4 |     return a()
  |            ^
error: aborting due to 1 error
1
```

## 152 — calls: use a void result

```stderr
error: test.ura:6:13 expected 'i32', found 'void'
  |
6 |     x i32 = v()
  |             ^
error: aborting due to 1 error
1
```

## 153 — types: string into i8

```stderr
error: test.ura:3:12 expected 'i8', found 'String'
  |
3 |     x i8 = "a"
  |            ^^^
error: aborting due to 1 error
1
```

## 154 — types: bool into i32

```stderr
error: test.ura:3:13 expected 'i32', found 'bool'
  |
3 |     a i32 = True
  |             ^^^^
error: aborting due to 1 error
1
```

## 155 — types: number into bool

```stderr
error: test.ura:3:14 expected 'bool', found 'i32'
  |
3 |     b bool = 1
  |              ^
error: aborting due to 1 error
1
```

## 156 — types: number too big for i8

```stderr
error: test.ura:3:12 300 doesn't fit in i8
  |
3 |     x i8 = 300
  |            ^^^
help: i8 goes from -128 to 127
error: aborting due to 1 error
1
```

## 157 — types: wrong return type

```stderr
error: test.ura:3:12 expected 'i32', found 'String'
  |
3 |     return "a"
  |            ^^^
error: aborting due to 1 error
1
```

## 158 — types: value returned from a void fn

```stderr
error: test.ura:3:12 expected 'void', found 'i32'
  |
3 |     return 1
  |            ^
error: aborting due to 1 error
1
```

## 159 — types: bare return in a fn with a type

```stderr
error: test.ura:3:5 'f' must return a value of type 'i32'
  |
3 |     return
  |     ^^^^^^
error: aborting due to 1 error
1
```

## 160 — types: number as an if condition

```stderr
error: test.ura:3:8 expected 'bool', found 'i32'
  |
3 |     if 5:
  |        ^
error: aborting due to 1 error
1
```

## 161 — types: number as a while condition

```stderr
error: test.ura:4:11 expected 'bool', found 'i32'
  |
4 |     while a:
  |           ^
help: write 'a != 0'
error: aborting due to 1 error
1
```

## 162 — control flow: continue outside a loop

```stderr
error: test.ura:3:5 continue outside a loop
  |
3 |     continue
  |     ^^^^^^^^
help: continue only works inside a while
error: aborting due to 1 error
1
```

## 163 — control flow: elif without if

```stderr
error: test.ura:3:5 elif without an if before it
  |
3 |     elif True:
  |     ^^^^
help: use if True:
error: aborting due to 1 error
1
```

## 164 — control flow: else without if

```stderr
error: test.ura:3:5 else without an if before it
  |
3 |     else:
  |     ^^^^
error: aborting due to 1 error
1
```

## 165 — control flow: missing colon after if

```stderr
error: test.ura:4:12 expected ':' after the if condition
  |
4 |     if a > 1
  |            ^
help: use if a > 1:
error: aborting due to 1 error
1
```

## 166 — control flow: missing colon after elif

```stderr
error: test.ura:6:14 expected ':' after the elif condition
  |
6 |     elif a > 0
  |              ^
help: use elif a > 0:
error: aborting due to 1 error
1
```

## 167 — control flow: missing colon after else

```stderr
error: test.ura:6:5 expected ':' after else
  |
6 |     else
  |     ^^^^
help: use else:
error: aborting due to 1 error
1
```

## 168 — control flow: missing colon after while

```stderr
error: test.ura:4:15 expected ':' after the while condition
  |
4 |     while a > 0
  |               ^
help: use while a > 0:
error: aborting due to 1 error
1
```

## 169 — control flow: if without a condition

```stderr
error: test.ura:3:5 expected a condition after 'if'
  |
3 |     if:
  |     ^^
error: aborting due to 1 error
1
```

## 170 — control flow: else with a condition

```stderr
error: test.ura:6:5 else has no condition
  |
6 |     else a > 0:
  |     ^^^^
help: use elif a > 0:
error: aborting due to 1 error
1
```

## 171 — recovery: three analysis errors

```stderr
error: test.ura:3:13 undeclared variable 'zz'
  |
3 |     a i32 = zz
  |             ^^
help: did you mean 'a'?
error: test.ura:4:13 undeclared variable 'u'
  |
4 |     b i32 = u.x
  |             ^
help: did you mean 'a'?
error: test.ura:5:12 no function named 'yy'
  |
5 |     return yy()
  |            ^^
error: aborting due to 3 errors
1
```

## 172 — recovery: two syntax errors in two functions

```stderr
error: test.ura:2:8 expected ':' after the function header
  |
2 | fn f() i32
  |        ^^^
help: use fn f() i32:
error: test.ura:6:13 expected ')' to close this '('
  |
6 |     return (2
  |             ^
help: use return (2)
error: aborting due to 2 errors
1
```

## 173 — diagnostics: error past line 9

```stderr
error: test.ura:12:12 undeclared variable 'zz'
   |
12 |     return zz
   |            ^^
help: did you mean 'a'?
error: aborting due to 1 error
1
```

## 174 — function: bare return in a void function

```llvm
define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

## 175 — type: compound assignment/unary operator

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 -10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a", 2
  store i32 %ADD, i32* %a, align 4
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 4
  store i32 %MUL, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  %DIV = sdiv i32 %"$a2", 2
  store i32 %DIV, i32* %a, align 4
  %"$a3" = load i32, i32* %a, align 4
  %SUB = sub i32 %"$a3", 2
  store i32 %SUB, i32* %a, align 4
  %"$a4" = load i32, i32* %a, align 4
  %MOD = srem i32 %"$a4", 3
  store i32 %MOD, i32* %a, align 4
  ret i32 0
}
```

## 176 — type: boolean type

```llvm
define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 177 — builtin: puts: simple messages, char comparison on hero grade

```llvm
@str = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

declare i32 @puts(i8*)

define i1 @is_s_rank(i8 %0) {
entry:
  %grade = alloca i8, align 1
  store i8 %0, i8* %grade, align 1
  %"$grade" = load i8, i8* %grade, align 1
  %EQ = icmp eq i8 %"$grade", 83
  ret i1 %EQ
}

define i32 @main() {
entry:
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$grade" = load i8, i8* %grade, align 1
  %call.is_s_rank = call i1 @is_s_rank(i8 %"$grade")
  br i1 %call.is_s_rank, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  %call.puts = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str, i32 0, i32 0))
  br label %endif

else1:                                            ; preds = %if0
  %call.puts1 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}
```

## 178 — builtin: proto printf: hello world with a format string

```llvm
@str = private unnamed_addr constant [13 x i8] c"hello %s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  store [4 x i8] c"ura\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$s", i32 42)
  ret i32 0
}
```

## 179 — builtin: custom putchar via write: print dungeon tiles

```llvm
declare i64 @write(i32, i8*, i64)

define i32 @putchar(i8 %0) {
entry:
  %c = alloca i8, align 1
  %buf = alloca i8*, align 8
  %arr = alloca [2 x i8], align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %1 = insertvalue [2 x i8] undef, i8 %"$c", 0
  %2 = insertvalue [2 x i8] %1, i8 0, 1
  store [2 x i8] %2, [2 x i8]* %arr, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store i8* %3, i8** %buf, align 8
  %"$buf" = load i8*, i8** %buf, align 8
  %call.write = call i64 @write(i32 1, i8* %"$buf", i64 1)
  ret i32 0
}

define i32 @main() {
entry:
  %call.putchar = call i32 @putchar(i8 68)
  %call.putchar1 = call i32 @putchar(i8 85)
  %call.putchar2 = call i32 @putchar(i8 78)
  %call.putchar3 = call i32 @putchar(i8 71)
  %call.putchar4 = call i32 @putchar(i8 69)
  %call.putchar5 = call i32 @putchar(i8 79)
  %call.putchar6 = call i32 @putchar(i8 78)
  %call.putchar7 = call i32 @putchar(i8 10)
  ret i32 0
}
```

## 180 — if: basic if: first floor greeting

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [13 x i8] c"Fresh start\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 1, i32* %floor, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$floor" = load i32, i32* %floor, align 4
  %EQ = icmp eq i32 %"$floor", 1
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 12, i32 0 })
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 181 — if: if-else: alive or dead check

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"Hero is alive\0A\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"Hero has fallen\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 0, i32* %hp, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 14, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0), i32 16, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 182 — if: if-elif-else: floor difficulty rating

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"Easy\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Normal\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"Hard\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %floor = alloca i32, align 4
  store i32 6, i32* %floor, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$floor" = load i32, i32* %floor, align 4
  %LT = icmp slt i32 %"$floor", 3
  br i1 %LT, label %if6.then, label %elif7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  br label %endif

elif7:                                            ; preds = %if6
  %"$floor1" = load i32, i32* %floor, align 4
  %LT2 = icmp slt i32 %"$floor1", 7
  br i1 %LT2, label %elif7.then, label %else8

elif7.then:                                       ; preds = %elif7
  call void @"io::out"(%String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 7, i32 0 })
  br label %endif

else8:                                            ; preds = %elif7
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i32 5, i32 0 })
  br label %endif

endif:                                            ; preds = %else8, %elif7.then, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 183 — function: void fn: announce dungeon floor

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [27 x i8] c"=== Entering floor %d ===\0A\00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c"BOSS: %s appears!\0A\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"The Lich King\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define void @announce_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str, i32 0, i32 0), i32 %"$floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str, i32 0, i32 0), i32 %"$floor")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret void
}

define void @announce_boss(i8* %0) {
entry:
  %name = alloca i8*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret void
}

define i32 @main() {
entry:
  call void @announce_floor(i32 1)
  call void @announce_floor(i32 5)
  call void @announce_boss(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 184 — function: fn with return value + single-line fns

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [19 x i8] c"Floor 3 bonus: %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [19 x i8] c"Hero alive:    %s\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [19 x i8] c"Boss floor:    %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @victory_bonus(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %MUL = mul i32 %"$floor", 50
  ret i32 %MUL
}

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  ret i1 %GT
}

define i1 @is_boss_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %"$floor" = load i32, i32* %floor, align 4
  %EQ = icmp eq i32 %"$floor", 10
  ret i1 %EQ
}

define i32 @main() {
entry:
  %bonus = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size7 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text12 = alloca i8*, align 8
  %format.size21 = alloca i32, align 4
  %bool.text22 = alloca i8*, align 8
  %format.text28 = alloca i8*, align 8
  %call.victory_bonus = call i32 @victory_bonus(i32 3)
  store i32 %call.victory_bonus, i32* %bonus, align 4
  %"$bonus" = load i32, i32* %bonus, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$bonus")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$bonus")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  %call.is_alive = call i1 @is_alive(i32 80)
  store i32 0, i32* %format.size7, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %call.is_alive, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %"$bool.text15" = load i8*, i8** %bool.text, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text15")
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text12, align 8
  %"$format.size20" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  %call.is_boss_floor = call i1 @is_boss_floor(i32 10)
  store i32 0, i32* %format.size21, align 4
  store i8* null, i8** %bool.text22, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %call.is_boss_floor, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text22, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text22, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text23" = load i8*, i8** %bool.text22, align 8
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text23")
  store i32 %call.snprintf24, i32* %format.size21, align 4
  %"$format.size25" = load i32, i32* %format.size21, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %AS27 = sext i32 %ADD26 to i64
  %call.calloc29 = call i8* @calloc(i64 %AS27, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc29, i8** %format.text28, align 8
  %"$format.text30" = load i8*, i8** %format.text28, align 8
  %"$bool.text31" = load i8*, i8** %bool.text22, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text30", i64 %AS27, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text31")
  %"$format.size33" = load i32, i32* %format.size21, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text28, align 8
  %"$format.size36" = load i32, i32* %format.size21, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %7 = insertvalue %String %6, i32 %"$format.size36", 1
  %8 = insertvalue %String %7, i32 %ADD34, 2
  call void @"io::out"(%String %8)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 185 — function: multi-fn: clamp, damage, is_dead

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [21 x i8] c"Orc takes %d damage\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [14 x i8] c"Orc dead: %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @clamp(i32 %0, i32 %1, i32 %2) {
entry:
  %val = alloca i32, align 4
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  store i32 %0, i32* %val, align 4
  store i32 %1, i32* %lo, align 4
  store i32 %2, i32* %hi, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$val" = load i32, i32* %val, align 4
  %"$lo" = load i32, i32* %lo, align 4
  %LT = icmp slt i32 %"$val", %"$lo"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$lo2" = load i32, i32* %lo, align 4
  ret i32 %"$lo2"

endif:                                            ; preds = %if6
  br label %if7

if7:                                              ; preds = %endif
  %"$val3" = load i32, i32* %val, align 4
  %"$hi" = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %"$val3", %"$hi"
  br i1 %GT, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  %"$hi4" = load i32, i32* %hi, align 4
  ret i32 %"$hi4"

endif1:                                           ; preds = %if7
  %"$val5" = load i32, i32* %val, align 4
  ret i32 %"$val5"
}

define i32 @damage(i32 %0, i32 %1) {
entry:
  %atk = alloca i32, align 4
  %def = alloca i32, align 4
  store i32 %0, i32* %atk, align 4
  store i32 %1, i32* %def, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk", %"$def"
  %call.clamp = call i32 @clamp(i32 %SUB, i32 0, i32 999)
  ret i32 %call.clamp
}

define i1 @is_dead(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %LE = icmp sle i32 %"$hp", 0
  ret i1 %LE
}

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size7 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text12 = alloca i8*, align 8
  %call.damage = call i32 @damage(i32 25, i32 8)
  store i32 %call.damage, i32* %d, align 4
  %"$d" = load i32, i32* %d, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %"$d")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %"$d")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %"$d6" = load i32, i32* %d, align 4
  %SUB = sub i32 %"$d6", 60
  %call.is_dead = call i1 @is_dead(i32 %SUB)
  store i32 0, i32* %format.size7, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if8

if8:                                              ; preds = %entry
  br i1 %call.is_dead, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %"$bool.text15" = load i8*, i8** %bool.text, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text15")
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text12, align 8
  %"$format.size20" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 186 — function: return an integer literal

```llvm
define i32 @main() {
entry:
  ret i32 42
}
```

## 187 — function: declare a variable and return it

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 188 — function: function parameters and a direct call

```llvm
define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.add = call i32 @add(i32 40, i32 2)
  ret i32 %call.add
}
```

## 189 — function: forward reference: call a function declared later

```llvm
define i32 @main() {
entry:
  %call.dbl = call i32 @dbl(i32 21)
  ret i32 %call.dbl
}

define i32 @dbl(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", %"$n1"
  ret i32 %ADD
}
```

## 190 — function: a call used as an argument to another call

```llvm
define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", %"$n1"
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 5)
  %call.add = call i32 @add(i32 %call.square, i32 3)
  ret i32 %call.add
}
```

## 191 — function: value parameters: take_damage

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @take_damage(i32 %0, i32 %1) {
entry:
  %hp = alloca i32, align 4
  %atk = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  store i32 %1, i32* %atk, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %SUB = sub i32 %"$hp", %"$atk"
  ret i32 %SUB
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.take_damage = call i32 @take_damage(i32 100, i32 18)
  store i32 %call.take_damage, i32* %result, align 4
  %"$result" = load i32, i32* %result, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$result")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$result")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 192 — function: a by-value parameter does not change the caller

```llvm
define i32 @inc(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.inc = call i32 @inc(i32 %"$n")
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"
}
```

## 193 — function: redeclaring a function

```stderr
error: test.ura:7:4 'dup' is already defined with these parameters
  |
7 | fn dup() i32:
  |    ^^^
error: aborting due to 1 error
1
```

## 194 — function: calling a function that does not exist

```stderr
error: test.ura:5:12 no function named 'bar'
  |
5 |     return bar(3)
  |            ^^^
error: aborting due to 1 error
1
```

## 195 — function: too many arguments

```stderr
error: test.ura:8:13 'add' takes 2 arguments, not 3
  |
8 |     x i32 = add(1, 2, 3)
  |             ^^^
help: fn add(a i32, b i32) i32
error: aborting due to 1 error
1
```

## 196 — function: an argument of the wrong type

```stderr
error: test.ura:8:10 expected 'i8[]', found 'i32'
  |
8 |     show(42)
  |          ^^
error: aborting due to 1 error
1
```

## 197 — function: two parameters with the same name

```stderr
error: test.ura:4:15 'a' is already declared
  |
4 | fn add(a i32, a i32) i32:
  |               ^
error: aborting due to 1 error
1
```

## 198 — function: a variadic call missing its required arguments

```stderr
error: test.ura:7:5 'printf' takes at least 1 argument, not 0
  |
7 |     printf()
  |     ^^^^^^
help: proto printf(fmt i8[], ...) i32
error: aborting due to 1 error
1
```

## 199 — function: a bare return in a void function

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"floor cleared\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"down\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"still up\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define void @announce() {
entry:
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 14, i32 0 })
  ret void
}

define void @guard(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$hp" = load i32, i32* %hp, align 4
  %LE = icmp sle i32 %"$hp", 0
  br i1 %LE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 5, i32 0 })
  ret void

endif:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 9, i32 0 })
  ret void
}

define i32 @main() {
entry:
  call void @announce()
  call void @guard(i32 0)
  call void @guard(i32 5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 200 — lexer: unterminated string literal

```stderr
error: test.ura:5:12 unclosed string
  |
5 |     output("hello)
  |            ^^^^^^^
help: use output("hello)"
error: aborting due to 1 error
1
```

## 201 — lexer: unterminated character literal

```stderr
error: test.ura:5:14 unclosed character
  |
5 |     c char = 'a
  |              ^^
help: use c char = 'a'
error: aborting due to 1 error
1
```

## 202 — lexer: unterminated block comment

```stderr
error: test.ura:4:1 unclosed comment
  |
4 | /* this comment never closes
  | ^^
help: add */ to close it
error: aborting due to 1 error
1
```

## 203 — lexer: an unexpected character

```stderr
error: test.ura:5:15 unknown character '#'
  |
5 |     a i32 = 5 # 1
  |               ^
help: remove it
error: aborting due to 1 error
1
```

## 204 — lexer: an unknown escape sequence

```stderr
error: test.ura:5:15 unknown escape '\q'
  |
5 |     c char = '\q'
  |               ^^
help: the escapes are \n \t \r \0 \\ \" \'
error: aborting due to 1 error
1
```

## 205 — lexer: as with no type after it

```stderr
error: test.ura:5:15 expected a type after 'as'
  |
5 |     a i32 = 5 as
  |               ^^
error: aborting due to 1 error
1
```

## 206 — lexer: a call with no closing parenthesis

```stderr
error: test.ura:8:20 expected ')' to close the call to 'add'
  |
8 |     x i32 = add(1, 2
  |                    ^
help: use x i32 = add(1, 2)
error: aborting due to 1 error
1
```

## 207 — lexer: a function declaration missing its colon

```stderr
error: test.ura:4:13 expected ':' after the function header
  |
4 | fn answer() i32
  |             ^^^
help: use fn answer() i32:
error: aborting due to 1 error
1
```

## 208 — lexer: a function declaration missing its name

```stderr
error: test.ura:4:4 expected a name after 'fn'
  |
4 | fn () i32:
  |    ^
help: use fn name() i32:
error: aborting due to 1 error
1
```

## 209 — lexer: a parameter missing its type

```stderr
error: test.ura:4:8 parameter 'a' has no type
  |
4 | fn add(a, b i32) i32:
  |        ^
help: like: a i32
error: aborting due to 1 error
1
```

## 210 — function: a fn without a return type returns nothing

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define void @greet() {
entry:
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret void
}

define i32 @main() {
entry:
  call void @greet()
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 211 — lexer: if and while missing their colon

```stderr
error: test.ura:6:12 expected ':' after the if condition
  |
6 |     if x > 0
  |            ^
help: use if x > 0:
error: aborting due to 1 error
1
```

## 212 — lexer: proto not followed by fn

```stderr
error: test.ura:4:9 expected '(' after the function name
  |
4 | proto x i32
  |         ^^^
help: use proto x() i32
error: aborting due to 1 error
1
```

## 213 — lexer: a struct declaration missing its name

```stderr
error: test.ura:4:7 expected a struct name after 'struct'
  |
4 | struct:
  |       ^
help: use struct Name:
error: aborting due to 1 error
1
```

## 214 — lexer: an unexpected token in an expression

```stderr
error: test.ura:5:13 unexpected '*'
  |
5 |     x i32 = *
  |             ^
error: aborting due to 1 error
1
```

## 215 — lexer: member access with no member name after .

```stderr
error: test.ura:11:6 expected an attribute or method name after '.'
   |
11 |     p.
   |      ^
error: aborting due to 1 error
1
```

## 216 — literal: bool literals: declare, zero-init, bool function, reassign

```llvm
define i1 @flag() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %c = alloca i1, align 1
  %d = alloca i1, align 1
  %call.flag = call i1 @flag()
  store i1 %call.flag, i1* %b, align 1
  store i1 false, i1* %c, align 1
  store i1 false, i1* %d, align 1
  store i1 true, i1* %c, align 1
  ret i32 9
}
```

## 217 — literal: char literals: declare, print, escapes, in a function

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"!\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i8 @grade() {
entry:
  ret i8 65
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size7 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  store i8 122, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c" to i32
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %AS)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS1 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS1, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %AS)
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  call void @"io::out"(%String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 2, i32 0 })
  %call.grade = call i8 @grade()
  %AS8 = sext i8 %call.grade to i32
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %AS8)
  store i32 %call.snprintf9, i32* %format.size7, align 4
  %"$format.size10" = load i32, i32* %format.size7, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %AS8)
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  call void @"io::out"(%String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 1, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 218 — loop: basic while: count enemy kills

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 0, i32* %kills, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$kills" = load i32, i32* %kills, align 4
  %LT = icmp slt i32 %"$kills", 5
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"$kills1" = load i32, i32* %kills, align 4
  %ADD = add i32 %"$kills1", 1
  store i32 %ADD, i32* %kills, align 4
  br label %while6

endloop:                                          ; preds = %while6
  %"$kills2" = load i32, i32* %kills, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills2")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD3 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills2")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 220 — operator: all arithmetic ops in dungeon combat context

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [21 x i8] c"atk + def:       %d\0A\00", align 1
@str.1 = private unnamed_addr constant [21 x i8] c"atk - def:       %d\0A\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"kills * 50:      %d\0A\00", align 1
@str.3 = private unnamed_addr constant [21 x i8] c"total / count:   %d\0A\00", align 1
@str.4 = private unnamed_addr constant [22 x i8] c"atk %% xp_cap:    %d\0A\00", align 1
@str.5 = private unnamed_addr constant [21 x i8] c"10 + 5:          %d\0A\00", align 1
@str.6 = private unnamed_addr constant [21 x i8] c"base_atk + atk:  %d\0A\00", align 1
@str.7 = private unnamed_addr constant [21 x i8] c"2 + 3 * 4:       %d\0A\00", align 1
@str.8 = private unnamed_addr constant [21 x i8] c"(2 + 3) * 4:     %d\0A\00", align 1
@str.9 = private unnamed_addr constant [10 x i8] c"add:  %f\0A\00", align 1
@str.10 = private unnamed_addr constant [10 x i8] c"sub:  %f\0A\00", align 1
@str.11 = private unnamed_addr constant [10 x i8] c"mul:  %f\0A\00", align 1
@str.12 = private unnamed_addr constant [10 x i8] c"div:  %f\0A\00", align 1
@str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [10 x i8] c"lt:   %s\0A\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [10 x i8] c"gt:   %s\0A\00", align 1
@str.19 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.20 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.21 = private unnamed_addr constant [10 x i8] c"lte:  %s\0A\00", align 1
@str.22 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.24 = private unnamed_addr constant [10 x i8] c"gte:  %s\0A\00", align 1
@str.25 = private unnamed_addr constant [17 x i8] c"clamp(0.52): %f\0A\00", align 1
@str.26 = private unnamed_addr constant [17 x i8] c"clamp(0.01): %f\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @base_atk() {
entry:
  ret i32 10
}

define float @clamp(float %0) {
entry:
  %t = alloca float, align 4
  store float %0, float* %t, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$t" = load float, float* %t, align 4
  %LT = fcmp olt float %"$t", 0x3FA99999A0000000
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  ret float 0x3FA99999A0000000

endif:                                            ; preds = %if6
  %"$t1" = load float, float* %t, align 4
  ret float %"$t1"
}

define i32 @main() {
entry:
  %atk = alloca i32, align 4
  %def = alloca i32, align 4
  %kills = alloca i32, align 4
  %total = alloca i32, align 4
  %count = alloca i32, align 4
  %xp_cap = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size12 = alloca i32, align 4
  %format.text17 = alloca i8*, align 8
  %format.size25 = alloca i32, align 4
  %format.text30 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %format.text43 = alloca i8*, align 8
  %format.size52 = alloca i32, align 4
  %format.text57 = alloca i8*, align 8
  %format.size65 = alloca i32, align 4
  %format.text70 = alloca i8*, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %format.size93 = alloca i32, align 4
  %format.text98 = alloca i8*, align 8
  %format.size106 = alloca i32, align 4
  %format.text111 = alloca i8*, align 8
  %a = alloca float, align 4
  %b = alloca float, align 4
  %format.size120 = alloca i32, align 4
  %format.text126 = alloca i8*, align 8
  %format.size137 = alloca i32, align 4
  %format.text143 = alloca i8*, align 8
  %format.size154 = alloca i32, align 4
  %format.text160 = alloca i8*, align 8
  %format.size171 = alloca i32, align 4
  %format.text177 = alloca i8*, align 8
  %format.size187 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text192 = alloca i8*, align 8
  %format.size203 = alloca i32, align 4
  %bool.text204 = alloca i8*, align 8
  %format.text210 = alloca i8*, align 8
  %format.size221 = alloca i32, align 4
  %bool.text222 = alloca i8*, align 8
  %format.text228 = alloca i8*, align 8
  %format.size239 = alloca i32, align 4
  %bool.text240 = alloca i8*, align 8
  %format.text246 = alloca i8*, align 8
  %t = alloca float, align 4
  %format.size255 = alloca i32, align 4
  %format.text261 = alloca i8*, align 8
  %format.size270 = alloca i32, align 4
  %format.text276 = alloca i8*, align 8
  store i32 25, i32* %atk, align 4
  store i32 8, i32* %def, align 4
  store i32 3, i32* %kills, align 4
  store i32 100, i32* %total, align 4
  store i32 4, i32* %count, align 4
  store i32 70, i32* %xp_cap, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %ADD = add i32 %"$atk", %"$def"
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %ADD)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i32 %ADD)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  %"$atk10" = load i32, i32* %atk, align 4
  %"$def11" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk10", %"$def11"
  %call.snprintf13 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  store i32 %call.snprintf13, i32* %format.size12, align 4
  %"$format.size14" = load i32, i32* %format.size12, align 4
  %ADD15 = add i32 %"$format.size14", 1
  %AS16 = sext i32 %ADD15 to i64
  %call.calloc18 = call i8* @calloc(i64 %AS16, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc18, i8** %format.text17, align 8
  %"$format.text19" = load i8*, i8** %format.text17, align 8
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text19", i64 %AS16, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  %"$format.size21" = load i32, i32* %format.size12, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %"$format.text23" = load i8*, i8** %format.text17, align 8
  %"$format.size24" = load i32, i32* %format.size12, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text23", 0
  %4 = insertvalue %String %3, i32 %"$format.size24", 1
  %5 = insertvalue %String %4, i32 %ADD22, 2
  call void @"io::out"(%String %5)
  %"$kills" = load i32, i32* %kills, align 4
  %MUL = mul i32 %"$kills", 50
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  store i32 %call.snprintf26, i32* %format.size25, align 4
  %"$format.size27" = load i32, i32* %format.size25, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %AS29 = sext i32 %ADD28 to i64
  %call.calloc31 = call i8* @calloc(i64 %AS29, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc31, i8** %format.text30, align 8
  %"$format.text32" = load i8*, i8** %format.text30, align 8
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text32", i64 %AS29, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  %"$format.size34" = load i32, i32* %format.size25, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %"$format.text36" = load i8*, i8** %format.text30, align 8
  %"$format.size37" = load i32, i32* %format.size25, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text36", 0
  %7 = insertvalue %String %6, i32 %"$format.size37", 1
  %8 = insertvalue %String %7, i32 %ADD35, 2
  call void @"io::out"(%String %8)
  %"$total" = load i32, i32* %total, align 4
  %"$count" = load i32, i32* %count, align 4
  %DIV = sdiv i32 %"$total", %"$count"
  %call.snprintf39 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.3, i32 0, i32 0), i32 %DIV)
  store i32 %call.snprintf39, i32* %format.size38, align 4
  %"$format.size40" = load i32, i32* %format.size38, align 4
  %ADD41 = add i32 %"$format.size40", 1
  %AS42 = sext i32 %ADD41 to i64
  %call.calloc44 = call i8* @calloc(i64 %AS42, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc44, i8** %format.text43, align 8
  %"$format.text45" = load i8*, i8** %format.text43, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text45", i64 %AS42, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.3, i32 0, i32 0), i32 %DIV)
  %"$format.size47" = load i32, i32* %format.size38, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text43, align 8
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %10 = insertvalue %String %9, i32 %"$format.size50", 1
  %11 = insertvalue %String %10, i32 %ADD48, 2
  call void @"io::out"(%String %11)
  %"$atk51" = load i32, i32* %atk, align 4
  %"$xp_cap" = load i32, i32* %xp_cap, align 4
  %MOD = srem i32 %"$atk51", %"$xp_cap"
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %MOD)
  store i32 %call.snprintf53, i32* %format.size52, align 4
  %"$format.size54" = load i32, i32* %format.size52, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %AS56 = sext i32 %ADD55 to i64
  %call.calloc58 = call i8* @calloc(i64 %AS56, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc58, i8** %format.text57, align 8
  %"$format.text59" = load i8*, i8** %format.text57, align 8
  %call.snprintf60 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text59", i64 %AS56, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %MOD)
  %"$format.size61" = load i32, i32* %format.size52, align 4
  %ADD62 = add i32 %"$format.size61", 1
  %"$format.text63" = load i8*, i8** %format.text57, align 8
  %"$format.size64" = load i32, i32* %format.size52, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text63", 0
  %13 = insertvalue %String %12, i32 %"$format.size64", 1
  %14 = insertvalue %String %13, i32 %ADD62, 2
  call void @"io::out"(%String %14)
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.5, i32 0, i32 0), i32 15)
  store i32 %call.snprintf66, i32* %format.size65, align 4
  %"$format.size67" = load i32, i32* %format.size65, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %call.snprintf73 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.5, i32 0, i32 0), i32 15)
  %"$format.size74" = load i32, i32* %format.size65, align 4
  %ADD75 = add i32 %"$format.size74", 1
  %"$format.text76" = load i8*, i8** %format.text70, align 8
  %"$format.size77" = load i32, i32* %format.size65, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text76", 0
  %16 = insertvalue %String %15, i32 %"$format.size77", 1
  %17 = insertvalue %String %16, i32 %ADD75, 2
  call void @"io::out"(%String %17)
  %call.base_atk = call i32 @base_atk()
  %"$atk78" = load i32, i32* %atk, align 4
  %ADD79 = add i32 %call.base_atk, %"$atk78"
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.6, i32 0, i32 0), i32 %ADD79)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.6, i32 0, i32 0), i32 %ADD79)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.snprintf94 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.7, i32 0, i32 0), i32 14)
  store i32 %call.snprintf94, i32* %format.size93, align 4
  %"$format.size95" = load i32, i32* %format.size93, align 4
  %ADD96 = add i32 %"$format.size95", 1
  %AS97 = sext i32 %ADD96 to i64
  %call.calloc99 = call i8* @calloc(i64 %AS97, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc99, i8** %format.text98, align 8
  %"$format.text100" = load i8*, i8** %format.text98, align 8
  %call.snprintf101 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text100", i64 %AS97, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.7, i32 0, i32 0), i32 14)
  %"$format.size102" = load i32, i32* %format.size93, align 4
  %ADD103 = add i32 %"$format.size102", 1
  %"$format.text104" = load i8*, i8** %format.text98, align 8
  %"$format.size105" = load i32, i32* %format.size93, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text104", 0
  %22 = insertvalue %String %21, i32 %"$format.size105", 1
  %23 = insertvalue %String %22, i32 %ADD103, 2
  call void @"io::out"(%String %23)
  %call.snprintf107 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.8, i32 0, i32 0), i32 20)
  store i32 %call.snprintf107, i32* %format.size106, align 4
  %"$format.size108" = load i32, i32* %format.size106, align 4
  %ADD109 = add i32 %"$format.size108", 1
  %AS110 = sext i32 %ADD109 to i64
  %call.calloc112 = call i8* @calloc(i64 %AS110, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc112, i8** %format.text111, align 8
  %"$format.text113" = load i8*, i8** %format.text111, align 8
  %call.snprintf114 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text113", i64 %AS110, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.8, i32 0, i32 0), i32 20)
  %"$format.size115" = load i32, i32* %format.size106, align 4
  %ADD116 = add i32 %"$format.size115", 1
  %"$format.text117" = load i8*, i8** %format.text111, align 8
  %"$format.size118" = load i32, i32* %format.size106, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text117", 0
  %25 = insertvalue %String %24, i32 %"$format.size118", 1
  %26 = insertvalue %String %25, i32 %ADD116, 2
  call void @"io::out"(%String %26)
  store float 1.500000e+00, float* %a, align 4
  store float 0x3FD3333340000000, float* %b, align 4
  %"$a" = load float, float* %a, align 4
  %"$b" = load float, float* %b, align 4
  %ADD119 = fadd float %"$a", %"$b"
  %AS121 = fpext float %ADD119 to double
  %call.snprintf122 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %AS121)
  store i32 %call.snprintf122, i32* %format.size120, align 4
  %"$format.size123" = load i32, i32* %format.size120, align 4
  %ADD124 = add i32 %"$format.size123", 1
  %AS125 = sext i32 %ADD124 to i64
  %call.calloc127 = call i8* @calloc(i64 %AS125, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc127, i8** %format.text126, align 8
  %"$format.text128" = load i8*, i8** %format.text126, align 8
  %call.snprintf129 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text128", i64 %AS125, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.9, i32 0, i32 0), double %AS121)
  %"$format.size130" = load i32, i32* %format.size120, align 4
  %ADD131 = add i32 %"$format.size130", 1
  %"$format.text132" = load i8*, i8** %format.text126, align 8
  %"$format.size133" = load i32, i32* %format.size120, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text132", 0
  %28 = insertvalue %String %27, i32 %"$format.size133", 1
  %29 = insertvalue %String %28, i32 %ADD131, 2
  call void @"io::out"(%String %29)
  %"$a134" = load float, float* %a, align 4
  %"$b135" = load float, float* %b, align 4
  %SUB136 = fsub float %"$a134", %"$b135"
  %AS138 = fpext float %SUB136 to double
  %call.snprintf139 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.10, i32 0, i32 0), double %AS138)
  store i32 %call.snprintf139, i32* %format.size137, align 4
  %"$format.size140" = load i32, i32* %format.size137, align 4
  %ADD141 = add i32 %"$format.size140", 1
  %AS142 = sext i32 %ADD141 to i64
  %call.calloc144 = call i8* @calloc(i64 %AS142, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc144, i8** %format.text143, align 8
  %"$format.text145" = load i8*, i8** %format.text143, align 8
  %call.snprintf146 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text145", i64 %AS142, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.10, i32 0, i32 0), double %AS138)
  %"$format.size147" = load i32, i32* %format.size137, align 4
  %ADD148 = add i32 %"$format.size147", 1
  %"$format.text149" = load i8*, i8** %format.text143, align 8
  %"$format.size150" = load i32, i32* %format.size137, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text149", 0
  %31 = insertvalue %String %30, i32 %"$format.size150", 1
  %32 = insertvalue %String %31, i32 %ADD148, 2
  call void @"io::out"(%String %32)
  %"$a151" = load float, float* %a, align 4
  %"$b152" = load float, float* %b, align 4
  %MUL153 = fmul float %"$a151", %"$b152"
  %AS155 = fpext float %MUL153 to double
  %call.snprintf156 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), double %AS155)
  store i32 %call.snprintf156, i32* %format.size154, align 4
  %"$format.size157" = load i32, i32* %format.size154, align 4
  %ADD158 = add i32 %"$format.size157", 1
  %AS159 = sext i32 %ADD158 to i64
  %call.calloc161 = call i8* @calloc(i64 %AS159, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc161, i8** %format.text160, align 8
  %"$format.text162" = load i8*, i8** %format.text160, align 8
  %call.snprintf163 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text162", i64 %AS159, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), double %AS155)
  %"$format.size164" = load i32, i32* %format.size154, align 4
  %ADD165 = add i32 %"$format.size164", 1
  %"$format.text166" = load i8*, i8** %format.text160, align 8
  %"$format.size167" = load i32, i32* %format.size154, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text166", 0
  %34 = insertvalue %String %33, i32 %"$format.size167", 1
  %35 = insertvalue %String %34, i32 %ADD165, 2
  call void @"io::out"(%String %35)
  %"$a168" = load float, float* %a, align 4
  %"$b169" = load float, float* %b, align 4
  %DIV170 = fdiv float %"$a168", %"$b169"
  %AS172 = fpext float %DIV170 to double
  %call.snprintf173 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), double %AS172)
  store i32 %call.snprintf173, i32* %format.size171, align 4
  %"$format.size174" = load i32, i32* %format.size171, align 4
  %ADD175 = add i32 %"$format.size174", 1
  %AS176 = sext i32 %ADD175 to i64
  %call.calloc178 = call i8* @calloc(i64 %AS176, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc178, i8** %format.text177, align 8
  %"$format.text179" = load i8*, i8** %format.text177, align 8
  %call.snprintf180 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text179", i64 %AS176, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), double %AS172)
  %"$format.size181" = load i32, i32* %format.size171, align 4
  %ADD182 = add i32 %"$format.size181", 1
  %"$format.text183" = load i8*, i8** %format.text177, align 8
  %"$format.size184" = load i32, i32* %format.size171, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text183", 0
  %37 = insertvalue %String %36, i32 %"$format.size184", 1
  %38 = insertvalue %String %37, i32 %ADD182, 2
  call void @"io::out"(%String %38)
  %"$a185" = load float, float* %a, align 4
  %"$b186" = load float, float* %b, align 4
  %LT = fcmp olt float %"$a185", %"$b186"
  store i32 0, i32* %format.size187, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if7

if7:                                              ; preds = %entry
  br i1 %LT, label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else8, %if7.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf188 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf188, i32* %format.size187, align 4
  %"$format.size189" = load i32, i32* %format.size187, align 4
  %ADD190 = add i32 %"$format.size189", 1
  %AS191 = sext i32 %ADD190 to i64
  %call.calloc193 = call i8* @calloc(i64 %AS191, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc193, i8** %format.text192, align 8
  %"$format.text194" = load i8*, i8** %format.text192, align 8
  %"$bool.text195" = load i8*, i8** %bool.text, align 8
  %call.snprintf196 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text194", i64 %AS191, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text195")
  %"$format.size197" = load i32, i32* %format.size187, align 4
  %ADD198 = add i32 %"$format.size197", 1
  %"$format.text199" = load i8*, i8** %format.text192, align 8
  %"$format.size200" = load i32, i32* %format.size187, align 4
  %39 = insertvalue %String zeroinitializer, i8* %"$format.text199", 0
  %40 = insertvalue %String %39, i32 %"$format.size200", 1
  %41 = insertvalue %String %40, i32 %ADD198, 2
  call void @"io::out"(%String %41)
  %"$a201" = load float, float* %a, align 4
  %"$b202" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a201", %"$b202"
  store i32 0, i32* %format.size203, align 4
  store i8* null, i8** %bool.text204, align 8
  br label %if9

if9:                                              ; preds = %endif
  br i1 %GT, label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text204, align 8
  br label %endif1

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text204, align 8
  br label %endif1

endif1:                                           ; preds = %else10, %if9.then
  %"$bool.text205" = load i8*, i8** %bool.text204, align 8
  %call.snprintf206 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text205")
  store i32 %call.snprintf206, i32* %format.size203, align 4
  %"$format.size207" = load i32, i32* %format.size203, align 4
  %ADD208 = add i32 %"$format.size207", 1
  %AS209 = sext i32 %ADD208 to i64
  %call.calloc211 = call i8* @calloc(i64 %AS209, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc211, i8** %format.text210, align 8
  %"$format.text212" = load i8*, i8** %format.text210, align 8
  %"$bool.text213" = load i8*, i8** %bool.text204, align 8
  %call.snprintf214 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text212", i64 %AS209, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text213")
  %"$format.size215" = load i32, i32* %format.size203, align 4
  %ADD216 = add i32 %"$format.size215", 1
  %"$format.text217" = load i8*, i8** %format.text210, align 8
  %"$format.size218" = load i32, i32* %format.size203, align 4
  %42 = insertvalue %String zeroinitializer, i8* %"$format.text217", 0
  %43 = insertvalue %String %42, i32 %"$format.size218", 1
  %44 = insertvalue %String %43, i32 %ADD216, 2
  call void @"io::out"(%String %44)
  %"$a219" = load float, float* %a, align 4
  %"$b220" = load float, float* %b, align 4
  %LE = fcmp ole float %"$a219", %"$b220"
  store i32 0, i32* %format.size221, align 4
  store i8* null, i8** %bool.text222, align 8
  br label %if11

if11:                                             ; preds = %endif1
  br i1 %LE, label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.19, i32 0, i32 0), i8** %bool.text222, align 8
  br label %endif2

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.20, i32 0, i32 0), i8** %bool.text222, align 8
  br label %endif2

endif2:                                           ; preds = %else12, %if11.then
  %"$bool.text223" = load i8*, i8** %bool.text222, align 8
  %call.snprintf224 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0), i8* %"$bool.text223")
  store i32 %call.snprintf224, i32* %format.size221, align 4
  %"$format.size225" = load i32, i32* %format.size221, align 4
  %ADD226 = add i32 %"$format.size225", 1
  %AS227 = sext i32 %ADD226 to i64
  %call.calloc229 = call i8* @calloc(i64 %AS227, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc229, i8** %format.text228, align 8
  %"$format.text230" = load i8*, i8** %format.text228, align 8
  %"$bool.text231" = load i8*, i8** %bool.text222, align 8
  %call.snprintf232 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text230", i64 %AS227, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0), i8* %"$bool.text231")
  %"$format.size233" = load i32, i32* %format.size221, align 4
  %ADD234 = add i32 %"$format.size233", 1
  %"$format.text235" = load i8*, i8** %format.text228, align 8
  %"$format.size236" = load i32, i32* %format.size221, align 4
  %45 = insertvalue %String zeroinitializer, i8* %"$format.text235", 0
  %46 = insertvalue %String %45, i32 %"$format.size236", 1
  %47 = insertvalue %String %46, i32 %ADD234, 2
  call void @"io::out"(%String %47)
  %"$a237" = load float, float* %a, align 4
  %"$b238" = load float, float* %b, align 4
  %GE = fcmp oge float %"$a237", %"$b238"
  store i32 0, i32* %format.size239, align 4
  store i8* null, i8** %bool.text240, align 8
  br label %if13

if13:                                             ; preds = %endif2
  br i1 %GE, label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.22, i32 0, i32 0), i8** %bool.text240, align 8
  br label %endif3

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0), i8** %bool.text240, align 8
  br label %endif3

endif3:                                           ; preds = %else14, %if13.then
  %"$bool.text241" = load i8*, i8** %bool.text240, align 8
  %call.snprintf242 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0), i8* %"$bool.text241")
  store i32 %call.snprintf242, i32* %format.size239, align 4
  %"$format.size243" = load i32, i32* %format.size239, align 4
  %ADD244 = add i32 %"$format.size243", 1
  %AS245 = sext i32 %ADD244 to i64
  %call.calloc247 = call i8* @calloc(i64 %AS245, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc247, i8** %format.text246, align 8
  %"$format.text248" = load i8*, i8** %format.text246, align 8
  %"$bool.text249" = load i8*, i8** %bool.text240, align 8
  %call.snprintf250 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text248", i64 %AS245, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0), i8* %"$bool.text249")
  %"$format.size251" = load i32, i32* %format.size239, align 4
  %ADD252 = add i32 %"$format.size251", 1
  %"$format.text253" = load i8*, i8** %format.text246, align 8
  %"$format.size254" = load i32, i32* %format.size239, align 4
  %48 = insertvalue %String zeroinitializer, i8* %"$format.text253", 0
  %49 = insertvalue %String %48, i32 %"$format.size254", 1
  %50 = insertvalue %String %49, i32 %ADD252, 2
  call void @"io::out"(%String %50)
  store float 0x3FE0A3D700000000, float* %t, align 4
  %"$t" = load float, float* %t, align 4
  %call.clamp = call float @clamp(float %"$t")
  %AS256 = fpext float %call.clamp to double
  %call.snprintf257 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.25, i32 0, i32 0), double %AS256)
  store i32 %call.snprintf257, i32* %format.size255, align 4
  %"$format.size258" = load i32, i32* %format.size255, align 4
  %ADD259 = add i32 %"$format.size258", 1
  %AS260 = sext i32 %ADD259 to i64
  %call.calloc262 = call i8* @calloc(i64 %AS260, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc262, i8** %format.text261, align 8
  %"$format.text263" = load i8*, i8** %format.text261, align 8
  %call.snprintf264 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text263", i64 %AS260, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.25, i32 0, i32 0), double %AS256)
  %"$format.size265" = load i32, i32* %format.size255, align 4
  %ADD266 = add i32 %"$format.size265", 1
  %"$format.text267" = load i8*, i8** %format.text261, align 8
  %"$format.size268" = load i32, i32* %format.size255, align 4
  %51 = insertvalue %String zeroinitializer, i8* %"$format.text267", 0
  %52 = insertvalue %String %51, i32 %"$format.size268", 1
  %53 = insertvalue %String %52, i32 %ADD266, 2
  call void @"io::out"(%String %53)
  %call.clamp269 = call float @clamp(float 0x3F847AE140000000)
  %AS271 = fpext float %call.clamp269 to double
  %call.snprintf272 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.26, i32 0, i32 0), double %AS271)
  store i32 %call.snprintf272, i32* %format.size270, align 4
  %"$format.size273" = load i32, i32* %format.size270, align 4
  %ADD274 = add i32 %"$format.size273", 1
  %AS275 = sext i32 %ADD274 to i64
  %call.calloc277 = call i8* @calloc(i64 %AS275, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc277, i8** %format.text276, align 8
  %"$format.text278" = load i8*, i8** %format.text276, align 8
  %call.snprintf279 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text278", i64 %AS275, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.26, i32 0, i32 0), double %AS271)
  %"$format.size280" = load i32, i32* %format.size270, align 4
  %ADD281 = add i32 %"$format.size280", 1
  %"$format.text282" = load i8*, i8** %format.text276, align 8
  %"$format.size283" = load i32, i32* %format.size270, align 4
  %54 = insertvalue %String zeroinitializer, i8* %"$format.text282", 0
  %55 = insertvalue %String %54, i32 %"$format.size283", 1
  %56 = insertvalue %String %55, i32 %ADD281, 2
  call void @"io::out"(%String %56)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 221 — operator: arithmetic with precedence (2*3 + 20/4 - 1)

```llvm
define i32 @main() {
entry:
  ret i32 10
}
```

## 222 — operator: reassign an existing variable (2*10 + 3)

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 10
  %ADD = add i32 %MUL, 3
  store i32 %ADD, i32* %a, align 4
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

## 223 — operator: modulo

```llvm
define i32 @main() {
entry:
  ret i32 2
}
```

## 224 — operator: all comparison ops on hero/enemy stats

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [23 x i8] c"hero_hp == min_hp: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [23 x i8] c"hero_hp != orc_hp: %s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [23 x i8] c"orc_hp < hero_hp:  %s\0A\00", align 1
@str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [23 x i8] c"orc_hp > hero_hp:  %s\0A\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [23 x i8] c"orc_hp <= 60:      %s\0A\00", align 1
@str.15 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.16 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.17 = private unnamed_addr constant [23 x i8] c"hero_hp >= 100:    %s\0A\00", align 1
@str.18 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@str.19 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@str.20 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @enemy_atk() {
entry:
  ret i32 18
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  %orc_hp = alloca i32, align 4
  %min_hp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %format.size33 = alloca i32, align 4
  %bool.text34 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %format.size51 = alloca i32, align 4
  %bool.text52 = alloca i8*, align 8
  %format.text58 = alloca i8*, align 8
  %format.size68 = alloca i32, align 4
  %bool.text69 = alloca i8*, align 8
  %format.text75 = alloca i8*, align 8
  %format.size85 = alloca i32, align 4
  %bool.text86 = alloca i8*, align 8
  %format.text92 = alloca i8*, align 8
  store i32 100, i32* %hero_hp, align 4
  store i32 60, i32* %orc_hp, align 4
  store i32 100, i32* %min_hp, align 4
  %"$hero_hp" = load i32, i32* %hero_hp, align 4
  %"$min_hp" = load i32, i32* %min_hp, align 4
  %EQ = icmp eq i32 %"$hero_hp", %"$min_hp"
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %EQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text, align 8
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text8")
  %"$format.size10" = load i32, i32* %format.size, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %"$format.text12" = load i8*, i8** %format.text, align 8
  %"$format.size13" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text12", 0
  %1 = insertvalue %String %0, i32 %"$format.size13", 1
  %2 = insertvalue %String %1, i32 %ADD11, 2
  call void @"io::out"(%String %2)
  %"$hero_hp14" = load i32, i32* %hero_hp, align 4
  %"$orc_hp" = load i32, i32* %orc_hp, align 4
  %NQ = icmp ne i32 %"$hero_hp14", %"$orc_hp"
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %NQ, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$orc_hp31" = load i32, i32* %orc_hp, align 4
  %"$hero_hp32" = load i32, i32* %hero_hp, align 4
  %LT = icmp slt i32 %"$orc_hp31", %"$hero_hp32"
  store i32 0, i32* %format.size33, align 4
  store i8* null, i8** %bool.text34, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %LT, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size33, align 4
  %"$format.size37" = load i32, i32* %format.size33, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text34, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size33, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size33, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  %"$orc_hp49" = load i32, i32* %orc_hp, align 4
  %"$hero_hp50" = load i32, i32* %hero_hp, align 4
  %GT = icmp sgt i32 %"$orc_hp49", %"$hero_hp50"
  store i32 0, i32* %format.size51, align 4
  store i8* null, i8** %bool.text52, align 8
  br label %if12

if12:                                             ; preds = %endif2
  br i1 %GT, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif3

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif3

endif3:                                           ; preds = %else13, %if12.then
  %"$bool.text53" = load i8*, i8** %bool.text52, align 8
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text53")
  store i32 %call.snprintf54, i32* %format.size51, align 4
  %"$format.size55" = load i32, i32* %format.size51, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %AS57 = sext i32 %ADD56 to i64
  %call.calloc59 = call i8* @calloc(i64 %AS57, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc59, i8** %format.text58, align 8
  %"$format.text60" = load i8*, i8** %format.text58, align 8
  %"$bool.text61" = load i8*, i8** %bool.text52, align 8
  %call.snprintf62 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text60", i64 %AS57, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text61")
  %"$format.size63" = load i32, i32* %format.size51, align 4
  %ADD64 = add i32 %"$format.size63", 1
  %"$format.text65" = load i8*, i8** %format.text58, align 8
  %"$format.size66" = load i32, i32* %format.size51, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text65", 0
  %10 = insertvalue %String %9, i32 %"$format.size66", 1
  %11 = insertvalue %String %10, i32 %ADD64, 2
  call void @"io::out"(%String %11)
  %"$orc_hp67" = load i32, i32* %orc_hp, align 4
  %LE = icmp sle i32 %"$orc_hp67", 60
  store i32 0, i32* %format.size68, align 4
  store i8* null, i8** %bool.text69, align 8
  br label %if14

if14:                                             ; preds = %endif3
  br i1 %LE, label %if14.then, label %else15

if14.then:                                        ; preds = %if14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text69, align 8
  br label %endif4

else15:                                           ; preds = %if14
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text69, align 8
  br label %endif4

endif4:                                           ; preds = %else15, %if14.then
  %"$bool.text70" = load i8*, i8** %bool.text69, align 8
  %call.snprintf71 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text70")
  store i32 %call.snprintf71, i32* %format.size68, align 4
  %"$format.size72" = load i32, i32* %format.size68, align 4
  %ADD73 = add i32 %"$format.size72", 1
  %AS74 = sext i32 %ADD73 to i64
  %call.calloc76 = call i8* @calloc(i64 %AS74, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc76, i8** %format.text75, align 8
  %"$format.text77" = load i8*, i8** %format.text75, align 8
  %"$bool.text78" = load i8*, i8** %bool.text69, align 8
  %call.snprintf79 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text77", i64 %AS74, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text78")
  %"$format.size80" = load i32, i32* %format.size68, align 4
  %ADD81 = add i32 %"$format.size80", 1
  %"$format.text82" = load i8*, i8** %format.text75, align 8
  %"$format.size83" = load i32, i32* %format.size68, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text82", 0
  %13 = insertvalue %String %12, i32 %"$format.size83", 1
  %14 = insertvalue %String %13, i32 %ADD81, 2
  call void @"io::out"(%String %14)
  %"$hero_hp84" = load i32, i32* %hero_hp, align 4
  %GE = icmp sge i32 %"$hero_hp84", 100
  store i32 0, i32* %format.size85, align 4
  store i8* null, i8** %bool.text86, align 8
  br label %if16

if16:                                             ; preds = %endif4
  br i1 %GE, label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0), i8** %bool.text86, align 8
  br label %endif5

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i8** %bool.text86, align 8
  br label %endif5

endif5:                                           ; preds = %else17, %if16.then
  %"$bool.text87" = load i8*, i8** %bool.text86, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.17, i32 0, i32 0), i8* %"$bool.text87")
  store i32 %call.snprintf88, i32* %format.size85, align 4
  %"$format.size89" = load i32, i32* %format.size85, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %AS91 = sext i32 %ADD90 to i64
  %call.calloc93 = call i8* @calloc(i64 %AS91, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc93, i8** %format.text92, align 8
  %"$format.text94" = load i8*, i8** %format.text92, align 8
  %"$bool.text95" = load i8*, i8** %bool.text86, align 8
  %call.snprintf96 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text94", i64 %AS91, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.17, i32 0, i32 0), i8* %"$bool.text95")
  %"$format.size97" = load i32, i32* %format.size85, align 4
  %ADD98 = add i32 %"$format.size97", 1
  %"$format.text99" = load i8*, i8** %format.text92, align 8
  %"$format.size100" = load i32, i32* %format.size85, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text99", 0
  %16 = insertvalue %String %15, i32 %"$format.size100", 1
  %17 = insertvalue %String %16, i32 %ADD98, 2
  call void @"io::out"(%String %17)
  br label %if18

if18:                                             ; preds = %endif5
  %call.enemy_atk = call i32 @enemy_atk()
  %GT101 = icmp sgt i32 %call.enemy_atk, 15
  br i1 %GT101, label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.18, i32 0, i32 0), i32 13, i32 0 })
  br label %endif6

else19:                                           ; preds = %if18
  call void @"io::out"(%String { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.19, i32 0, i32 0), i32 11, i32 0 })
  br label %endif6

endif6:                                           ; preds = %else19, %if18.then
  br label %if20

if20:                                             ; preds = %endif6
  %"$hero_hp102" = load i32, i32* %hero_hp, align 4
  %EQ103 = icmp eq i32 %"$hero_hp102", 100
  %"$orc_hp104" = load i32, i32* %orc_hp, align 4
  %"$hero_hp105" = load i32, i32* %hero_hp, align 4
  %LT106 = icmp slt i32 %"$orc_hp104", %"$hero_hp105"
  %AND = and i1 %EQ103, %LT106
  br i1 %AND, label %if20.then, label %endif7

if20.then:                                        ; preds = %if20
  call void @"io::out"(%String { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.20, i32 0, i32 0), i32 35, i32 0 })
  br label %endif7

endif7:                                           ; preds = %if20.then, %if20
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 225 — operator: comparisons produce bools (with precedence)

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %t = alloca i1, align 1
  %u = alloca i1, align 1
  %v = alloca i1, align 1
  %w = alloca i1, align 1
  %x = alloca i1, align 1
  %y = alloca i1, align 1
  %z = alloca i1, align 1
  %m = alloca i1, align 1
  store i32 5, i32* %a, align 4
  store i32 9, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %LT = icmp slt i32 %"$a", %"$b"
  store i1 %LT, i1* %t, align 1
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 5
  store i1 %EQ, i1* %u, align 1
  %"$b2" = load i32, i32* %b, align 4
  %"$a3" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$b2", %"$a3"
  store i1 %NQ, i1* %v, align 1
  %"$b4" = load i32, i32* %b, align 4
  %GE = icmp sge i32 %"$b4", 9
  store i1 %GE, i1* %w, align 1
  %"$a5" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a5", 4
  store i1 %LE, i1* %x, align 1
  %"$a6" = load i32, i32* %a, align 4
  %"$b7" = load i32, i32* %b, align 4
  %GT = icmp sgt i32 %"$a6", %"$b7"
  store i1 %GT, i1* %y, align 1
  %"$a8" = load i32, i32* %a, align 4
  %EQ9 = icmp eq i32 %"$a8", 5
  store i1 %EQ9, i1* %z, align 1
  %"$a10" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a10", 1
  %"$b11" = load i32, i32* %b, align 4
  %MUL = mul i32 %"$b11", 2
  %LT12 = icmp slt i32 %ADD, %MUL
  store i1 %LT12, i1* %m, align 1
  ret i32 3
}
```

## 226 — operator: compound assignment += -= *= /= %=

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %format.size24 = alloca i32, align 4
  %format.text30 = alloca i8*, align 8
  %format.size39 = alloca i32, align 4
  %format.text45 = alloca i8*, align 8
  %format.size54 = alloca i32, align 4
  %format.text60 = alloca i8*, align 8
  store i32 10, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 5
  store i32 %ADD, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x1")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD2 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD2 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x1")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %"$x8" = load i32, i32* %x, align 4
  %SUB = sub i32 %"$x8", 3
  store i32 %SUB, i32* %x, align 4
  %"$x10" = load i32, i32* %x, align 4
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x10")
  store i32 %call.snprintf11, i32* %format.size9, align 4
  %"$format.size12" = load i32, i32* %format.size9, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x10")
  %"$format.size19" = load i32, i32* %format.size9, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  %"$x23" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x23", 4
  store i32 %MUL, i32* %x, align 4
  %"$x25" = load i32, i32* %x, align 4
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$x25")
  store i32 %call.snprintf26, i32* %format.size24, align 4
  %"$format.size27" = load i32, i32* %format.size24, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %AS29 = sext i32 %ADD28 to i64
  %call.calloc31 = call i8* @calloc(i64 %AS29, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc31, i8** %format.text30, align 8
  %"$format.text32" = load i8*, i8** %format.text30, align 8
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text32", i64 %AS29, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$x25")
  %"$format.size34" = load i32, i32* %format.size24, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %"$format.text36" = load i8*, i8** %format.text30, align 8
  %"$format.size37" = load i32, i32* %format.size24, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text36", 0
  %7 = insertvalue %String %6, i32 %"$format.size37", 1
  %8 = insertvalue %String %7, i32 %ADD35, 2
  call void @"io::out"(%String %8)
  %"$x38" = load i32, i32* %x, align 4
  %DIV = sdiv i32 %"$x38", 6
  store i32 %DIV, i32* %x, align 4
  %"$x40" = load i32, i32* %x, align 4
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$x40")
  store i32 %call.snprintf41, i32* %format.size39, align 4
  %"$format.size42" = load i32, i32* %format.size39, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$x40")
  %"$format.size49" = load i32, i32* %format.size39, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %"$format.text51" = load i8*, i8** %format.text45, align 8
  %"$format.size52" = load i32, i32* %format.size39, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text51", 0
  %10 = insertvalue %String %9, i32 %"$format.size52", 1
  %11 = insertvalue %String %10, i32 %ADD50, 2
  call void @"io::out"(%String %11)
  %"$x53" = load i32, i32* %x, align 4
  %MOD = srem i32 %"$x53", 5
  store i32 %MOD, i32* %x, align 4
  %"$x55" = load i32, i32* %x, align 4
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$x55")
  store i32 %call.snprintf56, i32* %format.size54, align 4
  %"$format.size57" = load i32, i32* %format.size54, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %AS59 = sext i32 %ADD58 to i64
  %call.calloc61 = call i8* @calloc(i64 %AS59, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc61, i8** %format.text60, align 8
  %"$format.text62" = load i8*, i8** %format.text60, align 8
  %call.snprintf63 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text62", i64 %AS59, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$x55")
  %"$format.size64" = load i32, i32* %format.size54, align 4
  %ADD65 = add i32 %"$format.size64", 1
  %"$format.text66" = load i8*, i8** %format.text60, align 8
  %"$format.size67" = load i32, i32* %format.size54, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text66", 0
  %13 = insertvalue %String %12, i32 %"$format.size67", 1
  %14 = insertvalue %String %13, i32 %ADD65, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 227 — operator: logical ops: &&, ||, !, and, or, not

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"can_proceed: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [17 x i8] c"escape:      %s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [17 x i8] c"blocked:     %s\0A\00", align 1
@str.9 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@str.10 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@str.11 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [13 x i8] c"!!alive: %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %GT = icmp sgt i32 %"$hp", 0
  ret i1 %GT
}

define i1 @has_key(i32 %0) {
entry:
  %keys = alloca i32, align 4
  store i32 %0, i32* %keys, align 4
  %"$keys" = load i32, i32* %keys, align 4
  %GT = icmp sgt i32 %"$keys", 0
  ret i1 %GT
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %keys = alloca i32, align 4
  %exhausted = alloca i1, align 1
  %can_proceed = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %escape = alloca i1, align 1
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %blocked = alloca i1, align 1
  %format.size33 = alloca i32, align 4
  %bool.text34 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %alive = alloca i1, align 1
  %format.size63 = alloca i32, align 4
  %bool.text64 = alloca i8*, align 8
  %format.text70 = alloca i8*, align 8
  store i32 60, i32* %hp, align 4
  store i32 1, i32* %keys, align 4
  store i1 false, i1* %exhausted, align 1
  %"$hp" = load i32, i32* %hp, align 4
  %call.is_alive = call i1 @is_alive(i32 %"$hp")
  %"$keys" = load i32, i32* %keys, align 4
  %call.has_key = call i1 @has_key(i32 %"$keys")
  %AND = and i1 %call.is_alive, %call.has_key
  %"$exhausted" = load i1, i1* %exhausted, align 1
  %NOT = xor i1 %"$exhausted", true
  %AND6 = and i1 %AND, %NOT
  store i1 %AND6, i1* %can_proceed, align 1
  store i32 0, i32* %format.size, align 4
  %"$can_proceed" = load i1, i1* %can_proceed, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$can_proceed", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text7" = load i8*, i8** %bool.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text7")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  %"$hp13" = load i32, i32* %hp, align 4
  %LT = icmp slt i32 %"$hp13", 10
  %"$keys14" = load i32, i32* %keys, align 4
  %EQ = icmp eq i32 %"$keys14", 0
  %OR = or i1 %LT, %EQ
  store i1 %OR, i1* %escape, align 1
  store i32 0, i32* %format.size15, align 4
  %"$escape" = load i1, i1* %escape, align 1
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"$escape", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$can_proceed31" = load i1, i1* %can_proceed, align 1
  %NOT32 = xor i1 %"$can_proceed31", true
  store i1 %NOT32, i1* %blocked, align 1
  store i32 0, i32* %format.size33, align 4
  %"$blocked" = load i1, i1* %blocked, align 1
  store i8* null, i8** %bool.text34, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %"$blocked", label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size33, align 4
  %"$format.size37" = load i32, i32* %format.size33, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text34, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size33, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size33, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  br label %if12

if12:                                             ; preds = %endif2
  %"$hp49" = load i32, i32* %hp, align 4
  %call.is_alive50 = call i1 @is_alive(i32 %"$hp49")
  %"$exhausted51" = load i1, i1* %exhausted, align 1
  %NOT52 = xor i1 %"$exhausted51", true
  %AND53 = and i1 %call.is_alive50, %NOT52
  br i1 %AND53, label %if12.then, label %endif3

if12.then:                                        ; preds = %if12
  call void @"io::out"(%String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.9, i32 0, i32 0), i32 12, i32 0 })
  br label %endif3

endif3:                                           ; preds = %if12.then, %if12
  br label %if13

if13:                                             ; preds = %endif3
  %"$hp54" = load i32, i32* %hp, align 4
  %LT55 = icmp slt i32 %"$hp54", 10
  %"$keys56" = load i32, i32* %keys, align 4
  %EQ57 = icmp eq i32 %"$keys56", 0
  %OR58 = or i1 %LT55, %EQ57
  br i1 %OR58, label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.10, i32 0, i32 0), i32 14, i32 0 })
  br label %endif4

else14:                                           ; preds = %if13
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.11, i32 0, i32 0), i32 14, i32 0 })
  br label %endif4

endif4:                                           ; preds = %else14, %if13.then
  %"$hp59" = load i32, i32* %hp, align 4
  %call.is_alive60 = call i1 @is_alive(i32 %"$hp59")
  store i1 %call.is_alive60, i1* %alive, align 1
  %"$alive" = load i1, i1* %alive, align 1
  %NOT61 = xor i1 %"$alive", true
  %NOT62 = xor i1 %NOT61, true
  store i32 0, i32* %format.size63, align 4
  store i8* null, i8** %bool.text64, align 8
  br label %if15

if15:                                             ; preds = %endif4
  br i1 %NOT62, label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif5

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif5

endif5:                                           ; preds = %else16, %if15.then
  %"$bool.text65" = load i8*, i8** %bool.text64, align 8
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text65")
  store i32 %call.snprintf66, i32* %format.size63, align 4
  %"$format.size67" = load i32, i32* %format.size63, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %"$bool.text73" = load i8*, i8** %bool.text64, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text73")
  %"$format.size75" = load i32, i32* %format.size63, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %"$format.text77" = load i8*, i8** %format.text70, align 8
  %"$format.size78" = load i32, i32* %format.size63, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text77", 0
  %10 = insertvalue %String %9, i32 %"$format.size78", 1
  %11 = insertvalue %String %10, i32 %ADD76, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 228 — operator: and / or with comparisons and precedence

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %bool.text39 = alloca i8*, align 8
  %format.text45 = alloca i8*, align 8
  %format.size61 = alloca i32, align 4
  %bool.text62 = alloca i8*, align 8
  %format.text68 = alloca i8*, align 8
  store i32 5, i32* %a, align 4
  store i32 9, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %LT = icmp slt i32 %"$a", %"$b"
  %"$b4" = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %"$b4", 9
  %AND = and i1 %LT, %EQ
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %AND, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text5" = load i8*, i8** %bool.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text5")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  %"$a11" = load i32, i32* %a, align 4
  %"$b12" = load i32, i32* %b, align 4
  %GT = icmp sgt i32 %"$a11", %"$b12"
  %"$b13" = load i32, i32* %b, align 4
  %EQ14 = icmp eq i32 %"$b13", 9
  %OR = or i1 %GT, %EQ14
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %OR, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %"$a31" = load i32, i32* %a, align 4
  %"$b32" = load i32, i32* %b, align 4
  %LT33 = icmp slt i32 %"$a31", %"$b32"
  %"$a34" = load i32, i32* %a, align 4
  %"$b35" = load i32, i32* %b, align 4
  %GT36 = icmp sgt i32 %"$a34", %"$b35"
  %AND37 = and i1 %LT33, %GT36
  store i32 0, i32* %format.size38, align 4
  store i8* null, i8** %bool.text39, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %AND37, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text40" = load i8*, i8** %bool.text39, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text40")
  store i32 %call.snprintf41, i32* %format.size38, align 4
  %"$format.size42" = load i32, i32* %format.size38, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %"$bool.text48" = load i8*, i8** %bool.text39, align 8
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text48")
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %"$format.text52" = load i8*, i8** %format.text45, align 8
  %"$format.size53" = load i32, i32* %format.size38, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text52", 0
  %7 = insertvalue %String %6, i32 %"$format.size53", 1
  %8 = insertvalue %String %7, i32 %ADD51, 2
  call void @"io::out"(%String %8)
  %"$a54" = load i32, i32* %a, align 4
  %"$b55" = load i32, i32* %b, align 4
  %LT56 = icmp slt i32 %"$a54", %"$b55"
  %"$a57" = load i32, i32* %a, align 4
  %"$b58" = load i32, i32* %b, align 4
  %GT59 = icmp sgt i32 %"$a57", %"$b58"
  %OR60 = or i1 %LT56, %GT59
  store i32 0, i32* %format.size61, align 4
  store i8* null, i8** %bool.text62, align 8
  br label %if12

if12:                                             ; preds = %endif2
  br i1 %OR60, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif3

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif3

endif3:                                           ; preds = %else13, %if12.then
  %"$bool.text63" = load i8*, i8** %bool.text62, align 8
  %call.snprintf64 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text63")
  store i32 %call.snprintf64, i32* %format.size61, align 4
  %"$format.size65" = load i32, i32* %format.size61, align 4
  %ADD66 = add i32 %"$format.size65", 1
  %AS67 = sext i32 %ADD66 to i64
  %call.calloc69 = call i8* @calloc(i64 %AS67, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc69, i8** %format.text68, align 8
  %"$format.text70" = load i8*, i8** %format.text68, align 8
  %"$bool.text71" = load i8*, i8** %bool.text62, align 8
  %call.snprintf72 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text70", i64 %AS67, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i8* %"$bool.text71")
  %"$format.size73" = load i32, i32* %format.size61, align 4
  %ADD74 = add i32 %"$format.size73", 1
  %"$format.text75" = load i8*, i8** %format.text68, align 8
  %"$format.size76" = load i32, i32* %format.size61, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text75", 0
  %10 = insertvalue %String %9, i32 %"$format.size76", 1
  %11 = insertvalue %String %10, i32 %ADD74, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 229 — operator: type mismatch between operands

```stderr
error: test.ura:7:12 'twice' is a function, not a value
  |
7 |     return twice + 1
  |            ^^^^^
help: call it: twice(...)
error: aborting due to 1 error
1
```

## 230 — operator: a comparison result is a bool, not an i32

```stderr
error: test.ura:6:15 expected 'i32', found 'bool'
  |
6 |     n i32 = a < 9
  |               ^
error: aborting due to 1 error
1
```

## 231 — variable: hero stats: declare all common types

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [31 x i8] c"=== %s enters the dungeon ===\0A\00", align 1
@str.1 = private unnamed_addr constant [29 x i8] c"HP: %d | MP: %d | Speed: %f\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %alive = alloca i1, align 1
  %speed = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  %format.size20 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text25 = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i1 true, i1* %alive, align 1
  store float 1.500000e+00, float* %speed, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  %"$hp" = load i32, i32* %hp, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS7 = fpext float %"$speed" to double
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0), i32 %"$hp", i32 %"$mp", double %AS7)
  store i32 %call.snprintf8, i32* %format.size6, align 4
  %"$format.size9" = load i32, i32* %format.size6, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0), i32 %"$hp", i32 %"$mp", double %AS7)
  %"$format.size16" = load i32, i32* %format.size6, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size6, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %5 = insertvalue %String %4, i32 %"$format.size19", 1
  %6 = insertvalue %String %5, i32 %ADD17, 2
  call void @"io::out"(%String %6)
  store i32 0, i32* %format.size20, align 4
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf21, i32* %format.size20, align 4
  %"$format.size22" = load i32, i32* %format.size20, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %AS24 = sext i32 %ADD23 to i64
  %call.calloc26 = call i8* @calloc(i64 %AS24, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc26, i8** %format.text25, align 8
  %"$format.text27" = load i8*, i8** %format.text25, align 8
  %"$bool.text28" = load i8*, i8** %bool.text, align 8
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text27", i64 %AS24, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text28")
  %"$format.size30" = load i32, i32* %format.size20, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %"$format.text32" = load i8*, i8** %format.text25, align 8
  %"$format.size33" = load i32, i32* %format.size20, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text32", 0
  %8 = insertvalue %String %7, i32 %"$format.size33", 1
  %9 = insertvalue %String %8, i32 %ADD31, 2
  call void @"io::out"(%String %9)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 232 — variable: reassignment: hp loss, xp gain, mp drain

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"MP: %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"XP: %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %xp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %format.size23 = alloca i32, align 4
  %format.text29 = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i32 0, i32* %xp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %SUB = sub i32 %"$hp", 12
  store i32 %SUB, i32* %hp, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %SUB1 = sub i32 %"$mp", 10
  store i32 %SUB1, i32* %mp, align 4
  %"$xp" = load i32, i32* %xp, align 4
  %ADD = add i32 %"$xp", 50
  store i32 %ADD, i32* %xp, align 4
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 14, i32 0 })
  %"$hp2" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$hp2")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD3 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$hp2")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  %"$mp10" = load i32, i32* %mp, align 4
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %"$mp10")
  store i32 %call.snprintf11, i32* %format.size9, align 4
  %"$format.size12" = load i32, i32* %format.size9, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %"$mp10")
  %"$format.size19" = load i32, i32* %format.size9, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  %"$xp24" = load i32, i32* %xp, align 4
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"$xp24")
  store i32 %call.snprintf25, i32* %format.size23, align 4
  %"$format.size26" = load i32, i32* %format.size23, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %AS28 = sext i32 %ADD27 to i64
  %call.calloc30 = call i8* @calloc(i64 %AS28, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc30, i8** %format.text29, align 8
  %"$format.text31" = load i8*, i8** %format.text29, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text31", i64 %AS28, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"$xp24")
  %"$format.size33" = load i32, i32* %format.size23, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text29, align 8
  %"$format.size36" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %7 = insertvalue %String %6, i32 %"$format.size36", 1
  %8 = insertvalue %String %7, i32 %ADD34, 2
  call void @"io::out"(%String %8)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 233 — variable: all primitive types in one place

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"name:  %s\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"hp:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"xp:    %lld\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"level: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"grade: %c\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [11 x i8] c"alive: %s\0A\00", align 1
@str.8 = private unnamed_addr constant [11 x i8] c"ratio: %f\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %xp = alloca i64, align 8
  %level = alloca i16, align 2
  %grade = alloca i8, align 1
  %alive = alloca i1, align 1
  %ratio = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %format.size32 = alloca i32, align 4
  %format.text38 = alloca i8*, align 8
  %format.size46 = alloca i32, align 4
  %format.text52 = alloca i8*, align 8
  %format.size60 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text65 = alloca i8*, align 8
  %format.size74 = alloca i32, align 4
  %format.text80 = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i64 999999, i64* %xp, align 8
  store i16 12, i16* %level, align 2
  store i8 83, i8* %grade, align 1
  store i1 true, i1* %alive, align 1
  store float 0x3FEB333340000000, float* %ratio, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %5 = insertvalue %String %4, i32 %"$format.size18", 1
  %6 = insertvalue %String %5, i32 %ADD16, 2
  call void @"io::out"(%String %6)
  %"$xp" = load i64, i64* %xp, align 8
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %"$xp")
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i64 %"$xp")
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %8 = insertvalue %String %7, i32 %"$format.size31", 1
  %9 = insertvalue %String %8, i32 %ADD29, 2
  call void @"io::out"(%String %9)
  %"$level" = load i16, i16* %level, align 2
  %AS33 = sext i16 %"$level" to i32
  %call.snprintf34 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %AS33)
  store i32 %call.snprintf34, i32* %format.size32, align 4
  %"$format.size35" = load i32, i32* %format.size32, align 4
  %ADD36 = add i32 %"$format.size35", 1
  %AS37 = sext i32 %ADD36 to i64
  %call.calloc39 = call i8* @calloc(i64 %AS37, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc39, i8** %format.text38, align 8
  %"$format.text40" = load i8*, i8** %format.text38, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text40", i64 %AS37, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %AS33)
  %"$format.size42" = load i32, i32* %format.size32, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %"$format.text44" = load i8*, i8** %format.text38, align 8
  %"$format.size45" = load i32, i32* %format.size32, align 4
  %10 = insertvalue %String zeroinitializer, i8* %"$format.text44", 0
  %11 = insertvalue %String %10, i32 %"$format.size45", 1
  %12 = insertvalue %String %11, i32 %ADD43, 2
  call void @"io::out"(%String %12)
  %"$grade" = load i8, i8* %grade, align 1
  %AS47 = sext i8 %"$grade" to i32
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i32 %AS47)
  store i32 %call.snprintf48, i32* %format.size46, align 4
  %"$format.size49" = load i32, i32* %format.size46, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %AS51 = sext i32 %ADD50 to i64
  %call.calloc53 = call i8* @calloc(i64 %AS51, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc53, i8** %format.text52, align 8
  %"$format.text54" = load i8*, i8** %format.text52, align 8
  %call.snprintf55 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text54", i64 %AS51, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i32 %AS47)
  %"$format.size56" = load i32, i32* %format.size46, align 4
  %ADD57 = add i32 %"$format.size56", 1
  %"$format.text58" = load i8*, i8** %format.text52, align 8
  %"$format.size59" = load i32, i32* %format.size46, align 4
  %13 = insertvalue %String zeroinitializer, i8* %"$format.text58", 0
  %14 = insertvalue %String %13, i32 %"$format.size59", 1
  %15 = insertvalue %String %14, i32 %ADD57, 2
  call void @"io::out"(%String %15)
  store i32 0, i32* %format.size60, align 4
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf61, i32* %format.size60, align 4
  %"$format.size62" = load i32, i32* %format.size60, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %AS64 = sext i32 %ADD63 to i64
  %call.calloc66 = call i8* @calloc(i64 %AS64, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc66, i8** %format.text65, align 8
  %"$format.text67" = load i8*, i8** %format.text65, align 8
  %"$bool.text68" = load i8*, i8** %bool.text, align 8
  %call.snprintf69 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text67", i64 %AS64, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text68")
  %"$format.size70" = load i32, i32* %format.size60, align 4
  %ADD71 = add i32 %"$format.size70", 1
  %"$format.text72" = load i8*, i8** %format.text65, align 8
  %"$format.size73" = load i32, i32* %format.size60, align 4
  %16 = insertvalue %String zeroinitializer, i8* %"$format.text72", 0
  %17 = insertvalue %String %16, i32 %"$format.size73", 1
  %18 = insertvalue %String %17, i32 %ADD71, 2
  call void @"io::out"(%String %18)
  %"$ratio" = load float, float* %ratio, align 4
  %AS75 = fpext float %"$ratio" to double
  %call.snprintf76 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), double %AS75)
  store i32 %call.snprintf76, i32* %format.size74, align 4
  %"$format.size77" = load i32, i32* %format.size74, align 4
  %ADD78 = add i32 %"$format.size77", 1
  %AS79 = sext i32 %ADD78 to i64
  %call.calloc81 = call i8* @calloc(i64 %AS79, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc81, i8** %format.text80, align 8
  %"$format.text82" = load i8*, i8** %format.text80, align 8
  %call.snprintf83 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text82", i64 %AS79, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.8, i32 0, i32 0), double %AS75)
  %"$format.size84" = load i32, i32* %format.size74, align 4
  %ADD85 = add i32 %"$format.size84", 1
  %"$format.text86" = load i8*, i8** %format.text80, align 8
  %"$format.size87" = load i32, i32* %format.size74, align 4
  %19 = insertvalue %String zeroinitializer, i8* %"$format.text86", 0
  %20 = insertvalue %String %19, i32 %"$format.size87", 1
  %21 = insertvalue %String %20, i32 %ADD85, 2
  call void @"io::out"(%String %21)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 234 — variable: a use before a shadowing declaration binds to the outer one

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"outer %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"inner %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"loop y %d\0A\00", align 1
@str.4 = private unnamed_addr constant [13 x i8] c"shadow y %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @pick(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %x6 = alloca i32, align 4
  %format.size7 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %format.size21 = alloca i32, align 4
  %format.text27 = alloca i8*, align 8
  %y = alloca i32, align 4
  %format.size35 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %y49 = alloca i32, align 4
  %format.size50 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  store i32 1, i32* %x, align 4
  br label %if6

if6:                                              ; preds = %entry
  br i1 true, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store i32 2, i32* %x6, align 4
  %"$x8" = load i32, i32* %x6, align 4
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$x8")
  store i32 %call.snprintf9, i32* %format.size7, align 4
  %"$format.size10" = load i32, i32* %format.size7, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$x8")
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"$x22" = load i32, i32* %x, align 4
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$x22")
  store i32 %call.snprintf23, i32* %format.size21, align 4
  %"$format.size24" = load i32, i32* %format.size21, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc28 = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc28, i8** %format.text27, align 8
  %"$format.text29" = load i8*, i8** %format.text27, align 8
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text29", i64 %AS26, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"$x22")
  %"$format.size31" = load i32, i32* %format.size21, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %"$format.text33" = load i8*, i8** %format.text27, align 8
  %"$format.size34" = load i32, i32* %format.size21, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text33", 0
  %7 = insertvalue %String %6, i32 %"$format.size34", 1
  %8 = insertvalue %String %7, i32 %ADD32, 2
  call void @"io::out"(%String %8)
  %call.pick = call i32 @pick(i32 3)
  store i32 %call.pick, i32* %y, align 4
  br label %while7

while7:                                           ; preds = %endif
  %"$y" = load i32, i32* %y, align 4
  %GT = icmp sgt i32 %"$y", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$y36" = load i32, i32* %y, align 4
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %"$y36")
  store i32 %call.snprintf37, i32* %format.size35, align 4
  %"$format.size38" = load i32, i32* %format.size35, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %"$y36")
  %"$format.size45" = load i32, i32* %format.size35, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size35, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %10 = insertvalue %String %9, i32 %"$format.size48", 1
  %11 = insertvalue %String %10, i32 %ADD46, 2
  call void @"io::out"(%String %11)
  store i32 0, i32* %y49, align 4
  %"$y51" = load i32, i32* %y49, align 4
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0), i32 %"$y51")
  store i32 %call.snprintf52, i32* %format.size50, align 4
  %"$format.size53" = load i32, i32* %format.size50, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.4, i32 0, i32 0), i32 %"$y51")
  %"$format.size60" = load i32, i32* %format.size50, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size50, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %13 = insertvalue %String %12, i32 %"$format.size63", 1
  %14 = insertvalue %String %13, i32 %ADD61, 2
  call void @"io::out"(%String %14)
  br label %endloop

endloop:                                          ; preds = %while7.then, %while7
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 235 — variable: redeclaring a variable in the same scope

```stderr
error: test.ura:6:5 'a' is already declared, on line 5
  |
6 |     a i32 = 2
  |     ^
help: use a = 2
error: aborting due to 1 error
1
```

## 236 — variable: using a name that was never declared

```stderr
error: test.ura:6:16 undeclared variable 'y'
  |
6 |     io::out($"{y}\n")
  |                ^
help: did you mean 'x'?
error: aborting due to 1 error
1
```

## 237 — variable: assigning to a name that was never declared

```stderr
error: test.ura:5:5 undeclared variable 'score'
  |
5 |     score = 100
  |     ^^^^^
help: declare it first, like: score i32 = 0
error: test.ura:6:16 undeclared variable 'score'
  |
6 |     io::out($"{score}\n")
  |                ^^^^^
help: declare it first, like: score i32 = 0
error: aborting due to 2 errors
1
```

## 238 — variable: a variable does not outlive the block that declared it

```stderr
error: test.ura:8:16 undeclared variable 'y'
  |
8 |     io::out($"{y}\n")
  |                ^
help: did you mean 'x'?
error: aborting due to 1 error
1
```

## 239 — variable: redeclaring a global

```stderr
error: test.ura:5:1 'score' is already declared, on line 4
  |
5 | score i32 = 10
  | ^^^^^
help: use score = 10
error: aborting due to 1 error
1
```

## 240 — variable: a bool takes only True or False

```stderr
error: test.ura:5:14 expected 'bool', found 'i32'
  |
5 |     b bool = 5
  |              ^
error: aborting due to 1 error
1
```

## 241 — variable: assigning a string or a struct to an integer

```stderr
error: test.ura:8:13 expected 'i32', found 'String'
  |
8 |     a i32 = "hello"
  |             ^^^^^^^
error: test.ura:10:13 expected 'i32', found 'Point'
   |
10 |     b i32 = p
   |             ^
error: aborting due to 2 errors
1
```

## 242 — proto: a string passed to a fn with an i8[] param

```llvm
@str = private unnamed_addr constant [7 x i8] c"hi %s\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"ura\00", align 1

declare i32 @printf(i8*, ...)

define void @greet(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* %"$name")
  ret void
}

define i32 @main() {
entry:
  call void @greet(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

## 243 — variables: unary minus on a variable and a parameter

```llvm
define i32 @neg(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %NEG = sub i32 0, %"$n"
  ret i32 %NEG
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %NEG = sub i32 0, %"$a"
  store i32 %NEG, i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.neg = call i32 @neg(i32 %"$b")
  %ADD = add i32 %call.neg, 7
  ret i32 %ADD
}
```

## 244 — global scope: a top-level statement runs before main

```llvm
@count = global i32 0

define i32 @main() {
entry:
  store i32 10, i32* @count, align 4
  %"$count" = load i32, i32* @count, align 4
  %ADD = add i32 %"$count", 5
  store i32 %ADD, i32* @count, align 4
  %"$count1" = load i32, i32* @count, align 4
  ret i32 %"$count1"
}
```

## 245 — function: early bare return in a void function

```llvm
@total = global i32 0

define void @add_small(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 2
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %"$total" = load i32, i32* @total, align 4
  %ADD = add i32 %"$total", %"$n1"
  store i32 %ADD, i32* @total, align 4
  ret void
}

define i32 @main() {
entry:
  store i32 0, i32* @total, align 4
  call void @add_small(i32 1)
  call void @add_small(i32 5)
  call void @add_small(i32 2)
  %"$total" = load i32, i32* @total, align 4
  ret i32 %"$total"
}
```

## 246 — bool: i1 and bool are the same type

```llvm
define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 true, i1* %a, align 1
  %"$a" = load i1, i1* %a, align 1
  store i1 %"$a", i1* %b, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 2

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 247 — tokenizer: a block comment over two lines, /*/ does not close it

```llvm
define i32 @main() {
entry:
  ret i32 5
}
```

## 248 — calls: arguments over several lines

```llvm
define i32 @add(i32 %0, i32 %1, i32 %2) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store i32 %2, i32* %c, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  %"$c" = load i32, i32* %c, align 4
  %ADD1 = add i32 %ADD, %"$c"
  ret i32 %ADD1
}

define i32 @main() {
entry:
  %call.add = call i32 @add(i32 1, i32 2, i32 3)
  ret i32 %call.add
}
```

## 249 — variables: a number literal takes the type of its i8 target

```llvm
define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 5, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 5
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 250 — variables: a variable first set inside an if still starts at zero

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 0, i32* %c, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 50
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 7, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a2" = load i32, i32* %a, align 4
  %GT3 = icmp sgt i32 %"$a2", 5
  br i1 %GT3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store i32 3, i32* %b, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  %"$a4" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a4", %"$b"
  %"$c" = load i32, i32* %c, align 4
  %ADD5 = add i32 %ADD, %"$c"
  ret i32 %ADD5
}
```

## 251 — variables: a variable with the name of a parameter

```stderr
error: test.ura:3:5 'r' is already declared, on line 2
  |
3 |     r i32 = 2
  |     ^
help: use r = 2
error: aborting due to 1 error
1
```

## 252 — unsigned: u8 wraps, u32 divides and compares unsigned, u64, casts

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@TYPEOF = private unnamed_addr constant [3 x i8] c"u8\00", align 1
@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"%s %u %u %u %s %llu\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %f %u\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %small = alloca i8, align 1
  %big = alloca i32, align 4
  %half = alloca i32, align 4
  %wide = alloca i64, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size14 = alloca i32, align 4
  %format.text19 = alloca i8*, align 8
  store i8 -6, i8* %small, align 1
  %"$small" = load i8, i8* %small, align 1
  %ADD = add i8 %"$small", 10
  store i8 %ADD, i8* %small, align 1
  store i32 -294967296, i32* %big, align 4
  %"$big" = load i32, i32* %big, align 4
  %DIV = udiv i32 %"$big", 2
  store i32 %DIV, i32* %half, align 4
  store i64 18000000000, i64* %wide, align 8
  %"$big1" = load i32, i32* %big, align 4
  %GT = icmp ugt i32 %"$big1", 100
  store i32 0, i32* %format.size, align 4
  %"$small2" = load i8, i8* %small, align 1
  %AS = zext i8 %"$small2" to i32
  %"$big3" = load i32, i32* %big, align 4
  %"$half" = load i32, i32* %half, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$wide" = load i64, i64* %wide, align 8
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @TYPEOF, i32 0, i32 0), i32 %AS, i32 %"$big3", i32 %"$half", i8* %"$bool.text", i64 %"$wide")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS5 = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS5, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS5, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @TYPEOF, i32 0, i32 0), i32 %AS, i32 %"$big3", i32 %"$half", i8* %"$bool.text6", i64 %"$wide")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  %"$big12" = load i32, i32* %big, align 4
  %AS13 = uitofp i32 %"$big12" to double
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), double %AS13, i32 255)
  store i32 %call.snprintf15, i32* %format.size14, align 4
  %"$format.size16" = load i32, i32* %format.size14, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %AS18 = sext i32 %ADD17 to i64
  %call.calloc20 = call i8* @calloc(i64 %AS18, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc20, i8** %format.text19, align 8
  %"$format.text21" = load i8*, i8** %format.text19, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text21", i64 %AS18, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), double %AS13, i32 255)
  %"$format.size23" = load i32, i32* %format.size14, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text19, align 8
  %"$format.size26" = load i32, i32* %format.size14, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %4 = insertvalue %String %3, i32 %"$format.size26", 1
  %5 = insertvalue %String %4, i32 %ADD24, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 253 — unsigned: a number that doesn't fit in u8

```stderr
error: test.ura:3:12 256 doesn't fit in u8
  |
3 |     x u8 = 256
  |            ^^^
help: u8 goes from 0 to 255
error: aborting due to 1 error
1
```

## 254 — variables: the type written before the name

```stderr
error: test.ura:6:5 the type goes after the name
  |
6 |     i32 x = 1
  |     ^^^
help: use x i32 = 1
error: test.ura:7:5 the type goes after the name
  |
7 |     i8[] s = "ab"
  |     ^^^^
help: use s i8[] = "ab"
error: test.ura:8:5 the type goes after the name
  |
8 |     Point p
  |     ^^^^^
help: use p Point
error: aborting due to 3 errors
1
```

## 255 — bool: not, with and / or and in if / elif

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"no\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"yes %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %big = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store i32 5, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %LT = icmp slt i32 %"$x", 3
  %NOT = xor i1 %LT, true
  store i1 %NOT, i1* %big, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"$big" = load i1, i1* %big, align 1
  %NOT2 = xor i1 %"$big", true
  %"$x3" = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %"$x3", 0
  %OR = or i1 %NOT2, %EQ
  br i1 %OR, label %if6.then, label %elif7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  br label %endif1

elif7:                                            ; preds = %if6
  %"$x4" = load i32, i32* %x, align 4
  %GT = icmp sgt i32 %"$x4", 10
  %NOT5 = xor i1 %GT, true
  %"$big6" = load i1, i1* %big, align 1
  %AND = and i1 %NOT5, %"$big6"
  br i1 %AND, label %elif7.then, label %endif1

elif7.then:                                       ; preds = %elif7
  %"$big7" = load i1, i1* %big, align 1
  %NOT8 = xor i1 %"$big7", true
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if8

if8:                                              ; preds = %elif7.then
  br i1 %NOT8, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text9" = load i8*, i8** %bool.text, align 8
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text9")
  %"$format.size11" = load i32, i32* %format.size, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %"$format.text13" = load i8*, i8** %format.text, align 8
  %"$format.size14" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text13", 0
  %1 = insertvalue %String %0, i32 %"$format.size14", 1
  %2 = insertvalue %String %1, i32 %ADD12, 2
  call void @"io::out"(%String %2)
  br label %endif1

endif1:                                           ; preds = %endif, %elif7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 256 — bool: not on an i32

```stderr
error: test.ura:4:16 expected 'bool', found 'i32'
  |
4 |     return not x
  |                ^
error: test.ura:4:12 expected 'i32', found 'bool'
  |
4 |     return not x
  |            ^^^
error: aborting due to 2 errors
1
```

## 257 — float: 2.5f is an f32

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"%f %f %f\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store float -2.500000e+00, float* %a, align 4
  %"$a" = load float, float* %a, align 4
  %ADD = fadd float 1.500000e+00, %"$a"
  store float %ADD, float* %b, align 4
  store float 5.000000e-01, float* %c, align 4
  %"$a1" = load float, float* %a, align 4
  %AS = fpext float %"$a1" to double
  %"$b" = load float, float* %b, align 4
  %AS2 = fpext float %"$b" to double
  %"$c" = load float, float* %c, align 4
  %AS3 = fpext float %"$c" to double
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %AS, double %AS2, double %AS3)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS5 = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS5, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS5, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), double %AS, double %AS2, double %AS3)
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 258 — float: a negative f64 literal into an f32

```stderr
error: test.ura:4:13 expected 'f32', found 'f64'
  |
4 |     a f32 = -2.5
  |             ^
help: write -2.5f for an f32
error: aborting due to 1 error
1
```
