# m03 llvm ir

## 001 — global scope: no main: the file is the program

```llvm
%Finder = type { i32*, i32 }

@f = global %Finder zeroinitializer

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*self" = load %Finder*, %Finder** %self, align 8
  %"*self.size" = getelementptr inbounds %Finder, %Finder* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %EQ = icmp eq i32 %"$i", %"$*self.size"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"*self2" = load %Finder*, %Finder** %self, align 8
  %"*self.data" = getelementptr inbounds %Finder, %Finder* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ4 = icmp eq i32 %"$*self.data[]", %"$v"
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  ret i32 %"$i5"

endif1:                                           ; preds = %if2
  %"$i6" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i6", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %arr = alloca [6 x i32], align 4
  store [6 x i32] [i32 4, i32 8, i32 15, i32 16, i32 23, i32 42], [6 x i32]* %arr, align 4
  %0 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 0), align 8
  store i32 6, i32* getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 1), align 4
  %call.Finder.index_of = call i32 @Finder.index_of(%Finder* @f, i32 23)
  %MUL = mul i32 %call.Finder.index_of, 10
  %call.Finder.index_of1 = call i32 @Finder.index_of(%Finder* @f, i32 4)
  %ADD = add i32 %MUL, %call.Finder.index_of1
  %call.Finder.index_of2 = call i32 @Finder.index_of(%Finder* @f, i32 99)
  %ADD3 = add i32 %call.Finder.index_of2, 1
  %MUL4 = mul i32 %ADD3, 100
  %ADD5 = add i32 %ADD, %MUL4
  ret i32 %ADD5
}
```

## 002 — struct: declare: attributes start at zero

```llvm
%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.a" = load i32, i32* %u.a, align 4
  %"$u.b" = load i32, i32* %u.b, align 4
  %ADD = add i32 %"$u.a", %"$u.b"
  ret i32 %ADD
}
```

## 003 — struct: write an attribute, read it back

```llvm
%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %u.b, align 4
  %u.b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b1, align 4
  ret i32 %"$u.b"
}
```

## 004 — struct: attributes don't overlap

```llvm
%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 4, i32* %u.b, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %u.b2 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b2, align 4
  %ADD = add i32 %MUL, %"$u.b"
  ret i32 %ADD
}
```

## 005 — struct: compound assignment on an attribute

```llvm
%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$u.a", 2
  store i32 %ADD, i32* %u.a1, align 4
  %u.a2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a3" = load i32, i32* %u.a2, align 4
  %MUL = mul i32 %"$u.a3", 3
  store i32 %MUL, i32* %u.a2, align 4
  %u.a4 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a5" = load i32, i32* %u.a4, align 4
  ret i32 %"$u.a5"
}
```

## 006 — struct: two variables of one struct are separate

```llvm
%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %v = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User zeroinitializer, %User* %v, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 1, i32* %u.a, align 4
  %v.a = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  store i32 2, i32* %v.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %v.a2 = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  %"$v.a" = load i32, i32* %v.a2, align 4
  %ADD = add i32 %MUL, %"$v.a"
  ret i32 %ADD
}
```

## 007 — struct: attributes of different types

```llvm
%Item = type { i32, i1, i8 }

define i32 @main() {
entry:
  %it = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %it, align 4
  %it.id = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  store i32 4, i32* %it.id, align 4
  %it.used = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  store i1 true, i1* %it.used, align 1
  %it.tag = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  store i8 120, i8* %it.tag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %it.used1 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  %it.tag2 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  %"$it.tag" = load i8, i8* %it.tag2, align 1
  %EQ = icmp eq i8 %"$it.tag", 120
  %"$it.used" = load i1, i1* %it.used1, align 1
  %AND = and i1 %"$it.used", %EQ
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %it.id3 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %"$it.id" = load i32, i32* %it.id3, align 4
  ret i32 %"$it.id"

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 008 — struct: nested struct: r.b.x

```llvm
%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x = getelementptr inbounds %Point, %Point* %r.a, i32 0, i32 0
  store i32 1, i32* %r.a.x, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 5, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y = getelementptr inbounds %Point, %Point* %r.b1, i32 0, i32 1
  store i32 2, i32* %r.b.y, align 4
  %r.b2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x3 = getelementptr inbounds %Point, %Point* %r.b2, i32 0, i32 0
  %r.a4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x5 = getelementptr inbounds %Point, %Point* %r.a4, i32 0, i32 0
  %"$r.b.x" = load i32, i32* %r.b.x3, align 4
  %"$r.a.x" = load i32, i32* %r.a.x5, align 4
  %SUB = sub i32 %"$r.b.x", %"$r.a.x"
  %r.b6 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y7 = getelementptr inbounds %Point, %Point* %r.b6, i32 0, i32 1
  %"$r.b.y" = load i32, i32* %r.b.y7, align 4
  %ADD = add i32 %SUB, %"$r.b.y"
  ret i32 %ADD
}
```

## 009 — struct: attributes in a loop

```llvm
%Counter = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %c, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %LT = icmp slt i32 %"$c.n", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n2" = load i32, i32* %c.n1, align 4
  %ADD = add i32 %"$c.n2", 1
  store i32 %ADD, i32* %c.n1, align 4
  %c.total = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %c.n3 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n4" = load i32, i32* %c.n3, align 4
  %"$c.total" = load i32, i32* %c.total, align 4
  %ADD5 = add i32 %"$c.total", %"$c.n4"
  store i32 %ADD5, i32* %c.total, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.total6 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %"$c.total7" = load i32, i32* %c.total6, align 4
  ret i32 %"$c.total7"
}
```

## 010 — struct: array attribute

```llvm
%Row = type { i32* }

define i32 @main() {
entry:
  %r = alloca %Row, align 8
  %arr = alloca [3 x i32], align 4
  store %Row zeroinitializer, %Row* %r, align 8
  %r.cells = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %r.cells, align 8
  %r.cells1 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells" = load i32*, i32** %r.cells1, align 8
  %"r.cells[]" = getelementptr inbounds i32, i32* %"$r.cells", i32 0
  store i32 9, i32* %"r.cells[]", align 4
  %r.cells2 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells3" = load i32*, i32** %r.cells2, align 8
  %"r.cells[]4" = getelementptr inbounds i32, i32* %"$r.cells3", i32 0
  %r.cells5 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells6" = load i32*, i32** %r.cells5, align 8
  %"r.cells[]7" = getelementptr inbounds i32, i32* %"$r.cells6", i32 2
  %"$r.cells[]" = load i32, i32* %"r.cells[]4", align 4
  %"$r.cells[]8" = load i32, i32* %"r.cells[]7", align 4
  %ADD = add i32 %"$r.cells[]", %"$r.cells[]8"
  ret i32 %ADD
}
```

## 011 — struct: print attributes

```llvm
%User = type { i32, i8* }

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 8
  %u.id = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 42, i32* %u.id, align 4
  %u.name = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %u.name, align 8
  %u.name1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %u.id2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.name" = load i8*, i8** %u.name1, align 8
  %"$u.id" = load i32, i32* %u.id2, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %"$u.name", i32 %"$u.id")
  ret i32 0
}
```

## 012 — struct: a function returns a struct

```llvm
%User = type { i32 }

define %User @make(i32 %0) {
entry:
  %a = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 %0, i32* %a, align 4
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %u.a, align 4
  %"$u" = load %User, %User* %u, align 4
  ret %User %"$u"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %call.make = call %User @make(i32 5)
  store %User %call.make, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

## 013 — struct: a ref variable to a struct

```llvm
%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User* %u, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 8, i32* %"*p.a", align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

## 014 — struct: write through a ref parameter

```llvm
%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 3, i32* %"*p.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

## 015 — struct: read through a ref parameter

```llvm
%User = type { i32, i32 }

define i32 @total(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  %"*p1" = load %User*, %User** %p, align 8
  %"*p.b" = getelementptr inbounds %User, %User* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %ADD = add i32 %"$*p.a", %"$*p.b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 2, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 5, i32* %u.b, align 4
  %call.total = call i32 @total(%User* %u)
  ret i32 %call.total
}
```

## 016 — method: a method reads self

```llvm
%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.User.get = call i32 @User.get(%User* %u)
  ret i32 %call.User.get
}
```

## 017 — method: a method writes self

```llvm
%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u, i32 9)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

## 018 — method: a method reads and writes self

```llvm
%User = type { i32 }

define i32 @User.bump(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %ADD = add i32 %"$*self.a", 1
  store i32 %ADD, i32* %"*self.a", align 4
  %"*self1" = load %User*, %User** %self, align 8
  %"*self.a2" = getelementptr inbounds %User, %User* %"*self1", i32 0, i32 0
  %"$*self.a3" = load i32, i32* %"*self.a2", align 4
  ret i32 %"$*self.a3"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.bump = call i32 @User.bump(%User* %u)
  %call.User.bump1 = call i32 @User.bump(%User* %u)
  ret i32 %call.User.bump1
}
```

## 019 — method: a method with arguments

```llvm
%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.a", %"$n"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.add = call i32 @User.add(%User* %u, i32 3)
  ret i32 %call.User.add
}
```

## 020 — method: a method calls another method

```llvm
%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @User.twice(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %call.User.get = call i32 @User.get(%User* %"*self")
  %MUL = mul i32 %call.User.get, 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 6, i32* %u.a, align 4
  %call.User.twice = call i32 @User.twice(%User* %u)
  ret i32 %call.User.twice
}
```

## 021 — method: a method returns a bool

```llvm
%User = type { i32 }

