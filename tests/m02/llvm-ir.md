# m02 llvm ir

## 001 — array: literal and indexing

```llvm
define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 4, i32 5, i32 6], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 2
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %ADD = add i32 %"$a[]", %"$a[]3"
  ret i32 %ADD
}
```

## 002 — array: write through an index

```llvm
define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 1
  store i32 9, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a[]" = load i32, i32* %"a[]2", align 4
  ret i32 %"$a[]"
}
```

## 003 — array: index from a variable

```llvm
define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  store [3 x i32] [i32 7, i32 8, i32 9], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i"
  %"$a[]" = load i32, i32* %"a[]", align 4
  ret i32 %"$a[]"
}
```

## 004 — array: index from an expression

```llvm
define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  store [3 x i32] [i32 7, i32 8, i32 9], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %"$i" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i", 1
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %SUB
  %"$a[]" = load i32, i32* %"a[]", align 4
  ret i32 %"$a[]"
}
```

## 005 — array: literal made of variables

```llvm
define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store i32 10, i32* %x, align 4
  store i32 4, i32* %y, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %0 = insertvalue [3 x i32] undef, i32 %"$x", 0
  %1 = insertvalue [3 x i32] %0, i32 2, 1
  %2 = insertvalue [3 x i32] %1, i32 %"$y", 2
  store [3 x i32] %2, [3 x i32]* %arr, align 4
  %3 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %3, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %ADD = add i32 %"$a[]", %"$a[]3"
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 2
  %"$a[]6" = load i32, i32* %"a[]5", align 4
  %ADD7 = add i32 %ADD, %"$a[]6"
  ret i32 %ADD7
}
```

## 006 — array: compound assignment on an element

```llvm
define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a[]" = load i32, i32* %"a[]", align 4
  %ADD = add i32 %"$a[]", 4
  store i32 %ADD, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 2
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %MUL = mul i32 %"$a[]3", 3
  store i32 %MUL, i32* %"a[]2", align 4
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 0
  %"$a6" = load i32*, i32** %a, align 8
  %"a[]7" = getelementptr inbounds i32, i32* %"$a6", i32 1
  %"$a[]8" = load i32, i32* %"a[]5", align 4
  %"$a[]9" = load i32, i32* %"a[]7", align 4
  %ADD10 = add i32 %"$a[]8", %"$a[]9"
  %"$a11" = load i32*, i32** %a, align 8
  %"a[]12" = getelementptr inbounds i32, i32* %"$a11", i32 2
  %"$a[]13" = load i32, i32* %"a[]12", align 4
  %ADD14 = add i32 %ADD10, %"$a[]13"
  %ADD15 = add i32 %ADD14, 2
  ret i32 %ADD15
}
```

## 007 — array: loop over an array

```llvm
@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  store [5 x i32] [i32 3, i32 1, i32 4, i32 1, i32 5], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$a[]")
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %LT3 = icmp slt i32 %"$i2", 4
  br i1 %LT3, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$a5" = load i32*, i32** %a, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"a[]7" = getelementptr inbounds i32, i32* %"$a5", i32 %"$i6"
  %"$a[]8" = load i32, i32* %"a[]7", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$a[]8"
  store i32 %ADD, i32* %sum, align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD10 = add i32 %"$i9", 1
  store i32 %ADD10, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %"$sum12" = load i32, i32* %sum, align 4
  ret i32 %"$sum12"
}
```

## 008 — array: an array passed to a function

```llvm
define i32 @sum(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$a[]"
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
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  %call.sum = call i32 @sum(i32* %"$values", i32 3)
  ret i32 %call.sum
}
```

## 009 — array: a function writes into the caller's array

```llvm
define void @set_first(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %v = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %v, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"a[]", align 4
  ret void
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  call void @set_first(i32* %"$values", i32 7)
  %"$values1" = load i32*, i32** %values, align 8
  %"values[]" = getelementptr inbounds i32, i32* %"$values1", i32 0
  %"$values[]" = load i32, i32* %"values[]", align 4
  ret i32 %"$values[]"
}
```

## 010 — array: a string is a writable i8[]

```llvm
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  store i8 120, i8* %"s[]", align 1
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  br label %if0

if0:                                              ; preds = %entry
  %"$s2" = load i8*, i8** %s, align 8
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 1
  %"$s[]" = load i8, i8* %"s[]3", align 1
  %EQ = icmp eq i8 %"$s[]", 98
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 011 — array: an array of bools

```llvm
define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %arr = alloca [3 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store [3 x i1] [i1 true, i1 false, i1 true], [3 x i1]* %arr, align 1
  %0 = getelementptr inbounds [3 x i1], [3 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$flags" = load i1*, i1** %flags, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"flags[]" = getelementptr inbounds i1, i1* %"$flags", i32 %"$i1"
  %"$flags[]" = load i1, i1* %"flags[]", align 1
  br i1 %"$flags[]", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n4" = load i32, i32* %n, align 4
  ret i32 %"$n4"
}
```

## 012 — proto: a string in a variable

```llvm
@str = private unnamed_addr constant [6 x i8] c"<%s>\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

## 014 — proto: proto variadic function

```llvm
@str = private unnamed_addr constant [5 x i8] c"<%s>\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

## 015 — proto: chars type

```llvm
define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  ret i32 0
}
```

## 016 — ref: write through a ref

```llvm
@str = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  store i32 12, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$a")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

## 017 — ref: read through a ref, then write

```llvm
@str = private unnamed_addr constant [9 x i8] c"p = %d, \00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*p")
  %"*p1" = load i32*, i32** %p, align 8
  %"*p2" = load i32*, i32** %p, align 8
  %"$*p3" = load i32, i32* %"*p2", align 4
  %ADD = add i32 %"$*p3", 5
  store i32 %ADD, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$a")
  %"$a5" = load i32, i32* %a, align 4
  ret i32 %"$a5"
}
```

## 018 — ref: compound assignment through a ref

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 2
  store i32 %ADD, i32* %"*p", align 4
  %"*p1" = load i32*, i32** %p, align 8
  %"$*p2" = load i32, i32* %"*p1", align 4
  %MUL = mul i32 %"$*p2", 2
  store i32 %MUL, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 019 — ref: swap with ref parameters

```llvm
@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"$*x" = load i32, i32* %"*x", align 4
  store i32 %"$*x", i32* %t, align 4
  %"*x1" = load i32*, i32** %x, align 8
  %"*y" = load i32*, i32** %y, align 8
  %"$*y" = load i32, i32* %"*y", align 4
  store i32 %"$*y", i32* %"*x1", align 4
  %"*y2" = load i32*, i32** %y, align 8
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*y2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 10
  %"$b2" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b2"
  ret i32 %ADD
}
```

## 020 — ref: a ref parameter changes the caller

```llvm
define void @inc(i32* %0) {
entry:
  %c = alloca i32*, align 8
  store i32* %0, i32** %c, align 8
  %"*c" = load i32*, i32** %c, align 8
  %"$*c" = load i32, i32* %"*c", align 4
  %ADD = add i32 %"$*c", 1
  store i32 %ADD, i32* %"*c", align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

## 021 — ref: p1 = p2 copies the value

```llvm
@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  %"$*p2" = load i32, i32* %"*p2", align 4
  store i32 %"$*p2", i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

## 022 — ref: p1 = &p2 rebinds

```llvm
@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  store i32* %"*p2", i32** %p1, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  store i32 9, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$b1" = load i32, i32* %b, align 4
  ret i32 %"$b1"
}
```

## 023 — ref: pass a ref parameter on to another function

```llvm
define void @add_one(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 1
  store i32 %ADD, i32* %"*p", align 4
  ret void
}

define void @add_two(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p")
  %"*p1" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p1")
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  call void @add_two(i32* %a)
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 024 — ref: a ref to a bool

```llvm
define i32 @main() {
entry:
  %b = alloca i1, align 1
  %p = alloca i1*, align 8
  store i1 false, i1* %b, align 1
  store i1* %b, i1** %p, align 8
  %"*p" = load i1*, i1** %p, align 8
  store i1 true, i1* %"*p", align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 025 — ref: a ref in a loop

```llvm
@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %total = alloca i32, align 4
  %p = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %total, align 4
  store i32* %total, i32** %p, align 8
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i1")
  %"*p" = load i32*, i32** %p, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", %"$i2"
  store i32 %ADD, i32* %"*p", align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$total" = load i32, i32* %total, align 4
  ret i32 %"$total"
}
```

## 026 — ref: take the address of a variable

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  ret i32 0
}
```

## 027 — crazy: fibonacci with a memo array

```llvm
define i32 @fib(i32 %0, i32* %1) {
entry:
  %n = alloca i32, align 4
  %memo = alloca i32*, align 8
  store i32 %0, i32* %n, align 4
  store i32* %1, i32** %memo, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$memo" = load i32*, i32** %memo, align 8
  %"$n3" = load i32, i32* %n, align 4
  %"memo[]" = getelementptr inbounds i32, i32* %"$memo", i32 %"$n3"
  %"$memo[]" = load i32, i32* %"memo[]", align 4
  %NQ = icmp ne i32 %"$memo[]", 0
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$memo4" = load i32*, i32** %memo, align 8
  %"$n5" = load i32, i32* %n, align 4
  %"memo[]6" = getelementptr inbounds i32, i32* %"$memo4", i32 %"$n5"
  %"$memo[]7" = load i32, i32* %"memo[]6", align 4
  ret i32 %"$memo[]7"

endif1:                                           ; preds = %if1
  %"$memo8" = load i32*, i32** %memo, align 8
  %"$n9" = load i32, i32* %n, align 4
  %"memo[]10" = getelementptr inbounds i32, i32* %"$memo8", i32 %"$n9"
  %"$n11" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n11", 1
  %"$memo12" = load i32*, i32** %memo, align 8
  %call.fib = call i32 @fib(i32 %SUB, i32* %"$memo12")
  %"$n13" = load i32, i32* %n, align 4
  %SUB14 = sub i32 %"$n13", 2
  %"$memo15" = load i32*, i32** %memo, align 8
  %call.fib16 = call i32 @fib(i32 %SUB14, i32* %"$memo15")
  %ADD = add i32 %call.fib, %call.fib16
  store i32 %ADD, i32* %"memo[]10", align 4
  %"$memo17" = load i32*, i32** %memo, align 8
  %"$n18" = load i32, i32* %n, align 4
  %"memo[]19" = getelementptr inbounds i32, i32* %"$memo17", i32 %"$n18"
  %"$memo[]20" = load i32, i32* %"memo[]19", align 4
  ret i32 %"$memo[]20"
}

