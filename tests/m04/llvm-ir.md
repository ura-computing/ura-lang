# m04 llvm ir

## 001 — template: struct with a type parameter

```llvm
%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  store i32 7, i32* %b.value, align 4
  %b.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value1, align 4
  ret i32 %"$b.value"
}
```

## 002 — template: method that takes the type parameter

```llvm
%"Box<i32>" = type { i32 }

define void @"Box<i32>.set"(%"Box<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Box<i32>"*, align 8
  %v = alloca i32, align 4
  store %"Box<i32>"* %0, %"Box<i32>"** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %"Box<i32>"*, %"Box<i32>"** %self, align 8
  %"*self.value" = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %"*self", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.value", align 4
  ret void
}

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  call void @"Box<i32>.set"(%"Box<i32>"* %b, i32 20)
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value, align 4
  ret i32 %"$b.value"
}
```

## 003 — template: two type parameters

```llvm
%"Pair<i8,i32>" = type { i8, i32 }

define i32 @main() {
entry:
  %p = alloca %"Pair<i8,i32>", align 8
  store %"Pair<i8,i32>" zeroinitializer, %"Pair<i8,i32>"* %p, align 4
  %p.key = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 0
  store i8 122, i8* %p.key, align 1
  %p.value = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  store i32 5, i32* %p.value, align 4
  %p.value1 = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  %"$p.value" = load i32, i32* %p.value1, align 4
  ret i32 %"$p.value"
}
```

## 004 — template: two instances of the same template

```llvm
%"Box<i32>" = type { i32 }
%"Box<i8>" = type { i8 }

define i32 @main() {
entry:
  %a = alloca %"Box<i32>", align 8
  %b = alloca %"Box<i8>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %a, align 4
  store %"Box<i8>" zeroinitializer, %"Box<i8>"* %b, align 1
  %a.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  store i32 3, i32* %a.value, align 4
  %b.value = getelementptr inbounds %"Box<i8>", %"Box<i8>"* %b, i32 0, i32 0
  store i8 97, i8* %b.value, align 1
  %a.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  ret i32 %"$a.value"
}
```

## 005 — template: a template inside itself

```llvm
%"Box<Box<i32>>" = type { %"Box<i32>" }
%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %n = alloca %"Box<Box<i32>>", align 8
  store %"Box<Box<i32>>" zeroinitializer, %"Box<Box<i32>>"* %n, align 4
  %n.value = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value, i32 0, i32 0
  store i32 9, i32* %n.value.value, align 4
  %n.value1 = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value2 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value1, i32 0, i32 0
  %"$n.value.value" = load i32, i32* %n.value.value2, align 4
  ret i32 %"$n.value.value"
}
```

## 006 — template: proto inside a template keeps its symbol

```llvm
%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @"Vec<i32>.init"(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"*self" = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %"*self.arr" = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %"*self", i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 40)
  store i32* %call.malloc, i32** %"*self.arr", align 8
  ret void
}

define i32 @main() {
entry:
  %v = alloca %"Vec<i32>", align 8
  store %"Vec<i32>" zeroinitializer, %"Vec<i32>"* %v, align 8
  call void @"Vec<i32>.init"(%"Vec<i32>"* %v)
  ret i32 0
}
```

## 007 — template: function called with type arguments

```llvm
define i32 @"twice<i32>"(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %"$v" = load i32, i32* %v, align 4
  %"$v1" = load i32, i32* %v, align 4
  %ADD = add i32 %"$v", %"$v1"
  ret i32 %ADD
}

define i8 @"twice<i8>"(i8 %0) {
entry:
  %v = alloca i8, align 1
  store i8 %0, i8* %v, align 1
  %"$v" = load i8, i8* %v, align 1
  %"$v1" = load i8, i8* %v, align 1
  %ADD = add i8 %"$v", %"$v1"
  ret i8 %ADD
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i8, align 1
  %c = alloca i32, align 4
  %"call.twice<i32>" = call i32 @"twice<i32>"(i32 5)
  store i32 %"call.twice<i32>", i32* %a, align 4
  %"call.twice<i8>" = call i8 @"twice<i8>"(i8 3)
  store i8 %"call.twice<i8>", i8* %b, align 1
  store i32 0, i32* %c, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i8, i8* %b, align 1
  %EQ = icmp eq i8 %"$b", 6
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 1, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a" = load i32, i32* %a, align 4
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$a", %"$c"
  ret i32 %ADD
}
```

## 008 — template: sizeof of the type parameter