define i1 @User.adult(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.age" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.age" = load i32, i32* %"*self.age", align 4
  %GE = icmp sge i32 %"$*self.age", 18
  ret i1 %GE
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.age = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 20, i32* %u.age, align 4
  br label %if0

if0:                                              ; preds = %entry
  %call.User.adult = call i1 @User.adult(%User* %u)
  br i1 %call.User.adult, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

## 022 — method: a method in a loop

```llvm
%Counter = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @Counter.tick(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", 1
  store i32 %ADD, i32* %"*self.n", align 4
  ret void
}

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @Counter.tick(%Counter* %c)
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$c.n")
  %c.n2 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n3" = load i32, i32* %c.n2, align 4
  ret i32 %"$c.n3"
}
```

## 023 — method: two structs, one method name

```llvm
%A = type { i32 }
%B = type { i32 }

define i32 @A.get(%A* %0) {
entry:
  %self = alloca %A*, align 8
  store %A* %0, %A** %self, align 8
  %"*self" = load %A*, %A** %self, align 8
  %"*self.v" = getelementptr inbounds %A, %A* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  ret i32 %"$*self.v"
}

define i32 @B.get(%B* %0) {
entry:
  %self = alloca %B*, align 8
  store %B* %0, %B** %self, align 8
  %"*self" = load %B*, %B** %self, align 8
  %"*self.v" = getelementptr inbounds %B, %B* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  %MUL = mul i32 %"$*self.v", 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca %A, align 8
  %b = alloca %B, align 8
  store %A zeroinitializer, %A* %a, align 4
  store %B zeroinitializer, %B* %b, align 4
  %a.v = getelementptr inbounds %A, %A* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  %b.v = getelementptr inbounds %B, %B* %b, i32 0, i32 0
  store i32 2, i32* %b.v, align 4
  %call.A.get = call i32 @A.get(%A* %a)
  %call.B.get = call i32 @B.get(%B* %b)
  %ADD = add i32 %call.A.get, %call.B.get
  ret i32 %ADD
}
```

## 024 — method: a method on a nested attribute

```llvm
%Point = type { i32 }
%Rect = type { %Point, %Point }

define i32 @Point.get(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  ret i32 %"$*self.x"
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 7, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %call.Point.get = call i32 @Point.get(%Point* %r.b1)
  ret i32 %call.Point.get
}
```

## 025 — method: overloaded method

```llvm
%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  store i32 1, i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %first = alloca i32, align 4
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set.1(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  store i32 %"$u.a", i32* %first, align 4
  call void @User.set(%User* %u, i32 5)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$first" = load i32, i32* %first, align 4
  %"$u.a2" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$first", %"$u.a2"
  ret i32 %ADD
}
```

## 026 — method: a method on a ref receiver

```llvm
%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %call.User.get = call i32 @User.get(%User* %"*p")
  ret i32 %call.User.get
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.via = call i32 @via(%User* %u)
  ret i32 %call.via
}
```

## 027 — method: a method through a ref changes the caller

```llvm
%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @reset(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  call void @User.set(%User* %"*p", i32 11)
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  call void @reset(%User* %u)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  ret i32 %"$u.a"
}
```

## 028 — crazy: a method called three times through a ref

```llvm
%Pair = type { i32, i32 }

define void @Pair.swap(%Pair* %0) {
entry:
  %self = alloca %Pair*, align 8
  %t = alloca i32, align 4
  store %Pair* %0, %Pair** %self, align 8
  %"*self" = load %Pair*, %Pair** %self, align 8
  %"*self.a" = getelementptr inbounds %Pair, %Pair* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  store i32 %"$*self.a", i32* %t, align 4
  %"*self1" = load %Pair*, %Pair** %self, align 8
  %"*self.a2" = getelementptr inbounds %Pair, %Pair* %"*self1", i32 0, i32 0
  %"*self3" = load %Pair*, %Pair** %self, align 8
  %"*self.b" = getelementptr inbounds %Pair, %Pair* %"*self3", i32 0, i32 1
  %"$*self.b" = load i32, i32* %"*self.b", align 4
  store i32 %"$*self.b", i32* %"*self.a2", align 4
  %"*self4" = load %Pair*, %Pair** %self, align 8
  %"*self.b5" = getelementptr inbounds %Pair, %Pair* %"*self4", i32 0, i32 1
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*self.b5", align 4
  ret void
}

define void @thrice(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  %"*p" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p")
  %"*p1" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p1")
  %"*p2" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p2")
  ret void
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 1, i32* %x.a, align 4
  %x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 2, i32* %x.b, align 4
  call void @thrice(%Pair* %x)
  %x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %"$x.a" = load i32, i32* %x.a1, align 4
  %MUL = mul i32 %"$x.a", 10
  %x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %"$x.b" = load i32, i32* %x.b2, align 4
  %ADD = add i32 %MUL, %"$x.b"
  ret i32 %ADD
}
```

## 029 — crazy: methods of a nested struct, called through self

```llvm
%Point = type { i32, i32 }
%Rect = type { %Point, %Point }

define void @Point.move(%Point* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Point*, align 8
  %dx = alloca i32, align 4
  %dy = alloca i32, align 4
  store %Point* %0, %Point** %self, align 8
  store i32 %1, i32* %dx, align 4
  store i32 %2, i32* %dy, align 4
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"$dx" = load i32, i32* %dx, align 4
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %ADD = add i32 %"$*self.x", %"$dx"
  store i32 %ADD, i32* %"*self.x", align 4
  %"*self1" = load %Point*, %Point** %self, align 8
  %"*self.y" = getelementptr inbounds %Point, %Point* %"*self1", i32 0, i32 1
  %"$dy" = load i32, i32* %dy, align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %ADD2 = add i32 %"$*self.y", %"$dy"
  store i32 %ADD2, i32* %"*self.y", align 4
  ret void
}

define void @Rect.shift(%Rect* %0, i32 %1) {
entry:
  %self = alloca %Rect*, align 8
  %d = alloca i32, align 4
  store %Rect* %0, %Rect** %self, align 8
  store i32 %1, i32* %d, align 4
  %"*self" = load %Rect*, %Rect** %self, align 8
  %"*self.a" = getelementptr inbounds %Rect, %Rect* %"*self", i32 0, i32 0
  %"$d" = load i32, i32* %d, align 4
  %"$d1" = load i32, i32* %d, align 4
  call void @Point.move(%Point* %"*self.a", i32 %"$d", i32 %"$d1")
  %"*self2" = load %Rect*, %Rect** %self, align 8
  %"*self.b" = getelementptr inbounds %Rect, %Rect* %"*self2", i32 0, i32 1
  %"$d3" = load i32, i32* %d, align 4
  %"$d4" = load i32, i32* %d, align 4
  call void @Point.move(%Point* %"*self.b", i32 %"$d3", i32 %"$d4")
  ret void
}

define i32 @Rect.area(%Rect* %0) {
entry:
  %self = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %self, align 8
  %"*self" = load %Rect*, %Rect** %self, align 8
  %"*self.b" = getelementptr inbounds %Rect, %Rect* %"*self", i32 0, i32 1
  %"*self.b.x" = getelementptr inbounds %Point, %Point* %"*self.b", i32 0, i32 0
  %"*self1" = load %Rect*, %Rect** %self, align 8
  %"*self.a" = getelementptr inbounds %Rect, %Rect* %"*self1", i32 0, i32 0
  %"*self.a.x" = getelementptr inbounds %Point, %Point* %"*self.a", i32 0, i32 0
  %"$*self.b.x" = load i32, i32* %"*self.b.x", align 4
  %"$*self.a.x" = load i32, i32* %"*self.a.x", align 4
  %SUB = sub i32 %"$*self.b.x", %"$*self.a.x"
  %"*self2" = load %Rect*, %Rect** %self, align 8
  %"*self.b3" = getelementptr inbounds %Rect, %Rect* %"*self2", i32 0, i32 1
  %"*self.b.y" = getelementptr inbounds %Point, %Point* %"*self.b3", i32 0, i32 1
  %"*self4" = load %Rect*, %Rect** %self, align 8
  %"*self.a5" = getelementptr inbounds %Rect, %Rect* %"*self4", i32 0, i32 0
  %"*self.a.y" = getelementptr inbounds %Point, %Point* %"*self.a5", i32 0, i32 1
  %"$*self.b.y" = load i32, i32* %"*self.b.y", align 4
  %"$*self.a.y" = load i32, i32* %"*self.a.y", align 4
  %SUB6 = sub i32 %"$*self.b.y", %"$*self.a.y"
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  call void @Point.move(%Point* %r.b, i32 4, i32 5)
  call void @Rect.shift(%Rect* %r, i32 10)
  %call.Rect.area = call i32 @Rect.area(%Rect* %r)
  %r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x = getelementptr inbounds %Point, %Point* %r.a, i32 0, i32 0
  %"$r.a.x" = load i32, i32* %r.a.x, align 4
  %ADD = add i32 %call.Rect.area, %"$r.a.x"
  ret i32 %ADD
}
```

## 030 — crazy: bubble sort: a struct with an array, methods calling methods

```llvm
%List = type { i32*, i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @List.swap(%List* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 %1, i32* %i, align 4
  store i32 %2, i32* %j, align 4
  %"*self" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$i" = load i32, i32* %i, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$i"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  store i32 %"$*self.items[]", i32* %t, align 4
  %"*self1" = load %List*, %List** %self, align 8
  %"*self.items2" = getelementptr inbounds %List, %List* %"*self1", i32 0, i32 0
  %"$*self.items3" = load i32*, i32** %"*self.items2", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.items[]5" = getelementptr inbounds i32, i32* %"$*self.items3", i32 %"$i4"
  %"*self6" = load %List*, %List** %self, align 8
  %"*self.items7" = getelementptr inbounds %List, %List* %"*self6", i32 0, i32 0
  %"$*self.items8" = load i32*, i32** %"*self.items7", align 8
  %"$j" = load i32, i32* %j, align 4
  %"*self.items[]9" = getelementptr inbounds i32, i32* %"$*self.items8", i32 %"$j"
  %"$*self.items[]10" = load i32, i32* %"*self.items[]9", align 4
  store i32 %"$*self.items[]10", i32* %"*self.items[]5", align 4
  %"*self11" = load %List*, %List** %self, align 8
  %"*self.items12" = getelementptr inbounds %List, %List* %"*self11", i32 0, i32 0
  %"$*self.items13" = load i32*, i32** %"*self.items12", align 8
  %"$j14" = load i32, i32* %j, align 4
  %"*self.items[]15" = getelementptr inbounds i32, i32* %"$*self.items13", i32 %"$j14"
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*self.items[]15", align 4
  ret void
}

define void @List.sort(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"*self" = load %List*, %List** %self, align 8
  %"*self.size" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %LT = icmp slt i32 %"$i", %"$*self.size"
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"*self2" = load %List*, %List** %self, align 8
  %"*self.size3" = getelementptr inbounds %List, %List* %"*self2", i32 0, i32 1
  %"$*self.size4" = load i32, i32* %"*self.size3", align 4
  %SUB = sub i32 %"$*self.size4", 1
  %"$i5" = load i32, i32* %i, align 4
  %SUB6 = sub i32 %SUB, %"$i5"
  %"$j" = load i32, i32* %j, align 4
  %LT7 = icmp slt i32 %"$j", %SUB6
  br i1 %LT7, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"*self8" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self8", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$j9" = load i32, i32* %j, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$j9"
  %"*self10" = load %List*, %List** %self, align 8
  %"*self.items11" = getelementptr inbounds %List, %List* %"*self10", i32 0, i32 0
  %"$j12" = load i32, i32* %j, align 4
  %ADD = add i32 %"$j12", 1
  %"$*self.items13" = load i32*, i32** %"*self.items11", align 8
  %"*self.items[]14" = getelementptr inbounds i32, i32* %"$*self.items13", i32 %ADD
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  %"$*self.items[]15" = load i32, i32* %"*self.items[]14", align 4
  %GT = icmp sgt i32 %"$*self.items[]", %"$*self.items[]15"
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self16" = load %List*, %List** %self, align 8
  %"$j17" = load i32, i32* %j, align 4
  %ADD18 = add i32 %"$j17", 1
  %"$j19" = load i32, i32* %j, align 4
  call void @List.swap(%List* %"*self16", i32 %"$j19", i32 %ADD18)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$j20" = load i32, i32* %j, align 4
  %ADD21 = add i32 %"$j20", 1
  store i32 %ADD21, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i22" = load i32, i32* %i, align 4
  %ADD23 = add i32 %"$i22", 1
  store i32 %ADD23, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  ret void
}

define void @List.show(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %endif, %entry
  %"*self" = load %List*, %List** %self, align 8
  %"*self.size" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %LT = icmp slt i32 %"$i", %"$*self.size"
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  br label %if4

if4:                                              ; preds = %while3.then
  %"$i1" = load i32, i32* %i, align 4
  %GT = icmp sgt i32 %"$i1", 0
  br i1 %GT, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  %"*self2" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self2", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$i3"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.items[]")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %call.printf6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %l = alloca %List, align 8
  %arr = alloca [5 x i32], align 4
  store %List zeroinitializer, %List* %l, align 8
  %l.items = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  store [5 x i32] [i32 5, i32 3, i32 9, i32 1, i32 4], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %l.items, align 8
  %l.size = getelementptr inbounds %List, %List* %l, i32 0, i32 1
  store i32 5, i32* %l.size, align 4
  call void @List.sort(%List* %l)
  call void @List.show(%List* %l)
  %l.items1 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %"$l.items" = load i32*, i32** %l.items1, align 8
  %"l.items[]" = getelementptr inbounds i32, i32* %"$l.items", i32 0
  %"$l.items[]" = load i32, i32* %"l.items[]", align 4
  %MUL = mul i32 %"$l.items[]", 10
  %l.items2 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %"$l.items3" = load i32*, i32** %l.items2, align 8
  %"l.items[]4" = getelementptr inbounds i32, i32* %"$l.items3", i32 4
  %"$l.items[]5" = load i32, i32* %"l.items[]4", align 4
  %ADD = add i32 %MUL, %"$l.items[]5"
  ret i32 %ADD
}
```

## 031 — crazy: bank account: methods, a ref parameter, an array of bills

```llvm
%Account = type { i32, i32 }

@str = private unnamed_addr constant [31 x i8] c"balance %d, ops %d, failed %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @Account.deposit(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Account*, %Account** %self, align 8
  %"*self.balance" = getelementptr inbounds %Account, %Account* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  %"$*self.balance" = load i32, i32* %"*self.balance", align 4
  %ADD = add i32 %"$*self.balance", %"$n"
  store i32 %ADD, i32* %"*self.balance", align 4
  %"*self1" = load %Account*, %Account** %self, align 8
  %"*self.ops" = getelementptr inbounds %Account, %Account* %"*self1", i32 0, i32 1
  %"$*self.ops" = load i32, i32* %"*self.ops", align 4
  %ADD2 = add i32 %"$*self.ops", 1
  store i32 %ADD2, i32* %"*self.ops", align 4
  ret void
}

define i1 @Account.withdraw(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Account*, %Account** %self, align 8
  %"*self.balance" = getelementptr inbounds %Account, %Account* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  %"$*self.balance" = load i32, i32* %"*self.balance", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.balance"
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  %"*self1" = load %Account*, %Account** %self, align 8
  %"*self.balance2" = getelementptr inbounds %Account, %Account* %"*self1", i32 0, i32 0
  %"$n3" = load i32, i32* %n, align 4
  %"$*self.balance4" = load i32, i32* %"*self.balance2", align 4
  %SUB = sub i32 %"$*self.balance4", %"$n3"
  store i32 %SUB, i32* %"*self.balance2", align 4
  %"*self5" = load %Account*, %Account** %self, align 8
  %"*self.ops" = getelementptr inbounds %Account, %Account* %"*self5", i32 0, i32 1
  %"$*self.ops" = load i32, i32* %"*self.ops", align 4
  %ADD = add i32 %"$*self.ops", 1
  store i32 %ADD, i32* %"*self.ops", align 4
  ret i1 true
}

define i32 @pay_all(%Account* %0, i32* %1, i32 %2) {
entry:
  %acc = alloca %Account*, align 8
  %bills = alloca i32*, align 8
  %count = alloca i32, align 4
  %failed = alloca i32, align 4
  %i = alloca i32, align 4
  store %Account* %0, %Account** %acc, align 8
  store i32* %1, i32** %bills, align 8
  store i32 %2, i32* %count, align 4
  store i32 0, i32* %failed, align 4
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$count" = load i32, i32* %count, align 4
  %LT = icmp slt i32 %"$i", %"$count"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"*acc" = load %Account*, %Account** %acc, align 8
  %"$bills" = load i32*, i32** %bills, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"bills[]" = getelementptr inbounds i32, i32* %"$bills", i32 %"$i1"
  %"$bills[]" = load i32, i32* %"bills[]", align 4
  %call.Account.withdraw = call i1 @Account.withdraw(%Account* %"*acc", i32 %"$bills[]")
  %EQ = icmp eq i1 %call.Account.withdraw, false
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$failed" = load i32, i32* %failed, align 4
  %ADD = add i32 %"$failed", 1
  store i32 %ADD, i32* %failed, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$failed4" = load i32, i32* %failed, align 4
  ret i32 %"$failed4"
}

define i32 @main() {
entry:
  %a = alloca %Account, align 8
  %bills = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %failed = alloca i32, align 4
  store %Account zeroinitializer, %Account* %a, align 4
  call void @Account.deposit(%Account* %a, i32 100)
  store [4 x i32] [i32 30, i32 50, i32 40, i32 10], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %bills, align 8
  %"$bills" = load i32*, i32** %bills, align 8
  %call.pay_all = call i32 @pay_all(%Account* %a, i32* %"$bills", i32 4)
  store i32 %call.pay_all, i32* %failed, align 4
  %a.balance = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %a.ops = getelementptr inbounds %Account, %Account* %a, i32 0, i32 1
  %"$a.balance" = load i32, i32* %a.balance, align 4
  %"$a.ops" = load i32, i32* %a.ops, align 4
  %"$failed" = load i32, i32* %failed, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i32 %"$a.balance", i32 %"$a.ops", i32 %"$failed")
  %a.balance1 = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %"$a.balance2" = load i32, i32* %a.balance1, align 4
  ret i32 %"$a.balance2"
}
```

## 032 — crazy: an array of structs is a copy

```llvm
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %ps = alloca %Point*, align 8
  %arr = alloca [2 x %Point], align 8
  store %Point zeroinitializer, %Point* %p, align 4
  store %Point zeroinitializer, %Point* %q, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %p.x, align 4
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  store i32 2, i32* %q.x, align 4
  %"$p" = load %Point, %Point* %p, align 4
  %"$q" = load %Point, %Point* %q, align 4
  %0 = insertvalue [2 x %Point] undef, %Point %"$p", 0
  %1 = insertvalue [2 x %Point] %0, %Point %"$q", 1
  store [2 x %Point] %1, [2 x %Point]* %arr, align 4
  %2 = getelementptr inbounds [2 x %Point], [2 x %Point]* %arr, i32 0, i32 0
  store %Point* %2, %Point** %ps, align 8
  %"$ps" = load %Point*, %Point** %ps, align 8
  %"ps[]" = getelementptr inbounds %Point, %Point* %"$ps", i32 1
  %"ps[].x" = getelementptr inbounds %Point, %Point* %"ps[]", i32 0, i32 0
  store i32 7, i32* %"ps[].x", align 4
  %"$ps1" = load %Point*, %Point** %ps, align 8
  %"ps[]2" = getelementptr inbounds %Point, %Point* %"$ps1", i32 0
  %"ps[].x3" = getelementptr inbounds %Point, %Point* %"ps[]2", i32 0, i32 0
  %"$ps4" = load %Point*, %Point** %ps, align 8
  %"ps[]5" = getelementptr inbounds %Point, %Point* %"$ps4", i32 1
  %"ps[].x6" = getelementptr inbounds %Point, %Point* %"ps[]5", i32 0, i32 0
  %"$ps[].x" = load i32, i32* %"ps[].x3", align 4
  %"$ps[].x7" = load i32, i32* %"ps[].x6", align 4
  %ADD = add i32 %"$ps[].x", %"$ps[].x7"
  %q.x8 = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$q.x" = load i32, i32* %q.x8, align 4
  %ADD9 = add i32 %ADD, %"$q.x"
  ret i32 %ADD9
}
```

## 033 — crazy: vectors: struct parameters, struct returns, chained calls

```llvm
%Vec = type { i32, i32 }

define %Vec @Vec.add(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  %r = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  store %Vec zeroinitializer, %Vec* %r, align 4
  %r.x = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 0
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$o.x" = load i32, i32* %o.x, align 4
  %ADD = add i32 %"$*self.x", %"$o.x"
  store i32 %ADD, i32* %r.x, align 4
  %r.y = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 1
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$o.y" = load i32, i32* %o.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$o.y"
  store i32 %ADD2, i32* %r.y, align 4
  %"$r" = load %Vec, %Vec* %r, align 4
  ret %Vec %"$r"
}

define i32 @Vec.dot(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$o.x" = load i32, i32* %o.x, align 4
  %MUL = mul i32 %"$*self.x", %"$o.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$o.y" = load i32, i32* %o.y, align 4
  %MUL2 = mul i32 %"$*self.y", %"$o.y"
  %ADD = add i32 %MUL, %MUL2
  ret i32 %ADD
}

define %Vec @make(i32 %0, i32 %1) {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %x, align 4
  store i32 %1, i32* %y, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$x" = load i32, i32* %x, align 4
  store i32 %"$x", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$y" = load i32, i32* %y, align 4
  store i32 %"$y", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %call.make = call %Vec @make(i32 1, i32 2)
  store %Vec %call.make, %Vec* %a, align 4
  %call.make1 = call %Vec @make(i32 3, i32 4)
  store %Vec %call.make1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %call.Vec.add = call %Vec @Vec.add(%Vec* %a, %Vec %"$b")
  store %Vec %call.Vec.add, %Vec* %tmp, align 4
  %"$b2" = load %Vec, %Vec* %b, align 4
  %call.Vec.add3 = call %Vec @Vec.add(%Vec* %tmp, %Vec %"$b2")
  store %Vec %call.Vec.add3, %Vec* %c, align 4
  %"$a" = load %Vec, %Vec* %a, align 4
  %call.Vec.dot = call i32 @Vec.dot(%Vec* %c, %Vec %"$a")
  ret i32 %call.Vec.dot
}
```

## 034 — crazy: linked list: ref attributes, isNullable, rebinding a ref in a loop

```llvm
%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %b = alloca %Node, align 8
  %a = alloca %Node, align 8
  %sum = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %"$p" = load %Node*, %Node** %p, align 8
  %EQ = icmp eq %Node* %"$p", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*p.value"
  store i32 %ADD, i32* %sum, align 4
  %"*p2" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p2", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %a.next3 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next3, align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.value" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 0
  %"$**a.next.next.value" = load i32, i32* %"**a.next.next.value", align 4
  %MUL = mul i32 %"$**a.next.next.value", 10
  %"$sum4" = load i32, i32* %sum, align 4
  %ADD5 = add i32 %"$sum4", %MUL
  ret i32 %ADD5
}
```

## 035 — crazy: a recursive method that counts its calls

```llvm
%Math = type { i32 }

define i32 @Math.fact(%Math* %0, i32 %1) {
entry:
  %self = alloca %Math*, align 8
  %n = alloca i32, align 4
  store %Math* %0, %Math** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Math*, %Math** %self, align 8
  %"*self.calls" = getelementptr inbounds %Math, %Math* %"*self", i32 0, i32 0
  %"$*self.calls" = load i32, i32* %"*self.calls", align 4
  %ADD = add i32 %"$*self.calls", 1
  store i32 %ADD, i32* %"*self.calls", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$n", 1
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"*self1" = load %Math*, %Math** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  %call.Math.fact = call i32 @Math.fact(%Math* %"*self1", i32 %SUB)
  %"$n3" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n3", %call.Math.fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %m = alloca %Math, align 8
  %r = alloca i32, align 4
  store %Math zeroinitializer, %Math* %m, align 4
  %call.Math.fact = call i32 @Math.fact(%Math* %m, i32 5)
  store i32 %call.Math.fact, i32* %r, align 4
  %m.calls = getelementptr inbounds %Math, %Math* %m, i32 0, i32 0
  %"$r" = load i32, i32* %r, align 4
  %"$m.calls" = load i32, i32* %m.calls, align 4
  %ADD = add i32 %"$r", %"$m.calls"
  ret i32 %ADD
}
```

## 036 — crazy: a method takes a ref to its own struct type

```llvm
%Counter = type { i32 }

define void @Counter.absorb(%Counter* %0, %Counter* %1) {
entry:
  %self = alloca %Counter*, align 8
  %other = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  store %Counter* %1, %Counter** %other, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"*other" = load %Counter*, %Counter** %other, align 8
  %"*other.n" = getelementptr inbounds %Counter, %Counter* %"*other", i32 0, i32 0
  %"$*other.n" = load i32, i32* %"*other.n", align 4
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", %"$*other.n"
  store i32 %ADD, i32* %"*self.n", align 4
  %"*other1" = load %Counter*, %Counter** %other, align 8
  %"*other.n2" = getelementptr inbounds %Counter, %Counter* %"*other1", i32 0, i32 0
  store i32 0, i32* %"*other.n2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %a.n = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  store i32 4, i32* %a.n, align 4
  %b.n = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  store i32 6, i32* %b.n, align 4
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  %a.n1 = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  %"$a.n" = load i32, i32* %a.n1, align 4
  %MUL = mul i32 %"$a.n", 10
  %b.n2 = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  %"$b.n" = load i32, i32* %b.n2, align 4
  %ADD = add i32 %MUL, %"$b.n"
  ret i32 %ADD
}
```

## 037 — crazy: stack: push squares, pop until empty

```llvm
%Stack = type { i32*, i32 }

define void @Stack.push(%Stack* %0, i32 %1) {
entry:
  %self = alloca %Stack*, align 8
  %v = alloca i32, align 4
  store %Stack* %0, %Stack** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.items" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 0
  %"*self1" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self1", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.top"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.items[]", align 4
  %"*self2" = load %Stack*, %Stack** %self, align 8
  %"*self.top3" = getelementptr inbounds %Stack, %Stack* %"*self2", i32 0, i32 1
  %"$*self.top4" = load i32, i32* %"*self.top3", align 4
  %ADD = add i32 %"$*self.top4", 1
  store i32 %ADD, i32* %"*self.top3", align 4
  ret void
}

define i32 @Stack.pop(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 1
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %SUB = sub i32 %"$*self.top", 1
  store i32 %SUB, i32* %"*self.top", align 4
  %"*self1" = load %Stack*, %Stack** %self, align 8
  %"*self.items" = getelementptr inbounds %Stack, %Stack* %"*self1", i32 0, i32 0
  %"*self2" = load %Stack*, %Stack** %self, align 8
  %"*self.top3" = getelementptr inbounds %Stack, %Stack* %"*self2", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.top4" = load i32, i32* %"*self.top3", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.top4"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  ret i32 %"$*self.items[]"
}

define i1 @Stack.empty(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 1
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %EQ = icmp eq i32 %"$*self.top", 0
  ret i1 %EQ
}

define i32 @main() {
entry:
  %s = alloca %Stack, align 8
  %arr = alloca [8 x i32], align 4
  %i = alloca i32, align 4
  %total = alloca i32, align 4
  store %Stack zeroinitializer, %Stack* %s, align 8
  %s.items = getelementptr inbounds %Stack, %Stack* %s, i32 0, i32 0
  store [8 x i32] zeroinitializer, [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %s.items, align 8
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  %"$i3" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i2", %"$i3"
  call void @Stack.push(%Stack* %s, i32 %MUL)
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  store i32 0, i32* %total, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %endloop
  %call.Stack.empty = call i1 @Stack.empty(%Stack* %s)
  %NOT = xor i1 %call.Stack.empty, true
  br i1 %NOT, label %while1.then, label %endloop1

while1.then:                                      ; preds = %while1
  %"$total" = load i32, i32* %total, align 4
  %MUL5 = mul i32 %"$total", 2
  %call.Stack.pop = call i32 @Stack.pop(%Stack* %s)
  %MOD = srem i32 %call.Stack.pop, 10
  %ADD6 = add i32 %MUL5, %MOD
  store i32 %ADD6, i32* %total, align 4
  br label %while1

endloop1:                                         ; preds = %while1
  %"$total7" = load i32, i32* %total, align 4
  ret i32 %"$total7"
}
```

## 038 — crazy: ring-buffer queue: full, wrap around, drain

```llvm
%Queue = type { i32*, i32, i32, i32, i32 }

define i1 @Queue.put(%Queue* %0, i32 %1) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  store i32 %1, i32* %v, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Queue*, %Queue** %self, align 8
  %"*self.size" = getelementptr inbounds %Queue, %Queue* %"*self", i32 0, i32 3
  %"*self1" = load %Queue*, %Queue** %self, align 8
  %"*self.cap" = getelementptr inbounds %Queue, %Queue* %"*self1", i32 0, i32 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.size", %"$*self.cap"
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  %"*self2" = load %Queue*, %Queue** %self, align 8
  %"*self.items" = getelementptr inbounds %Queue, %Queue* %"*self2", i32 0, i32 0
  %"*self3" = load %Queue*, %Queue** %self, align 8
  %"*self.tail" = getelementptr inbounds %Queue, %Queue* %"*self3", i32 0, i32 2
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.tail" = load i32, i32* %"*self.tail", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.tail"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.items[]", align 4
  %"*self4" = load %Queue*, %Queue** %self, align 8
  %"*self.tail5" = getelementptr inbounds %Queue, %Queue* %"*self4", i32 0, i32 2
  %"*self6" = load %Queue*, %Queue** %self, align 8
  %"*self.tail7" = getelementptr inbounds %Queue, %Queue* %"*self6", i32 0, i32 2
  %"$*self.tail8" = load i32, i32* %"*self.tail7", align 4
  %ADD = add i32 %"$*self.tail8", 1
  %"*self9" = load %Queue*, %Queue** %self, align 8
  %"*self.cap10" = getelementptr inbounds %Queue, %Queue* %"*self9", i32 0, i32 4
  %"$*self.cap11" = load i32, i32* %"*self.cap10", align 4
  %MOD = srem i32 %ADD, %"$*self.cap11"
  store i32 %MOD, i32* %"*self.tail5", align 4
  %"*self12" = load %Queue*, %Queue** %self, align 8
  %"*self.size13" = getelementptr inbounds %Queue, %Queue* %"*self12", i32 0, i32 3
  %"$*self.size14" = load i32, i32* %"*self.size13", align 4
  %ADD15 = add i32 %"$*self.size14", 1
  store i32 %ADD15, i32* %"*self.size13", align 4
  ret i1 true
}

define i32 @Queue.take(%Queue* %0) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  %"*self" = load %Queue*, %Queue** %self, align 8
  %"*self.items" = getelementptr inbounds %Queue, %Queue* %"*self", i32 0, i32 0
  %"*self1" = load %Queue*, %Queue** %self, align 8
  %"*self.head" = getelementptr inbounds %Queue, %Queue* %"*self1", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.head" = load i32, i32* %"*self.head", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.head"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  store i32 %"$*self.items[]", i32* %v, align 4
  %"*self2" = load %Queue*, %Queue** %self, align 8
  %"*self.head3" = getelementptr inbounds %Queue, %Queue* %"*self2", i32 0, i32 1
  %"*self4" = load %Queue*, %Queue** %self, align 8
  %"*self.head5" = getelementptr inbounds %Queue, %Queue* %"*self4", i32 0, i32 1
  %"$*self.head6" = load i32, i32* %"*self.head5", align 4
  %ADD = add i32 %"$*self.head6", 1
  %"*self7" = load %Queue*, %Queue** %self, align 8
  %"*self.cap" = getelementptr inbounds %Queue, %Queue* %"*self7", i32 0, i32 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %MOD = srem i32 %ADD, %"$*self.cap"
  store i32 %MOD, i32* %"*self.head3", align 4
  %"*self8" = load %Queue*, %Queue** %self, align 8
  %"*self.size" = getelementptr inbounds %Queue, %Queue* %"*self8", i32 0, i32 3
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %SUB = sub i32 %"$*self.size", 1
  store i32 %SUB, i32* %"*self.size", align 4
  %"$v" = load i32, i32* %v, align 4
  ret i32 %"$v"
}

define i32 @main() {
entry:
  %q = alloca %Queue, align 8
  %arr = alloca [3 x i32], align 4
  %dropped = alloca i32, align 4
  %i = alloca i32, align 4
  %first = alloca i32, align 4
  %sum = alloca i32, align 4
  store %Queue zeroinitializer, %Queue* %q, align 8
  %q.items = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 0
  store [3 x i32] zeroinitializer, [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %q.items, align 8
  %q.cap = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 4
  store i32 3, i32* %q.cap, align 4
  store i32 0, i32* %dropped, align 4
  store i32 1, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i2" = load i32, i32* %i, align 4
  %call.Queue.put = call i1 @Queue.put(%Queue* %q, i32 %"$i2")
  %NOT = xor i1 %call.Queue.put, true
  br i1 %NOT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$dropped" = load i32, i32* %dropped, align 4
  %ADD = add i32 %"$dropped", 1
  store i32 %ADD, i32* %dropped, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %call.Queue.take = call i32 @Queue.take(%Queue* %q)
  store i32 %call.Queue.take, i32* %first, align 4
  %call.Queue.put5 = call i1 @Queue.put(%Queue* %q, i32 9)
  store i32 0, i32* %sum, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %endloop
  %q.size = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 3
  %"$q.size" = load i32, i32* %q.size, align 4
  %GT = icmp sgt i32 %"$q.size", 0
  br i1 %GT, label %while3.then, label %endloop1

while3.then:                                      ; preds = %while3
  %call.Queue.take6 = call i32 @Queue.take(%Queue* %q)
  %"$sum" = load i32, i32* %sum, align 4
  %ADD7 = add i32 %"$sum", %call.Queue.take6
  store i32 %ADD7, i32* %sum, align 4
  br label %while3

endloop1:                                         ; preds = %while3
  %"$first" = load i32, i32* %first, align 4
  %MUL = mul i32 %"$first", 100
  %"$sum8" = load i32, i32* %sum, align 4
  %MUL9 = mul i32 %"$sum8", 10
  %ADD10 = add i32 %MUL, %MUL9
  %"$dropped11" = load i32, i32* %dropped, align 4
  %ADD12 = add i32 %ADD10, %"$dropped11"
  ret i32 %ADD12
}
```

## 039 — crazy: binary tree: recursion through ref attributes

```llvm
%Tree = type { i32, %Tree*, %Tree* }

define i32 @sum(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %t, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*t1" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t1", i32 0, i32 0
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.sum = call i32 @sum(%Tree* %"$*t.left")
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %ADD = add i32 %"$*t.value", %call.sum
  %"*t3" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t3", i32 0, i32 2
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.sum4 = call i32 @sum(%Tree* %"$*t.right")
  %ADD5 = add i32 %ADD, %call.sum4
  ret i32 %ADD5
}

define i32 @depth(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %t, align 8
  br label %if1

if1:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 0

endif:                                            ; preds = %if1
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.depth = call i32 @depth(%Tree* %"$*t.left")
  store i32 %call.depth, i32* %l, align 4
  %"*t3" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t3", i32 0, i32 2
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.depth4 = call i32 @depth(%Tree* %"$*t.right")
  store i32 %call.depth4, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %endif
  %"$l" = load i32, i32* %l, align 4
  %"$r" = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %"$l", %"$r"
  br i1 %GT, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$l5" = load i32, i32* %l, align 4
  %ADD = add i32 %"$l5", 1
  ret i32 %ADD

endif1:                                           ; preds = %if2
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 1
  ret i32 %ADD7
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %d = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  store %Tree zeroinitializer, %Tree* %d, align 8
  %a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %d.value = getelementptr inbounds %Tree, %Tree* %d, i32 0, i32 0
  store i32 4, i32* %d.value, align 4
  %a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %a.left, align 8
  %a.right = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 2
  store %Tree* %c, %Tree** %a.right, align 8
  %b.left = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 1
  store %Tree* %d, %Tree** %b.left, align 8
  %call.sum = call i32 @sum(%Tree* %a)
  %MUL = mul i32 %call.sum, 10
  %call.depth = call i32 @depth(%Tree* %a)
  %ADD = add i32 %MUL, %call.depth
  ret i32 %ADD
}
```

## 040 — crazy: a linked list over array elements

```llvm
%Node = type { i32, %Node* }

define i32 @length(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*n" = load %Node*, %Node** %n, align 8
  %"$n" = load %Node*, %Node** %n, align 8
  %EQ = icmp eq %Node* %"$n", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*n1" = load %Node*, %Node** %n, align 8
  %"*n.next" = getelementptr inbounds %Node, %Node* %"*n1", i32 0, i32 1
  %"$*n.next" = load %Node*, %Node** %"*n.next", align 8
  %call.length = call i32 @length(%Node* %"$*n.next")
  %ADD = add i32 1, %call.length
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %nodes = alloca %Node*, align 8
  %arr = alloca [3 x %Node], align 8
  %i = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %"$a" = load %Node, %Node* %a, align 8
  %"$b" = load %Node, %Node* %b, align 8
  %"$c" = load %Node, %Node* %c, align 8
  %0 = insertvalue [3 x %Node] undef, %Node %"$a", 0
  %1 = insertvalue [3 x %Node] %0, %Node %"$b", 1
  %2 = insertvalue [3 x %Node] %1, %Node %"$c", 2
  store [3 x %Node] %2, [3 x %Node]* %arr, align 8
  %3 = getelementptr inbounds [3 x %Node], [3 x %Node]* %arr, i32 0, i32 0
  store %Node* %3, %Node** %nodes, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$nodes" = load %Node*, %Node** %nodes, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"nodes[]" = getelementptr inbounds %Node, %Node* %"$nodes", i32 %"$i1"
  %"nodes[].value" = getelementptr inbounds %Node, %Node* %"nodes[]", i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  %MUL = mul i32 %ADD, 10
  store i32 %MUL, i32* %"nodes[].value", align 4
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i3" = load i32, i32* %i, align 4
  %LT4 = icmp slt i32 %"$i3", 2
  br i1 %LT4, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$nodes5" = load %Node*, %Node** %nodes, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"nodes[]7" = getelementptr inbounds %Node, %Node* %"$nodes5", i32 %"$i6"
  %"nodes[].next" = getelementptr inbounds %Node, %Node* %"nodes[]7", i32 0, i32 1
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  %"$nodes10" = load %Node*, %Node** %nodes, align 8
  %"nodes[]11" = getelementptr inbounds %Node, %Node* %"$nodes10", i32 %ADD9
  store %Node* %"nodes[]11", %Node** %"nodes[].next", align 8
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i12" = load i32, i32* %i, align 4
  %ADD13 = add i32 %"$i12", 1
  store i32 %ADD13, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$nodes14" = load %Node*, %Node** %nodes, align 8
  %"nodes[]15" = getelementptr inbounds %Node, %Node* %"$nodes14", i32 0
  %call.length = call i32 @length(%Node* %"nodes[]15")
  %"$nodes16" = load %Node*, %Node** %nodes, align 8
  %"nodes[]17" = getelementptr inbounds %Node, %Node* %"$nodes16", i32 0
  %"nodes[].next18" = getelementptr inbounds %Node, %Node* %"nodes[]17", i32 0, i32 1
  %"*nodes[].next" = load %Node*, %Node** %"nodes[].next18", align 8
  %"*nodes[].next.next" = getelementptr inbounds %Node, %Node* %"*nodes[].next", i32 0, i32 1
  %"**nodes[].next.next" = load %Node*, %Node** %"*nodes[].next.next", align 8
  %"**nodes[].next.next.value" = getelementptr inbounds %Node, %Node* %"**nodes[].next.next", i32 0, i32 0
  %"$**nodes[].next.next.value" = load i32, i32* %"**nodes[].next.next.value", align 4
  %ADD19 = add i32 %call.length, %"$**nodes[].next.next.value"
  ret i32 %ADD19
}
```

## 041 — crazy: search a linked list, -1 when missing

```llvm
%Node = type { i32, %Node* }

define i32 @find(%Node* %0, i32 %1) {
entry:
  %head = alloca %Node*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %head, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  %"*head" = load %Node*, %Node** %head, align 8
  store %Node* %"*head", %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %"$p" = load %Node*, %Node** %p, align 8
  %EQ = icmp eq %Node* %"$p", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ2 = icmp eq i32 %"$*p.value", %"$v"
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i" = load i32, i32* %i, align 4
  ret i32 %"$i"

endif:                                            ; preds = %if1
  %"*p3" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p3", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %r = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 5, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 8, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 13, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %call.find = call i32 @find(%Node* %a, i32 13)
  %MUL = mul i32 %call.find, 10
  %call.find1 = call i32 @find(%Node* %a, i32 8)
  %ADD = add i32 %MUL, %call.find1
  store i32 %ADD, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %call.find2 = call i32 @find(%Node* %a, i32 99)
  %EQ = icmp eq i32 %call.find2, -1
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD3 = add i32 %"$r", 100
  store i32 %ADD3, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$r4" = load i32, i32* %r, align 4
  ret i32 %"$r4"
}
```

## 042 — crazy: assigning a struct copies it

```llvm
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 2, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  store %Point %"$a", %Point* %b, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 10, i32* %b.x, align 4
  %"$b" = load %Point, %Point* %b, align 4
  store %Point %"$b", %Point* %c, align 4
  %c.y = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  store i32 20, i32* %c.y, align 4
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %a.y2 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  %"$a.y" = load i32, i32* %a.y2, align 4
  %MUL = mul i32 %"$a.y", 10
  %"$a.x" = load i32, i32* %a.x1, align 4
  %ADD = add i32 %"$a.x", %MUL
  %b.x3 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  %"$b.x" = load i32, i32* %b.x3, align 4
  %ADD4 = add i32 %ADD, %"$b.x"
  %b.y = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y" = load i32, i32* %b.y, align 4
  %ADD5 = add i32 %ADD4, %"$b.y"
  %c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  %"$c.x" = load i32, i32* %c.x, align 4
  %ADD6 = add i32 %ADD5, %"$c.x"
  %c.y7 = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  %"$c.y" = load i32, i32* %c.y7, align 4
  %ADD8 = add i32 %ADD6, %"$c.y"
  ret i32 %ADD8
}
```

## 043 — crazy: a struct parameter is a copy

```llvm
%Point = type { i32 }

define i32 @bump(%Point %0) {
entry:
  %p = alloca %Point, align 8
  store %Point %0, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$p.x" = load i32, i32* %p.x, align 4
  %ADD = add i32 %"$p.x", 5
  store i32 %ADD, i32* %p.x, align 4
  %p.x1 = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$p.x2" = load i32, i32* %p.x1, align 4
  ret i32 %"$p.x2"
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %r = alloca i32, align 4
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %"$a" = load %Point, %Point* %a, align 4
  %call.bump = call i32 @bump(%Point %"$a")
  store i32 %call.bump, i32* %r, align 4
  %"$r" = load i32, i32* %r, align 4
  %MUL = mul i32 %"$r", 10
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %"$a.x" = load i32, i32* %a.x1, align 4
  %ADD = add i32 %MUL, %"$a.x"
  ret i32 %ADD
}
```

## 044 — crazy: compound assignment through a ref attribute

```llvm
%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %a.next1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next1, align 8
  %"*a.next.value" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 0
  %"$*a.next.value" = load i32, i32* %"*a.next.value", align 4
  %ADD = add i32 %"$*a.next.value", 5
  store i32 %ADD, i32* %"*a.next.value", align 4
  %a.next2 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next3" = load %Node*, %Node** %a.next2, align 8
  %"*a.next.value4" = getelementptr inbounds %Node, %Node* %"*a.next3", i32 0, i32 0
  %"$*a.next.value5" = load i32, i32* %"*a.next.value4", align 4
  %MUL = mul i32 %"$*a.next.value5", 3
  store i32 %MUL, i32* %"*a.next.value4", align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value, align 4
  %SUB = sub i32 %"$b.value", 1
  store i32 %SUB, i32* %b.value, align 4
  %b.value6 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$b.value7" = load i32, i32* %b.value6, align 4
  ret i32 %"$b.value7"
}
```

## 045 — crazy: a method on an array element

```llvm
%Counter = type { i32 }

define void @Counter.add(%Counter* %0, i32 %1) {
entry:
  %self = alloca %Counter*, align 8
  %k = alloca i32, align 4
  store %Counter* %0, %Counter** %self, align 8
  store i32 %1, i32* %k, align 4
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", %"$k"
  store i32 %ADD, i32* %"*self.n", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  %cs = alloca %Counter*, align 8
  %arr = alloca [2 x %Counter], align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %"$a" = load %Counter, %Counter* %a, align 4
  %"$b" = load %Counter, %Counter* %b, align 4
  %0 = insertvalue [2 x %Counter] undef, %Counter %"$a", 0
  %1 = insertvalue [2 x %Counter] %0, %Counter %"$b", 1
  store [2 x %Counter] %1, [2 x %Counter]* %arr, align 4
  %2 = getelementptr inbounds [2 x %Counter], [2 x %Counter]* %arr, i32 0, i32 0
  store %Counter* %2, %Counter** %cs, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$cs" = load %Counter*, %Counter** %cs, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"cs[]" = getelementptr inbounds %Counter, %Counter* %"$cs", i32 %"$i1"
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 3
  call void @Counter.add(%Counter* %"cs[]", i32 %ADD)
  %"$cs3" = load %Counter*, %Counter** %cs, align 8
  %"$i4" = load i32, i32* %i, align 4
  %"cs[]5" = getelementptr inbounds %Counter, %Counter* %"$cs3", i32 %"$i4"
  call void @Counter.add(%Counter* %"cs[]5", i32 1)
  %"$i6" = load i32, i32* %i, align 4
  %ADD7 = add i32 %"$i6", 1
  store i32 %ADD7, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$cs8" = load %Counter*, %Counter** %cs, align 8
  %"cs[]9" = getelementptr inbounds %Counter, %Counter* %"$cs8", i32 0
  %"cs[].n" = getelementptr inbounds %Counter, %Counter* %"cs[]9", i32 0, i32 0
  %"$cs[].n" = load i32, i32* %"cs[].n", align 4
  %MUL = mul i32 %"$cs[].n", 10
  %"$cs10" = load %Counter*, %Counter** %cs, align 8
  %"cs[]11" = getelementptr inbounds %Counter, %Counter* %"$cs10", i32 1
  %"cs[].n12" = getelementptr inbounds %Counter, %Counter* %"cs[]11", i32 0, i32 0
  %"$cs[].n13" = load i32, i32* %"cs[].n12", align 4
  %ADD14 = add i32 %MUL, %"$cs[].n13"
  ret i32 %ADD14
}
```

## 046 — crazy: builder: a method returns a ref to self, chained

```llvm
%Builder = type { i32 }

define %Builder* @Builder.add(%Builder* %0, i32 %1) {
entry:
  %self = alloca %Builder*, align 8
  %v = alloca i32, align 4
  store %Builder* %0, %Builder** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %Builder*, %Builder** %self, align 8
  %"*self.total" = getelementptr inbounds %Builder, %Builder* %"*self", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  %"$*self.total" = load i32, i32* %"*self.total", align 4
  %ADD = add i32 %"$*self.total", %"$v"
  store i32 %ADD, i32* %"*self.total", align 4
  %"*self1" = load %Builder*, %Builder** %self, align 8
  ret %Builder* %"*self1"
}

define i32 @main() {
entry:
  %b = alloca %Builder, align 8
  %ref = alloca %Builder*, align 8
  %ref2 = alloca %Builder*, align 8
  %ref5 = alloca %Builder*, align 8
  store %Builder zeroinitializer, %Builder* %b, align 4
  %call.Builder.add = call %Builder* @Builder.add(%Builder* %b, i32 1)
  store %Builder* %call.Builder.add, %Builder** %ref, align 8
  %"*ref" = load %Builder*, %Builder** %ref, align 8
  %call.Builder.add1 = call %Builder* @Builder.add(%Builder* %"*ref", i32 2)
  store %Builder* %call.Builder.add1, %Builder** %ref2, align 8
  %"*ref3" = load %Builder*, %Builder** %ref2, align 8
  %call.Builder.add4 = call %Builder* @Builder.add(%Builder* %"*ref3", i32 3)
  store %Builder* %call.Builder.add4, %Builder** %ref5, align 8
  %"*ref6" = load %Builder*, %Builder** %ref5, align 8
  %b.total = getelementptr inbounds %Builder, %Builder* %b, i32 0, i32 0
  %"$b.total" = load i32, i32* %b.total, align 4
  ret i32 %"$b.total"
}
```

## 047 — crazy: a struct holding an array of structs

```llvm
%Shape = type { %Point*, i32 }
%Point = type { i32, i32 }

define i32 @Shape.width(%Shape* %0) {
entry:
  %self = alloca %Shape*, align 8
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  %i = alloca i32, align 4
  %x = alloca i32, align 4
  store %Shape* %0, %Shape** %self, align 8
  %"*self" = load %Shape*, %Shape** %self, align 8
  %"*self.points" = getelementptr inbounds %Shape, %Shape* %"*self", i32 0, i32 0
  %"$*self.points" = load %Point*, %Point** %"*self.points", align 8
  %"*self.points[]" = getelementptr inbounds %Point, %Point* %"$*self.points", i32 0
  %"*self.points[].x" = getelementptr inbounds %Point, %Point* %"*self.points[]", i32 0, i32 0
  %"$*self.points[].x" = load i32, i32* %"*self.points[].x", align 4
  store i32 %"$*self.points[].x", i32* %lo, align 4
  %"$lo" = load i32, i32* %lo, align 4
  store i32 %"$lo", i32* %hi, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  %"*self2" = load %Shape*, %Shape** %self, align 8
  %"*self.count" = getelementptr inbounds %Shape, %Shape* %"*self2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*self3" = load %Shape*, %Shape** %self, align 8
  %"*self.points4" = getelementptr inbounds %Shape, %Shape* %"*self3", i32 0, i32 0
  %"$*self.points5" = load %Point*, %Point** %"*self.points4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.points[]7" = getelementptr inbounds %Point, %Point* %"$*self.points5", i32 %"$i6"
  %"*self.points[].x8" = getelementptr inbounds %Point, %Point* %"*self.points[]7", i32 0, i32 0
  %"$*self.points[].x9" = load i32, i32* %"*self.points[].x8", align 4
  store i32 %"$*self.points[].x9", i32* %x, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$x" = load i32, i32* %x, align 4
  %"$lo10" = load i32, i32* %lo, align 4
  %LT11 = icmp slt i32 %"$x", %"$lo10"
  br i1 %LT11, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$x12" = load i32, i32* %x, align 4
  store i32 %"$x12", i32* %lo, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif
  %"$x13" = load i32, i32* %x, align 4
  %"$hi" = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %"$x13", %"$hi"
  br i1 %GT, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$x14" = load i32, i32* %x, align 4
  store i32 %"$x14", i32* %hi, align 4
  br label %endif1

endif1:                                           ; preds = %if2.then, %if2
  %"$i15" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i15", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$hi16" = load i32, i32* %hi, align 4
  %"$lo17" = load i32, i32* %lo, align 4
  %SUB = sub i32 %"$hi16", %"$lo17"
  ret i32 %SUB
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  %s = alloca %Shape, align 8
  %arr = alloca [3 x %Point], align 8
  store %Point zeroinitializer, %Point* %a, align 4
  store %Point zeroinitializer, %Point* %b, align 4
  store %Point zeroinitializer, %Point* %c, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 4, i32* %a.x, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 -3, i32* %b.x, align 4
  %c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  store i32 9, i32* %c.x, align 4
  store %Shape zeroinitializer, %Shape* %s, align 8
  %s.points = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 0
  %"$a" = load %Point, %Point* %a, align 4
  %"$b" = load %Point, %Point* %b, align 4
  %"$c" = load %Point, %Point* %c, align 4
  %0 = insertvalue [3 x %Point] undef, %Point %"$a", 0
  %1 = insertvalue [3 x %Point] %0, %Point %"$b", 1
  %2 = insertvalue [3 x %Point] %1, %Point %"$c", 2
  store [3 x %Point] %2, [3 x %Point]* %arr, align 4
  %3 = getelementptr inbounds [3 x %Point], [3 x %Point]* %arr, i32 0, i32 0
  store %Point* %3, %Point** %s.points, align 8
  %s.count = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 1
  store i32 3, i32* %s.count, align 4
  %call.Shape.width = call i32 @Shape.width(%Shape* %s)
  ret i32 %call.Shape.width
}
```

## 048 — crazy: tree walk with a ref out-parameter

```llvm
%Tree = type { i32, %Tree*, %Tree* }

define i32 @walk(%Tree* %0, i32* %1) {
entry:
  %t = alloca %Tree*, align 8
  %count = alloca i32*, align 8
  store %Tree* %0, %Tree** %t, align 8
  store i32* %1, i32** %count, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*count" = load i32*, i32** %count, align 8
  %"$*count" = load i32, i32* %"*count", align 4
  %ADD = add i32 %"$*count", 1
  store i32 %ADD, i32* %"*count", align 4
  %"*t1" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t1", i32 0, i32 0
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"*count3" = load i32*, i32** %count, align 8
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.walk = call i32 @walk(%Tree* %"$*t.left", i32* %"*count3")
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %ADD4 = add i32 %"$*t.value", %call.walk
  %"*t5" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t5", i32 0, i32 2
  %"*count6" = load i32*, i32** %count, align 8
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.walk7 = call i32 @walk(%Tree* %"$*t.right", i32* %"*count6")
  %ADD8 = add i32 %ADD4, %call.walk7
  ret i32 %ADD8
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %n = alloca i32, align 4
  %s = alloca i32, align 4
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  %a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 5, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 6, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 7, i32* %c.value, align 4
  %a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %a.left, align 8
  %b.right = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 2
  store %Tree* %c, %Tree** %b.right, align 8
  store i32 0, i32* %n, align 4
  %call.walk = call i32 @walk(%Tree* %a, i32* %n)
  store i32 %call.walk, i32* %s, align 4
  %"$s" = load i32, i32* %s, align 4
  %MUL = mul i32 %"$s", 10
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %MUL, %"$n"
  ret i32 %ADD
}
```

## 049 — crazy: a function returns a ref into a struct, the caller binds it

```llvm
%Pair = type { i32, i32 }

define i32* @bigger(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*p" = load %Pair*, %Pair** %p, align 8
  %"*p.a" = getelementptr inbounds %Pair, %Pair* %"*p", i32 0, i32 0
  %"*p1" = load %Pair*, %Pair** %p, align 8
  %"*p.b" = getelementptr inbounds %Pair, %Pair* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %GT = icmp sgt i32 %"$*p.a", %"$*p.b"
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*p2" = load %Pair*, %Pair** %p, align 8
  %"*p.a3" = getelementptr inbounds %Pair, %Pair* %"*p2", i32 0, i32 0
  ret i32* %"*p.a3"

endif:                                            ; preds = %if0
  %"*p4" = load %Pair*, %Pair** %p, align 8
  %"*p.b5" = getelementptr inbounds %Pair, %Pair* %"*p4", i32 0, i32 1
  ret i32* %"*p.b5"
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  %r = alloca i32*, align 8
  %ref = alloca i32*, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 3, i32* %x.a, align 4
  %x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 7, i32* %x.b, align 4
  %call.bigger = call i32* @bigger(%Pair* %x)
  store i32* %call.bigger, i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32* %"*ref", i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 100, i32* %"*r", align 4
  %x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %"$x.a" = load i32, i32* %x.a1, align 4
  %"$x.b" = load i32, i32* %x.b2, align 4
  %ADD = add i32 %"$x.a", %"$x.b"
  ret i32 %ADD
}
```

## 050 — crazy: fibonacci as a pair: recursion returning a struct, .a on a call

```llvm
%Pair = type { i32, i32 }

define %Pair @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %Pair, align 8
  %p = alloca %Pair, align 8
  store i32 %0, i32* %n, align 4
  store %Pair zeroinitializer, %Pair* %r, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.a = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  store i32 0, i32* %r.a, align 4
  %r.b = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  store i32 1, i32* %r.b, align 4
  %"$r" = load %Pair, %Pair* %r, align 4
  ret %Pair %"$r"

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.fib = call %Pair @fib(i32 %SUB)
  store %Pair %call.fib, %Pair* %p, align 4
  %r.a2 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  %p.b = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.b" = load i32, i32* %p.b, align 4
  store i32 %"$p.b", i32* %r.a2, align 4
  %r.b3 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  %p.a = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  %p.b4 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.a" = load i32, i32* %p.a, align 4
  %"$p.b5" = load i32, i32* %p.b4, align 4
  %ADD = add i32 %"$p.a", %"$p.b5"
  store i32 %ADD, i32* %r.b3, align 4
  %"$r6" = load %Pair, %Pair* %r, align 4
  ret %Pair %"$r6"
}

define i32 @main() {
entry:
  %tmp = alloca %Pair, align 8
  %call.fib = call %Pair @fib(i32 10)
  store %Pair %call.fib, %Pair* %tmp, align 4
  %tmp.a = getelementptr inbounds %Pair, %Pair* %tmp, i32 0, i32 0
  %"$tmp.a" = load i32, i32* %tmp.a, align 4
  ret i32 %"$tmp.a"
}
```

## 051 — crazy: method calls nested as arguments

```llvm
%Acc = type { i32 }

define i32 @Acc.get(%Acc* %0) {
entry:
  %self = alloca %Acc*, align 8
  store %Acc* %0, %Acc** %self, align 8
  %"*self" = load %Acc*, %Acc** %self, align 8
  %"*self.v" = getelementptr inbounds %Acc, %Acc* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  ret i32 %"$*self.v"
}

define i32 @Acc.add(%Acc* %0, i32 %1) {
entry:
  %self = alloca %Acc*, align 8
  %k = alloca i32, align 4
  store %Acc* %0, %Acc** %self, align 8
  store i32 %1, i32* %k, align 4
  %"*self" = load %Acc*, %Acc** %self, align 8
  %"*self.v" = getelementptr inbounds %Acc, %Acc* %"*self", i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  %ADD = add i32 %"$*self.v", %"$k"
  store i32 %ADD, i32* %"*self.v", align 4
  %"*self1" = load %Acc*, %Acc** %self, align 8
  %"*self.v2" = getelementptr inbounds %Acc, %Acc* %"*self1", i32 0, i32 0
  %"$*self.v3" = load i32, i32* %"*self.v2", align 4
  ret i32 %"$*self.v3"
}

define i32 @main() {
entry:
  %a = alloca %Acc, align 8
  %b = alloca %Acc, align 8
  store %Acc zeroinitializer, %Acc* %a, align 4
  store %Acc zeroinitializer, %Acc* %b, align 4
  %b.v = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  store i32 5, i32* %b.v, align 4
  %call.Acc.get = call i32 @Acc.get(%Acc* %b)
  %call.Acc.add = call i32 @Acc.add(%Acc* %a, i32 %call.Acc.get)
  %call.Acc.add1 = call i32 @Acc.add(%Acc* %b, i32 %call.Acc.add)
  %call.Acc.add2 = call i32 @Acc.add(%Acc* %a, i32 %call.Acc.add1)
  %a.v = getelementptr inbounds %Acc, %Acc* %a, i32 0, i32 0
  %"$a.v" = load i32, i32* %a.v, align 4
  %MUL = mul i32 %"$a.v", 10
  %b.v3 = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  %"$b.v" = load i32, i32* %b.v3, align 4
  %ADD = add i32 %MUL, %"$b.v"
  ret i32 %ADD
}
```

## 052 — crazy: print a linked list backwards with recursion

```llvm
%Node = type { i32, %Node* }

@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @print_rev(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*n" = load %Node*, %Node** %n, align 8
  %"$n" = load %Node*, %Node** %n, align 8
  %EQ = icmp eq %Node* %"$n", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"*n1" = load %Node*, %Node** %n, align 8
  %"*n.next" = getelementptr inbounds %Node, %Node* %"*n1", i32 0, i32 1
  %"$*n.next" = load %Node*, %Node** %"*n.next", align 8
  call void @print_rev(%Node* %"$*n.next")
  %"*n2" = load %Node*, %Node** %n, align 8
  %"*n.value" = getelementptr inbounds %Node, %Node* %"*n2", i32 0, i32 0
  %"$*n.value" = load i32, i32* %"*n.value", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$*n.value")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  call void @print_rev(%Node* %a)
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %b.value2 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  %"$b.value" = load i32, i32* %b.value2, align 4
  %ADD = add i32 %"$a.value", %"$b.value"
  %c.value3 = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  %"$c.value" = load i32, i32* %c.value3, align 4
  %ADD4 = add i32 %ADD, %"$c.value"
  ret i32 %ADD4
}
```

## 053 — crazy: sort structs in an array by an attribute

```llvm
%Item = type { i32, i8 }

@str = private unnamed_addr constant [8 x i8] c"%c%c%c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca %Item, align 8
  %b = alloca %Item, align 8
  %c = alloca %Item, align 8
  %items = alloca %Item*, align 8
  %arr = alloca [3 x %Item], align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %a, align 4
  store %Item zeroinitializer, %Item* %b, align 4
  store %Item zeroinitializer, %Item* %c, align 4
  %a.key = getelementptr inbounds %Item, %Item* %a, i32 0, i32 0
  store i32 3, i32* %a.key, align 4
  %a.tag = getelementptr inbounds %Item, %Item* %a, i32 0, i32 1
  store i8 99, i8* %a.tag, align 1
  %b.key = getelementptr inbounds %Item, %Item* %b, i32 0, i32 0
  store i32 1, i32* %b.key, align 4
  %b.tag = getelementptr inbounds %Item, %Item* %b, i32 0, i32 1
  store i8 97, i8* %b.tag, align 1
  %c.key = getelementptr inbounds %Item, %Item* %c, i32 0, i32 0
  store i32 2, i32* %c.key, align 4
  %c.tag = getelementptr inbounds %Item, %Item* %c, i32 0, i32 1
  store i8 98, i8* %c.tag, align 1
  %"$a" = load %Item, %Item* %a, align 4
  %"$b" = load %Item, %Item* %b, align 4
  %"$c" = load %Item, %Item* %c, align 4
  %0 = insertvalue [3 x %Item] undef, %Item %"$a", 0
  %1 = insertvalue [3 x %Item] %0, %Item %"$b", 1
  %2 = insertvalue [3 x %Item] %1, %Item %"$c", 2
  store [3 x %Item] %2, [3 x %Item]* %arr, align 4
  %3 = getelementptr inbounds [3 x %Item], [3 x %Item]* %arr, i32 0, i32 0
  store %Item* %3, %Item** %items, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT3 = icmp slt i32 %"$j", 3
  br i1 %LT3, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$items" = load %Item*, %Item** %items, align 8
  %"$j4" = load i32, i32* %j, align 4
  %"items[]" = getelementptr inbounds %Item, %Item* %"$items", i32 %"$j4"
  %"items[].key" = getelementptr inbounds %Item, %Item* %"items[]", i32 0, i32 0
  %"$items5" = load %Item*, %Item** %items, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"items[]7" = getelementptr inbounds %Item, %Item* %"$items5", i32 %"$i6"
  %"items[].key8" = getelementptr inbounds %Item, %Item* %"items[]7", i32 0, i32 0
  %"$items[].key" = load i32, i32* %"items[].key", align 4
  %"$items[].key9" = load i32, i32* %"items[].key8", align 4
  %LT10 = icmp slt i32 %"$items[].key", %"$items[].key9"
  br i1 %LT10, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$items11" = load %Item*, %Item** %items, align 8
  %"$i12" = load i32, i32* %i, align 4
  %"items[]13" = getelementptr inbounds %Item, %Item* %"$items11", i32 %"$i12"
  %"$items[]" = load %Item, %Item* %"items[]13", align 4
  store %Item %"$items[]", %Item* %t, align 4
  %"$items14" = load %Item*, %Item** %items, align 8
  %"$i15" = load i32, i32* %i, align 4
  %"items[]16" = getelementptr inbounds %Item, %Item* %"$items14", i32 %"$i15"
  %"$items17" = load %Item*, %Item** %items, align 8
  %"$j18" = load i32, i32* %j, align 4
  %"items[]19" = getelementptr inbounds %Item, %Item* %"$items17", i32 %"$j18"
  %"$items[]20" = load %Item, %Item* %"items[]19", align 4
  store %Item %"$items[]20", %Item* %"items[]16", align 4
  %"$items21" = load %Item*, %Item** %items, align 8
  %"$j22" = load i32, i32* %j, align 4
  %"items[]23" = getelementptr inbounds %Item, %Item* %"$items21", i32 %"$j22"
  %"$t" = load %Item, %Item* %t, align 4
  store %Item %"$t", %Item* %"items[]23", align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$j24" = load i32, i32* %j, align 4
  %ADD25 = add i32 %"$j24", 1
  store i32 %ADD25, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i26" = load i32, i32* %i, align 4
  %ADD27 = add i32 %"$i26", 1
  store i32 %ADD27, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$items28" = load %Item*, %Item** %items, align 8
  %"items[]29" = getelementptr inbounds %Item, %Item* %"$items28", i32 0
  %"items[].tag" = getelementptr inbounds %Item, %Item* %"items[]29", i32 0, i32 1
  %"$items30" = load %Item*, %Item** %items, align 8
  %"items[]31" = getelementptr inbounds %Item, %Item* %"$items30", i32 1
  %"items[].tag32" = getelementptr inbounds %Item, %Item* %"items[]31", i32 0, i32 1
  %"$items33" = load %Item*, %Item** %items, align 8
  %"items[]34" = getelementptr inbounds %Item, %Item* %"$items33", i32 2
  %"items[].tag35" = getelementptr inbounds %Item, %Item* %"items[]34", i32 0, i32 1
  %"$items[].tag" = load i8, i8* %"items[].tag", align 1
  %"$items[].tag36" = load i8, i8* %"items[].tag32", align 1
  %"$items[].tag37" = load i8, i8* %"items[].tag35", align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i8 %"$items[].tag", i8 %"$items[].tag36", i8 %"$items[].tag37")
  %"$items38" = load %Item*, %Item** %items, align 8
  %"items[]39" = getelementptr inbounds %Item, %Item* %"$items38", i32 0
  %"items[].key40" = getelementptr inbounds %Item, %Item* %"items[]39", i32 0, i32 0
  %"$items[].key41" = load i32, i32* %"items[].key40", align 4
  %MUL = mul i32 %"$items[].key41", 100
  %"$items42" = load %Item*, %Item** %items, align 8
  %"items[]43" = getelementptr inbounds %Item, %Item* %"$items42", i32 1
  %"items[].key44" = getelementptr inbounds %Item, %Item* %"items[]43", i32 0, i32 0
  %"$items[].key45" = load i32, i32* %"items[].key44", align 4
  %MUL46 = mul i32 %"$items[].key45", 10
  %ADD47 = add i32 %MUL, %MUL46
  %"$items48" = load %Item*, %Item** %items, align 8
  %"items[]49" = getelementptr inbounds %Item, %Item* %"$items48", i32 2
  %"items[].key50" = getelementptr inbounds %Item, %Item* %"items[]49", i32 0, i32 0
  %"$items[].key51" = load i32, i32* %"items[].key50", align 4
  %ADD52 = add i32 %ADD47, %"$items[].key51"
  ret i32 %ADD52
}
```

## 054 — crazy: ping pong: two structs that name each other, before either is declared

```llvm
%Ping = type { i32 }
%Pong = type { i32 }

define void @Ping.hit(%Ping* %0, %Pong* %1, i32 %2) {
entry:
  %self = alloca %Ping*, align 8
  %o = alloca %Pong*, align 8
  %n = alloca i32, align 4
  store %Ping* %0, %Ping** %self, align 8
  store %Pong* %1, %Pong** %o, align 8
  store i32 %2, i32* %n, align 4
  %"*self" = load %Ping*, %Ping** %self, align 8
  %"*self.hits" = getelementptr inbounds %Ping, %Ping* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %ADD = add i32 %"$*self.hits", 1
  store i32 %ADD, i32* %"*self.hits", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*o" = load %Pong*, %Pong** %o, align 8
  %"*self1" = load %Ping*, %Ping** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  call void @Pong.hit(%Pong* %"*o", %Ping* %"*self1", i32 %SUB)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret void
}

define void @Pong.hit(%Pong* %0, %Ping* %1, i32 %2) {
entry:
  %self = alloca %Pong*, align 8
  %o = alloca %Ping*, align 8
  %n = alloca i32, align 4
  store %Pong* %0, %Pong** %self, align 8
  store %Ping* %1, %Ping** %o, align 8
  store i32 %2, i32* %n, align 4
  %"*self" = load %Pong*, %Pong** %self, align 8
  %"*self.hits" = getelementptr inbounds %Pong, %Pong* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %ADD = add i32 %"$*self.hits", 1
  store i32 %ADD, i32* %"*self.hits", align 4
  br label %if1

if1:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  br i1 %GT, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"*o" = load %Ping*, %Ping** %o, align 8
  %"*self1" = load %Pong*, %Pong** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  call void @Ping.hit(%Ping* %"*o", %Pong* %"*self1", i32 %SUB)
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Ping, align 8
  %b = alloca %Pong, align 8
  store %Ping zeroinitializer, %Ping* %a, align 4
  store %Pong zeroinitializer, %Pong* %b, align 4
  call void @Ping.hit(%Ping* %a, %Pong* %b, i32 5)
  %a.hits = getelementptr inbounds %Ping, %Ping* %a, i32 0, i32 0
  %"$a.hits" = load i32, i32* %a.hits, align 4
  %MUL = mul i32 %"$a.hits", 10
  %b.hits = getelementptr inbounds %Pong, %Pong* %b, i32 0, i32 0
  %"$b.hits" = load i32, i32* %b.hits, align 4
  %ADD = add i32 %MUL, %"$b.hits"
  ret i32 %ADD
}
```

## 055 — crazy: while True with break and return inside a method

```llvm
%Finder = type { i32*, i32 }

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*self" = load %Finder*, %Finder** %self, align 8
  %"*self.size" = getelementptr inbounds %Finder, %Finder* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %EQ = icmp eq i32 %"$i", %"$*self.size"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"*self2" = load %Finder*, %Finder** %self, align 8
  %"*self.data" = getelementptr inbounds %Finder, %Finder* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ4 = icmp eq i32 %"$*self.data[]", %"$v"
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  ret i32 %"$i5"

endif1:                                           ; preds = %if2
  %"$i6" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i6", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %f = alloca %Finder, align 8
  %arr = alloca [6 x i32], align 4
  store %Finder zeroinitializer, %Finder* %f, align 8
  %f.data = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 0
  store [6 x i32] [i32 4, i32 8, i32 15, i32 16, i32 23, i32 42], [6 x i32]* %arr, align 4
  %0 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %f.data, align 8
  %f.size = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 1
  store i32 6, i32* %f.size, align 4
  %call.Finder.index_of = call i32 @Finder.index_of(%Finder* %f, i32 23)
  %MUL = mul i32 %call.Finder.index_of, 10
  %call.Finder.index_of1 = call i32 @Finder.index_of(%Finder* %f, i32 4)
  %ADD = add i32 %MUL, %call.Finder.index_of1
  %call.Finder.index_of2 = call i32 @Finder.index_of(%Finder* %f, i32 99)
  %ADD3 = add i32 %call.Finder.index_of2, 1
  %MUL4 = mul i32 %ADD3, 100
  %ADD5 = add i32 %ADD, %MUL4
  ret i32 %ADD5
}
```

## 056 — crazy: script: a global stack evaluates postfix 3 4 + 2 *

```llvm
%Stack = type { i32*, i32 }

@stack = global %Stack zeroinitializer

define void @push(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %"$stack.items" = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %"$stack.top" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"stack.items[]" = getelementptr inbounds i32, i32* %"$stack.items", i32 %"$stack.top"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"stack.items[]", align 4
  %"$stack.top1" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %ADD = add i32 %"$stack.top1", 1
  store i32 %ADD, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  ret void
}

define i32 @pop() {
entry:
  %"$stack.top" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %SUB = sub i32 %"$stack.top", 1
  store i32 %SUB, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"$stack.items" = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %"$stack.top1" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"stack.items[]" = getelementptr inbounds i32, i32* %"$stack.items", i32 %"$stack.top1"
  %"$stack.items[]" = load i32, i32* %"stack.items[]", align 4
  ret i32 %"$stack.items[]"
}

define i32 @main() {
entry:
  %arr = alloca [8 x i32], align 4
  store [8 x i32] zeroinitializer, [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  call void @push(i32 3)
  call void @push(i32 4)
  %call.pop = call i32 @pop()
  %call.pop1 = call i32 @pop()
  %ADD = add i32 %call.pop, %call.pop1
  call void @push(i32 %ADD)
  call void @push(i32 2)
  %call.pop2 = call i32 @pop()
  %call.pop3 = call i32 @pop()
  %MUL = mul i32 %call.pop2, %call.pop3
  call void @push(i32 %MUL)
  %call.pop4 = call i32 @pop()
  ret i32 %call.pop4
}
```

## 057 — crazy: structs used before declared, a struct local to a function

```llvm
%Point = type { i32, i32 }
%Rect = type { %Point, %Point }
%Local = type { i32 }

@origin = global %Point zeroinitializer
@r = global %Rect zeroinitializer

define i32 @area(%Rect* %0) {
entry:
  %r = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %r, align 8
  %"*r" = load %Rect*, %Rect** %r, align 8
  %"*r.b" = getelementptr inbounds %Rect, %Rect* %"*r", i32 0, i32 1
  %"*r.b.x" = getelementptr inbounds %Point, %Point* %"*r.b", i32 0, i32 0
  %"*r1" = load %Rect*, %Rect** %r, align 8
  %"*r.a" = getelementptr inbounds %Rect, %Rect* %"*r1", i32 0, i32 0
  %"*r.a.x" = getelementptr inbounds %Point, %Point* %"*r.a", i32 0, i32 0
  %"$*r.b.x" = load i32, i32* %"*r.b.x", align 4
  %"$*r.a.x" = load i32, i32* %"*r.a.x", align 4
  %SUB = sub i32 %"$*r.b.x", %"$*r.a.x"
  %"*r2" = load %Rect*, %Rect** %r, align 8
  %"*r.b3" = getelementptr inbounds %Rect, %Rect* %"*r2", i32 0, i32 1
  %"*r.b.y" = getelementptr inbounds %Point, %Point* %"*r.b3", i32 0, i32 1
  %"*r4" = load %Rect*, %Rect** %r, align 8
  %"*r.a5" = getelementptr inbounds %Rect, %Rect* %"*r4", i32 0, i32 0
  %"*r.a.y" = getelementptr inbounds %Point, %Point* %"*r.a5", i32 0, i32 1
  %"$*r.b.y" = load i32, i32* %"*r.b.y", align 4
  %"$*r.a.y" = load i32, i32* %"*r.a.y", align 4
  %SUB6 = sub i32 %"$*r.b.y", %"$*r.a.y"
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @make() {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %l.v = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  store i32 3, i32* %l.v, align 4
  %l.v1 = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  %"$l.v" = load i32, i32* %l.v1, align 4
  ret i32 %"$l.v"
}

define i32 @main() {
entry:
  store i32 2, i32* getelementptr inbounds (%Point, %Point* @origin, i32 0, i32 0), align 4
  %"$origin" = load %Point, %Point* @origin, align 4
  store %Point %"$origin", %Point* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 0), align 4
  store i32 7, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 0), align 4
  store i32 4, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 1), align 4
  %call.area = call i32 @area(%Rect* @r)
  %MUL = mul i32 %call.area, 10
  %call.make = call i32 @make()
  %ADD = add i32 %MUL, %call.make
  ret i32 %ADD
}
```

## 059 — crazy: binary search tree: recursive methods through ref attributes

```llvm
%Tree = type { i32, %Tree*, %Tree* }

@a = global %Tree zeroinitializer
@b = global %Tree zeroinitializer
@c = global %Tree zeroinitializer
@d = global %Tree zeroinitializer
@e = global %Tree zeroinitializer

define void @Tree.insert(%Tree* %0, %Tree* %1) {
entry:
  %self = alloca %Tree*, align 8
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %self, align 8
  store %Tree* %1, %Tree** %t, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t", i32 0, i32 0
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.value" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 0
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %"$*self.value" = load i32, i32* %"*self.value", align 4
  %LT = icmp slt i32 %"$*t.value", %"$*self.value"
  br i1 %LT, label %if0.then, label %else3

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  br i1 %EQ, label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  %"*self4" = load %Tree*, %Tree** %self, align 8
  %"*self.left5" = getelementptr inbounds %Tree, %Tree* %"*self4", i32 0, i32 1
  %"*t6" = load %Tree*, %Tree** %t, align 8
  store %Tree* %"*t6", %Tree** %"*self.left5", align 8
  br label %endif

else2:                                            ; preds = %if1
  %"*self7" = load %Tree*, %Tree** %self, align 8
  %"*self.left8" = getelementptr inbounds %Tree, %Tree* %"*self7", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left8", align 8
  %"*t9" = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %"**self.left", %Tree* %"*t9")
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  br label %endif2

else3:                                            ; preds = %if0
  br label %if4

if4:                                              ; preds = %else3
  %"*self10" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self10", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ11 = icmp eq %Tree* %"$*self.right", null
  br i1 %EQ11, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  %"*self12" = load %Tree*, %Tree** %self, align 8
  %"*self.right13" = getelementptr inbounds %Tree, %Tree* %"*self12", i32 0, i32 2
  %"*t14" = load %Tree*, %Tree** %t, align 8
  store %Tree* %"*t14", %Tree** %"*self.right13", align 8
  br label %endif1

else5:                                            ; preds = %if4
  %"*self15" = load %Tree*, %Tree** %self, align 8
  %"*self.right16" = getelementptr inbounds %Tree, %Tree* %"*self15", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right16", align 8
  %"*t17" = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %"**self.right", %Tree* %"*t17")
  br label %endif1

endif1:                                           ; preds = %else5, %if4.then
  br label %endif2

endif2:                                           ; preds = %endif1, %endif
  ret void
}

define i32 @Tree.sum(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %s = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.value" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 0
  %"$*self.value" = load i32, i32* %"*self.value", align 4
  store i32 %"$*self.value", i32* %s, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self2" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self2", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left4" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left4", align 8
  %call.Tree.sum = call i32 @Tree.sum(%Tree* %"**self.left")
  %"$s" = load i32, i32* %s, align 4
  %ADD = add i32 %"$s", %call.Tree.sum
  store i32 %ADD, i32* %s, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self5" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self5", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ6 = icmp eq %Tree* %"$*self.right", null
  %NOT7 = xor i1 %EQ6, true
  br i1 %NOT7, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  %"*self8" = load %Tree*, %Tree** %self, align 8
  %"*self.right9" = getelementptr inbounds %Tree, %Tree* %"*self8", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right9", align 8
  %call.Tree.sum10 = call i32 @Tree.sum(%Tree* %"**self.right")
  %"$s11" = load i32, i32* %s, align 4
  %ADD12 = add i32 %"$s11", %call.Tree.sum10
  store i32 %ADD12, i32* %s, align 4
  br label %endif1

endif1:                                           ; preds = %if7.then, %if7
  %"$s13" = load i32, i32* %s, align 4
  ret i32 %"$s13"
}

define i32 @Tree.depth(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  store i32 0, i32* %l, align 4
  store i32 0, i32* %r, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left4" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left4", align 8
  %call.Tree.depth = call i32 @Tree.depth(%Tree* %"**self.left")
  store i32 %call.Tree.depth, i32* %l, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif
  %"*self5" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self5", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ6 = icmp eq %Tree* %"$*self.right", null
  %NOT7 = xor i1 %EQ6, true
  br i1 %NOT7, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  %"*self8" = load %Tree*, %Tree** %self, align 8
  %"*self.right9" = getelementptr inbounds %Tree, %Tree* %"*self8", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right9", align 8
  %call.Tree.depth10 = call i32 @Tree.depth(%Tree* %"**self.right")
  store i32 %call.Tree.depth10, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if9.then, %if9
  br label %if10

if10:                                             ; preds = %endif1
  %"$l" = load i32, i32* %l, align 4
  %"$r" = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %"$l", %"$r"
  br i1 %GT, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  %"$l11" = load i32, i32* %l, align 4
  %ADD = add i32 %"$l11", 1
  ret i32 %ADD

endif2:                                           ; preds = %if10
  %"$r12" = load i32, i32* %r, align 4
  %ADD13 = add i32 %"$r12", 1
  ret i32 %ADD13
}

define i32 @main() {
entry:
  store i32 50, i32* getelementptr inbounds (%Tree, %Tree* @a, i32 0, i32 0), align 4
  store i32 30, i32* getelementptr inbounds (%Tree, %Tree* @b, i32 0, i32 0), align 4
  store i32 70, i32* getelementptr inbounds (%Tree, %Tree* @c, i32 0, i32 0), align 4
  store i32 20, i32* getelementptr inbounds (%Tree, %Tree* @d, i32 0, i32 0), align 4
  store i32 40, i32* getelementptr inbounds (%Tree, %Tree* @e, i32 0, i32 0), align 4
  call void @Tree.insert(%Tree* @a, %Tree* @b)
  call void @Tree.insert(%Tree* @a, %Tree* @c)
  call void @Tree.insert(%Tree* @a, %Tree* @d)
  call void @Tree.insert(%Tree* @a, %Tree* @e)
  %call.Tree.sum = call i32 @Tree.sum(%Tree* @a)
  %call.Tree.depth = call i32 @Tree.depth(%Tree* @a)
  %ADD = add i32 %call.Tree.sum, %call.Tree.depth
  ret i32 %ADD
}
```

## 060 — crazy: 2x2 matrix product with methods and ref parameters

```llvm
%Mat = type { i32* }

@a = global %Mat zeroinitializer
@b = global %Mat zeroinitializer
@c = global %Mat zeroinitializer

define i32 @Mat.at(%Mat* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store i32 %1, i32* %r, align 4
  store i32 %2, i32* %c, align 4
  %"*self" = load %Mat*, %Mat** %self, align 8
  %"*self.v" = getelementptr inbounds %Mat, %Mat* %"*self", i32 0, i32 0
  %"$r" = load i32, i32* %r, align 4
  %MUL = mul i32 %"$r", 2
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %"$c"
  %"$*self.v" = load i32*, i32** %"*self.v", align 8
  %"*self.v[]" = getelementptr inbounds i32, i32* %"$*self.v", i32 %ADD
  %"$*self.v[]" = load i32, i32* %"*self.v[]", align 4
  ret i32 %"$*self.v[]"
}

define void @Mat.mul(%Mat* %0, %Mat* %1, %Mat* %2) {
entry:
  %self = alloca %Mat*, align 8
  %o = alloca %Mat*, align 8
  %out = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %s = alloca i32, align 4
  %k = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store %Mat* %1, %Mat** %o, align 8
  store %Mat* %2, %Mat** %out, align 8
  store i32 0, i32* %r, align 4
  br label %while0

while0:                                           ; preds = %endloop1, %entry
  %"$r" = load i32, i32* %r, align 4
  %LT = icmp slt i32 %"$r", 2
  br i1 %LT, label %while0.then, label %endloop2

while0.then:                                      ; preds = %while0
  store i32 0, i32* %c, align 4
  br label %while1

while1:                                           ; preds = %endloop, %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT3 = icmp slt i32 %"$c", 2
  br i1 %LT3, label %while1.then, label %endloop1

while1.then:                                      ; preds = %while1
  store i32 0, i32* %s, align 4
  store i32 0, i32* %k, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %while1.then
  %"$k" = load i32, i32* %k, align 4
  %LT4 = icmp slt i32 %"$k", 2
  br i1 %LT4, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"*self" = load %Mat*, %Mat** %self, align 8
  %"$r5" = load i32, i32* %r, align 4
  %"$k6" = load i32, i32* %k, align 4
  %call.Mat.at = call i32 @Mat.at(%Mat* %"*self", i32 %"$r5", i32 %"$k6")
  %"*o" = load %Mat*, %Mat** %o, align 8
  %"$k7" = load i32, i32* %k, align 4
  %"$c8" = load i32, i32* %c, align 4
  %call.Mat.at9 = call i32 @Mat.at(%Mat* %"*o", i32 %"$k7", i32 %"$c8")
  %MUL = mul i32 %call.Mat.at, %call.Mat.at9
  %"$s" = load i32, i32* %s, align 4
  %ADD = add i32 %"$s", %MUL
  store i32 %ADD, i32* %s, align 4
  %"$k10" = load i32, i32* %k, align 4
  %ADD11 = add i32 %"$k10", 1
  store i32 %ADD11, i32* %k, align 4
  br label %while2

endloop:                                          ; preds = %while2
  %"*out" = load %Mat*, %Mat** %out, align 8
  %"*out.v" = getelementptr inbounds %Mat, %Mat* %"*out", i32 0, i32 0
  %"$r12" = load i32, i32* %r, align 4
  %MUL13 = mul i32 %"$r12", 2
  %"$c14" = load i32, i32* %c, align 4
  %ADD15 = add i32 %MUL13, %"$c14"
  %"$*out.v" = load i32*, i32** %"*out.v", align 8
  %"*out.v[]" = getelementptr inbounds i32, i32* %"$*out.v", i32 %ADD15
  %"$s16" = load i32, i32* %s, align 4
  store i32 %"$s16", i32* %"*out.v[]", align 4
  %"$c17" = load i32, i32* %c, align 4
  %ADD18 = add i32 %"$c17", 1
  store i32 %ADD18, i32* %c, align 4
  br label %while1

endloop1:                                         ; preds = %while1
  %"$r19" = load i32, i32* %r, align 4
  %ADD20 = add i32 %"$r19", 1
  store i32 %ADD20, i32* %r, align 4
  br label %while0

endloop2:                                         ; preds = %while0
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [4 x i32], align 4
  %arr1 = alloca [4 x i32], align 4
  %arr2 = alloca [4 x i32], align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Mat, %Mat* @a, i32 0, i32 0), align 8
  store [4 x i32] [i32 5, i32 6, i32 7, i32 8], [4 x i32]* %arr1, align 4
  %1 = getelementptr inbounds [4 x i32], [4 x i32]* %arr1, i32 0, i32 0
  store i32* %1, i32** getelementptr inbounds (%Mat, %Mat* @b, i32 0, i32 0), align 8
  store [4 x i32] zeroinitializer, [4 x i32]* %arr2, align 4
  %2 = getelementptr inbounds [4 x i32], [4 x i32]* %arr2, i32 0, i32 0
  store i32* %2, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  call void @Mat.mul(%Mat* @a, %Mat* @b, %Mat* @c)
  %"$c.v" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]" = getelementptr inbounds i32, i32* %"$c.v", i32 0
  %"$c.v3" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]4" = getelementptr inbounds i32, i32* %"$c.v3", i32 1
  %"$c.v[]" = load i32, i32* %"c.v[]", align 4
  %"$c.v[]5" = load i32, i32* %"c.v[]4", align 4
  %ADD = add i32 %"$c.v[]", %"$c.v[]5"
  %"$c.v6" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]7" = getelementptr inbounds i32, i32* %"$c.v6", i32 2
  %"$c.v[]8" = load i32, i32* %"c.v[]7", align 4
  %ADD9 = add i32 %ADD, %"$c.v[]8"
  %"$c.v10" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]11" = getelementptr inbounds i32, i32* %"$c.v10", i32 3
  %"$c.v[]12" = load i32, i32* %"c.v[]11", align 4
  %ADD13 = add i32 %ADD9, %"$c.v[]12"
  ret i32 %ADD13
}
```

## 061 — name error: a struct declared twice

```stderr
error: test.ura:6:8 struct 'User' is already declared, on line 3
  |
6 | struct User:
  |        ^^^^
error: aborting due to 1 error
1
```

## 062 — name error: unknown attribute

```stderr
error: test.ura:10:14 'User' has no attribute 'age'
   |
10 |     return u.age
   |              ^^^
help: User has: name, id
error: aborting due to 1 error
1
```

## 063 — name error: unknown method

```stderr
error: test.ura:12:14 'User' has no method 'run'
   |
12 |     return u.run()
   |              ^^^
help: User has: get
error: aborting due to 1 error
1
```

## 064 — name error: self outside a method

```stderr
error: test.ura:4:12 'self' only exists inside a method
  |
4 |     return self.a
  |            ^^^^
error: aborting due to 1 error
1
```

## 065 — structs: missing name

```stderr
error: test.ura:2:8 expected a struct name after 'struct'
  |
2 | struct :
  |        ^
help: use struct Name :
error: aborting due to 1 error
1
```

## 066 — structs: missing colon

```stderr
error: test.ura:2:8 expected ':' after the struct name
  |
2 | struct User
  |        ^^^^
help: use struct User:
error: aborting due to 1 error
1
```

## 067 — structs: expression inside a struct

```stderr
error: test.ura:4:7 a struct holds only attributes and methods
  |
4 |     1 + 2
  |       ^
error: aborting due to 1 error
1
```

## 068 — structs: attribute without a type

```stderr
error: test.ura:3:5 attribute 'a' needs a type
  |
3 |     a
  |     ^
help: like: a i32
error: aborting due to 1 error
1
```

## 069 — structs: attribute declared twice

```stderr
error: test.ura:4:5 'a' is already declared, on line 3
  |
4 |     a i32
  |     ^
error: aborting due to 1 error
1
```

## 070 — structs: no attributes

```stderr
error: test.ura:2:8 struct 'E' has no attributes
  |
2 | struct E:
  |        ^
error: aborting due to 1 error
1
```

## 071 — structs: contains itself by value

```stderr
error: test.ura:3:10 struct 'N' can't contain itself
  |
3 |     next N
  |          ^
help: use a ref: '&N'
error: aborting due to 1 error
1
```

## 072 — structs: unknown attribute

```stderr
error: test.ura:7:14 'User' has no attribute 'zz'
  |
7 |     return u.zz
  |              ^^
help: User has: a
error: aborting due to 1 error
1
```

## 073 — structs: unknown method

```stderr
error: test.ura:7:14 'User' has no method 'zz'
  |
7 |     return u.zz()
  |              ^^
error: aborting due to 1 error
1
```

## 074 — structs: method with wrong arguments

```stderr
error: test.ura:10:14 'User' has no method 'add' that takes (i32, i32)
   |
10 |     return u.add(1, 2)
   |              ^^^
help: 'add' takes (i32)
error: aborting due to 1 error
1
```

## 075 — structs: bare attribute inside a method

```stderr
error: test.ura:6:16 undeclared variable 'value'
  |
6 |         return value
  |                ^^^^^
help: did you mean 'self.value'?
error: aborting due to 1 error
1
```

## 076 — structs: bare method call inside a method

```stderr
error: test.ura:9:16 no function named 'get'
  |
9 |         return get() * 2
  |                ^^^
help: did you mean 'self.get()'?
error: aborting due to 1 error
1
```

## 077 — structs: method called on the type

```stderr
error: test.ura:9:12 'User' is a type, not a value
  |
9 |     return User.get()
  |            ^^^^
error: aborting due to 1 error
1
```

## 078 — structs: self outside a method

```stderr
error: test.ura:3:12 'self' only exists inside a method
  |
3 |     return self.a
  |            ^^^^
error: aborting due to 1 error
1
```

## 079 — expressions: add a struct and a number

```stderr
error: test.ura:7:14 'User' has no operator '+' that takes 'i32'
  |
7 |     return u + 1
  |              ^
error: test.ura:7:14 expected 'i32', found 'User'
  |
7 |     return u + 1
  |              ^
error: aborting due to 2 errors
1
```

## 080 — expressions: compare structs

```stderr
error: test.ura:8:10 'User' has no operator '==' that takes 'User'
  |
8 |     if u == v:
  |          ^^
error: aborting due to 1 error
1
```

## 081 — types: struct into another struct

```stderr
error: test.ura:10:14 expected 'User', found 'Addr'
   |
10 |     u User = v
   |              ^
error: aborting due to 1 error
1
```

## 082 — struct: init a struct

```llvm
%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 0, i32* %c, align 4
  store %User zeroinitializer, %User* %u, align 4
  ret i32 0
}
```

## 083 — struct: modify struct attribute

```llvm
%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 0, i32* %c, align 4
  store %User zeroinitializer, %User* %u, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 10, i32* %u.b, align 4
  ret i32 0
}
```

## 084 — struct: method reads and writes self

```llvm
define i32 @main() {
entry:
  ret i32 0
}
```

## 085 — struct: overloaded method

```llvm
%User = type { i32 }

define void @User.set(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  store i32 1, i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

## 086 — struct: linked list

```llvm
%Node = type { i32, %Node* }

define %Node @create(i32 %0) {
entry:
  %value = alloca i32, align 4
  %res = alloca %Node, align 8
  store i32 %0, i32* %value, align 4
  store %Node zeroinitializer, %Node* %res, align 8
  %res.value = getelementptr inbounds %Node, %Node* %res, i32 0, i32 0
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %res.value, align 4
  %res.next = getelementptr inbounds %Node, %Node* %res, i32 0, i32 1
  store %Node* null, %Node** %res.next, align 8
  %"$res" = load %Node, %Node* %res, align 8
  ret %Node %"$res"
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %p = alloca %Node*, align 8
  %sum = alloca i32, align 4
  %call.create = call %Node @create(i32 1)
  store %Node %call.create, %Node* %a, align 8
  %call.create1 = call %Node @create(i32 2)
  store %Node %call.create1, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Node* %a, %Node** %p, align 8
  store i32 0, i32* %sum, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %"*p", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*p2" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p2", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*p.value"
  store i32 %ADD, i32* %sum, align 4
  %"*p3" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p3", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 0
}
```

## 087 — program: linked list walked through ref attributes

```llvm
%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %b = alloca %Node, align 8
  %a = alloca %Node, align 8
  %sum = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %c.next = getelementptr inbounds %Node, %Node* %c, i32 0, i32 1
  store %Node* null, %Node** %c.next, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %"*p", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*p.value"
  store i32 %ADD, i32* %sum, align 4
  %"*p2" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p2", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %a.next3 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next3, align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.value" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 0
  %"$**a.next.next.value" = load i32, i32* %"**a.next.next.value", align 4
  %MUL = mul i32 %"$**a.next.next.value", 10
  %"$sum4" = load i32, i32* %sum, align 4
  %ADD5 = add i32 %"$sum4", %MUL
  ret i32 %ADD5
}
```

## 088 — method: a method reads self

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [21 x i8] c"room %s on floor %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

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

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.name" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"*self1" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self1", i32 0, i32 1
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i8* %"$*self.name", i32 %"$*self.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i8* %"$*self.name", i32 %"$*self.floor")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  call void @"io::out"(%String %3)
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %r.name, align 8
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  store i32 2, i32* %r.floor, align 4
  call void @Room.describe(%Room* %r)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 089 — method: a method mutates through self, and the caller sees it

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"floor = %d\0A\00", align 1

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

define void @Room.climb(%Room* %0, i32 %1) {
entry:
  %self = alloca %Room*, align 8
  %n = alloca i32, align 4
  store %Room* %0, %Room** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"*self1" = load %Room*, %Room** %self, align 8
  %"*self.floor2" = getelementptr inbounds %Room, %Room* %"*self1", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor2", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.floor", %"$n"
  store i32 %ADD, i32* %"*self.floor", align 4
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 2, i32* %r.floor, align 4
  call void @Room.climb(%Room* %r, i32 3)
  %r.floor1 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
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

## 090 — method: a method with a return value

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"depth = %d\0A\00", align 1

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

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %MUL = mul i32 %"$*self.floor", 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 4, i32* %r.floor, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %r)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
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

## 091 — method: a method calling a sibling method declared below it

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }

@str = private unnamed_addr constant [10 x i8] c"floor %d\0A\00", align 1

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

define void @Room.show(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %call.Room.twice = call i32 @Room.twice(%Room* %"*self")
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call.Room.twice)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call.Room.twice)
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

define i32 @Room.twice(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %MUL = mul i32 %"$*self.floor", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 5, i32* %r.floor, align 4
  call void @Room.show(%Room* %r)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 092 — method: two structs may declare the same method name

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [18 x i8] c"room on floor %d\0A\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"tower of height %d\0A\00", align 1

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

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$*self.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$*self.floor")
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

define void @Tower.describe(%Tower* %0) {
entry:
  %self = alloca %Tower*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Tower* %0, %Tower** %self, align 8
  %"*self" = load %Tower*, %Tower** %self, align 8
  %"*self.height" = getelementptr inbounds %Tower, %Tower* %"*self", i32 0, i32 0
  %"$*self.height" = load i32, i32* %"*self.height", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.height")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.height")
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
  %r = alloca %Room, align 8
  %t = alloca %Tower, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 1, i32* %r.floor, align 4
  store %Tower zeroinitializer, %Tower* %t, align 4
  %t.height = getelementptr inbounds %Tower, %Tower* %t, i32 0, i32 0
  store i32 9, i32* %t.height, align 4
  call void @Room.describe(%Room* %r)
  call void @Tower.describe(%Tower* %t)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 094 — method: a method on a nested struct field

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }
%Dungeon = type { %Room }

@str = private unnamed_addr constant [16 x i8] c"entry depth %d\0A\00", align 1

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

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  ret i32 %"$*self.floor"
}

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 4
  %d.entry = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %d.entry.floor = getelementptr inbounds %Room, %Room* %d.entry, i32 0, i32 0
  store i32 3, i32* %d.entry.floor, align 4
  %d.entry1 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %call.Room.depth = call i32 @Room.depth(%Room* %d.entry1)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
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

## 095 — method: calling a method the struct does not have

```stderr
error: test.ura:9:7 'Room' has no method 'nope'
  |