define i32 @main() {
entry:
  %memo = alloca i32*, align 8
  %arr = alloca [21 x i32], align 4
  store [21 x i32] zeroinitializer, [21 x i32]* %arr, align 4
  %0 = getelementptr inbounds [21 x i32], [21 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %memo, align 8
  %"$memo" = load i32*, i32** %memo, align 8
  %call.fib = call i32 @fib(i32 20, i32* %"$memo")
  %MOD = srem i32 %call.fib, 256
  ret i32 %MOD
}
```

## 028 — crazy: upper-case a string in place

```llvm
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @length(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$n"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @upper(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 97
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %SUB = sub i8 %"$s[]12", 32
  store i8 %SUB, i8* %"s[]11", align 1
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i13" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i13", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [11 x i8], align 1
  store [11 x i8] c"hello, ura\00", [11 x i8]* %arr, align 1
  %0 = getelementptr inbounds [11 x i8], [11 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void @upper(i8* %"$s")
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  %"$s2" = load i8*, i8** %s, align 8
  %call.length = call i32 @length(i8* %"$s2")
  ret i32 %call.length
}
```

## 029 — crazy: not and ! on bools, in a loop

```llvm
define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store i1 true, i1* %a, align 1
  %"$a" = load i1, i1* %a, align 1
  %NOT = xor i1 %"$a", true
  store i1 %NOT, i1* %b, align 1
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a2" = load i1, i1* %a, align 1
  %NOT3 = xor i1 %"$a2", true
  store i1 %NOT3, i1* %a, align 1
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a4" = load i1, i1* %a, align 1
  br i1 %"$a4", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  br label %if2

if2:                                              ; preds = %endloop
  %"$b" = load i1, i1* %b, align 1
  %NOT7 = xor i1 %"$b", true
  %"$n8" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n8", 3
  %NOT9 = xor i1 %EQ, true
  %AND = and i1 %NOT7, %NOT9
  br i1 %AND, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 10
  store i32 %ADD11, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if2.then, %if2
  %"$n12" = load i32, i32* %n, align 4
  ret i32 %"$n12"
}
```

## 030 — crazy: count trues with as, then a cast that wraps

```llvm
define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %arr = alloca [4 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %big = alloca i32, align 4
  store [4 x i1] [i1 true, i1 false, i1 true, i1 true], [4 x i1]* %arr, align 1
  %0 = getelementptr inbounds [4 x i1], [4 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$flags" = load i1*, i1** %flags, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"flags[]" = getelementptr inbounds i1, i1* %"$flags", i32 %"$i1"
  %"$flags[]" = load i1, i1* %"flags[]", align 1
  %AS = zext i1 %"$flags[]" to i32
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", %AS
  store i32 %ADD, i32* %n, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  store i32 1000, i32* %big, align 4
  %"$n4" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n4", 10
  %"$big" = load i32, i32* %big, align 4
  %AS5 = trunc i32 %"$big" to i8
  %AS6 = sext i8 %AS5 to i32
  %ADD7 = add i32 %MUL, %AS6
  ret i32 %ADD7
}
```

## 031 — crazy: caesar cipher: char arithmetic with as

```llvm
@str = private unnamed_addr constant [6 x i8] c"%s | \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @shift(i8 %0, i32 %1) {
entry:
  %c = alloca i8, align 1
  %k = alloca i32, align 4
  %n = alloca i32, align 4
  store i8 %0, i8* %c, align 1
  store i32 %1, i32* %k, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %LT = icmp slt i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %"$c1", 122
  %OR = or i1 %LT, %GT
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$c2" = load i8, i8* %c, align 1
  ret i8 %"$c2"

endif:                                            ; preds = %if0
  %"$c3" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c3" to i32
  %SUB = sub i32 %AS, 97
  store i32 %SUB, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$k" = load i32, i32* %k, align 4
  %ADD = add i32 %"$n", %"$k"
  %MOD = srem i32 %ADD, 26
  store i32 %MOD, i32* %n, align 4
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 97
  %AS6 = trunc i32 %ADD5 to i8
  ret i8 %AS6
}

define void @encode(i8* %0, i32 %1) {
entry:
  %s = alloca i8*, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 %1, i32* %k, align 4
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s4" = load i8*, i8** %s, align 8
  %"$i5" = load i32, i32* %i, align 4
  %"s[]6" = getelementptr inbounds i8, i8* %"$s4", i32 %"$i5"
  %"$s[]7" = load i8, i8* %"s[]6", align 1
  %"$k" = load i32, i32* %k, align 4
  %call.shift = call i8 @shift(i8 %"$s[]7", i32 %"$k")
  store i8 %call.shift, i8* %"s[]3", align 1
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [13 x i8], align 1
  store [13 x i8] c"hello, world\00", [13 x i8]* %arr, align 1
  %0 = getelementptr inbounds [13 x i8], [13 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void @encode(i8* %"$s", i32 3)
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  %"$s2" = load i8*, i8** %s, align 8
  call void @encode(i8* %"$s2", i32 23)
  %"$s3" = load i8*, i8** %s, align 8
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %"$s3")
  %"$s5" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s5", i32 0
  %"$s[]" = load i8, i8* %"s[]", align 1
  %AS = sext i8 %"$s[]" to i32
  ret i32 %AS
}
```

## 032 — crazy: print the alphabet with a char counter

```llvm
@str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 97, i8* %c, align 1
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$c" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c", 101
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$c1" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8 %"$c1")
  %"$c2" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c2" to i32
  %ADD = add i32 %AS, 1
  %AS3 = trunc i32 %ADD to i8
  store i8 %AS3, i8* %c, align 1
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$c5" = load i8, i8* %c, align 1
  %AS6 = sext i8 %"$c5" to i32
  ret i32 %AS6
}
```

## 033 — crazy: isNullable on array elements and chars

```llvm
define i32 @count_zero(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %EQ = icmp eq i32 %"$a[]", 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c4" = load i32, i32* %c, align 4
  ret i32 %"$c4"
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  %s = alloca i8*, align 8
  %arr2 = alloca [3 x i8], align 1
  %r = alloca i32, align 4
  store [5 x i32] [i32 0, i32 5, i32 0, i32 0, i32 7], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store [3 x i8] c"ab\00", [3 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %arr2, i32 0, i32 0
  store i8* %1, i8** %s, align 8
  %"$a" = load i32*, i32** %a, align 8
  %call.count_zero = call i32 @count_zero(i32* %"$a", i32 5)
  %MUL = mul i32 %call.count_zero, 10
  store i32 %MUL, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 2
  %"$s[]" = load i8, i8* %"s[]", align 1
  %EQ = icmp eq i8 %"$s[]", 0
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %"$s3" = load i8*, i8** %s, align 8
  %"s[]4" = getelementptr inbounds i8, i8* %"$s3", i32 0
  %"$s[]5" = load i8, i8* %"s[]4", align 1
  %EQ6 = icmp eq i8 %"$s[]5", 0
  %NOT = xor i1 %EQ6, true
  br i1 %NOT, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$r7" = load i32, i32* %r, align 4
  %ADD8 = add i32 %"$r7", 2
  store i32 %ADD8, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if3.then, %if3
  %"$r9" = load i32, i32* %r, align 4
  ret i32 %"$r9"
}
```

## 034 — crazy: transpose a 3x3 matrix stored flat

```llvm
define i32 @get(i32* %0, i32 %1, i32 %2, i32 %3) {
entry:
  %m = alloca i32*, align 8
  %w = alloca i32, align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store i32* %0, i32** %m, align 8
  store i32 %1, i32* %w, align 4
  store i32 %2, i32* %r, align 4
  store i32 %3, i32* %c, align 4
  %"$r" = load i32, i32* %r, align 4
  %"$w" = load i32, i32* %w, align 4
  %MUL = mul i32 %"$r", %"$w"
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %"$c"
  %"$m" = load i32*, i32** %m, align 8
  %"m[]" = getelementptr inbounds i32, i32* %"$m", i32 %ADD
  %"$m[]" = load i32, i32* %"m[]", align 4
  ret i32 %"$m[]"
}

define i32 @main() {
entry:
  %m = alloca i32*, align 8
  %arr = alloca [9 x i32], align 4
  %t = alloca i32*, align 8
  %arr3 = alloca [9 x i32], align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %trace = alloca i32, align 4
  %i = alloca i32, align 4
  store [9 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9], [9 x i32]* %arr, align 4
  %0 = getelementptr inbounds [9 x i32], [9 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %m, align 8
  store [9 x i32] zeroinitializer, [9 x i32]* %arr3, align 4
  %1 = getelementptr inbounds [9 x i32], [9 x i32]* %arr3, i32 0, i32 0
  store i32* %1, i32** %t, align 8
  store i32 0, i32* %r, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$r" = load i32, i32* %r, align 4
  %LT = icmp slt i32 %"$r", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %c, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT4 = icmp slt i32 %"$c", 3
  br i1 %LT4, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c5" = load i32, i32* %c, align 4
  %MUL = mul i32 %"$c5", 3
  %"$r6" = load i32, i32* %r, align 4
  %ADD = add i32 %MUL, %"$r6"
  %"$t" = load i32*, i32** %t, align 8
  %"t[]" = getelementptr inbounds i32, i32* %"$t", i32 %ADD
  %"$m" = load i32*, i32** %m, align 8
  %"$r7" = load i32, i32* %r, align 4
  %"$c8" = load i32, i32* %c, align 4
  %call.get = call i32 @get(i32* %"$m", i32 3, i32 %"$r7", i32 %"$c8")
  store i32 %call.get, i32* %"t[]", align 4
  %"$c9" = load i32, i32* %c, align 4
  %ADD10 = add i32 %"$c9", 1
  store i32 %ADD10, i32* %c, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$r11" = load i32, i32* %r, align 4
  %ADD12 = add i32 %"$r11", 1
  store i32 %ADD12, i32* %r, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  store i32 0, i32* %trace, align 4
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop1
  %"$i" = load i32, i32* %i, align 4
  %LT13 = icmp slt i32 %"$i", 3
  br i1 %LT13, label %while2.then, label %endloop2

while2.then:                                      ; preds = %while2
  %"$i14" = load i32, i32* %i, align 4
  %MUL15 = mul i32 %"$i14", 3
  %"$i16" = load i32, i32* %i, align 4
  %ADD17 = add i32 %MUL15, %"$i16"
  %"$t18" = load i32*, i32** %t, align 8
  %"t[]19" = getelementptr inbounds i32, i32* %"$t18", i32 %ADD17
  %"$t[]" = load i32, i32* %"t[]19", align 4
  %"$trace" = load i32, i32* %trace, align 4
  %ADD20 = add i32 %"$trace", %"$t[]"
  store i32 %ADD20, i32* %trace, align 4
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while2

endloop2:                                         ; preds = %while2
  %"$trace23" = load i32, i32* %trace, align 4
  %MUL24 = mul i32 %"$trace23", 10
  %"$t25" = load i32*, i32** %t, align 8
  %"t[]26" = getelementptr inbounds i32, i32* %"$t25", i32 1
  %"$t[]27" = load i32, i32* %"t[]26", align 4
  %ADD28 = add i32 %MUL24, %"$t[]27"
  ret i32 %ADD28
}
```

## 035 — crazy: a ref to an array element

```llvm
define void @inc(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 1
  store i32 %ADD, i32* %"*p", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %p = alloca i32*, align 8
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 1
  call void @inc(i32* %"a[]")
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  call void @inc(i32* %"a[]2")
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 2
  store i32* %"a[]4", i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %MUL = mul i32 %"$*p", 2
  store i32 %MUL, i32* %"*p", align 4
  %"$a5" = load i32*, i32** %a, align 8
  %"a[]6" = getelementptr inbounds i32, i32* %"$a5", i32 0
  %"$a7" = load i32*, i32** %a, align 8
  %"a[]8" = getelementptr inbounds i32, i32* %"$a7", i32 1
  %"$a[]" = load i32, i32* %"a[]6", align 4
  %"$a[]9" = load i32, i32* %"a[]8", align 4
  %ADD = add i32 %"$a[]", %"$a[]9"
  %"$a10" = load i32*, i32** %a, align 8
  %"a[]11" = getelementptr inbounds i32, i32* %"$a10", i32 2
  %"$a[]12" = load i32, i32* %"a[]11", align 4
  %ADD13 = add i32 %ADD, %"$a[]12"
  ret i32 %ADD13
}
```

## 036 — crazy: an array literal made of calls

```llvm
define i32 @sq(i32 %0) {
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
  %k = alloca i32, align 4
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  store i32 2, i32* %k, align 4
  %call.sq = call i32 @sq(i32 1)
  %"$k" = load i32, i32* %k, align 4
  %call.sq1 = call i32 @sq(i32 %"$k")
  %"$k2" = load i32, i32* %k, align 4
  %ADD = add i32 %"$k2", 1
  %call.sq3 = call i32 @sq(i32 %ADD)
  %"$k4" = load i32, i32* %k, align 4
  %MUL = mul i32 %"$k4", 10
  %"$k5" = load i32, i32* %k, align 4
  %call.sq6 = call i32 @sq(i32 %"$k5")
  %call.sq7 = call i32 @sq(i32 %call.sq6)
  %0 = insertvalue [5 x i32] undef, i32 %call.sq, 0
  %1 = insertvalue [5 x i32] %0, i32 %call.sq1, 1
  %2 = insertvalue [5 x i32] %1, i32 %call.sq3, 2
  %3 = insertvalue [5 x i32] %2, i32 %MUL, 3
  %4 = insertvalue [5 x i32] %3, i32 %call.sq7, 4
  store [5 x i32] %4, [5 x i32]* %arr, align 4
  %5 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %5, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a8" = load i32*, i32** %a, align 8
  %"a[]9" = getelementptr inbounds i32, i32* %"$a8", i32 1
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]10" = load i32, i32* %"a[]9", align 4
  %ADD11 = add i32 %"$a[]", %"$a[]10"
  %"$a12" = load i32*, i32** %a, align 8
  %"a[]13" = getelementptr inbounds i32, i32* %"$a12", i32 2
  %"$a[]14" = load i32, i32* %"a[]13", align 4
  %ADD15 = add i32 %ADD11, %"$a[]14"
  %"$a16" = load i32*, i32** %a, align 8
  %"a[]17" = getelementptr inbounds i32, i32* %"$a16", i32 3
  %"$a[]18" = load i32, i32* %"a[]17", align 4
  %ADD19 = add i32 %ADD15, %"$a[]18"
  %"$a20" = load i32*, i32** %a, align 8
  %"a[]21" = getelementptr inbounds i32, i32* %"$a20", i32 4
  %"$a[]22" = load i32, i32* %"a[]21", align 4
  %ADD23 = add i32 %ADD19, %"$a[]22"
  ret i32 %ADD23
}
```

## 037 — crazy: elif chain on one line each, with not / and / or

```llvm
define i8 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GE = icmp sge i32 %"$n", 90
  br i1 %GE, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i8 65

elif1:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %GE2 = icmp sge i32 %"$n1", 80
  %"$n3" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n3", 85
  %NOT = xor i1 %EQ, true
  %AND = and i1 %GE2, %NOT
  br i1 %AND, label %elif1.then, label %elif2

elif1.then:                                       ; preds = %elif1
  ret i8 66

elif2:                                            ; preds = %elif1
  %"$n4" = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %"$n4", 85
  %"$n6" = load i32, i32* %n, align 4
  %GE7 = icmp sge i32 %"$n6", 70
  %OR = or i1 %EQ5, %GE7
  br i1 %OR, label %elif2.then, label %else3

elif2.then:                                       ; preds = %elif2
  ret i8 67

else3:                                            ; preds = %elif2
  ret i8 70

endif:                                            ; No predecessors!
  ret i8 0
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  br label %if4

if4:                                              ; preds = %entry
  %call.grade = call i8 @grade(i32 95)
  %EQ = icmp eq i8 %call.grade, 65
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif
  %call.grade5 = call i8 @grade(i32 82)
  %EQ6 = icmp eq i8 %call.grade5, 66
  br i1 %EQ6, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  %"$r7" = load i32, i32* %r, align 4
  %ADD8 = add i32 %"$r7", 2
  store i32 %ADD8, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif1
  %call.grade9 = call i8 @grade(i32 85)
  %EQ10 = icmp eq i8 %call.grade9, 67
  br i1 %EQ10, label %if6.then, label %endif2

if6.then:                                         ; preds = %if6
  %"$r11" = load i32, i32* %r, align 4
  %ADD12 = add i32 %"$r11", 4
  store i32 %ADD12, i32* %r, align 4
  br label %endif2

endif2:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif2
  %call.grade13 = call i8 @grade(i32 72)
  %EQ14 = icmp eq i8 %call.grade13, 67
  br i1 %EQ14, label %if7.then, label %endif3

if7.then:                                         ; preds = %if7
  %"$r15" = load i32, i32* %r, align 4
  %ADD16 = add i32 %"$r15", 8
  store i32 %ADD16, i32* %r, align 4
  br label %endif3

endif3:                                           ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif3
  %call.grade17 = call i8 @grade(i32 50)
  %EQ18 = icmp eq i8 %call.grade17, 70
  br i1 %EQ18, label %if8.then, label %endif4

if8.then:                                         ; preds = %if8
  %"$r19" = load i32, i32* %r, align 4
  %ADD20 = add i32 %"$r19", 16
  store i32 %ADD20, i32* %r, align 4
  br label %endif4

endif4:                                           ; preds = %if8.then, %if8
  %"$r21" = load i32, i32* %r, align 4
  ret i32 %"$r21"
}
```

## 038 — crazy: reverse a string, palindromes, a while on one line

```llvm
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i32 @len(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$n"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define i1 @is_palindrome(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %"$s" = load i8*, i8** %s, align 8
  %call.len = call i32 @len(i8* %"$s")
  %SUB = sub i32 %call.len, 1
  store i32 %SUB, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s3" = load i8*, i8** %s, align 8
  %"$j4" = load i32, i32* %j, align 4
  %"s[]5" = getelementptr inbounds i8, i8* %"$s3", i32 %"$j4"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %"$s[]6" = load i8, i8* %"s[]5", align 1
  %NQ = icmp ne i8 %"$s[]", %"$s[]6"
  br i1 %NQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  ret i1 false

endif:                                            ; preds = %if2
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  %"$j8" = load i32, i32* %j, align 4
  %SUB9 = sub i32 %"$j8", 1
  store i32 %SUB9, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret i1 true
}

define void @reverse(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i8, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %"$s" = load i8*, i8** %s, align 8
  %call.len = call i32 @len(i8* %"$s")
  %SUB = sub i32 %call.len, 1
  store i32 %SUB, i32* %j, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]" = load i8, i8* %"s[]", align 1
  store i8 %"$s[]", i8* %t, align 1
  %"$s3" = load i8*, i8** %s, align 8
  %"$i4" = load i32, i32* %i, align 4
  %"s[]5" = getelementptr inbounds i8, i8* %"$s3", i32 %"$i4"
  %"$s6" = load i8*, i8** %s, align 8
  %"$j7" = load i32, i32* %j, align 4
  %"s[]8" = getelementptr inbounds i8, i8* %"$s6", i32 %"$j7"
  %"$s[]9" = load i8, i8* %"s[]8", align 1
  store i8 %"$s[]9", i8* %"s[]5", align 1
  %"$s10" = load i8*, i8** %s, align 8
  %"$j11" = load i32, i32* %j, align 4
  %"s[]12" = getelementptr inbounds i8, i8* %"$s10", i32 %"$j11"
  %"$t" = load i8, i8* %t, align 1
  store i8 %"$t", i8* %"s[]12", align 1
  %"$i13" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i13", 1
  store i32 %ADD, i32* %i, align 4
  %"$j14" = load i32, i32* %j, align 4
  %SUB15 = sub i32 %"$j14", 1
  store i32 %SUB15, i32* %j, align 4
  br label %while3