```llvm
%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @"Vec<i32>.init"(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"*self" = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %"*self.arr" = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %"*self", i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 mul (i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32), i32 10))
  store i32* %call.malloc, i32** %"*self.arr", align 8
  ret void
}

define i32 @main() {
entry:
  %x = alloca %"Vec<i32>", align 8
  store %"Vec<i32>" zeroinitializer, %"Vec<i32>"* %x, align 8
  call void @"Vec<i32>.init"(%"Vec<i32>"* %x)
  %x.arr = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %"$x.arr" = load i32*, i32** %x.arr, align 8
  %"x.arr[]" = getelementptr inbounds i32, i32* %"$x.arr", i32 3
  store i32 7, i32* %"x.arr[]", align 4
  %x.arr1 = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %"$x.arr2" = load i32*, i32** %x.arr1, align 8
  %"x.arr[]3" = getelementptr inbounds i32, i32* %"$x.arr2", i32 3
  %"$x.arr[]" = load i32, i32* %"x.arr[]3", align 4
  ret i32 %"$x.arr[]"
}
```

## 009 — templates: too many type arguments

```stderr
error: 'Box' takes 1 type argument, got 2
at test.ura:7:6
  |
7 |    b Box<i32, i8>
  |      ^^^
help: it is declared 'template<T>'
error: aborting due to 1 error
1
```

## 010 — templates: used without type arguments

```stderr
error: expected '<' after 'Box'
at test.ura:7:6
  |
7 |    b Box
  |      ^^^
error: aborting due to 1 error
1
```

## 011 — templates: parameter is not a name

```stderr
error: expected names after template
at test.ura:2:10
  |
2 | template<3>:
  |          ^
help: use template<T, V>
error: aborting due to 1 error
1
```

## 012 — templates: body holds something else

```stderr
error: expected struct/fn/operator after template
at test.ura:3:4
  |
3 |    value T
  |    ^^^^^
error: aborting due to 1 error
1
```

## 013 — templates: missing colon

```stderr
error: expected ':' after 'template'
at test.ura:2:11
  |
2 | template<T>
  |           ^
help: use template<T>:
error: aborting due to 1 error
1
```

## 014 — template: declared inside a function

```llvm
%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  store i32 4, i32* %b.value, align 4
  %b.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value1, align 4
  ret i32 %"$b.value"
}
```

## 015 — templates: used outside the scope it is declared in

```stderr
error: no template named 'Box'
at test.ura:9:7
  |
9 |     b Box<i32>
  |       ^^^
help: declare it: template<T>: struct Box:
error: aborting due to 1 error
1
```