9 |     r.nope()
  |       ^^^^
error: aborting due to 1 error
1
```

## 096 — method: calling a method on a non-struct

```stderr
error: test.ura:6:5 'x' is 'i32', not a struct
  |
6 |     x.nope()
  |     ^
error: aborting due to 1 error
1
```

## 097 — method: wrong argument count to a method

```stderr
error: test.ura:12:7 'Room' has no method 'climb' that takes ()
   |
12 |     r.climb()
   |       ^^^^^
help: 'climb' takes (i32)
error: aborting due to 1 error
1
```

## 098 — method: chaining a field access onto a call

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }

@str = private unnamed_addr constant [11 x i8] c"static %d\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"plain  %d\0A\00", align 1

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

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define %Room @make(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define i32 @main() {
entry:
  %tmp = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp6 = alloca %Room, align 8
  %format.size8 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  %call.Room.create = call %Room @Room.create(i32 7)
  store %Room %call.Room.create, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %"$tmp.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %"$tmp.floor")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.make = call %Room @make(i32 3)
  store %Room %call.make, %Room* %tmp6, align 4
  %tmp.floor7 = getelementptr inbounds %Room, %Room* %tmp6, i32 0, i32 0
  %"$tmp.floor9" = load i32, i32* %tmp.floor7, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor9")
  store i32 %call.snprintf10, i32* %format.size8, align 4
  %"$format.size11" = load i32, i32* %format.size8, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor9")
  %"$format.size18" = load i32, i32* %format.size8, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size8, align 4
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

## 099 — struct: declare a struct and a local: named type, alloca, zero-init

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Player = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %p = alloca %Player, align 8
  store %Player zeroinitializer, %Player* %p, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 100 — struct: a field whose type is another struct

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 101 — struct: forward reference: a field type declared later in the file

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 102 — struct: a struct declared inside a function body

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Local = type { i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 103 — struct: a struct declared inside an if block

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Inner = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %n = alloca i32, align 4
  %i = alloca %Inner, align 8
  store i32 1, i32* %n, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 1
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  store %Inner zeroinitializer, %Inner* %i, align 4
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 104 — struct: a struct declared inside a loop body

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Range = type { i32, i32, i32 }
%Tick = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %range = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %range, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*range" = load %Range*, %Range** %range, align 8
  %"*range.start" = getelementptr inbounds %Range, %Range* %"*range", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*range1" = load %Range*, %Range** %range, align 8
  %"*range.step" = getelementptr inbounds %Range, %Range* %"*range1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*range.step" = load i32, i32* %"*range.step", align 4
  %MUL = mul i32 %"$*i", %"$*range.step"
  %"$*range.start" = load i32, i32* %"*range.start", align 4
  %ADD = add i32 %"$*range.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*range2" = load %Range*, %Range** %range, align 8
  %"*range.step3" = getelementptr inbounds %Range, %Range* %"*range2", i32 0, i32 2
  %"$*range.step4" = load i32, i32* %"*range.step3", align 4
  %GT = icmp sgt i32 %"$*range.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*range6" = load %Range*, %Range** %range, align 8
  %"*range.stop" = getelementptr inbounds %Range, %Range* %"*range6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*range.stop" = load i32, i32* %"*range.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*range.stop"
  %AND = and i1 %GT, %GE
  %"*range7" = load %Range*, %Range** %range, align 8
  %"*range.step8" = getelementptr inbounds %Range, %Range* %"*range7", i32 0, i32 2
  %"$*range.step9" = load i32, i32* %"*range.step8", align 4
  %LT = icmp slt i32 %"$*range.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*range11" = load %Range*, %Range** %range, align 8
  %"*range.stop12" = getelementptr inbounds %Range, %Range* %"*range11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*range.stop14" = load i32, i32* %"*range.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*range.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if7

if7:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  ret i1 false

endif:                                            ; preds = %if7
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
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
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i1 = alloca i32, align 4
  %t = alloca %Tick, align 8
  %call.Range.create = call %Range @Range.create(i32 0, i32 2)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i1, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i1)
  br i1 %call.Range.in, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  store %Tick zeroinitializer, %Tick* %t, align 4
  br label %while8

endloop:                                          ; preds = %while8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 105 — struct: an array-of-struct field is a fat pointer to the named type

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Tree = type { i8*, %Room* }
%Room = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 106 — struct: a struct nested inside another struct's body

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Outer = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %o = alloca %Outer, align 8
  store %Outer zeroinitializer, %Outer* %o, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 107 — struct: self reference through an array stays finite

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Tree = type { i8*, %Tree* }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 108 — struct: an unused struct emits no type: LLVM drops unreferenced named types

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 109 — struct: the same struct name in three nested scopes: innermost wins

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Local.0 = type { i32, i32 }
%Local.1 = type { i8* }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

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
  %l0 = alloca %Local.0, align 8
  %n = alloca i32, align 4
  %l3 = alloca %Local.1, align 8
  %l = alloca %Local.0, align 8
  store %Local.0 zeroinitializer, %Local.0* %l0, align 4
  store i32 1, i32* %n, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 1
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  store %Local.1 zeroinitializer, %Local.1* %l3, align 8
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  store %Local.0 zeroinitializer, %Local.0* %l, align 4
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 110 — struct: read and write a field

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [7 x i8] c"armory\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"%s on floor %d\0A\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"floor now %d\0A\00", align 1

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
  %p = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size13 = alloca i32, align 4
  %format.text19 = alloca i8*, align 8
  store %Room zeroinitializer, %Room* %p, align 8
  %p.name = getelementptr inbounds %Room, %Room* %p, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %p.name, align 8
  %p.floor = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  store i32 3, i32* %p.floor, align 4
  %p.name1 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 0
  %p.floor2 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.name" = load i8*, i8** %p.name1, align 8
  %"$p.floor" = load i32, i32* %p.floor2, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i8* %"$p.name", i32 %"$p.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i8* %"$p.name", i32 %"$p.floor")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %p.floor8 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %p.floor9 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.floor10" = load i32, i32* %p.floor9, align 4
  %ADD11 = add i32 %"$p.floor10", 10
  store i32 %ADD11, i32* %p.floor8, align 4
  %p.floor12 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.floor14" = load i32, i32* %p.floor12, align 4
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i32 %"$p.floor14")
  store i32 %call.snprintf15, i32* %format.size13, align 4
  %"$format.size16" = load i32, i32* %format.size13, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %AS18 = sext i32 %ADD17 to i64
  %call.calloc20 = call i8* @calloc(i64 %AS18, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc20, i8** %format.text19, align 8
  %"$format.text21" = load i8*, i8** %format.text19, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text21", i64 %AS18, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i32 %"$p.floor14")
  %"$format.size23" = load i32, i32* %format.size13, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text19, align 8
  %"$format.size26" = load i32, i32* %format.size13, align 4
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

## 111 — struct: nested field access through a struct field

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.2 = private unnamed_addr constant [22 x i8] c"%s entry %s floor %d\0A\00", align 1

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
  %d = alloca %Dungeon, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %d.name = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %d.name, align 8
  %d.entry = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.name = getelementptr inbounds %Room, %Room* %d.entry, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %d.entry.name, align 8
  %d.entry1 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.floor = getelementptr inbounds %Room, %Room* %d.entry1, i32 0, i32 1
  store i32 1, i32* %d.entry.floor, align 4
  %d.name2 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %d.entry3 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.name4 = getelementptr inbounds %Room, %Room* %d.entry3, i32 0, i32 0
  %d.entry5 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.floor6 = getelementptr inbounds %Room, %Room* %d.entry5, i32 0, i32 1
  %"$d.name" = load i8*, i8** %d.name2, align 8
  %"$d.entry.name" = load i8*, i8** %d.entry.name4, align 8
  %"$d.entry.floor" = load i32, i32* %d.entry.floor6, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.2, i32 0, i32 0), i8* %"$d.name", i8* %"$d.entry.name", i32 %"$d.entry.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.2, i32 0, i32 0), i8* %"$d.name", i8* %"$d.entry.name", i32 %"$d.entry.floor")
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

## 115 — struct: 'stack' and 'heap' are ordinary identifiers

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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
  %stack = alloca i32, align 4
  %heap = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 5, i32* %stack, align 4
  store i32 7, i32* %heap, align 4
  %"$stack" = load i32, i32* %stack, align 4
  %"$heap" = load i32, i32* %heap, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$stack", i32 %"$heap")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$stack", i32 %"$heap")
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

## 116 — struct: a 'ref?' field may refer to its own struct

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Node = type { i32, %Node* }

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$a.value")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$a.value")
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

## 117 — struct: bind, read and write through a 'ref?' field

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Node = type { i32, %Node* }

@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %a.next1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next1, align 8
  %"*a.next.value" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 0
  store i32 20, i32* %"*a.next.value", align 4
  %a.next2 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next3" = load %Node*, %Node** %a.next2, align 8
  %"*a.next.value4" = getelementptr inbounds %Node, %Node* %"*a.next3", i32 0, i32 0
  %a.next5 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next6" = load %Node*, %Node** %a.next5, align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next6", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.value" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 0
  %b.value7 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$*a.next.value" = load i32, i32* %"*a.next.value4", align 4
  %"$**a.next.next.value" = load i32, i32* %"**a.next.next.value", align 4
  %"$b.value" = load i32, i32* %b.value7, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*a.next.value", i32 %"$**a.next.next.value", i32 %"$b.value")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*a.next.value", i32 %"$**a.next.next.value", i32 %"$b.value")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 118 — struct: struct assignment copies by value

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"%s %d / %s %d\0A\00", align 1

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
  %a = alloca %Room, align 8
  %b = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.name = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %a.name, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  %"$a" = load %Room, %Room* %a, align 8
  store %Room %"$a", %Room* %b, align 8
  %b.floor = getelementptr inbounds %Room, %Room* %b, i32 0, i32 1
  store i32 9, i32* %b.floor, align 4
  %b.name = getelementptr inbounds %Room, %Room* %b, i32 0, i32 0
  %b.floor1 = getelementptr inbounds %Room, %Room* %b, i32 0, i32 1
  %a.name2 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  %a.floor3 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$b.name" = load i8*, i8** %b.name, align 8
  %"$b.floor" = load i32, i32* %b.floor1, align 4
  %"$a.name" = load i8*, i8** %a.name2, align 8
  %"$a.floor" = load i32, i32* %a.floor3, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i8* %"$b.name", i32 %"$b.floor", i8* %"$a.name", i32 %"$a.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i8* %"$b.name", i32 %"$b.floor", i8* %"$a.name", i32 %"$a.floor")
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

## 119 — struct: a struct as a by-value parameter

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [12 x i8] c"room %s@%d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

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

define void @show(%Room %0) {
entry:
  %r = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room %0, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$r.name" = load i8*, i8** %r.name, align 8
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$r.name", i32 %"$r.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$r.name", i32 %"$r.floor")
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
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.name = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %a.name, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  %"$a" = load %Room, %Room* %a, align 8
  call void @show(%Room %"$a")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 120 — struct: a 'ref' struct parameter mutates the caller's struct

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1

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

define void @bump(%Room* %0) {
entry:
  %r = alloca %Room*, align 8
  store %Room* %0, %Room** %r, align 8
  %"*r" = load %Room*, %Room** %r, align 8
  %"*r.floor" = getelementptr inbounds %Room, %Room* %"*r", i32 0, i32 1
  %"*r1" = load %Room*, %Room** %r, align 8
  %"*r.floor2" = getelementptr inbounds %Room, %Room* %"*r1", i32 0, i32 1
  %"$*r.floor" = load i32, i32* %"*r.floor2", align 4
  %ADD = add i32 %"$*r.floor", 1
  store i32 %ADD, i32* %"*r.floor", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  call void @bump(%Room* %a)
  %a.floor1 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$a.floor" = load i32, i32* %a.floor1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$a.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$a.floor")
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

## 121 — struct: returning a struct: the create() constructor convention

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"made\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1

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

define %Room @create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %r.name, align 8
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 8
  ret %Room %"$r"
}

define i8* @describe(%Room %0) {
entry:
  %r = alloca %Room, align 8
  store %Room %0, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.name" = load i8*, i8** %r.name, align 8
  ret i8* %"$r.name"
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.create = call %Room @create(i32 4)
  store %Room %call.create, %Room* %a, align 8
  %"$a" = load %Room, %Room* %a, align 8
  %call.describe = call i8* @describe(%Room %"$a")
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %call.describe, i32 %"$a.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %call.describe, i32 %"$a.floor")
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

## 122 — struct: a field declared twice

```stderr
error: test.ura:7:5 'hp' is already declared, on line 6
  |