endloop:                                          ; preds = %while3
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i8*, align 8
  %arr = alloca [8 x i8], align 1
  %b = alloca i8*, align 8
  %arr2 = alloca [9 x i8], align 1
  %r = alloca i32, align 4
  store [8 x i8] c"racecar\00", [8 x i8]* %arr, align 1
  %0 = getelementptr inbounds [8 x i8], [8 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %a, align 8
  store [9 x i8] c"ura lang\00", [9 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %arr2, i32 0, i32 0
  store i8* %1, i8** %b, align 8
  %"$b" = load i8*, i8** %b, align 8
  call void @reverse(i8* %"$b")
  %"$b3" = load i8*, i8** %b, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$b3")
  store i32 0, i32* %r, align 4
  br label %if4

if4:                                              ; preds = %entry
  %"$a" = load i8*, i8** %a, align 8
  %call.is_palindrome = call i1 @is_palindrome(i8* %"$a")
  br i1 %call.is_palindrome, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif
  %"$b4" = load i8*, i8** %b, align 8
  %call.is_palindrome5 = call i1 @is_palindrome(i8* %"$b4")
  %NOT = xor i1 %call.is_palindrome5, true
  br i1 %NOT, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 2
  store i32 %ADD7, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if5.then, %if5
  %"$b8" = load i8*, i8** %b, align 8
  %call.len = call i32 @len(i8* %"$b8")
  %MUL = mul i32 %call.len, 10
  %"$r9" = load i32, i32* %r, align 4
  %ADD10 = add i32 %"$r9", %MUL
  ret i32 %ADD10
}
```

## 039 — crazy: sieve of eratosthenes on a bool array

```llvm
define i32 @main() {
entry:
  %composite = alloca i1*, align 8
  %arr = alloca [20 x i1], align 1
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store [20 x i1] zeroinitializer, [20 x i1]* %arr, align 1
  %0 = getelementptr inbounds [20 x i1], [20 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %composite, align 8
  store i32 0, i32* %sum, align 4
  store i32 2, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 20
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$composite" = load i1*, i1** %composite, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"composite[]" = getelementptr inbounds i1, i1* %"$composite", i32 %"$i2"
  %"$composite[]" = load i1, i1* %"composite[]", align 1
  %NOT = xor i1 %"$composite[]", true
  br i1 %NOT, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$i3"
  store i32 %ADD, i32* %sum, align 4
  %"$i4" = load i32, i32* %i, align 4
  %"$i5" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i4", %"$i5"
  store i32 %MUL, i32* %j, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %if1.then
  %"$j" = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %"$j", 20
  br i1 %LT6, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"$composite7" = load i1*, i1** %composite, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"composite[]9" = getelementptr inbounds i1, i1* %"$composite7", i32 %"$j8"
  store i1 true, i1* %"composite[]9", align 1
  %"$i10" = load i32, i32* %i, align 4
  %"$j11" = load i32, i32* %j, align 4
  %ADD12 = add i32 %"$j11", %"$i10"
  store i32 %ADD12, i32* %j, align 4
  br label %while2

endloop:                                          ; preds = %while2
  br label %endif

endif:                                            ; preds = %endloop, %if1
  %"$i13" = load i32, i32* %i, align 4
  %ADD14 = add i32 %"$i13", 1
  store i32 %ADD14, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$sum15" = load i32, i32* %sum, align 4
  ret i32 %"$sum15"
}
```

## 040 — crazy: atoi: parse numbers from strings

```llvm
@str = private unnamed_addr constant [4 x i8] c"123\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"-45\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"7x\00", align 1

define i32 @atoi(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %neg = alloca i1, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  store i1 false, i1* %neg, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  %"$s[]" = load i8, i8* %"s[]", align 1
  %EQ = icmp eq i8 %"$s[]", 45
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i1 true, i1* %neg, align 1
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$s2" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 %"$i"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 48
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 57
  %AND = and i1 %GE, %LE
  br i1 %AND, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %AS = sext i8 %"$s[]12" to i32
  %ADD = add i32 %MUL, %AS
  %SUB = sub i32 %ADD, 48
  store i32 %SUB, i32* %n, align 4
  %"$i13" = load i32, i32* %i, align 4
  %ADD14 = add i32 %"$i13", 1
  store i32 %ADD14, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  br label %if2

if2:                                              ; preds = %endloop
  %"$neg" = load i1, i1* %neg, align 1
  br i1 %"$neg", label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$n15" = load i32, i32* %n, align 4
  %NEG = sub i32 0, %"$n15"
  ret i32 %NEG

endif1:                                           ; preds = %if2
  %"$n16" = load i32, i32* %n, align 4
  ret i32 %"$n16"
}

define i32 @main() {
entry:
  %call.atoi = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  %call.atoi1 = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  %ADD = add i32 %call.atoi, %call.atoi1
  %call.atoi2 = call i32 @atoi(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  %ADD3 = add i32 %ADD, %call.atoi2
  ret i32 %ADD3
}
```

## 041 — crazy: itoa: write a number's digits into a buffer

```llvm
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @itoa(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %digits = alloca i8*, align 8
  %arr = alloca [11 x i8], align 1
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store [11 x i8] c"0000000000\00", [11 x i8]* %arr, align 1
  %2 = getelementptr inbounds [11 x i8], [11 x i8]* %arr, i32 0, i32 0
  store i8* %2, i8** %digits, align 8
  store i32 0, i32* %k, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$out" = load i8*, i8** %out, align 8
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 0
  store i8 48, i8* %"out[]", align 1
  %"$out2" = load i8*, i8** %out, align 8
  %"out[]3" = getelementptr inbounds i8, i8* %"$out2", i32 1
  store i8 0, i8* %"out[]3", align 1
  ret void

endif:                                            ; preds = %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$n4" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n4", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$digits" = load i8*, i8** %digits, align 8
  %"$k" = load i32, i32* %k, align 4
  %"digits[]" = getelementptr inbounds i8, i8* %"$digits", i32 %"$k"
  %"$n5" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n5", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  store i8 %AS, i8* %"digits[]", align 1
  %"$n6" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n6", 10
  store i32 %DIV, i32* %n, align 4
  %"$k7" = load i32, i32* %k, align 4
  %ADD8 = add i32 %"$k7", 1
  store i32 %ADD8, i32* %k, align 4
  br label %while1

endloop:                                          ; preds = %while1
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop
  %"$i" = load i32, i32* %i, align 4
  %"$k9" = load i32, i32* %k, align 4
  %LT = icmp slt i32 %"$i", %"$k9"
  br i1 %LT, label %while2.then, label %endloop1

while2.then:                                      ; preds = %while2
  %"$out10" = load i8*, i8** %out, align 8
  %"$i11" = load i32, i32* %i, align 4
  %"out[]12" = getelementptr inbounds i8, i8* %"$out10", i32 %"$i11"
  %"$k13" = load i32, i32* %k, align 4
  %SUB = sub i32 %"$k13", 1
  %"$i14" = load i32, i32* %i, align 4
  %SUB15 = sub i32 %SUB, %"$i14"
  %"$digits16" = load i8*, i8** %digits, align 8
  %"digits[]17" = getelementptr inbounds i8, i8* %"$digits16", i32 %SUB15
  %"$digits[]" = load i8, i8* %"digits[]17", align 1
  store i8 %"$digits[]", i8* %"out[]12", align 1
  %"$i18" = load i32, i32* %i, align 4
  %ADD19 = add i32 %"$i18", 1
  store i32 %ADD19, i32* %i, align 4
  br label %while2

endloop1:                                         ; preds = %while2
  %"$out20" = load i8*, i8** %out, align 8
  %"$k21" = load i32, i32* %k, align 4
  %"out[]22" = getelementptr inbounds i8, i8* %"$out20", i32 %"$k21"
  store i8 0, i8* %"out[]22", align 1
  ret void
}

define i32 @main() {
entry:
  %buf = alloca i8*, align 8
  %arr = alloca [13 x i8], align 1
  store [13 x i8] c"............\00", [13 x i8]* %arr, align 1
  %0 = getelementptr inbounds [13 x i8], [13 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %buf, align 8
  %"$buf" = load i8*, i8** %buf, align 8
  call void @itoa(i32 9071, i8* %"$buf")
  %"$buf1" = load i8*, i8** %buf, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$buf1")
  %"$buf2" = load i8*, i8** %buf, align 8
  %"buf[]" = getelementptr inbounds i8, i8* %"$buf2", i32 0
  %"$buf[]" = load i8, i8* %"buf[]", align 1
  %AS = sext i8 %"$buf[]" to i32
  ret i32 %AS
}
```

## 042 — crazy: print a number in hex with as

```llvm
@buf = global i8* null
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @hex_digit(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 10
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 48, %"$n1"
  %AS = trunc i32 %ADD to i8
  ret i8 %AS

endif:                                            ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  %ADD3 = add i32 97, %"$n2"
  %SUB = sub i32 %ADD3, 10
  %AS4 = trunc i32 %SUB to i8
  ret i8 %AS4
}

define void @to_hex(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store i32 7, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$out" = load i8*, i8** %out, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 %"$i1"
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n", 16
  %call.hex_digit = call i8 @hex_digit(i32 %MOD)
  store i8 %call.hex_digit, i8* %"out[]", align 1
  %"$n2" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n2", 16
  store i32 %DIV, i32* %n, align 4
  %"$i3" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i3", 1
  store i32 %SUB, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$out4" = load i8*, i8** %out, align 8
  %"out[]5" = getelementptr inbounds i8, i8* %"$out4", i32 8
  store i8 0, i8* %"out[]5", align 1
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [10 x i8], align 1
  store [10 x i8] c".........\00", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @buf, align 8
  %"$buf" = load i8*, i8** @buf, align 8
  call void @to_hex(i32 48879, i8* %"$buf")
  %"$buf1" = load i8*, i8** @buf, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$buf1")
  %"$buf2" = load i8*, i8** @buf, align 8
  %"buf[]" = getelementptr inbounds i8, i8* %"$buf2", i32 4
  %"$buf[]" = load i8, i8* %"buf[]", align 1
  %AS = sext i8 %"$buf[]" to i32
  ret i32 %AS
}
```

## 043 — crazy: a string builder on a global buffer

```llvm
@out = global i8* null
@len = global i32 0
@str = private unnamed_addr constant [3 x i8] c"x=\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c", y=\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @put(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$out" = load i8*, i8** @out, align 8
  %"$len" = load i32, i32* @len, align 4
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 %"$len"
  %"$c" = load i8, i8* %c, align 1
  store i8 %"$c", i8* %"out[]", align 1
  %"$len1" = load i32, i32* @len, align 4
  %ADD = add i32 %"$len1", 1
  store i32 %ADD, i32* @len, align 4
  ret void
}

define void @put_num(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GE = icmp sge i32 %"$n", 10
  br i1 %GE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n1", 10
  call void @put_num(i32 %DIV)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$n2" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n2", 10
  %ADD = add i32 48, %MOD
  %AS = trunc i32 %ADD to i8
  call void @put(i8 %AS)
  ret void
}

define void @put_str(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  call void @put(i8 %"$s[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [33 x i8], align 1
  store [33 x i8] c"                                \00", [33 x i8]* %arr, align 1
  %0 = getelementptr inbounds [33 x i8], [33 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @out, align 8
  store i32 0, i32* @len, align 4
  call void @put_str(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0))
  call void @put_num(i32 1234)
  call void @put_str(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  call void @put_num(i32 7)
  call void @put(i8 0)
  %"$out" = load i8*, i8** @out, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$out")
  %"$len" = load i32, i32* @len, align 4
  ret i32 %"$len"
}
```

## 044 — crazy: filter with as bool, not and isNullable

```llvm
@a = global i32* null

define i32 @count_if(i32* %0, i32 %1, i1 %2) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %odd = alloca i1, align 1
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  %is_odd = alloca i1, align 1
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i1 %2, i1* %odd, align 1
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %MOD = srem i32 %"$a[]", 2
  %AS = icmp ne i32 %MOD, 0
  store i1 %AS, i1* %is_odd, align 1
  br label %if1

if1:                                              ; preds = %while0.then
  %"$is_odd" = load i1, i1* %is_odd, align 1
  %"$odd" = load i1, i1* %odd, align 1
  %EQ = icmp eq i1 %"$is_odd", %"$odd"
  %"$a2" = load i32*, i32** %a, align 8
  %"$i3" = load i32, i32* %i, align 4
  %"a[]4" = getelementptr inbounds i32, i32* %"$a2", i32 %"$i3"
  %"$a[]5" = load i32, i32* %"a[]4", align 4
  %EQ6 = icmp eq i32 %"$a[]5", 0
  %NOT = xor i1 %EQ6, true
  %AND = and i1 %EQ, %NOT
  br i1 %AND, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c9" = load i32, i32* %c, align 4
  ret i32 %"$c9"
}

define i32 @main() {
entry:
  %arr = alloca [8 x i32], align 4
  store [8 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 -3, i32 0], [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** @a, align 8
  %"$a" = load i32*, i32** @a, align 8
  %call.count_if = call i32 @count_if(i32* %"$a", i32 8, i1 true)
  %MUL = mul i32 %call.count_if, 10
  %"$a1" = load i32*, i32** @a, align 8
  %call.count_if2 = call i32 @count_if(i32* %"$a1", i32 8, i1 false)
  %ADD = add i32 %MUL, %call.count_if2
  ret i32 %ADD
}
```

## 045 — parser error: unclosed [ in an array literal

```stderr
error: test.ura:5:19 expected ']' to close the array
  |
5 |     a i32[] = [1, 2
  |                   ^
help: use a i32[] = [1, 2]
error: aborting due to 1 error
1
```

## 046 — refs: ref to a ref

```stderr
error: test.ura:4:7 a ref can't point to a ref
  |
4 |     p &&i32 = &a
  |       ^^
error: aborting due to 1 error
1
```

## 047 — refs: missing type after &

```stderr
error: test.ura:4:7 expected a type after '&'
  |
4 |     p & = &a
  |       ^
error: aborting due to 1 error
1
```

## 048 — refs: initialized with a value

```stderr
error: test.ura:4:14 'p' is a ref, initialize it with an address
  |
4 |     p &i32 = a
  |              ^
help: write 'p &i32 = &a'
error: aborting due to 1 error
1
```

## 049 — refs: address of a literal

```stderr
error: test.ura:3:15 '&' expects a variable or a struct attribute
  |
3 |     p &i32 = &5
  |               ^
error: aborting due to 1 error
1
```

## 050 — refs: address of an expression

```stderr
error: test.ura:4:18 '&' expects a variable or a struct attribute
  |
4 |     p &i32 = &(a + 1)
  |                  ^
error: aborting due to 1 error
1
```

## 051 — refs: address of a call

```stderr
error: test.ura:6:15 '&' expects a variable or a struct attribute
  |
6 |     p &i32 = &f()
  |               ^
error: aborting due to 1 error
1
```

## 052 — refs: address of an address

```stderr
error: test.ura:4:14 can't take the address of an address
  |
4 |     p &i32 = &&a
  |              ^^
error: aborting due to 1 error
1
```

## 053 — refs: ref to a different type

```stderr
error: test.ura:4:14 expected '&i32', found '&i8'
  |
4 |     p &i32 = &c
  |              ^
error: aborting due to 1 error
1
```

## 054 — null: assigned to a value

```stderr
error: test.ura:3:13 'i32' can't be null
  |
3 |     x i32 = null
  |             ^^^^
help: null fits a ref (&T) or an array (T[])
error: aborting due to 1 error
1
```

## 055 — null: compared with a value

```stderr
error: test.ura:4:14 'a' is never null, so this comparison is always the same
  |
4 |     b bool = a == null
  |              ^
error: aborting due to 1 error
1
```

## 056 — null: address of null

```stderr
error: test.ura:3:15 '&' expects a variable or a struct attribute
  |
3 |     p &i32 = &null
  |               ^^^^
error: aborting due to 1 error
1
```

## 057 — expressions: compound assign on chars

```stderr
error: test.ura:4:7 can't apply '+=' to 'i8[]' and 'i32'
  |
4 |     s += 1
  |       ^^
error: aborting due to 1 error
1
```

## 058 — expressions: negate a string

```stderr
error: test.ura:3:14 can't apply '-' to 'String'
  |
3 |     s i8[] = -"a"
  |              ^
error: aborting due to 1 error
1
```

## 059 — calls: missing & on a ref argument

```stderr
error: test.ura:7:7 expected '&i32', found 'i32'
  |
7 |     f(a)
  |       ^
help: pass its address: '&a'
error: aborting due to 1 error
1
```

## 060 — arrays: unclosed type

```stderr
error: test.ura:3:10 expected ']' after '['
  |
3 |     x i32[
  |          ^
help: use x i32[]
error: aborting due to 1 error
1
```

## 062 — arrays: index a non-array

```stderr
error: test.ura:4:12 'a' is 'i32', not an array
  |
4 |     return a[0]
  |            ^
error: aborting due to 1 error
1
```

## 063 — arrays: index with a string

```stderr
error: test.ura:4:14 expected an integer index, found 'String'
  |
4 |     return x["a"]
  |              ^^^
error: aborting due to 1 error
1
```

## 064 — arrays: mixed element types

```stderr
error: test.ura:3:19 expected 'i32', found 'String'
  |
3 |     x i32[] = [1, "a"]
  |                   ^^^
error: aborting due to 1 error
1
```

## 065 — arrays: array into a scalar

```stderr
error: test.ura:3:13 expected 'i32', found an array literal
  |
3 |     a i32 = [1, 2]
  |             ^
error: aborting due to 1 error
1
```

## 066 — arrays: unclosed literal

```stderr
error: test.ura:3:19 expected ']' to close the array
  |
3 |     x i32[] = [1, 2
  |                   ^
help: use x i32[] = [1, 2]
error: aborting due to 1 error
1
```

## 067 — arrays: missing comma between elements

```stderr
error: test.ura:3:18 expected ',' between elements
  |
3 |     x i32[] = [1 2]
  |                  ^
error: aborting due to 1 error
1
```

## 068 — array: string as i8[] is writable

```llvm
define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  %c = alloca i8, align 1
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  store i8 120, i8* %"s[]", align 1
  %"$s1" = load i8*, i8** %s, align 8
  %"s[]2" = getelementptr inbounds i8, i8* %"$s1", i32 1
  %"$s[]" = load i8, i8* %"s[]2", align 1
  store i8 %"$s[]", i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 98
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 069 — array: array passed to a function

```llvm
define i32 @sum(i32* %0) {
entry:
  %a = alloca i32*, align 8
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$a[]"
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
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  %call.sum = call i32 @sum(i32* %"$values")
  ret i32 %call.sum
}
```

## 070 — ref: write through a ref

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  store i32 12, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 071 — ref: ref as parameter

```llvm
define void @foo(i32* %0) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %"*a" = load i32*, i32** %a, align 8
  store i32 2, i32* %"*a", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  call void @foo(i32* %a)
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 072 — ref: read through a ref

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  store i32 %"$*p", i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

## 073 — ref: math through a ref

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"*p1" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p1", align 4
  %ADD = add i32 %"$*p", 5
  store i32 %ADD, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 074 — ref: copy the pointed value between refs

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  %"$*p2" = load i32, i32* %"*p2", align 4
  store i32 %"$*p2", i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

## 075 — ref: rebind a ref

```llvm
define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  store i32* %"*p2", i32** %p1, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  store i32 9, i32* %"*p1", align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

## 076 — function: fn returns ref

```llvm
define i32* @same(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  ret i32* %"*p"
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %q = alloca i32*, align 8
  %ref = alloca i32*, align 8
  store i32 6, i32* %a, align 4
  %call.same = call i32* @same(i32* %a)
  store i32* %call.same, i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32* %"*ref", i32** %q, align 8
  %"*q" = load i32*, i32** %q, align 8
  %"$*q" = load i32, i32* %"*q", align 4
  ret i32 %"$*q"
}
```

## 077 — function: swap

```llvm
define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"$*x" = load i32, i32* %"*x", align 4
  store i32 %"$*x", i32* %t, align 4
  %"*x1" = load i32*, i32** %x, align 8
  %"*y" = load i32*, i32** %y, align 8
  %"$*y" = load i32, i32* %"*y", align 4
  store i32 %"$*y", i32* %"*x1", align 4
  %"*y2" = load i32*, i32** %y, align 8
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*y2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 10
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b"
  ret i32 %ADD
}
```

## 078 — function: overloaded function

```llvm
@str = private unnamed_addr constant [8 x i8] c"int %d\0A\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"str %s\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"hi\00", align 1

declare void @printf(i8*, ...)

define void @show(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$n")
  ret void
}

define void @show.1(i8* %0) {
entry:
  %s = alloca i8*, align 8
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %"$s")
  ret void
}

define i32 @main() {
entry:
  call void @show(i32 42)
  call void @show.1(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}
```

## 079 — program: null compared to null

```llvm
define i32 @main() {
entry:
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 7

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 081 — array: 1D i32 array: literal, index read, indexed write

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [25 x i8] c"a[0]=%d a[1]=%d a[2]=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c"after a[1]=99: %d\0A\00", align 1

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
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size16 = alloca i32, align 4
  %format.text22 = alloca i8*, align 8
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 2
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]5" = load i32, i32* %"a[]2", align 4
  %"$a[]6" = load i32, i32* %"a[]4", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]5", i32 %"$a[]6")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]5", i32 %"$a[]6")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %2 = insertvalue %String %1, i32 %"$format.size11", 1
  %3 = insertvalue %String %2, i32 %ADD9, 2
  call void @"io::out"(%String %3)
  %"$a12" = load i32*, i32** %a, align 8
  %"a[]13" = getelementptr inbounds i32, i32* %"$a12", i32 1
  store i32 99, i32* %"a[]13", align 4
  %"$a14" = load i32*, i32** %a, align 8
  %"a[]15" = getelementptr inbounds i32, i32* %"$a14", i32 1
  %"$a[]17" = load i32, i32* %"a[]15", align 4
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i32 %"$a[]17")
  store i32 %call.snprintf18, i32* %format.size16, align 4
  %"$format.size19" = load i32, i32* %format.size16, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i32 %"$a[]17")
  %"$format.size26" = load i32, i32* %format.size16, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %"$format.text28" = load i8*, i8** %format.text22, align 8
  %"$format.size29" = load i32, i32* %format.size16, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text28", 0
  %5 = insertvalue %String %4, i32 %"$format.size29", 1
  %6 = insertvalue %String %5, i32 %ADD27, 2
  call void @"io::out"(%String %6)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 082 — array: variable index and element arithmetic

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [19 x i8] c"a[i]=%d a[i-1]=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"a[0]+a[3]=%d\0A\00", align 1

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
  %a = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %i = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size17 = alloca i32, align 4
  %format.text22 = alloca i8*, align 8
  store [4 x i32] [i32 5, i32 10, i32 15, i32 20], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 3, i32* %i, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i"
  %"$i1" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i1", 1
  %"$a2" = load i32*, i32** %a, align 8
  %"a[]3" = getelementptr inbounds i32, i32* %"$a2", i32 %SUB
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]4" = load i32, i32* %"a[]3", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]4")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]4")
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %2 = insertvalue %String %1, i32 %"$format.size9", 1
  %3 = insertvalue %String %2, i32 %ADD7, 2
  call void @"io::out"(%String %3)
  %"$a10" = load i32*, i32** %a, align 8
  %"a[]11" = getelementptr inbounds i32, i32* %"$a10", i32 0
  %"$a12" = load i32*, i32** %a, align 8
  %"a[]13" = getelementptr inbounds i32, i32* %"$a12", i32 3
  %"$a[]14" = load i32, i32* %"a[]11", align 4
  %"$a[]15" = load i32, i32* %"a[]13", align 4
  %ADD16 = add i32 %"$a[]14", %"$a[]15"
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %ADD16)
  store i32 %call.snprintf18, i32* %format.size17, align 4
  %"$format.size19" = load i32, i32* %format.size17, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %ADD16)
  %"$format.size26" = load i32, i32* %format.size17, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %"$format.text28" = load i8*, i8** %format.text22, align 8
  %"$format.size29" = load i32, i32* %format.size17, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text28", 0
  %5 = insertvalue %String %4, i32 %"$format.size29", 1
  %6 = insertvalue %String %5, i32 %ADD27, 2
  call void @"io::out"(%String %6)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 083 — array: arrays of bool, char, f32

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [25 x i8] c"b[0]=%s c[1]=%c f[2]=%f\0A\00", align 1

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
  %b = alloca i1*, align 8
  %arr = alloca [3 x i1], align 1
  %c = alloca i8*, align 8
  %arr1 = alloca [3 x i8], align 1
  %f = alloca float*, align 8
  %arr2 = alloca [3 x float], align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store [3 x i1] [i1 true, i1 false, i1 true], [3 x i1]* %arr, align 1
  %0 = getelementptr inbounds [3 x i1], [3 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %b, align 8
  store [3 x i8] c"abc", [3 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %arr1, i32 0, i32 0
  store i8* %1, i8** %c, align 8
  store [3 x float] [float 1.500000e+00, float 2.500000e+00, float 3.000000e+00], [3 x float]* %arr2, align 4
  %2 = getelementptr inbounds [3 x float], [3 x float]* %arr2, i32 0, i32 0
  store float* %2, float** %f, align 8
  %"$b" = load i1*, i1** %b, align 8
  %"b[]" = getelementptr inbounds i1, i1* %"$b", i32 0
  %"$c" = load i8*, i8** %c, align 8
  %"c[]" = getelementptr inbounds i8, i8* %"$c", i32 1
  %"$f" = load float*, float** %f, align 8
  %"f[]" = getelementptr inbounds float, float* %"$f", i32 2
  store i32 0, i32* %format.size, align 4
  %"$b[]" = load i1, i1* %"b[]", align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$b[]", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$c[]" = load i8, i8* %"c[]", align 1
  %AS = sext i8 %"$c[]" to i32
  %"$f[]" = load float, float* %"f[]", align 4
  %AS3 = fpext float %"$f[]" to double
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text", i32 %AS, double %AS3)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS4 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS4, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text5" = load i8*, i8** %bool.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS4, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text5", i32 %AS, double %AS3)
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %4 = insertvalue %String %3, i32 %"$format.size10", 1
  %5 = insertvalue %String %4, i32 %ADD8, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 086 — array: indexing a non-array

```stderr
error: test.ura:5:16 'a' is 'i32', not an array
  |
5 |     io::out($"{a[0]}")
  |                ^
error: aborting due to 1 error
1
```

## 087 — array: non-integer index

```stderr
error: test.ura:5:18 expected an integer index, found 'f64'
  |
5 |     io::out($"{a[1.5]}")
  |                  ^^^
error: aborting due to 1 error
1
```

## 088 — array: array literal with mixed element types

```stderr
error: test.ura:4:19 expected 'i32', found 'bool'
  |
4 |     a i32[] = [1, True, 3]
  |                   ^^^^
error: aborting due to 1 error
1
```

## 089 — array: array size is not an integer

```stderr
error: test.ura:4:19 expected an integer index, found 'f64'
  |
4 |     a i32[] = i32[1.5]
  |                   ^^^
error: test.ura:4:15 'i32' is 'i32', not an array
  |
4 |     a i32[] = i32[1.5]
  |               ^^^
error: aborting due to 2 errors
1
```

## 090 — array: missing closing bracket on index

```stderr
error: test.ura:5:15 expected ']' after the index
  |
5 |     output(a[0)
  |               ^
help: use output(a[0])
error: aborting due to 1 error
1
```

## 092 — array: unknown member

```stderr
error: test.ura:5:16 'a' is 'i32[]', not a struct
  |
5 |     io::out($"{a.size}")
  |                ^
error: aborting due to 1 error
1
```

## 093 — function: single ref param: buff hero hp

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

define void @buff(i32* %0) {
entry:
  %hp = alloca i32*, align 8
  store i32* %0, i32** %hp, align 8
  %"*hp" = load i32*, i32** %hp, align 8
  %"*hp1" = load i32*, i32** %hp, align 8
  %"$*hp" = load i32, i32* %"*hp1", align 4
  %ADD = add i32 %"$*hp", 20
  store i32 %ADD, i32* %"*hp", align 4
  ret void
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 80, i32* %hero_hp, align 4
  call void @buff(i32* %hero_hp)
  %"$hero_hp" = load i32, i32* %hero_hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hero_hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hero_hp")
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

## 094 — function: swap two weapon damage values via refs

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

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

define void @swap_weapons(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %temp = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32* %1, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  %"$*a" = load i32, i32* %"*a", align 4
  store i32 %"$*a", i32* %temp, align 4
  %"*a1" = load i32*, i32** %a, align 8
  %"*b" = load i32*, i32** %b, align 8
  %"$*b" = load i32, i32* %"*b", align 4
  store i32 %"$*b", i32* %"*a1", align 4
  %"*b2" = load i32*, i32** %b, align 8
  %"$temp" = load i32, i32* %temp, align 4
  store i32 %"$temp", i32* %"*b2", align 4
  ret void
}

define i32 @main() {
entry:
  %sword = alloca i32, align 4
  %dagger = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 30, i32* %sword, align 4
  store i32 15, i32* %dagger, align 4
  call void @swap_weapons(i32* %sword, i32* %dagger)
  %"$sword" = load i32, i32* %sword, align 4
  %"$dagger" = load i32, i32* %dagger, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$sword", i32 %"$dagger")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$sword", i32 %"$dagger")
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

## 095 — function: refs used in expression, return result

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

define i32 @add_xp(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %result = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32* %1, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  %"*b" = load i32*, i32** %b, align 8
  %"$*a" = load i32, i32* %"*a", align 4
  %"$*b" = load i32, i32* %"*b", align 4
  %ADD = add i32 %"$*a", %"$*b"
  store i32 %ADD, i32* %result, align 4
  %"$result" = load i32, i32* %result, align 4
  ret i32 %"$result"
}

define i32 @main() {
entry:
  %kill_xp = alloca i32, align 4
  %bonus_xp = alloca i32, align 4
  %z = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 30, i32* %kill_xp, align 4
  store i32 20, i32* %bonus_xp, align 4
  %call.add_xp = call i32 @add_xp(i32* %kill_xp, i32* %bonus_xp)
  store i32 %call.add_xp, i32* %z, align 4
  %"$z" = load i32, i32* %z, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$z")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$z")
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

## 096 — function: compound ref assign through in a loop

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

define void @tick(i32* %0) {
entry:
  %kills = alloca i32*, align 8
  store i32* %0, i32** %kills, align 8
  %"*kills" = load i32*, i32** %kills, align 8
  %"$*kills" = load i32, i32* %"*kills", align 4
  %ADD = add i32 %"$*kills", 1
  store i32 %ADD, i32* %"*kills", align 4
  ret void
}

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  %i = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 0, i32* %kills, align 4
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  call void @tick(i32* %kills)
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  %"$kills" = load i32, i32* %kills, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD2 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD2 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 097 — function: ref-to-value: ref passing a where value is expected

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1

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

define void @show_hp(i32 %0) {
entry:
  %hp = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$hp")
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
  %hero_hp = alloca i32, align 4
  %r = alloca i32*, align 8
  store i32 75, i32* %hero_hp, align 4
  store i32* %hero_hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  call void @show_hp(i32 %"$*r")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 098 — function: ref parameter mutates the caller (counter)

```llvm
define i32 @tick(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %"*n" = load i32*, i32** %n, align 8
  %"*n1" = load i32*, i32** %n, align 8
  %"$*n" = load i32, i32* %"*n1", align 4
  %ADD = add i32 %"$*n", 1
  store i32 %ADD, i32* %"*n", align 4
  %"*n2" = load i32*, i32** %n, align 8
  %"$*n3" = load i32, i32* %"*n2", align 4
  ret i32 %"$*n3"
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %call.tick = call i32 @tick(i32* %n)
  %call.tick1 = call i32 @tick(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

## 099 — function: a void function with a ref parameter

```llvm
define void @bump(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %"*n" = load i32*, i32** %n, align 8
  %"*n1" = load i32*, i32** %n, align 8
  %"$*n" = load i32, i32* %"*n1", align 4
  %ADD = add i32 %"$*n", 1
  store i32 %ADD, i32* %"*n", align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  call void @bump(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

## 100 — ref: basic ref: bind to hp, ref write through

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
  %hp = alloca i32, align 4
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 0, i32* %"*r", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
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

## 101 — ref: aliasing: two refs to same hp, both see mutations

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
  %hp = alloca i32, align 4
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store i32* %hp, i32** %a, align 8
  store i32* %hp, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  store i32 80, i32* %"*a", align 4
  %"*b" = load i32*, i32** %b, align 8
  %"*b1" = load i32*, i32** %b, align 8
  %"$*b" = load i32, i32* %"*b1", align 4
  %SUB = sub i32 %"$*b", 10
  store i32 %SUB, i32* %"*b", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 102 — ref: compound ref assignment through in a loop

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
  %xp = alloca i32, align 4
  %xp_gain = alloca i32, align 4
  %r = alloca i32*, align 8
  %i = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 0, i32* %xp, align 4
  store i32 50, i32* %xp_gain, align 4
  store i32* %xp, i32** %r, align 8
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*r" = load i32*, i32** %r, align 8
  %"$xp_gain" = load i32, i32* %xp_gain, align 4
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", %"$xp_gain"
  store i32 %ADD, i32* %"*r", align 4
  %"$i1" = load i32, i32* %i, align 4
  %ADD2 = add i32 %"$i1", 1
  store i32 %ADD2, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  %"$xp" = load i32, i32* %xp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$xp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD3 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$xp")
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

## 103 — ref: separate refs to different vars: hp ref ref and mp

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [20 x i8] c"HP after hit:   %d\0A\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"MP after spell: %d\0A\00", align 1

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
  %shield = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %mana = alloca i32*, align 8
  %format.size9 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i32* %hp, i32** %shield, align 8
  %"*shield" = load i32*, i32** %shield, align 8
  %"*shield1" = load i32*, i32** %shield, align 8
  %"$*shield" = load i32, i32* %"*shield1", align 4
  %SUB = sub i32 %"$*shield", 30
  store i32 %SUB, i32* %"*shield", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  store i32* %mp, i32** %mana, align 8
  %"*mana" = load i32*, i32** %mana, align 8
  %"*mana7" = load i32*, i32** %mana, align 8
  %"$*mana" = load i32, i32* %"*mana7", align 4
  %SUB8 = sub i32 %"$*mana", 20
  store i32 %SUB8, i32* %"*mana", align 4
  %"$mp" = load i32, i32* %mp, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$mp")
  store i32 %call.snprintf10, i32* %format.size9, align 4
  %"$format.size11" = load i32, i32* %format.size9, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$mp")
  %"$format.size18" = load i32, i32* %format.size9, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %4 = insertvalue %String %3, i32 %"$format.size21", 1
  %5 = insertvalue %String %4, i32 %ADD19, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 104 — ref: multiply and subtract through refs: f64 shield, pay mana

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

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
  %shield = alloca i32, align 4
  %mp = alloca i32, align 4
  %s = alloca i32*, align 8
  %m = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 10, i32* %shield, align 4
  store i32 80, i32* %mp, align 4
  store i32* %shield, i32** %s, align 8
  store i32* %mp, i32** %m, align 8
  %"*s" = load i32*, i32** %s, align 8
  %"$*s" = load i32, i32* %"*s", align 4
  %MUL = mul i32 %"$*s", 2
  store i32 %MUL, i32* %"*s", align 4
  %"*m" = load i32*, i32** %m, align 8
  %"$*m" = load i32, i32* %"*m", align 4
  %SUB = sub i32 %"$*m", 25
  store i32 %SUB, i32* %"*m", align 4
  %"$shield" = load i32, i32* %shield, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$shield", i32 %"$mp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$shield", i32 %"$mp")
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

## 105 — ref: chain of compound mutations ref through a

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
  %score = alloca i32, align 4
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 0, i32* %score, align 4
  store i32* %score, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", 100
  store i32 %ADD, i32* %"*r", align 4
  %"*r1" = load i32*, i32** %r, align 8
  %"$*r2" = load i32, i32* %"*r1", align 4
  %ADD3 = add i32 %"$*r2", 50
  store i32 %ADD3, i32* %"*r1", align 4
  %"*r4" = load i32*, i32** %r, align 8
  %"$*r5" = load i32, i32* %"*r4", align 4
  %MUL = mul i32 %"$*r5", 2
  store i32 %MUL, i32* %"*r4", align 4
  %"*r6" = load i32*, i32** %r, align 8
  %"$*r7" = load i32, i32* %"*r6", align 4
  %SUB = sub i32 %"$*r7", 30
  store i32 %SUB, i32* %"*r6", align 4
  %"$score" = load i32, i32* %score, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$score")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD8 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$score")
  %"$format.size10" = load i32, i32* %format.size, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %"$format.text12" = load i8*, i8** %format.text, align 8
  %"$format.size13" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text12", 0
  %1 = insertvalue %String %0, i32 %"$format.size13", 1
  %2 = insertvalue %String %1, i32 %ADD11, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 106 — ref: reference in conditional

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"healthy\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"critical\0A\00", align 1

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
  %r = alloca i32*, align 8
  store i32 75, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %GT = icmp sgt i32 %"$*r", 50
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 8, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 107 — ref: a ref reads correctly through every operator + as a by-value arg

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %x = alloca i32, align 4
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size10 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %format.size25 = alloca i32, align 4
  %format.text30 = alloca i8*, align 8
  %format.size40 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text45 = alloca i8*, align 8
  %format.size56 = alloca i32, align 4
  %bool.text57 = alloca i8*, align 8
  %format.text63 = alloca i8*, align 8
  %format.size74 = alloca i32, align 4
  %format.text79 = alloca i8*, align 8
  %format.size89 = alloca i32, align 4
  %format.text94 = alloca i8*, align 8
  %format.size104 = alloca i32, align 4
  %format.text109 = alloca i8*, align 8
  %format.size119 = alloca i32, align 4
  %format.text124 = alloca i8*, align 8
  %format.size134 = alloca i32, align 4
  %format.text139 = alloca i8*, align 8
  store i32 12, i32* %x, align 4
  store i32* %x, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", 3
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %ADD)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD2 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD2 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %ADD)
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %"*r8" = load i32*, i32** %r, align 8
  %"$*r9" = load i32, i32* %"*r8", align 4
  %SUB = sub i32 %"$*r9", 5
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  store i32 %call.snprintf11, i32* %format.size10, align 4
  %"$format.size12" = load i32, i32* %format.size10, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  %"$format.size19" = load i32, i32* %format.size10, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size10, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  %"*r23" = load i32*, i32** %r, align 8
  %"$*r24" = load i32, i32* %"*r23", align 4
  %MUL = mul i32 %"$*r24", 2
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  store i32 %call.snprintf26, i32* %format.size25, align 4
  %"$format.size27" = load i32, i32* %format.size25, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %AS29 = sext i32 %ADD28 to i64
  %call.calloc31 = call i8* @calloc(i64 %AS29, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc31, i8** %format.text30, align 8
  %"$format.text32" = load i8*, i8** %format.text30, align 8
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text32", i64 %AS29, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  %"$format.size34" = load i32, i32* %format.size25, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %"$format.text36" = load i8*, i8** %format.text30, align 8
  %"$format.size37" = load i32, i32* %format.size25, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text36", 0
  %7 = insertvalue %String %6, i32 %"$format.size37", 1
  %8 = insertvalue %String %7, i32 %ADD35, 2
  call void @"io::out"(%String %8)
  %"*r38" = load i32*, i32** %r, align 8
  %"$*r39" = load i32, i32* %"*r38", align 4
  %GT = icmp sgt i32 %"$*r39", 10
  store i32 0, i32* %format.size40, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf41, i32* %format.size40, align 4
  %"$format.size42" = load i32, i32* %format.size40, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %"$bool.text48" = load i8*, i8** %bool.text, align 8
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text48")
  %"$format.size50" = load i32, i32* %format.size40, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %"$format.text52" = load i8*, i8** %format.text45, align 8
  %"$format.size53" = load i32, i32* %format.size40, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text52", 0
  %10 = insertvalue %String %9, i32 %"$format.size53", 1
  %11 = insertvalue %String %10, i32 %ADD51, 2
  call void @"io::out"(%String %11)
  %"*r54" = load i32*, i32** %r, align 8
  %"$*r55" = load i32, i32* %"*r54", align 4
  %EQ = icmp eq i32 %"$*r55", 12
  store i32 0, i32* %format.size56, align 4
  store i8* null, i8** %bool.text57, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %EQ, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text57, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text57, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text58" = load i8*, i8** %bool.text57, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text58")
  store i32 %call.snprintf59, i32* %format.size56, align 4
  %"$format.size60" = load i32, i32* %format.size56, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %AS62 = sext i32 %ADD61 to i64
  %call.calloc64 = call i8* @calloc(i64 %AS62, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc64, i8** %format.text63, align 8
  %"$format.text65" = load i8*, i8** %format.text63, align 8
  %"$bool.text66" = load i8*, i8** %bool.text57, align 8
  %call.snprintf67 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text65", i64 %AS62, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text66")
  %"$format.size68" = load i32, i32* %format.size56, align 4
  %ADD69 = add i32 %"$format.size68", 1
  %"$format.text70" = load i8*, i8** %format.text63, align 8
  %"$format.size71" = load i32, i32* %format.size56, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text70", 0
  %13 = insertvalue %String %12, i32 %"$format.size71", 1
  %14 = insertvalue %String %13, i32 %ADD69, 2
  call void @"io::out"(%String %14)
  %"*r72" = load i32*, i32** %r, align 8
  %"$*r73" = load i32, i32* %"*r72", align 4
  %BAND = and i32 %"$*r73", 10
  %call.snprintf75 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %BAND)
  store i32 %call.snprintf75, i32* %format.size74, align 4
  %"$format.size76" = load i32, i32* %format.size74, align 4
  %ADD77 = add i32 %"$format.size76", 1
  %AS78 = sext i32 %ADD77 to i64
  %call.calloc80 = call i8* @calloc(i64 %AS78, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc80, i8** %format.text79, align 8
  %"$format.text81" = load i8*, i8** %format.text79, align 8
  %call.snprintf82 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text81", i64 %AS78, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %BAND)
  %"$format.size83" = load i32, i32* %format.size74, align 4
  %ADD84 = add i32 %"$format.size83", 1
  %"$format.text85" = load i8*, i8** %format.text79, align 8
  %"$format.size86" = load i32, i32* %format.size74, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text85", 0
  %16 = insertvalue %String %15, i32 %"$format.size86", 1
  %17 = insertvalue %String %16, i32 %ADD84, 2
  call void @"io::out"(%String %17)
  %"*r87" = load i32*, i32** %r, align 8
  %"$*r88" = load i32, i32* %"*r87", align 4
  %LSHIFT = shl i32 %"$*r88", 1
  %call.snprintf90 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %LSHIFT)
  store i32 %call.snprintf90, i32* %format.size89, align 4
  %"$format.size91" = load i32, i32* %format.size89, align 4
  %ADD92 = add i32 %"$format.size91", 1
  %AS93 = sext i32 %ADD92 to i64
  %call.calloc95 = call i8* @calloc(i64 %AS93, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc95, i8** %format.text94, align 8
  %"$format.text96" = load i8*, i8** %format.text94, align 8
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text96", i64 %AS93, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %LSHIFT)
  %"$format.size98" = load i32, i32* %format.size89, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %"$format.text100" = load i8*, i8** %format.text94, align 8
  %"$format.size101" = load i32, i32* %format.size89, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text100", 0
  %19 = insertvalue %String %18, i32 %"$format.size101", 1
  %20 = insertvalue %String %19, i32 %ADD99, 2
  call void @"io::out"(%String %20)
  %"*r102" = load i32*, i32** %r, align 8
  %"$*r103" = load i32, i32* %"*r102", align 4
  %NEG = sub i32 0, %"$*r103"
  %call.snprintf105 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %NEG)
  store i32 %call.snprintf105, i32* %format.size104, align 4
  %"$format.size106" = load i32, i32* %format.size104, align 4
  %ADD107 = add i32 %"$format.size106", 1
  %AS108 = sext i32 %ADD107 to i64
  %call.calloc110 = call i8* @calloc(i64 %AS108, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc110, i8** %format.text109, align 8
  %"$format.text111" = load i8*, i8** %format.text109, align 8
  %call.snprintf112 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text111", i64 %AS108, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %NEG)
  %"$format.size113" = load i32, i32* %format.size104, align 4
  %ADD114 = add i32 %"$format.size113", 1
  %"$format.text115" = load i8*, i8** %format.text109, align 8
  %"$format.size116" = load i32, i32* %format.size104, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text115", 0
  %22 = insertvalue %String %21, i32 %"$format.size116", 1
  %23 = insertvalue %String %22, i32 %ADD114, 2
  call void @"io::out"(%String %23)
  %"*r117" = load i32*, i32** %r, align 8
  %"$*r118" = load i32, i32* %"*r117", align 4
  %BXOR = xor i32 %"$*r118", -1
  %call.snprintf120 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %BXOR)
  store i32 %call.snprintf120, i32* %format.size119, align 4
  %"$format.size121" = load i32, i32* %format.size119, align 4
  %ADD122 = add i32 %"$format.size121", 1
  %AS123 = sext i32 %ADD122 to i64
  %call.calloc125 = call i8* @calloc(i64 %AS123, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc125, i8** %format.text124, align 8
  %"$format.text126" = load i8*, i8** %format.text124, align 8
  %call.snprintf127 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text126", i64 %AS123, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %BXOR)
  %"$format.size128" = load i32, i32* %format.size119, align 4
  %ADD129 = add i32 %"$format.size128", 1
  %"$format.text130" = load i8*, i8** %format.text124, align 8
  %"$format.size131" = load i32, i32* %format.size119, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text130", 0
  %25 = insertvalue %String %24, i32 %"$format.size131", 1
  %26 = insertvalue %String %25, i32 %ADD129, 2
  call void @"io::out"(%String %26)
  %"*r132" = load i32*, i32** %r, align 8
  %"$*r133" = load i32, i32* %"*r132", align 4
  %call.twice = call i32 @twice(i32 %"$*r133")
  %call.snprintf135 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i32 %call.twice)
  store i32 %call.snprintf135, i32* %format.size134, align 4
  %"$format.size136" = load i32, i32* %format.size134, align 4
  %ADD137 = add i32 %"$format.size136", 1
  %AS138 = sext i32 %ADD137 to i64
  %call.calloc140 = call i8* @calloc(i64 %AS138, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc140, i8** %format.text139, align 8
  %"$format.text141" = load i8*, i8** %format.text139, align 8
  %call.snprintf142 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text141", i64 %AS138, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i32 %call.twice)
  %"$format.size143" = load i32, i32* %format.size134, align 4
  %ADD144 = add i32 %"$format.size143", 1
  %"$format.text145" = load i8*, i8** %format.text139, align 8
  %"$format.size146" = load i32, i32* %format.size134, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text145", 0
  %28 = insertvalue %String %27, i32 %"$format.size146", 1
  %29 = insertvalue %String %28, i32 %ADD144, 2
  call void @"io::out"(%String %29)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 108 — ref: ref? nullable: bound at declaration, guarded reads pass, write-through works

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %m = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size7 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  store i32 42, i32* %x, align 4
  store i32* %x, i32** %m, align 8
  %"*m" = load i32*, i32** %m, align 8
  %"$*m" = load i32, i32* %"*m", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*m")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*m")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %"*m6" = load i32*, i32** %m, align 8
  store i32 7, i32* %"*m6", align 4
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x")
  %"$format.size16" = load i32, i32* %format.size7, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %4 = insertvalue %String %3, i32 %"$format.size19", 1
  %5 = insertvalue %String %4, i32 %ADD17, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 109 — ref: nested fn shares an enclosing var through a ref parameter (the sanctioned path)

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  store i32 5, i32* %x, align 4
  call void @bump(i32* %x)
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
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

define void @bump(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %"*n" = load i32*, i32** %n, align 8
  %"*n1" = load i32*, i32** %n, align 8
  %"$*n" = load i32, i32* %"*n1", align 4
  %ADD = add i32 %"$*n", 1
  store i32 %ADD, i32* %"*n", align 4
  ret void
}

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 110 — ref: a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.12 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.21 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.22 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.23 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.24 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.26 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size14 = alloca i32, align 4
  %format.text19 = alloca i8*, align 8
  %format.size29 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text34 = alloca i8*, align 8
  %format.size45 = alloca i32, align 4
  %bool.text46 = alloca i8*, align 8
  %format.text52 = alloca i8*, align 8
  %format.size63 = alloca i32, align 4
  %bool.text64 = alloca i8*, align 8
  %format.text70 = alloca i8*, align 8
  %format.size81 = alloca i32, align 4
  %bool.text82 = alloca i8*, align 8
  %format.text88 = alloca i8*, align 8
  %format.size99 = alloca i32, align 4
  %format.text104 = alloca i8*, align 8
  %format.size114 = alloca i32, align 4
  %format.text119 = alloca i8*, align 8
  %format.size129 = alloca i32, align 4
  %format.text134 = alloca i8*, align 8
  %format.size145 = alloca i32, align 4
  %format.text150 = alloca i8*, align 8
  %flag = alloca i1, align 1
  %b = alloca i1*, align 8
  %format.size158 = alloca i32, align 4
  %bool.text159 = alloca i8*, align 8
  %format.text165 = alloca i8*, align 8
  %format.size176 = alloca i32, align 4
  %bool.text177 = alloca i8*, align 8
  %format.text183 = alloca i8*, align 8
  %format.size194 = alloca i32, align 4
  %bool.text195 = alloca i8*, align 8
  %format.text201 = alloca i8*, align 8
  store i32 20, i32* %a, align 4
  store i32* %a, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %DIV = sdiv i32 %"$*r", 6
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %DIV)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %DIV)
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  %"*r12" = load i32*, i32** %r, align 8
  %"$*r13" = load i32, i32* %"*r12", align 4
  %MOD = srem i32 %"$*r13", 6
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %MOD)
  store i32 %call.snprintf15, i32* %format.size14, align 4
  %"$format.size16" = load i32, i32* %format.size14, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %AS18 = sext i32 %ADD17 to i64
  %call.calloc20 = call i8* @calloc(i64 %AS18, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc20, i8** %format.text19, align 8
  %"$format.text21" = load i8*, i8** %format.text19, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text21", i64 %AS18, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %MOD)
  %"$format.size23" = load i32, i32* %format.size14, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text19, align 8
  %"$format.size26" = load i32, i32* %format.size14, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %4 = insertvalue %String %3, i32 %"$format.size26", 1
  %5 = insertvalue %String %4, i32 %ADD24, 2
  call void @"io::out"(%String %5)
  %"*r27" = load i32*, i32** %r, align 8
  %"$*r28" = load i32, i32* %"*r27", align 4
  %NQ = icmp ne i32 %"$*r28", 20
  store i32 0, i32* %format.size29, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %NQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf30, i32* %format.size29, align 4
  %"$format.size31" = load i32, i32* %format.size29, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %AS33 = sext i32 %ADD32 to i64
  %call.calloc35 = call i8* @calloc(i64 %AS33, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc35, i8** %format.text34, align 8
  %"$format.text36" = load i8*, i8** %format.text34, align 8
  %"$bool.text37" = load i8*, i8** %bool.text, align 8
  %call.snprintf38 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text36", i64 %AS33, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text37")
  %"$format.size39" = load i32, i32* %format.size29, align 4
  %ADD40 = add i32 %"$format.size39", 1
  %"$format.text41" = load i8*, i8** %format.text34, align 8
  %"$format.size42" = load i32, i32* %format.size29, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text41", 0
  %7 = insertvalue %String %6, i32 %"$format.size42", 1
  %8 = insertvalue %String %7, i32 %ADD40, 2
  call void @"io::out"(%String %8)
  %"*r43" = load i32*, i32** %r, align 8
  %"$*r44" = load i32, i32* %"*r43", align 4
  %LT = icmp slt i32 %"$*r44", 25
  store i32 0, i32* %format.size45, align 4
  store i8* null, i8** %bool.text46, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %LT, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text46, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text46, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text47" = load i8*, i8** %bool.text46, align 8
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text47")
  store i32 %call.snprintf48, i32* %format.size45, align 4
  %"$format.size49" = load i32, i32* %format.size45, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %AS51 = sext i32 %ADD50 to i64
  %call.calloc53 = call i8* @calloc(i64 %AS51, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc53, i8** %format.text52, align 8
  %"$format.text54" = load i8*, i8** %format.text52, align 8
  %"$bool.text55" = load i8*, i8** %bool.text46, align 8
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text54", i64 %AS51, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text55")
  %"$format.size57" = load i32, i32* %format.size45, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %"$format.text59" = load i8*, i8** %format.text52, align 8
  %"$format.size60" = load i32, i32* %format.size45, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text59", 0
  %10 = insertvalue %String %9, i32 %"$format.size60", 1
  %11 = insertvalue %String %10, i32 %ADD58, 2
  call void @"io::out"(%String %11)
  %"*r61" = load i32*, i32** %r, align 8
  %"$*r62" = load i32, i32* %"*r61", align 4
  %LE = icmp sle i32 %"$*r62", 20
  store i32 0, i32* %format.size63, align 4
  store i8* null, i8** %bool.text64, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %LE, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text64, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text65" = load i8*, i8** %bool.text64, align 8
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text65")
  store i32 %call.snprintf66, i32* %format.size63, align 4
  %"$format.size67" = load i32, i32* %format.size63, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %"$bool.text73" = load i8*, i8** %bool.text64, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text73")
  %"$format.size75" = load i32, i32* %format.size63, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %"$format.text77" = load i8*, i8** %format.text70, align 8
  %"$format.size78" = load i32, i32* %format.size63, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text77", 0
  %13 = insertvalue %String %12, i32 %"$format.size78", 1
  %14 = insertvalue %String %13, i32 %ADD76, 2
  call void @"io::out"(%String %14)
  %"*r79" = load i32*, i32** %r, align 8
  %"$*r80" = load i32, i32* %"*r79", align 4
  %GE = icmp sge i32 %"$*r80", 21
  store i32 0, i32* %format.size81, align 4
  store i8* null, i8** %bool.text82, align 8
  br label %if12

if12:                                             ; preds = %endif2
  br i1 %GE, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i8** %bool.text82, align 8
  br label %endif3

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0), i8** %bool.text82, align 8
  br label %endif3

endif3:                                           ; preds = %else13, %if12.then
  %"$bool.text83" = load i8*, i8** %bool.text82, align 8
  %call.snprintf84 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %"$bool.text83")
  store i32 %call.snprintf84, i32* %format.size81, align 4
  %"$format.size85" = load i32, i32* %format.size81, align 4
  %ADD86 = add i32 %"$format.size85", 1
  %AS87 = sext i32 %ADD86 to i64
  %call.calloc89 = call i8* @calloc(i64 %AS87, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc89, i8** %format.text88, align 8
  %"$format.text90" = load i8*, i8** %format.text88, align 8
  %"$bool.text91" = load i8*, i8** %bool.text82, align 8
  %call.snprintf92 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text90", i64 %AS87, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %"$bool.text91")
  %"$format.size93" = load i32, i32* %format.size81, align 4
  %ADD94 = add i32 %"$format.size93", 1
  %"$format.text95" = load i8*, i8** %format.text88, align 8
  %"$format.size96" = load i32, i32* %format.size81, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text95", 0
  %16 = insertvalue %String %15, i32 %"$format.size96", 1
  %17 = insertvalue %String %16, i32 %ADD94, 2
  call void @"io::out"(%String %17)
  %"*r97" = load i32*, i32** %r, align 8
  %"$*r98" = load i32, i32* %"*r97", align 4
  %BOR = or i32 %"$*r98", 3
  %call.snprintf100 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i32 %BOR)
  store i32 %call.snprintf100, i32* %format.size99, align 4
  %"$format.size101" = load i32, i32* %format.size99, align 4
  %ADD102 = add i32 %"$format.size101", 1
  %AS103 = sext i32 %ADD102 to i64
  %call.calloc105 = call i8* @calloc(i64 %AS103, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc105, i8** %format.text104, align 8
  %"$format.text106" = load i8*, i8** %format.text104, align 8
  %call.snprintf107 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text106", i64 %AS103, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i32 %BOR)
  %"$format.size108" = load i32, i32* %format.size99, align 4
  %ADD109 = add i32 %"$format.size108", 1
  %"$format.text110" = load i8*, i8** %format.text104, align 8
  %"$format.size111" = load i32, i32* %format.size99, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text110", 0
  %19 = insertvalue %String %18, i32 %"$format.size111", 1
  %20 = insertvalue %String %19, i32 %ADD109, 2
  call void @"io::out"(%String %20)
  %"*r112" = load i32*, i32** %r, align 8
  %"$*r113" = load i32, i32* %"*r112", align 4
  %BXOR = xor i32 %"$*r113", 4
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i32 %BXOR)
  store i32 %call.snprintf115, i32* %format.size114, align 4
  %"$format.size116" = load i32, i32* %format.size114, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %AS118 = sext i32 %ADD117 to i64
  %call.calloc120 = call i8* @calloc(i64 %AS118, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc120, i8** %format.text119, align 8
  %"$format.text121" = load i8*, i8** %format.text119, align 8
  %call.snprintf122 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text121", i64 %AS118, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i32 %BXOR)
  %"$format.size123" = load i32, i32* %format.size114, align 4
  %ADD124 = add i32 %"$format.size123", 1
  %"$format.text125" = load i8*, i8** %format.text119, align 8
  %"$format.size126" = load i32, i32* %format.size114, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text125", 0
  %22 = insertvalue %String %21, i32 %"$format.size126", 1
  %23 = insertvalue %String %22, i32 %ADD124, 2
  call void @"io::out"(%String %23)
  %"*r127" = load i32*, i32** %r, align 8
  %"$*r128" = load i32, i32* %"*r127", align 4
  %RSHIFT = ashr i32 %"$*r128", 2
  %call.snprintf130 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %RSHIFT)
  store i32 %call.snprintf130, i32* %format.size129, align 4
  %"$format.size131" = load i32, i32* %format.size129, align 4
  %ADD132 = add i32 %"$format.size131", 1
  %AS133 = sext i32 %ADD132 to i64
  %call.calloc135 = call i8* @calloc(i64 %AS133, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc135, i8** %format.text134, align 8
  %"$format.text136" = load i8*, i8** %format.text134, align 8
  %call.snprintf137 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text136", i64 %AS133, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %RSHIFT)
  %"$format.size138" = load i32, i32* %format.size129, align 4
  %ADD139 = add i32 %"$format.size138", 1
  %"$format.text140" = load i8*, i8** %format.text134, align 8
  %"$format.size141" = load i32, i32* %format.size129, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text140", 0
  %25 = insertvalue %String %24, i32 %"$format.size141", 1
  %26 = insertvalue %String %25, i32 %ADD139, 2
  call void @"io::out"(%String %26)
  %"*r142" = load i32*, i32** %r, align 8
  %"$*r143" = load i32, i32* %"*r142", align 4
  %AS144 = sext i32 %"$*r143" to i64
  %call.snprintf146 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i64 %AS144)
  store i32 %call.snprintf146, i32* %format.size145, align 4
  %"$format.size147" = load i32, i32* %format.size145, align 4
  %ADD148 = add i32 %"$format.size147", 1
  %AS149 = sext i32 %ADD148 to i64
  %call.calloc151 = call i8* @calloc(i64 %AS149, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc151, i8** %format.text150, align 8
  %"$format.text152" = load i8*, i8** %format.text150, align 8
  %call.snprintf153 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text152", i64 %AS149, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i64 %AS144)
  %"$format.size154" = load i32, i32* %format.size145, align 4
  %ADD155 = add i32 %"$format.size154", 1
  %"$format.text156" = load i8*, i8** %format.text150, align 8
  %"$format.size157" = load i32, i32* %format.size145, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text156", 0
  %28 = insertvalue %String %27, i32 %"$format.size157", 1
  %29 = insertvalue %String %28, i32 %ADD155, 2
  call void @"io::out"(%String %29)
  store i1 true, i1* %flag, align 1
  store i1* %flag, i1** %b, align 8
  %"*b" = load i1*, i1** %b, align 8
  %"$*b" = load i1, i1* %"*b", align 1
  %AND = and i1 %"$*b", false
  store i32 0, i32* %format.size158, align 4
  store i8* null, i8** %bool.text159, align 8
  br label %if14

if14:                                             ; preds = %endif3
  br i1 %AND, label %if14.then, label %else15

if14.then:                                        ; preds = %if14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.18, i32 0, i32 0), i8** %bool.text159, align 8
  br label %endif4

else15:                                           ; preds = %if14
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i8** %bool.text159, align 8
  br label %endif4

endif4:                                           ; preds = %else15, %if14.then
  %"$bool.text160" = load i8*, i8** %bool.text159, align 8
  %call.snprintf161 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text160")
  store i32 %call.snprintf161, i32* %format.size158, align 4
  %"$format.size162" = load i32, i32* %format.size158, align 4
  %ADD163 = add i32 %"$format.size162", 1
  %AS164 = sext i32 %ADD163 to i64
  %call.calloc166 = call i8* @calloc(i64 %AS164, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc166, i8** %format.text165, align 8
  %"$format.text167" = load i8*, i8** %format.text165, align 8
  %"$bool.text168" = load i8*, i8** %bool.text159, align 8
  %call.snprintf169 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text167", i64 %AS164, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text168")
  %"$format.size170" = load i32, i32* %format.size158, align 4
  %ADD171 = add i32 %"$format.size170", 1
  %"$format.text172" = load i8*, i8** %format.text165, align 8
  %"$format.size173" = load i32, i32* %format.size158, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text172", 0
  %31 = insertvalue %String %30, i32 %"$format.size173", 1
  %32 = insertvalue %String %31, i32 %ADD171, 2
  call void @"io::out"(%String %32)
  %"*b174" = load i1*, i1** %b, align 8
  %"$*b175" = load i1, i1* %"*b174", align 1
  %OR = or i1 %"$*b175", false
  store i32 0, i32* %format.size176, align 4
  store i8* null, i8** %bool.text177, align 8
  br label %if16

if16:                                             ; preds = %endif4
  br i1 %OR, label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.21, i32 0, i32 0), i8** %bool.text177, align 8
  br label %endif5

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.22, i32 0, i32 0), i8** %bool.text177, align 8
  br label %endif5

endif5:                                           ; preds = %else17, %if16.then
  %"$bool.text178" = load i8*, i8** %bool.text177, align 8
  %call.snprintf179 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.23, i32 0, i32 0), i8* %"$bool.text178")
  store i32 %call.snprintf179, i32* %format.size176, align 4
  %"$format.size180" = load i32, i32* %format.size176, align 4
  %ADD181 = add i32 %"$format.size180", 1
  %AS182 = sext i32 %ADD181 to i64
  %call.calloc184 = call i8* @calloc(i64 %AS182, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc184, i8** %format.text183, align 8
  %"$format.text185" = load i8*, i8** %format.text183, align 8
  %"$bool.text186" = load i8*, i8** %bool.text177, align 8
  %call.snprintf187 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text185", i64 %AS182, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.23, i32 0, i32 0), i8* %"$bool.text186")
  %"$format.size188" = load i32, i32* %format.size176, align 4
  %ADD189 = add i32 %"$format.size188", 1
  %"$format.text190" = load i8*, i8** %format.text183, align 8
  %"$format.size191" = load i32, i32* %format.size176, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text190", 0
  %34 = insertvalue %String %33, i32 %"$format.size191", 1
  %35 = insertvalue %String %34, i32 %ADD189, 2
  call void @"io::out"(%String %35)
  %"*b192" = load i1*, i1** %b, align 8
  %"$*b193" = load i1, i1* %"*b192", align 1
  %NOT = xor i1 %"$*b193", true
  store i32 0, i32* %format.size194, align 4
  store i8* null, i8** %bool.text195, align 8
  br label %if18

if18:                                             ; preds = %endif5
  br i1 %NOT, label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.24, i32 0, i32 0), i8** %bool.text195, align 8
  br label %endif6

else19:                                           ; preds = %if18
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0), i8** %bool.text195, align 8
  br label %endif6

endif6:                                           ; preds = %else19, %if18.then
  %"$bool.text196" = load i8*, i8** %bool.text195, align 8
  %call.snprintf197 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text196")
  store i32 %call.snprintf197, i32* %format.size194, align 4
  %"$format.size198" = load i32, i32* %format.size194, align 4
  %ADD199 = add i32 %"$format.size198", 1
  %AS200 = sext i32 %ADD199 to i64
  %call.calloc202 = call i8* @calloc(i64 %AS200, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc202, i8** %format.text201, align 8
  %"$format.text203" = load i8*, i8** %format.text201, align 8
  %"$bool.text204" = load i8*, i8** %bool.text195, align 8
  %call.snprintf205 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text203", i64 %AS200, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text204")
  %"$format.size206" = load i32, i32* %format.size194, align 4
  %ADD207 = add i32 %"$format.size206", 1
  %"$format.text208" = load i8*, i8** %format.text201, align 8
  %"$format.size209" = load i32, i32* %format.size194, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text208", 0
  %37 = insertvalue %String %36, i32 %"$format.size209", 1
  %38 = insertvalue %String %37, i32 %ADD207, 2
  call void @"io::out"(%String %38)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 111 — ref: a ref declared without a value starts as null

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"null\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"bound %d\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"null again\0A\00", align 1

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
  %r = alloca i32*, align 8
  %n = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32* null, i32** %r, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*r" = load i32*, i32** %r, align 8
  %"$r" = load i32*, i32** %r, align 8
  %EQ = icmp eq i32* %"$r", null
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  store i32 4, i32* %n, align 4
  store i32* %n, i32** %r, align 8
  br label %if7

if7:                                              ; preds = %endif
  %"*r3" = load i32*, i32** %r, align 8
  %"$r4" = load i32*, i32** %r, align 8
  %NQ = icmp ne i32* %"$r4", null
  br i1 %NQ, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  %"*r5" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r5", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$*r")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$*r")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  br label %endif1

endif1:                                           ; preds = %if7.then, %if7
  store i32* null, i32** %r, align 8
  br label %if8

if8:                                              ; preds = %endif1
  %"*r11" = load i32*, i32** %r, align 8
  %"$r12" = load i32*, i32** %r, align 8
  %EQ13 = icmp eq i32* %"$r12", null
  br i1 %EQ13, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  call void @"io::out"(%String { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 11, i32 0 })
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 112 — ref: binding a reference to the wrong type

```stderr
error: test.ura:6:17 expected '&i8[]', found '&i32'
  |
6 |     y &char[] = &x
  |                 ^
error: aborting due to 1 error
1
```

## 113 — ref: a ref parameter given a value

```stderr
error: test.ura:9:17 expected '&i32', found 'i32'
  |
9 |     return tick(5)
  |                 ^
error: aborting due to 1 error
1
```

## 114 — ref: taking a reference to a literal

```stderr
error: test.ura:5:15 '&' expects a variable or a struct attribute
  |
5 |     r &i32 = &5
  |               ^
error: aborting due to 1 error
1
```

## 115 — ref: a by-value parameter given a reference

```stderr
error: test.ura:9:18 expected 'i32', found '&i32'
  |
9 |     return twice(&x)
  |                  ^
error: aborting due to 1 error
1
```

## 116 — ref: a reference to a name that does not exist

```stderr
error: test.ura:5:15 undeclared variable 'ghost'
  |
5 |     r &i32 = &ghost
  |               ^^^^^
help: declare it first, like: ghost i32 = 0
error: aborting due to 1 error
1
```

## 117 — ref: a nested function may not capture an enclosing local

```stderr
error: test.ura:8:16 'r' belongs to an enclosing function
  |
8 |         return r
  |                ^
help: pass it as a parameter
error: aborting due to 1 error
1
```

## 118 — ref: linked list: build with ref, walk it with cur = ref cur.next

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Node = type { i32, %Node* }

@str = private unnamed_addr constant [8 x i8] c"sum=%d\0A\00", align 1

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
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %cur = alloca %Node*, align 8
  %sum = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 10, i32* %a.value, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 20, i32* %b.value, align 4
  store %Node zeroinitializer, %Node* %c, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 30, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  store %Node* %a, %Node** %cur, align 8
  store i32 0, i32* %sum, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*cur" = load %Node*, %Node** %cur, align 8
  %"$cur" = load %Node*, %Node** %cur, align 8
  %NQ = icmp ne %Node* %"$cur", null
  br i1 %NQ, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*cur1" = load %Node*, %Node** %cur, align 8
  %"*cur.value" = getelementptr inbounds %Node, %Node* %"*cur1", i32 0, i32 0
  %"$*cur.value" = load i32, i32* %"*cur.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*cur.value"
  store i32 %ADD, i32* %sum, align 4
  %"*cur2" = load %Node*, %Node** %cur, align 8
  %"*cur.next" = getelementptr inbounds %Node, %Node* %"*cur2", i32 0, i32 1
  %"$*cur.next" = load %Node*, %Node** %"*cur.next", align 8
  store %Node* %"$*cur.next", %Node** %cur, align 8
  br label %while6

endloop:                                          ; preds = %while6
  %"$sum3" = load i32, i32* %sum, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$sum3")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD4 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$sum3")
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 119 — ref: a ref parameter rebinds a struct's ref field in place (add-front)

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Lst = type { %Node* }
%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

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

define void @push(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  %node = alloca %Node*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  store %Node* %1, %Node** %node, align 8
  %"*node" = load %Node*, %Node** %node, align 8
  %"*node.next" = getelementptr inbounds %Node, %Node* %"*node", i32 0, i32 1
  %"*lst" = load %Lst*, %Lst** %lst, align 8
  %"*lst.head" = getelementptr inbounds %Lst, %Lst* %"*lst", i32 0, i32 0
  %"$*lst.head" = load %Node*, %Node** %"*lst.head", align 8
  store %Node* %"$*lst.head", %Node** %"*node.next", align 8
  %"*lst1" = load %Lst*, %Lst** %lst, align 8
  %"*lst.head2" = getelementptr inbounds %Lst, %Lst* %"*lst1", i32 0, i32 0
  %"*node3" = load %Node*, %Node** %node, align 8
  store %Node* %"*node3", %Node** %"*lst.head2", align 8
  ret void
}

define i32 @main() {
entry:
  %lst = alloca %Lst, align 8
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %cur = alloca %Node*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.content = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.content, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.content = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.content, align 4
  call void @push(%Lst* %lst, %Node* %a)
  call void @push(%Lst* %lst, %Node* %b)
  %lst.head = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"$lst.head" = load %Node*, %Node** %lst.head, align 8
  store %Node* %"$lst.head", %Node** %cur, align 8
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*cur" = load %Node*, %Node** %cur, align 8
  %"$cur" = load %Node*, %Node** %cur, align 8
  %NQ = icmp ne %Node* %"$cur", null
  br i1 %NQ, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*cur1" = load %Node*, %Node** %cur, align 8
  %"*cur.content" = getelementptr inbounds %Node, %Node* %"*cur1", i32 0, i32 0
  %"$*cur.content" = load i32, i32* %"*cur.content", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*cur.content")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*cur.content")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  %"*cur7" = load %Node*, %Node** %cur, align 8
  %"*cur.next" = getelementptr inbounds %Node, %Node* %"*cur7", i32 0, i32 1
  %"$*cur.next" = load %Node*, %Node** %"*cur.next", align 8
  store %Node* %"$*cur.next", %Node** %cur, align 8
  br label %while6

endloop:                                          ; preds = %while6
  call void @"io::out"(%String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 1, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 120 — ref: ref? compared to null with == and !=, bound and unbound

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%N = type { i32, %N* }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1

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
  %a = alloca %N, align 8
  %p = alloca %N*, align 8
  %q = alloca %N*, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text6 = alloca i8*, align 8
  %bool.text7 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store %N zeroinitializer, %N* %a, align 8
  %a.v = getelementptr inbounds %N, %N* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  store %N* %a, %N** %p, align 8
  store %N* null, %N** %q, align 8
  %"*p" = load %N*, %N** %p, align 8
  %"$p" = load %N*, %N** %p, align 8
  %EQ = icmp eq %N* %"$p", null
  %"*p3" = load %N*, %N** %p, align 8
  %"$p4" = load %N*, %N** %p, align 8
  %NQ = icmp ne %N* %"$p4", null
  %"*q" = load %N*, %N** %q, align 8
  %"$q" = load %N*, %N** %q, align 8
  %EQ5 = icmp eq %N* %"$q", null
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
  store i8* null, i8** %bool.text6, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %NQ, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  store i8* null, i8** %bool.text7, align 8
  br label %if10

if10:                                             ; preds = %endif1
  br i1 %EQ5, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif2

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif2

endif2:                                           ; preds = %else11, %if10.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text6, align 8
  %"$bool.text9" = load i8*, i8** %bool.text7, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text8", i8* %"$bool.text9")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text10" = load i8*, i8** %bool.text, align 8
  %"$bool.text11" = load i8*, i8** %bool.text6, align 8
  %"$bool.text12" = load i8*, i8** %bool.text7, align 8
  %call.snprintf13 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text10", i8* %"$bool.text11", i8* %"$bool.text12")
  %"$format.size14" = load i32, i32* %format.size, align 4
  %ADD15 = add i32 %"$format.size14", 1
  %"$format.text16" = load i8*, i8** %format.text, align 8
  %"$format.size17" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text16", 0
  %1 = insertvalue %String %0, i32 %"$format.size17", 1
  %2 = insertvalue %String %1, i32 %ADD15, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 121 — ref: chained rebind removes the head: lst.head = ref lst.head.next

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Node = type { i32, %Node* }
%Lst = type { %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %lst = alloca %Lst, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.v = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.v = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.v, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %lst.head = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  store %Node* %a, %Node** %lst.head, align 8
  %lst.head1 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %lst.head2 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"*lst.head" = load %Node*, %Node** %lst.head2, align 8
  %"*lst.head.next" = getelementptr inbounds %Node, %Node* %"*lst.head", i32 0, i32 1
  %"$*lst.head.next" = load %Node*, %Node** %"*lst.head.next", align 8
  store %Node* %"$*lst.head.next", %Node** %lst.head1, align 8
  %lst.head3 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"*lst.head4" = load %Node*, %Node** %lst.head3, align 8
  %"*lst.head.v" = getelementptr inbounds %Node, %Node* %"*lst.head4", i32 0, i32 0
  %"$*lst.head.v" = load i32, i32* %"*lst.head.v", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*lst.head.v")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*lst.head.v")
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 122 — ref: = ref rebinds the pointer, = value writes through the referent

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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
  %y = alloca i32, align 4
  %cur = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size7 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  store i32 10, i32* %x, align 4
  store i32 20, i32* %y, align 4
  store i32* %x, i32** %cur, align 8
  %"*cur" = load i32*, i32** %cur, align 8
  store i32 99, i32* %"*cur", align 4
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store i32* %y, i32** %cur, align 8
  %"*cur6" = load i32*, i32** %cur, align 8
  store i32 88, i32* %"*cur6", align 4
  %"$x8" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$x8", i32 %"$y")
  store i32 %call.snprintf9, i32* %format.size7, align 4
  %"$format.size10" = load i32, i32* %format.size7, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$x8", i32 %"$y")
  %"$format.size17" = load i32, i32* %format.size7, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
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

## 123 — ref: ref a == ref a is identity true, ref a == ref b is false

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%N = type { i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

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
  %a = alloca %N, align 8
  %b = alloca %N, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text3 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store %N zeroinitializer, %N* %a, align 4
  store %N zeroinitializer, %N* %b, align 4
  %EQ = icmp eq %N* %a, %a
  %EQ2 = icmp eq %N* %a, %b
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
  store i8* null, i8** %bool.text3, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %EQ2, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text4" = load i8*, i8** %bool.text3, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text4")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text5" = load i8*, i8** %bool.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text3, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text5", i8* %"$bool.text6")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 124 — ref: a value function cannot return a reference

```stderr
error: test.ura:5:12 expected 'i32', found '&i32'
  |
5 |     return &x
  |            ^
error: aborting due to 1 error
1
```

## 125 — ref: a ref declaration cannot be bound to a plain value

```stderr
error: test.ura:4:14 'm' is a ref, initialize it with an address
  |
4 |     m &i32 = 5
  |              ^
error: aborting due to 1 error
1
```

## 126 — ref: a non-nullable value cannot be compared to null

```stderr
error: test.ura:5:16 'x' is never null, so this comparison is always the same
  |
5 |     io::out($"{x == null}\n")
  |                ^
error: aborting due to 1 error
1
```

## 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [24 x i8] c"%d %d %d %d %d size %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [22 x i8] c"%d %d %d %d %d %d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [20 x i8] c"%d %d %d %d cap %d\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@TYPEOF = private unnamed_addr constant [10 x i8] c"Array<i8>\00", align 1
@str.9 = private unnamed_addr constant [9 x i8] c"%c%c %s\0A\00", align 1

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

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i8>.own"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
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
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i8>.append"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  call void @"Array<i8>.own"(%"Array<i8>"* %"*left")
  %"*left1" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load i8*, i8** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds i8, i8* %"$*left.data", i32 %"$index"
  ret i8* %"*left.data[]"
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if7

if7:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  ret void

endif1:                                           ; preds = %if9
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
  br label %if10

if10:                                             ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if10.then, %if10
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

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %if12

if12:                                             ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  ret void

endif1:                                           ; preds = %if12
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if13

if13:                                             ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if13.then, label %endif2

if13.then:                                        ; preds = %if13
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if13.then, %if13
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define void @"Array<i32>.resize"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %size = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self", i32 %"$size")
  br label %while15

while15:                                          ; preds = %while15.then, %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %SUB = sub i32 %"$*self.count5", 1
  store i32 %SUB, i32* %"*self.count4", align 4
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while15

endloop:                                          ; preds = %while15
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  br label %while17

while17:                                          ; preds = %while17.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define void @"Array<i32>.release"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if18

if18:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if18.then, %if18
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  store i32* null, i32** %"*self.data4", align 8
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 2
  store i32 0, i32* %"*self.cap6", align 4
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]", align 4
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.insert"(%"Array<i32>"* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 %2, i32* %value, align 4
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  store i32 %"$*self.count8", i32* %i, align 4
  br label %while21

while21:                                          ; preds = %while21.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i10"
  %"*self11" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self11", i32 0, i32 0
  %"$i13" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i13", 1
  %"$*self.data14" = load i32*, i32** %"*self.data12", align 8
  %"*self.data[]15" = getelementptr inbounds i32, i32* %"$*self.data14", i32 %SUB
  %"$*self.data[]" = load i32, i32* %"*self.data[]15", align 4
  store i32 %"$*self.data[]", i32* %"*self.data[]", align 4
  %"$i16" = load i32, i32* %i, align 4
  %SUB17 = sub i32 %"$i16", 1
  store i32 %SUB17, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
  %"*self18" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data19" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self18", i32 0, i32 0
  %"$*self.data20" = load i32*, i32** %"*self.data19", align 8
  %"$index21" = load i32, i32* %index, align 4
  %"*self.data[]22" = getelementptr inbounds i32, i32* %"$*self.data20", i32 %"$index21"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]22", align 4
  %"*self23" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count24" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %ADD26 = add i32 %"$*self.count25", 1
  store i32 %ADD26, i32* %"*self.count24", align 4
  ret void
}

define i32 @"Array<i32>.pop"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 0, i32* %value, align 4
  br label %if22

if22:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"

endif:                                            ; preds = %if22
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self2")
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index4"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %value, align 4
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while23

while23:                                          ; preds = %while23.then, %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while23.then, label %endloop

while23.then:                                     ; preds = %while23
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load i32*, i32** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds i32, i32* %"$*self.data12", i32 %"$i13"
  %"*self15" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data16" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self15", i32 0, i32 0
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  %"$*self.data18" = load i32*, i32** %"*self.data16", align 8
  %"*self.data[]19" = getelementptr inbounds i32, i32* %"$*self.data18", i32 %ADD
  %"$*self.data[]20" = load i32, i32* %"*self.data[]19", align 4
  store i32 %"$*self.data[]20", i32* %"*self.data[]14", align 4
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while23

endloop:                                          ; preds = %while23
  %"*self23" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count24" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %SUB26 = sub i32 %"$*self.count25", 1
  store i32 %SUB26, i32* %"*self.count24", align 4
  %"*self27" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data28" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self27", i32 0, i32 0
  %"*self29" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count30" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self29", i32 0, i32 1
  %"$*self.data31" = load i32*, i32** %"*self.data28", align 8
  %"$*self.count32" = load i32, i32* %"*self.count30", align 4
  %"*self.data[]33" = getelementptr inbounds i32, i32* %"$*self.data31", i32 %"$*self.count32"
  store i32 0, i32* %"*self.data[]33", align 4
  %"$value34" = load i32, i32* %value, align 4
  ret i32 %"$value34"
}

define i32 @"Array<i32>.pop.1"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<i32>.pop" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %"*self", i32 %SUB)
  ret i32 %"call.Array<i32>.pop"
}

define void @"Array<i32>.reverse"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  store i32 0, i32* %i, align 4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while24

while24:                                          ; preds = %while24.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %tmp, align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"$*self.data6" = load i32*, i32** %"*self.data5", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]8" = getelementptr inbounds i32, i32* %"$*self.data6", i32 %"$i7"
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data10" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"$*self.data11" = load i32*, i32** %"*self.data10", align 8
  %"$j12" = load i32, i32* %j, align 4
  %"*self.data[]13" = getelementptr inbounds i32, i32* %"$*self.data11", i32 %"$j12"
  %"$*self.data[]14" = load i32, i32* %"*self.data[]13", align 4
  store i32 %"$*self.data[]14", i32* %"*self.data[]8", align 4
  %"*self15" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data16" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self15", i32 0, i32 0
  %"$*self.data17" = load i32*, i32** %"*self.data16", align 8
  %"$j18" = load i32, i32* %j, align 4
  %"*self.data[]19" = getelementptr inbounds i32, i32* %"$*self.data17", i32 %"$j18"
  %"$tmp" = load i32, i32* %tmp, align 4
  store i32 %"$tmp", i32* %"*self.data[]19", align 4
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  %"$j21" = load i32, i32* %j, align 4
  %SUB22 = sub i32 %"$j21", 1
  store i32 %SUB22, i32* %j, align 4
  br label %while24

endloop:                                          ; preds = %while24
  ret void
}

define i32* @"Array<i32>.first"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 0
  ret i32* %"*self.data[]"
}

define i32* @"Array<i32>.last"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %SUB
  ret i32* %"*self.data[]"
}

define i32 @"Array<i32>.len"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @"Array<i32>.capacity"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @"Array<i32>.is_empty"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*left")
  %"*left1" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load i32*, i32** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds i32, i32* %"$*left.data", i32 %"$index"
  ret i32* %"*left.data[]"
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %value = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %value, align 8
  %"*value" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*value")
  br label %if25

if25:                                             ; preds = %entry
  %"*value1" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"*value2" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<i32>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  ret void
}

define i32 @main() {
entry:
  %a = alloca %"Array<i32>", align 8
  %ref = alloca i32*, align 8
  %ref3 = alloca i32*, align 8
  %ref6 = alloca i32*, align 8
  %ref9 = alloca i32*, align 8
  %ref12 = alloca i32*, align 8
  %ref15 = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %ref28 = alloca i32*, align 8
  %ref30 = alloca i32*, align 8
  %ref33 = alloca i32*, align 8
  %format.size35 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text42 = alloca i8*, align 8
  %ref52 = alloca i32*, align 8
  %ref55 = alloca i32*, align 8
  %ref58 = alloca i32*, align 8
  %ref61 = alloca i32*, align 8
  %format.size63 = alloca i32, align 4
  %format.text72 = alloca i8*, align 8
  %format.size82 = alloca i32, align 4
  %bool.text83 = alloca i8*, align 8
  %format.text89 = alloca i8*, align 8
  %c = alloca %"Array<i8>", align 8
  %ref98 = alloca i8*, align 8
  %ref101 = alloca i8*, align 8
  %format.size103 = alloca i32, align 4
  %format.text112 = alloca i8*, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 1)
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 2)
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 3)
  call void @"Array<i32>.insert"(%"Array<i32>"* %a, i32 0, i32 0)
  call void @"Array<i32>.insert"(%"Array<i32>"* %a, i32 2, i32 7)
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32 10, i32* %"*ref", align 4
  %"call.Array<i32>.[]2" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]2", i32** %ref3, align 8
  %"*ref4" = load i32*, i32** %ref3, align 8
  %"call.Array<i32>.[]5" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]5", i32** %ref6, align 8
  %"*ref7" = load i32*, i32** %ref6, align 8
  %"call.Array<i32>.[]8" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]8", i32** %ref9, align 8
  %"*ref10" = load i32*, i32** %ref9, align 8
  %"call.Array<i32>.[]11" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]11", i32** %ref12, align 8
  %"*ref13" = load i32*, i32** %ref12, align 8
  %"call.Array<i32>.[]14" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 4)
  store i32* %"call.Array<i32>.[]14", i32** %ref15, align 8
  %"*ref16" = load i32*, i32** %ref15, align 8
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"$*ref" = load i32, i32* %"*ref4", align 4
  %"$*ref17" = load i32, i32* %"*ref7", align 4
  %"$*ref18" = load i32, i32* %"*ref10", align 4
  %"$*ref19" = load i32, i32* %"*ref13", align 4
  %"$*ref20" = load i32, i32* %"*ref16", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  %"$format.size22" = load i32, i32* %format.size, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %"$format.text24" = load i8*, i8** %format.text, align 8
  %"$format.size25" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text24", 0
  %1 = insertvalue %String %0, i32 %"$format.size25", 1
  %2 = insertvalue %String %1, i32 %ADD23, 2
  call void @"io::out"(%String %2)
  %"call.Array<i32>.pop" = call i32 @"Array<i32>.pop.1"(%"Array<i32>"* %a)
  store i32 %"call.Array<i32>.pop", i32* %x, align 4
  %"call.Array<i32>.pop26" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 0)
  store i32 %"call.Array<i32>.pop26", i32* %y, align 4
  %"call.Array<i32>.pop27" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 1)
  store i32 %"call.Array<i32>.pop27", i32* %z, align 4
  %"call.Array<i32>.last" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last", i32** %ref28, align 8
  %"*ref29" = load i32*, i32** %ref28, align 8
  store i32 9, i32* %"*ref29", align 4
  %"call.Array<i32>.first" = call i32* @"Array<i32>.first"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.first", i32** %ref30, align 8
  %"*ref31" = load i32*, i32** %ref30, align 8
  %"call.Array<i32>.last32" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last32", i32** %ref33, align 8
  %"*ref34" = load i32*, i32** %ref33, align 8
  %"call.Array<i32>.len" = call i32 @"Array<i32>.len"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  store i32 0, i32* %format.size35, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$*ref36" = load i32, i32* %"*ref31", align 4
  %"$*ref37" = load i32, i32* %"*ref34", align 4
  store i8* null, i8** %bool.text, align 8
  br label %if26

if26:                                             ; preds = %entry
  br i1 %"call.Array<i32>.is_empty", label %if26.then, label %else27

if26.then:                                        ; preds = %if26
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else27:                                           ; preds = %if26
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else27, %if26.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf38 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref36", i32 %"$*ref37", i32 %"call.Array<i32>.len", i8* %"$bool.text")
  store i32 %call.snprintf38, i32* %format.size35, align 4
  %"$format.size39" = load i32, i32* %format.size35, align 4
  %ADD40 = add i32 %"$format.size39", 1
  %AS41 = sext i32 %ADD40 to i64
  %call.calloc43 = call i8* @calloc(i64 %AS41, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc43, i8** %format.text42, align 8
  %"$format.text44" = load i8*, i8** %format.text42, align 8
  %"$bool.text45" = load i8*, i8** %bool.text, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text44", i64 %AS41, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref36", i32 %"$*ref37", i32 %"call.Array<i32>.len", i8* %"$bool.text45")
  %"$format.size47" = load i32, i32* %format.size35, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text42, align 8
  %"$format.size50" = load i32, i32* %format.size35, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %4 = insertvalue %String %3, i32 %"$format.size50", 1
  %5 = insertvalue %String %4, i32 %ADD48, 2
  call void @"io::out"(%String %5)
  call void @"Array<i32>.resize"(%"Array<i32>"* %a, i32 4)
  call void @"Array<i32>.reverse"(%"Array<i32>"* %a)
  %"call.Array<i32>.[]51" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]51", i32** %ref52, align 8
  %"*ref53" = load i32*, i32** %ref52, align 8
  %"call.Array<i32>.[]54" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]54", i32** %ref55, align 8
  %"*ref56" = load i32*, i32** %ref55, align 8
  %"call.Array<i32>.[]57" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]57", i32** %ref58, align 8
  %"*ref59" = load i32*, i32** %ref58, align 8
  %"call.Array<i32>.[]60" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]60", i32** %ref61, align 8
  %"*ref62" = load i32*, i32** %ref61, align 8
  %"call.Array<i32>.capacity" = call i32 @"Array<i32>.capacity"(%"Array<i32>"* %a)
  %"$*ref64" = load i32, i32* %"*ref53", align 4
  %"$*ref65" = load i32, i32* %"*ref56", align 4
  %"$*ref66" = load i32, i32* %"*ref59", align 4
  %"$*ref67" = load i32, i32* %"*ref62", align 4
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.5, i32 0, i32 0), i32 %"$*ref64", i32 %"$*ref65", i32 %"$*ref66", i32 %"$*ref67", i32 %"call.Array<i32>.capacity")
  store i32 %call.snprintf68, i32* %format.size63, align 4
  %"$format.size69" = load i32, i32* %format.size63, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %AS71 = sext i32 %ADD70 to i64
  %call.calloc73 = call i8* @calloc(i64 %AS71, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc73, i8** %format.text72, align 8
  %"$format.text74" = load i8*, i8** %format.text72, align 8
  %call.snprintf75 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text74", i64 %AS71, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.5, i32 0, i32 0), i32 %"$*ref64", i32 %"$*ref65", i32 %"$*ref66", i32 %"$*ref67", i32 %"call.Array<i32>.capacity")
  %"$format.size76" = load i32, i32* %format.size63, align 4
  %ADD77 = add i32 %"$format.size76", 1
  %"$format.text78" = load i8*, i8** %format.text72, align 8
  %"$format.size79" = load i32, i32* %format.size63, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text78", 0
  %7 = insertvalue %String %6, i32 %"$format.size79", 1
  %8 = insertvalue %String %7, i32 %ADD77, 2
  call void @"io::out"(%String %8)
  call void @"Array<i32>.clear"(%"Array<i32>"* %a)
  %"call.Array<i32>.size80" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty81" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  store i32 0, i32* %format.size82, align 4
  store i8* null, i8** %bool.text83, align 8
  br label %if28

if28:                                             ; preds = %endif
  br i1 %"call.Array<i32>.is_empty81", label %if28.then, label %else29

if28.then:                                        ; preds = %if28
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

else29:                                           ; preds = %if28
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

endif1:                                           ; preds = %else29, %if28.then
  %"$bool.text84" = load i8*, i8** %bool.text83, align 8
  %call.snprintf85 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i32 %"call.Array<i32>.size80", i8* %"$bool.text84")
  store i32 %call.snprintf85, i32* %format.size82, align 4
  %"$format.size86" = load i32, i32* %format.size82, align 4
  %ADD87 = add i32 %"$format.size86", 1
  %AS88 = sext i32 %ADD87 to i64
  %call.calloc90 = call i8* @calloc(i64 %AS88, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc90, i8** %format.text89, align 8
  %"$format.text91" = load i8*, i8** %format.text89, align 8
  %"$bool.text92" = load i8*, i8** %bool.text83, align 8
  %call.snprintf93 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text91", i64 %AS88, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i32 %"call.Array<i32>.size80", i8* %"$bool.text92")
  %"$format.size94" = load i32, i32* %format.size82, align 4
  %ADD95 = add i32 %"$format.size94", 1
  %"$format.text96" = load i8*, i8** %format.text89, align 8
  %"$format.size97" = load i32, i32* %format.size82, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text96", 0
  %10 = insertvalue %String %9, i32 %"$format.size97", 1
  %11 = insertvalue %String %10, i32 %ADD95, 2
  call void @"io::out"(%String %11)
  call void @"Array<i32>.release"(%"Array<i32>"* %a)
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %c, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 104)
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 105)
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref98, align 8
  %"*ref99" = load i8*, i8** %ref98, align 8
  %"call.Array<i8>.[]100" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 1)
  store i8* %"call.Array<i8>.[]100", i8** %ref101, align 8
  %"*ref102" = load i8*, i8** %ref101, align 8
  %"$*ref104" = load i8, i8* %"*ref99", align 1
  %AS105 = sext i8 %"$*ref104" to i32
  %"$*ref106" = load i8, i8* %"*ref102", align 1
  %AS107 = sext i8 %"$*ref106" to i32
  %call.snprintf108 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.9, i32 0, i32 0), i32 %AS105, i32 %AS107, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf108, i32* %format.size103, align 4
  %"$format.size109" = load i32, i32* %format.size103, align 4
  %ADD110 = add i32 %"$format.size109", 1
  %AS111 = sext i32 %ADD110 to i64
  %call.calloc113 = call i8* @calloc(i64 %AS111, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc113, i8** %format.text112, align 8
  %"$format.text114" = load i8*, i8** %format.text112, align 8
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text114", i64 %AS111, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.9, i32 0, i32 0), i32 %AS105, i32 %AS107, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size116" = load i32, i32* %format.size103, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %"$format.text118" = load i8*, i8** %format.text112, align 8
  %"$format.size119" = load i32, i32* %format.size103, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text118", 0
  %13 = insertvalue %String %12, i32 %"$format.size119", 1
  %14 = insertvalue %String %13, i32 %ADD117, 2
  call void @"io::out"(%String %14)
  call void @"Array<i8>.drop"(%"Array<i8>"* %c)
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 129 — use: a package that doesn't exist

```stderr
error: test.ura:2:5 package '@/nothing' not found
  |
2 | use "@/nothing"
  |     ^^^^^^^^^^^
help: add a dependency that has it: ura deps add <name>
error: aborting due to 1 error
1
```

## 130 — use: an error after a use points at the user's file and line

```stderr
error: test.ura:6:7 'Array<i32>' has no method 'append' that takes (String)
  |
6 |     a.append("text")
  |       ^^^^^^
help: 'append' takes (i32)
error: aborting due to 1 error
1
```

## 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%d %d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %value = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %value, align 8
  %"*value" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*value")
  br label %if8

if8:                                              ; preds = %entry
  %"*value1" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*value2" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<i32>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  ret void
}

define void @"free<Array<i32>>"(%"Array<i32>"* %0) {
entry:
  %data = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %data, align 8
  %"$data" = load %"Array<i32>"*, %"Array<i32>"** %data, align 8
  %AS = bitcast %"Array<i32>"* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  br label %while10

while10:                                          ; preds = %while10.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self4" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count10"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]", align 8
  br label %while10

endloop:                                          ; preds = %while10
  ret void
}

define i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %value = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %value, align 8
  %"*value" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*value")
  br label %if11

if11:                                             ; preds = %entry
  %"*value1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*value2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %"Array<i32>"*, %"Array<i32>"** %"*value.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %u = alloca i32, align 4
  %format.size10 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %c = alloca i32, align 4
  %format.size34 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %r = alloca i32*, align 8
  %grid = alloca %"Array<Array<i32>>", align 8
  %format.size50 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  store i32 6, i32* %a, align 4
  store i32 3, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %BAND = and i32 %"$a", %"$b"
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %BOR = or i32 %"$a1", %"$b2"
  %"$a3" = load i32, i32* %a, align 4
  %"$b4" = load i32, i32* %b, align 4
  %BXOR = xor i32 %"$a3", %"$b4"
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %BAND, i32 %BOR, i32 %BXOR, i32 16, i32 -4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %BAND, i32 %BOR, i32 %BXOR, i32 16, i32 -4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  store i32 -16, i32* %u, align 4
  %"$u" = load i32, i32* %u, align 4
  %RSHIFT = lshr i32 %"$u", 28
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %RSHIFT)
  store i32 %call.snprintf11, i32* %format.size10, align 4
  %"$format.size12" = load i32, i32* %format.size10, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %RSHIFT)
  %"$format.size19" = load i32, i32* %format.size10, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size10, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  store i32 1, i32* %c, align 4
  %"$c" = load i32, i32* %c, align 4
  %LSHIFT = shl i32 %"$c", 3
  store i32 %LSHIFT, i32* %c, align 4
  %"$c23" = load i32, i32* %c, align 4
  %BOR24 = or i32 %"$c23", 1
  store i32 %BOR24, i32* %c, align 4
  %"$c25" = load i32, i32* %c, align 4
  %BAND26 = and i32 %"$c25", 13
  store i32 %BAND26, i32* %c, align 4
  %"$c27" = load i32, i32* %c, align 4
  %BXOR28 = xor i32 %"$c27", 4
  store i32 %BXOR28, i32* %c, align 4
  %"$c29" = load i32, i32* %c, align 4
  %RSHIFT30 = ashr i32 %"$c29", 1
  store i32 %RSHIFT30, i32* %c, align 4
  %"$a31" = load i32, i32* %a, align 4
  %"$b32" = load i32, i32* %b, align 4
  %BAND33 = and i32 %"$a31", %"$b32"
  %EQ = icmp eq i32 %BAND33, 2
  store i32 0, i32* %format.size34, align 4
  %"$c35" = load i32, i32* %c, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if12

if12:                                             ; preds = %entry
  br i1 %EQ, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else13, %if12.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$c35", i32 6, i8* %"$bool.text")
  store i32 %call.snprintf36, i32* %format.size34, align 4
  %"$format.size37" = load i32, i32* %format.size34, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$c35", i32 6, i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size34, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size34, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  store i32* %a, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 9, i32* %"*r", align 4
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %ADD49 = add i32 %"call.Array<Array<i32>>.size", 1
  %"$a51" = load i32, i32* %a, align 4
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %"$a51", i32 %ADD49)
  store i32 %call.snprintf52, i32* %format.size50, align 4
  %"$format.size53" = load i32, i32* %format.size50, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %"$a51", i32 %ADD49)
  %"$format.size60" = load i32, i32* %format.size50, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size50, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %10 = insertvalue %String %9, i32 %"$format.size63", 1
  %11 = insertvalue %String %10, i32 %ADD61, 2
  call void @"io::out"(%String %11)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 132 — array: any integer type is an index, an unsigned one is zero-extended

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1

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
  %a = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %i = alloca i64, align 8
  %j = alloca i8, align 1
  %k = alloca i64, align 8
  %big = alloca i8, align 1
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [4 x i32] [i32 10, i32 20, i32 30, i32 40], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i64 2, i64* %i, align 8
  store i8 1, i8* %j, align 1
  store i64 0, i64* %k, align 8
  store i8 3, i8* %big, align 1
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i64, i64* %i, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i64 %"$i"
  %"$a1" = load i32*, i32** %a, align 8
  %"$j" = load i8, i8* %j, align 1
  %AS = zext i8 %"$j" to i64
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i64 %AS
  %"$a3" = load i32*, i32** %a, align 8
  %"$k" = load i64, i64* %k, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i64 %"$k"
  %"$a5" = load i32*, i32** %a, align 8
  %"$big" = load i8, i8* %big, align 1
  %AS6 = zext i8 %"$big" to i64
  %"a[]7" = getelementptr inbounds i32, i32* %"$a5", i64 %AS6
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]8" = load i32, i32* %"a[]2", align 4
  %"$a[]9" = load i32, i32* %"a[]4", align 4
  %"$a[]10" = load i32, i32* %"a[]7", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]8", i32 %"$a[]9", i32 %"$a[]10")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS11 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf12 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS11, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]8", i32 %"$a[]9", i32 %"$a[]10")
  %"$format.size13" = load i32, i32* %format.size, align 4
  %ADD14 = add i32 %"$format.size13", 1
  %"$format.text15" = load i8*, i8** %format.text, align 8
  %"$format.size16" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text15", 0
  %2 = insertvalue %String %1, i32 %"$format.size16", 1
  %3 = insertvalue %String %2, i32 %ADD14, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 134 — as: a struct can't be cast, operator = converts it

```stderr
error: test.ura:8:18 can't cast 'Point' to 'i8[]'
  |
8 |     raw i8[] = p as i8[]
  |                  ^^
help: define operator =(left &i8[], right &Point), then assign it
error: aborting due to 1 error
1
```

## 135 — ref: &T and T[] are the same pointer, each fits where the other is expected

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
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

declare i64 @read(i32, i8*, i64)

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

define i32 @first(i32* %0) {
entry:
  %values = alloca i32*, align 8
  store i32* %0, i32** %values, align 8
  %"*values" = load i32*, i32** %values, align 8
  %"$*values" = load i32, i32* %"*values", align 4
  ret i32 %"$*values"
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %line = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %numbers = alloca i32*, align 8
  %arr = alloca [2 x i32], align 4
  %format.size7 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  store i8 0, i8* %c, align 1
  store %String zeroinitializer, %String* %line, align 8
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %call.read = call i64 @read(i32 0, i8* %c, i64 1)
  %EQ = icmp eq i64 %call.read, 1
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 10
  %AND = and i1 %EQ, %NQ
  br i1 %AND, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$c1" = load i8, i8* %c, align 1
  call void @String.append(%String* %line, i8 %"$c1")
  br label %while7

endloop:                                          ; preds = %while7
  %call.String.chars = call i8* @String.chars(%String* %line)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  store [2 x i32] [i32 7, i32 8], [2 x i32]* %arr, align 4
  %3 = getelementptr inbounds [2 x i32], [2 x i32]* %arr, i32 0, i32 0
  store i32* %3, i32** %numbers, align 8
  %"$numbers" = load i32*, i32** %numbers, align 8
  %call.first = call i32 @first(i32* %"$numbers")
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %call.first)
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %call.first)
  %"$format.size16" = load i32, i32* %format.size7, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size7, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %5 = insertvalue %String %4, i32 %"$format.size19", 1
  %6 = insertvalue %String %5, i32 %ADD17, 2
  call void @"io::out"(%String %6)
  %7 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```