## 016 — template: several functions in one template, each instance renames all of them; memory_alloc, memory_copy, memory_free

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%P = type { i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1
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

define i32* @"alloc<i32>"(i32 %0) {
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

define %P* @"alloc<P>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %P*
  ret %P* %AS1
}

define void @"free<P>"(%P* %0) {
entry:
  %data = alloca %P*, align 8
  store %P* %0, %P** %data, align 8
  %"$data" = load %P*, %P** %data, align 8
  %AS = bitcast %P* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %ps = alloca %P*, align 8
  %format.size24 = alloca i32, align 4
  %format.text29 = alloca i8*, align 8
  %"call.alloc<i32>" = call i32* @"alloc<i32>"(i32 3)
  store i32* %"call.alloc<i32>", i32** %a, align 8
  %"call.alloc<i32>1" = call i32* @"alloc<i32>"(i32 3)
  store i32* %"call.alloc<i32>1", i32** %b, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  store i32 10, i32* %"a[]", align 4
  %"$a2" = load i32*, i32** %a, align 8
  %"a[]3" = getelementptr inbounds i32, i32* %"$a2", i32 1
  store i32 20, i32* %"a[]3", align 4
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 2
  store i32 30, i32* %"a[]5", align 4
  %"$b" = load i32*, i32** %b, align 8
  %"$a6" = load i32*, i32** %a, align 8
  call void @"copy<i32>"(i32* %"$b", i32* %"$a6", i32 3)
  %"$b7" = load i32*, i32** %b, align 8
  %"b[]" = getelementptr inbounds i32, i32* %"$b7", i32 0
  %"$b8" = load i32*, i32** %b, align 8
  %"b[]9" = getelementptr inbounds i32, i32* %"$b8", i32 1
  %"$b10" = load i32*, i32** %b, align 8
  %"b[]11" = getelementptr inbounds i32, i32* %"$b10", i32 2
  %"$b[]" = load i32, i32* %"b[]", align 4
  %"$b[]12" = load i32, i32* %"b[]9", align 4
  %"$b[]13" = load i32, i32* %"b[]11", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$b[]", i32 %"$b[]12", i32 %"$b[]13")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$b[]", i32 %"$b[]12", i32 %"$b[]13")
  %"$format.size15" = load i32, i32* %format.size, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text, align 8
  %"$format.size18" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %1 = insertvalue %String %0, i32 %"$format.size18", 1
  %2 = insertvalue %String %1, i32 %ADD16, 2
  call void @"io::out"(%String %2)
  %"call.alloc<P>" = call %P* @"alloc<P>"(i32 2)
  store %P* %"call.alloc<P>", %P** %ps, align 8
  %"$ps" = load %P*, %P** %ps, align 8
  %"ps[]" = getelementptr inbounds %P, %P* %"$ps", i32 1
  %"ps[].y" = getelementptr inbounds %P, %P* %"ps[]", i32 0, i32 1
  store i32 7, i32* %"ps[].y", align 4
  %"$ps19" = load %P*, %P** %ps, align 8
  %"ps[]20" = getelementptr inbounds %P, %P* %"$ps19", i32 0
  %"ps[].x" = getelementptr inbounds %P, %P* %"ps[]20", i32 0, i32 0
  %"$ps21" = load %P*, %P** %ps, align 8
  %"ps[]22" = getelementptr inbounds %P, %P* %"$ps21", i32 1
  %"ps[].y23" = getelementptr inbounds %P, %P* %"ps[]22", i32 0, i32 1
  %"$ps[].x" = load i32, i32* %"ps[].x", align 4
  %"$ps[].y" = load i32, i32* %"ps[].y23", align 4
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$ps[].x", i32 %"$ps[].y")
  store i32 %call.snprintf25, i32* %format.size24, align 4
  %"$format.size26" = load i32, i32* %format.size24, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %AS28 = sext i32 %ADD27 to i64
  %call.calloc30 = call i8* @calloc(i64 %AS28, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc30, i8** %format.text29, align 8
  %"$format.text31" = load i8*, i8** %format.text29, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text31", i64 %AS28, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$ps[].x", i32 %"$ps[].y")
  %"$format.size33" = load i32, i32* %format.size24, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text29, align 8
  %"$format.size36" = load i32, i32* %format.size24, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %4 = insertvalue %String %3, i32 %"$format.size36", 1
  %5 = insertvalue %String %4, i32 %ADD34, 2
  call void @"io::out"(%String %5)
  %"$a37" = load i32*, i32** %a, align 8
  call void @"free<i32>"(i32* %"$a37")
  %"$b38" = load i32*, i32** %b, align 8
  call void @"free<i32>"(i32* %"$b38")
  %"$ps39" = load %P*, %P** %ps, align 8
  call void @"free<P>"(%P* %"$ps39")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 017 — template: an array type as the type argument, sizeof(T) with T = char[]

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"b\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

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

define i8** @"new<i8[]>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i8**
  ret i8** %AS1
}

define void @"free<i8[]>"(i8** %0) {
entry:
  %data = alloca i8**, align 8
  store i8** %0, i8*** %data, align 8
  %"$data" = load i8**, i8*** %data, align 8
  %AS = bitcast i8** %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %rows = alloca i8**, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %"call.new<i8[]>" = call i8** @"new<i8[]>"(i32 2)
  store i8** %"call.new<i8[]>", i8*** %rows, align 8
  %"$rows" = load i8**, i8*** %rows, align 8
  %"rows[]" = getelementptr inbounds i8*, i8** %"$rows", i32 0
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8** %"rows[]", align 8
  %"$rows1" = load i8**, i8*** %rows, align 8
  %"rows[]2" = getelementptr inbounds i8*, i8** %"$rows1", i32 1
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i8** %"rows[]2", align 8
  %"$rows3" = load i8**, i8*** %rows, align 8
  %"rows[]4" = getelementptr inbounds i8*, i8** %"$rows3", i32 0
  %"$rows5" = load i8**, i8*** %rows, align 8
  %"rows[]6" = getelementptr inbounds i8*, i8** %"$rows5", i32 1
  %"$rows[]" = load i8*, i8** %"rows[]4", align 8
  %"$rows[]7" = load i8*, i8** %"rows[]6", align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %"$rows[]", i8* %"$rows[]7")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %"$rows[]", i8* %"$rows[]7")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  %"$rows13" = load i8**, i8*** %rows, align 8
  call void @"free<i8[]>"(i8** %"$rows13")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