7 |     hp   i32
  |     ^^
error: aborting due to 1 error
1
```

## 123 — struct: a field whose type does not exist

```stderr
error: test.ura:5:11 unknown type 'Room'
  |
5 |     entry Room
  |           ^^^^
help: declare it: struct Room:
error: aborting due to 1 error
1
```

## 124 — struct: a struct that contains itself by value

```stderr
error: test.ura:6:11 struct 'Node' can't contain itself
  |
6 |     next  Node
  |           ^^^^
help: use a ref: '&Node'
error: aborting due to 1 error
1
```

## 125 — struct: two structs that contain each other by value

```stderr
error: test.ura:5:7 struct 'A' can't contain itself
  |
5 |     b B
  |       ^
help: use a ref: '&B'
error: aborting due to 1 error
1
```

## 126 — struct: a variable of a type that was never declared

```stderr
error: test.ura:5:7 unknown type 'Player'
  |
5 |     p Player
  |       ^^^^^^
help: declare it: struct Player:
error: aborting due to 1 error
1
```

## 127 — struct: reading a field the struct does not have

```stderr
error: test.ura:10:18 'Player' has no attribute 'nope'
   |
10 |     io::out($"{p.nope}")
   |                  ^^^^
help: Player has: name, hp
error: aborting due to 1 error
1
```

## 128 — struct: reading a field from a scalar

```stderr
error: test.ura:6:16 'x' is 'i32', not a struct
  |
6 |     io::out($"{x.foo}")
  |                ^
error: aborting due to 1 error
1
```

## 130 — struct: a struct declared in a block is not visible outside it

```stderr
error: test.ura:10:7 unknown type 'Inner'
   |
10 |     i Inner
   |       ^^^^^
help: declare it: struct Inner:
error: aborting due to 1 error
1
```

## 131 — struct: a struct declared twice in the same scope

```stderr
error: test.ura:7:8 struct 'Player' is already declared, on line 4
  |
7 | struct Player:
  |        ^^^^^^
error: aborting due to 1 error
1
```

## 132 — struct: a method and a field chain called on a ref? field

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Item = type { i32, %Item* }

@str = private unnamed_addr constant [10 x i8] c"worth %d\0A\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"chained field %d\0A\00", align 1

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

define %Item @Item.create(i32 %0) {
entry:
  %w = alloca i32, align 4
  %it = alloca %Item, align 8
  store i32 %0, i32* %w, align 4
  store %Item zeroinitializer, %Item* %it, align 8
  %it.worth = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %"$w" = load i32, i32* %w, align 4
  store i32 %"$w", i32* %it.worth, align 4
  %"$it" = load %Item, %Item* %it, align 8
  ret %Item %"$it"
}

define void @Item.show(%Item* %0) {
entry:
  %self = alloca %Item*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Item* %0, %Item** %self, align 8
  %"*self" = load %Item*, %Item** %self, align 8
  %"*self.worth" = getelementptr inbounds %Item, %Item* %"*self", i32 0, i32 0
  %"$*self.worth" = load i32, i32* %"*self.worth", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.worth")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.worth")
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
  %head = alloca %Item, align 8
  %mid = alloca %Item, align 8
  %tail = alloca %Item, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.Item.create = call %Item @Item.create(i32 5)
  store %Item %call.Item.create, %Item* %head, align 8
  %call.Item.create1 = call %Item @Item.create(i32 7)
  store %Item %call.Item.create1, %Item* %mid, align 8
  %call.Item.create2 = call %Item @Item.create(i32 9)
  store %Item %call.Item.create2, %Item* %tail, align 8
  %head.next = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  store %Item* %mid, %Item** %head.next, align 8
  %mid.next = getelementptr inbounds %Item, %Item* %mid, i32 0, i32 1
  store %Item* %tail, %Item** %mid.next, align 8
  call void @Item.show(%Item* %head)
  %head.next3 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next" = load %Item*, %Item** %head.next3, align 8
  call void @Item.show(%Item* %"*head.next")
  %head.next4 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next5" = load %Item*, %Item** %head.next4, align 8
  %"*head.next.next" = getelementptr inbounds %Item, %Item* %"*head.next5", i32 0, i32 1
  %"**head.next.next" = load %Item*, %Item** %"*head.next.next", align 8
  call void @Item.show(%Item* %"**head.next.next")
  %head.next6 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next7" = load %Item*, %Item** %head.next6, align 8
  %"*head.next.next8" = getelementptr inbounds %Item, %Item* %"*head.next7", i32 0, i32 1
  %"**head.next.next9" = load %Item*, %Item** %"*head.next.next8", align 8
  %"**head.next.next.worth" = getelementptr inbounds %Item, %Item* %"**head.next.next9", i32 0, i32 0
  %"$**head.next.next.worth" = load i32, i32* %"**head.next.next.worth", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 %"$**head.next.next.worth")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 %"$**head.next.next.worth")
  %"$format.size11" = load i32, i32* %format.size, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %"$format.text13" = load i8*, i8** %format.text, align 8
  %"$format.size14" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text13", 0
  %1 = insertvalue %String %0, i32 %"$format.size14", 1
  %2 = insertvalue %String %1, i32 %ADD12, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 133 — struct: struct redefinition

```stderr
error: test.ura:9:8 struct 'Point' is already declared, on line 5
  |
9 | struct Point:
  |        ^^^^^
error: aborting due to 1 error
1
```

## 134 — struct: accessing an undefined field

```stderr
error: test.ura:12:7 'Point' has no attribute 'z'
   |
12 |     p.z = 5
   |       ^
help: Point has: x, y
error: aborting due to 1 error
1
```

## 135 — struct: calling undefined method on struct

```stderr
error: test.ura:12:7 'Point' has no method 'normalize'
   |
12 |     p.normalize()
   |       ^^^^^^^^^
error: aborting due to 1 error
1
```

## 137 — struct: a struct as a parameter and as a return value

```llvm
%Point = type { i32, i32 }

define i32 @sum(%Point %0) {
entry:
  %p = alloca %Point, align 8
  store %Point %0, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$p.x" = load i32, i32* %p.x, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %ADD = add i32 %"$p.x", %"$p.y"
  ret i32 %ADD
}

define %Point @scale(%Point %0, i32 %1) {
entry:
  %p = alloca %Point, align 8
  %k = alloca i32, align 4
  store %Point %0, %Point* %p, align 4
  store i32 %1, i32* %k, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$p.x" = load i32, i32* %p.x, align 4
  %MUL = mul i32 %"$p.x", %"$k"
  store i32 %MUL, i32* %p.x, align 4
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$k1" = load i32, i32* %k, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %MUL2 = mul i32 %"$p.y", %"$k1"
  store i32 %MUL2, i32* %p.y, align 4
  %"$p" = load %Point, %Point* %p, align 4
  ret %Point %"$p"
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 3, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 4, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  %call.scale = call %Point @scale(%Point %"$a", i32 2)
  store %Point %call.scale, %Point* %b, align 4
  %"$a1" = load %Point, %Point* %a, align 4
  %call.sum = call i32 @sum(%Point %"$a1")
  %"$b" = load %Point, %Point* %b, align 4
  %call.sum2 = call i32 @sum(%Point %"$b")
  %ADD = add i32 %call.sum, %call.sum2
  ret i32 %ADD
}
```

## 138 — struct: assigning a struct copies it

```llvm
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 2, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  store %Point %"$a", %Point* %b, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 5, i32* %b.x, align 4
  %b.y = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y" = load i32, i32* %b.y, align 4
  %ADD = add i32 %"$b.y", 5
  store i32 %ADD, i32* %b.y, align 4
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %a.y2 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  %"$a.x" = load i32, i32* %a.x1, align 4
  %"$a.y" = load i32, i32* %a.y2, align 4
  %ADD3 = add i32 %"$a.x", %"$a.y"
  %b.x4 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  %"$b.x" = load i32, i32* %b.x4, align 4
  %ADD5 = add i32 %ADD3, %"$b.x"
  %b.y6 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y7" = load i32, i32* %b.y6, align 4
  %ADD8 = add i32 %ADD5, %"$b.y7"
  ret i32 %ADD8
}
```

## 139 — struct: a struct declared inside a function

```llvm
%Pair = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %p, align 4
  %p.a = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  store i32 4, i32* %p.a, align 4
  %p.b = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  store i32 5, i32* %p.b, align 4
  %p.a1 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  %p.b2 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.a" = load i32, i32* %p.a1, align 4
  %"$p.b" = load i32, i32* %p.b2, align 4
  %ADD = add i32 %"$p.a", %"$p.b"
  ret i32 %ADD
}
```

## 140 — struct: a nested struct as a parameter and a copy

```llvm
%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @size(%Rect %0) {
entry:
  %r = alloca %Rect, align 8
  store %Rect %0, %Rect* %r, align 4
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  %"$r.max.x" = load i32, i32* %r.max.x, align 4
  %"$r.min.x" = load i32, i32* %r.min.x, align 4
  %SUB = sub i32 %"$r.max.x", %"$r.min.x"
  %r.max1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max1, i32 0, i32 1
  %r.min2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min2, i32 0, i32 1
  %"$r.max.y" = load i32, i32* %r.max.y, align 4
  %"$r.min.y" = load i32, i32* %r.min.y, align 4
  %SUB3 = sub i32 %"$r.max.y", %"$r.min.y"
  %ADD = add i32 %SUB, %SUB3
  ret i32 %ADD
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  %c = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  store i32 1, i32* %r.min.x, align 4
  %r.min1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min1, i32 0, i32 1
  store i32 2, i32* %r.min.y, align 4
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  store i32 5, i32* %r.max.x, align 4
  %r.max2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max2, i32 0, i32 1
  store i32 8, i32* %r.max.y, align 4
  %"$r" = load %Rect, %Rect* %r, align 4
  store %Rect %"$r", %Rect* %c, align 4
  %"$c" = load %Rect, %Rect* %c, align 4
  %call.size = call i32 @size(%Rect %"$c")
  ret i32 %call.size
}
```

## 141 — struct: a struct value {x: 10, y: 11}: missing attributes are zero, nested, returned

```llvm
%Point = type { i32, i32 }
%Rect = type { %Point, %Point }

define %Point @origin() {
entry:
  ret %Point zeroinitializer
}

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %r = alloca %Rect, align 8
  %o = alloca %Point, align 8
  store %Point { i32 10, i32 11 }, %Point* %p, align 4
  store %Point { i32 3, i32 0 }, %Point* %q, align 4
  store %Rect { %Point { i32 1, i32 2 }, %Point { i32 5, i32 7 } }, %Rect* %r, align 4
  %call.origin = call %Point @origin()
  store %Point %call.origin, %Point* %o, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$p.x" = load i32, i32* %p.x, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %ADD = add i32 %"$p.x", %"$p.y"
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$q.x" = load i32, i32* %q.x, align 4
  %ADD1 = add i32 %ADD, %"$q.x"
  %q.y = getelementptr inbounds %Point, %Point* %q, i32 0, i32 1
  %"$q.y" = load i32, i32* %q.y, align 4
  %ADD2 = add i32 %ADD1, %"$q.y"
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  %"$r.min.x" = load i32, i32* %r.min.x, align 4
  %ADD3 = add i32 %ADD2, %"$r.min.x"
  %r.min4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min4, i32 0, i32 1
  %"$r.min.y" = load i32, i32* %r.min.y, align 4
  %ADD5 = add i32 %ADD3, %"$r.min.y"
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  %"$r.max.x" = load i32, i32* %r.max.x, align 4
  %ADD6 = add i32 %ADD5, %"$r.max.x"
  %r.max7 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max7, i32 0, i32 1
  %"$r.max.y" = load i32, i32* %r.max.y, align 4
  %ADD8 = add i32 %ADD6, %"$r.max.y"
  %o.x = getelementptr inbounds %Point, %Point* %o, i32 0, i32 0
  %"$o.x" = load i32, i32* %o.x, align 4
  %ADD9 = add i32 %ADD8, %"$o.x"
  ret i32 %ADD9
}
```

## 142 — struct: a struct value with an unknown attribute

```stderr
error: test.ura:8:22 'Point' has no attribute 'z'
  |
8 |     p Point = {x: 1, z: 2}
  |                      ^
error: aborting due to 1 error
1
```

## 143 — struct: a struct value given to an i32

```stderr
error: test.ura:4:13 expected 'i32', found a struct value
  |
4 |     n i32 = {x: 1}
  |             ^
error: aborting due to 1 error
1
```
