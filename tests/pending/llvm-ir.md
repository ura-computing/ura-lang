# pending llvm ir

## 001 — (was milestone02 061) arrays: size in the type

```stderr
error: test.ura:3:11 arrays have no size in the type
  |
3 |     x i32[3]
  |           ^
help: use 'i32[]'
error: aborting due to 1 error
1
```

## 003 — (was milestone02 127) variable: an array type carries no size

```stderr
error: test.ura:5:12 arrays have no size in the type
  |
5 |     xs i32[3]
  |            ^
help: use 'i32[]'
error: aborting due to 1 error
1
```

## 004 — (was milestone03 058) crazy: a linked list built with chained methods that return refs

```llvm
%Node = type { i32, %Node* }

@a = global %Node zeroinitializer
@b = global %Node zeroinitializer
@c = global %Node zeroinitializer
@d = global %Node zeroinitializer

define %Node* @Node.attach(%Node* %0, %Node* %1) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store %Node* %1, %Node** %n, align 8
  %"*self" = load %Node*, %Node** %self, align 8
  %"*self.next" = getelementptr inbounds %Node, %Node* %"*self", i32 0, i32 1
  %"*n" = load %Node*, %Node** %n, align 8
  store %Node* %"*n", %Node** %"*self.next", align 8
  %"*n1" = load %Node*, %Node** %n, align 8
  ret %Node* %"*n1"
}

define i32 @Node.length(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store i32 1, i32* %n, align 4
  %"*self" = load %Node*, %Node** %self, align 8
  store %Node* %"*self", %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  %EQ = icmp eq %Node* %"$*p.next", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.next2" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 1
  %"$*p.next3" = load %Node*, %Node** %"*p.next2", align 8
  store %Node* %"$*p.next3", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %"$n4" = load i32, i32* %n, align 4
  ret i32 %"$n4"
}

define i32 @main() {
entry:
  %ref = alloca %Node*, align 8
  %ref2 = alloca %Node*, align 8
  %ref5 = alloca %Node*, align 8
  store i32 4, i32* getelementptr inbounds (%Node, %Node* @d, i32 0, i32 0), align 4
  %call.Node.attach = call %Node* @Node.attach(%Node* @a, %Node* @b)
  store %Node* %call.Node.attach, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %call.Node.attach1 = call %Node* @Node.attach(%Node* %"*ref", %Node* @c)
  store %Node* %call.Node.attach1, %Node** %ref2, align 8
  %"*ref3" = load %Node*, %Node** %ref2, align 8
  %call.Node.attach4 = call %Node* @Node.attach(%Node* %"*ref3", %Node* @d)
  store %Node* %call.Node.attach4, %Node** %ref5, align 8
  %"*ref6" = load %Node*, %Node** %ref5, align 8
  %call.Node.length = call i32 @Node.length(%Node* @a)
  %MUL = mul i32 %call.Node.length, 10
  %"*a.next" = load %Node*, %Node** getelementptr inbounds (%Node, %Node* @a, i32 0, i32 1), align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.next" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 1
  %"***a.next.next.next" = load %Node*, %Node** %"**a.next.next.next", align 8
  %"***a.next.next.next.value" = getelementptr inbounds %Node, %Node* %"***a.next.next.next", i32 0, i32 0
  %"$***a.next.next.next.value" = load i32, i32* %"***a.next.next.next.value", align 4
  %ADD = add i32 %MUL, %"$***a.next.next.next.value"
  ret i32 %ADD
}
```

## 009 — (was milestone03 129) struct: '.len' on a scalar

```stderr
error: test.ura:6:16 'x' is 'i32', not a struct
  |
6 |     io::out($"{x.len}")
  |                ^
error: aborting due to 1 error
1
```

## 010 — (was milestone08 001) array: 2D array literal and nested indexing

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [23 x i8] c"m[0][0]=%d m[1][1]=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"m[0][1]=%d\0A\00", align 1

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
  %m = alloca i32**, align 8
  %arr = alloca [2 x i32], align 4
  %arr1 = alloca [2 x i32], align 4
  %arr2 = alloca [2 x i32*], align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size21 = alloca i32, align 4
  %format.text27 = alloca i8*, align 8
  store [2 x i32] [i32 1, i32 2], [2 x i32]* %arr, align 4
  %0 = getelementptr inbounds [2 x i32], [2 x i32]* %arr, i32 0, i32 0
  store [2 x i32] [i32 3, i32 4], [2 x i32]* %arr1, align 4
  %1 = getelementptr inbounds [2 x i32], [2 x i32]* %arr1, i32 0, i32 0
  %2 = insertvalue [2 x i32*] undef, i32* %0, 0
  %3 = insertvalue [2 x i32*] %2, i32* %1, 1
  store [2 x i32*] %3, [2 x i32*]* %arr2, align 8
  %4 = getelementptr inbounds [2 x i32*], [2 x i32*]* %arr2, i32 0, i32 0
  store i32** %4, i32*** %m, align 8
  %"$m" = load i32**, i32*** %m, align 8
  %"m[]" = getelementptr inbounds i32*, i32** %"$m", i32 0
  %"$m[]" = load i32*, i32** %"m[]", align 8
  %"m[][]" = getelementptr inbounds i32, i32* %"$m[]", i32 0
  %"$m3" = load i32**, i32*** %m, align 8
  %"m[]4" = getelementptr inbounds i32*, i32** %"$m3", i32 1
  %"$m[]5" = load i32*, i32** %"m[]4", align 8
  %"m[][]6" = getelementptr inbounds i32, i32* %"$m[]5", i32 1
  %"$m[][]" = load i32, i32* %"m[][]", align 4
  %"$m[][]7" = load i32, i32* %"m[][]6", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str, i32 0, i32 0), i32 %"$m[][]", i32 %"$m[][]7")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str, i32 0, i32 0), i32 %"$m[][]", i32 %"$m[][]7")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %5 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %6 = insertvalue %String %5, i32 %"$format.size12", 1
  %7 = insertvalue %String %6, i32 %ADD10, 2
  call void @"io::out"(%String %7)
  %"$m13" = load i32**, i32*** %m, align 8
  %"m[]14" = getelementptr inbounds i32*, i32** %"$m13", i32 0
  %"$m[]15" = load i32*, i32** %"m[]14", align 8
  %"m[][]16" = getelementptr inbounds i32, i32* %"$m[]15", i32 1
  store i32 99, i32* %"m[][]16", align 4
  %"$m17" = load i32**, i32*** %m, align 8
  %"m[]18" = getelementptr inbounds i32*, i32** %"$m17", i32 0
  %"$m[]19" = load i32*, i32** %"m[]18", align 8
  %"m[][]20" = getelementptr inbounds i32, i32* %"$m[]19", i32 1
  %"$m[][]22" = load i32, i32* %"m[][]20", align 4
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$m[][]22")
  store i32 %call.snprintf23, i32* %format.size21, align 4
  %"$format.size24" = load i32, i32* %format.size21, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc28 = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc28, i8** %format.text27, align 8
  %"$format.text29" = load i8*, i8** %format.text27, align 8
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text29", i64 %AS26, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$m[][]22")
  %"$format.size31" = load i32, i32* %format.size21, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %"$format.text33" = load i8*, i8** %format.text27, align 8
  %"$format.size34" = load i32, i32* %format.size21, align 4
  %8 = insertvalue %String zeroinitializer, i8* %"$format.text33", 0
  %9 = insertvalue %String %8, i32 %"$format.size34", 1
  %10 = insertvalue %String %9, i32 %ADD32, 2
  call void @"io::out"(%String %10)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 013 — (was milestone08 006) array: multi-dim heap: new<i32[]>(N), a row each, free each

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [34 x i8] c"m[0][0]=%d m[1][2]=%d m[0][1]=%d\0A\00", align 1

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

define i32** @"new<i32[]>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32**
  ret i32** %AS1
}

define void @"free<i32[]>"(i32** %0) {
entry:
  %data = alloca i32**, align 8
  store i32** %0, i32*** %data, align 8
  %"$data" = load i32**, i32*** %data, align 8
  %AS = bitcast i32** %"$data" to i8*
  call void @free(i8* %AS)
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

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %m = alloca i32**, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %"call.new<i32[]>" = call i32** @"new<i32[]>"(i32 2)
  store i32** %"call.new<i32[]>", i32*** %m, align 8
  %"$m" = load i32**, i32*** %m, align 8
  %"m[]" = getelementptr inbounds i32*, i32** %"$m", i32 0
  %"call.new<i32>" = call i32* @"new<i32>"(i32 3)
  store i32* %"call.new<i32>", i32** %"m[]", align 8
  %"$m1" = load i32**, i32*** %m, align 8
  %"m[]2" = getelementptr inbounds i32*, i32** %"$m1", i32 1
  %"call.new<i32>3" = call i32* @"new<i32>"(i32 3)
  store i32* %"call.new<i32>3", i32** %"m[]2", align 8
  %"$m4" = load i32**, i32*** %m, align 8
  %"m[]5" = getelementptr inbounds i32*, i32** %"$m4", i32 0
  %"$m[]" = load i32*, i32** %"m[]5", align 8
  %"m[][]" = getelementptr inbounds i32, i32* %"$m[]", i32 0
  store i32 1, i32* %"m[][]", align 4
  %"$m6" = load i32**, i32*** %m, align 8
  %"m[]7" = getelementptr inbounds i32*, i32** %"$m6", i32 1
  %"$m[]8" = load i32*, i32** %"m[]7", align 8
  %"m[][]9" = getelementptr inbounds i32, i32* %"$m[]8", i32 2
  store i32 9, i32* %"m[][]9", align 4
  %"$m10" = load i32**, i32*** %m, align 8
  %"m[]11" = getelementptr inbounds i32*, i32** %"$m10", i32 0
  %"$m[]12" = load i32*, i32** %"m[]11", align 8
  %"m[][]13" = getelementptr inbounds i32, i32* %"$m[]12", i32 0
  %"$m14" = load i32**, i32*** %m, align 8
  %"m[]15" = getelementptr inbounds i32*, i32** %"$m14", i32 1
  %"$m[]16" = load i32*, i32** %"m[]15", align 8
  %"m[][]17" = getelementptr inbounds i32, i32* %"$m[]16", i32 2
  %"$m18" = load i32**, i32*** %m, align 8
  %"m[]19" = getelementptr inbounds i32*, i32** %"$m18", i32 0
  %"$m[]20" = load i32*, i32** %"m[]19", align 8
  %"m[][]21" = getelementptr inbounds i32, i32* %"$m[]20", i32 1
  %"$m[][]" = load i32, i32* %"m[][]13", align 4
  %"$m[][]22" = load i32, i32* %"m[][]17", align 4
  %"$m[][]23" = load i32, i32* %"m[][]21", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0), i32 %"$m[][]", i32 %"$m[][]22", i32 %"$m[][]23")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0), i32 %"$m[][]", i32 %"$m[][]22", i32 %"$m[][]23")
  %"$format.size25" = load i32, i32* %format.size, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %"$format.text27" = load i8*, i8** %format.text, align 8
  %"$format.size28" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text27", 0
  %1 = insertvalue %String %0, i32 %"$format.size28", 1
  %2 = insertvalue %String %1, i32 %ADD26, 2
  call void @"io::out"(%String %2)
  %"$m29" = load i32**, i32*** %m, align 8
  %"m[]30" = getelementptr inbounds i32*, i32** %"$m29", i32 0
  %"$m[]31" = load i32*, i32** %"m[]30", align 8
  call void @"free<i32>"(i32* %"$m[]31")
  %"$m32" = load i32**, i32*** %m, align 8
  %"m[]33" = getelementptr inbounds i32*, i32** %"$m32", i32 1
  %"$m[]34" = load i32*, i32** %"m[]33", align 8
  call void @"free<i32>"(i32* %"$m[]34")
  %"$m35" = load i32**, i32*** %m, align 8
  call void @"free<i32[]>"(i32** %"$m35")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 015 — (was milestone08 012) array: .len on a non-array

```stderr
error: test.ura:5:16 'x' is 'i32', not a struct
  |
5 |     io::out($"{x.len}")
  |                ^
error: aborting due to 1 error
1
```

## 017 — (was milestone08 044) struct: a 2D heap array of structs frees every inner slice

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }

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

define %Room** @"new<Room[]>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room**
  ret %Room** %AS1
}

define void @"free<Room[]>"(%Room** %0) {
entry:
  %data = alloca %Room**, align 8
  store %Room** %0, %Room*** %data, align 8
  %"$data" = load %Room**, %Room*** %data, align 8
  %AS = bitcast %Room** %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define %Room* @"new<Room>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Room*
  ret %Room* %AS1
}

define void @"free<Room>"(%Room* %0) {
entry:
  %data = alloca %Room*, align 8
  store %Room* %0, %Room** %data, align 8
  %"$data" = load %Room*, %Room** %data, align 8
  %AS = bitcast %Room* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %g = alloca %Room**, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %"call.new<Room[]>" = call %Room** @"new<Room[]>"(i32 2)
  store %Room** %"call.new<Room[]>", %Room*** %g, align 8
  %"$g" = load %Room**, %Room*** %g, align 8
  %"g[]" = getelementptr inbounds %Room*, %Room** %"$g", i32 0
  %"call.new<Room>" = call %Room* @"new<Room>"(i32 2)
  store %Room* %"call.new<Room>", %Room** %"g[]", align 8
  %"$g1" = load %Room**, %Room*** %g, align 8
  %"g[]2" = getelementptr inbounds %Room*, %Room** %"$g1", i32 1
  %"call.new<Room>3" = call %Room* @"new<Room>"(i32 2)
  store %Room* %"call.new<Room>3", %Room** %"g[]2", align 8
  %"$g4" = load %Room**, %Room*** %g, align 8
  %"g[]5" = getelementptr inbounds %Room*, %Room** %"$g4", i32 0
  %"$g[]" = load %Room*, %Room** %"g[]5", align 8
  %"g[][]" = getelementptr inbounds %Room, %Room* %"$g[]", i32 0
  %"g[][].floor" = getelementptr inbounds %Room, %Room* %"g[][]", i32 0, i32 0
  store i32 1, i32* %"g[][].floor", align 4
  %"$g6" = load %Room**, %Room*** %g, align 8
  %"g[]7" = getelementptr inbounds %Room*, %Room** %"$g6", i32 1
  %"$g[]8" = load %Room*, %Room** %"g[]7", align 8
  %"g[][]9" = getelementptr inbounds %Room, %Room* %"$g[]8", i32 1
  %"g[][].floor10" = getelementptr inbounds %Room, %Room* %"g[][]9", i32 0, i32 0
  store i32 4, i32* %"g[][].floor10", align 4
  %"$g11" = load %Room**, %Room*** %g, align 8
  %"g[]12" = getelementptr inbounds %Room*, %Room** %"$g11", i32 0
  %"$g[]13" = load %Room*, %Room** %"g[]12", align 8
  %"g[][]14" = getelementptr inbounds %Room, %Room* %"$g[]13", i32 0
  %"g[][].floor15" = getelementptr inbounds %Room, %Room* %"g[][]14", i32 0, i32 0
  %"$g16" = load %Room**, %Room*** %g, align 8
  %"g[]17" = getelementptr inbounds %Room*, %Room** %"$g16", i32 1
  %"$g[]18" = load %Room*, %Room** %"g[]17", align 8
  %"g[][]19" = getelementptr inbounds %Room, %Room* %"$g[]18", i32 1
  %"g[][].floor20" = getelementptr inbounds %Room, %Room* %"g[][]19", i32 0, i32 0
  %"$g[][].floor" = load i32, i32* %"g[][].floor15", align 4
  %"$g[][].floor21" = load i32, i32* %"g[][].floor20", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$g[][].floor", i32 %"$g[][].floor21")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$g[][].floor", i32 %"$g[][].floor21")
  %"$format.size23" = load i32, i32* %format.size, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text, align 8
  %"$format.size26" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %1 = insertvalue %String %0, i32 %"$format.size26", 1
  %2 = insertvalue %String %1, i32 %ADD24, 2
  call void @"io::out"(%String %2)
  %"$g27" = load %Room**, %Room*** %g, align 8
  %"g[]28" = getelementptr inbounds %Room*, %Room** %"$g27", i32 0
  %"$g[]29" = load %Room*, %Room** %"g[]28", align 8
  call void @"free<Room>"(%Room* %"$g[]29")
  %"$g30" = load %Room**, %Room*** %g, align 8
  %"g[]31" = getelementptr inbounds %Room*, %Room** %"$g30", i32 1
  %"$g[]32" = load %Room*, %Room** %"g[]31", align 8
  call void @"free<Room>"(%Room* %"$g[]32")
  %"$g33" = load %Room**, %Room*** %g, align 8
  call void @"free<Room[]>"(%Room** %"$g33")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 023 — (was milestone13 015) function: a function-typed parameter given a value

```stderr
error: test.ura:7:10 parameter 'f' has no type
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |          ^
help: like: f i32
error: test.ura:7:12 expected ',' between parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
error: test.ura:7:12 expected ')' after the parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f) fn(i32) i32, x i32) i32:
error: test.ura:7:12 expected ':' after the function header
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f: fn(i32) i32, x i32) i32:
error: test.ura:7:15 expected ')' after the parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |               ^^^
help: use fn apply(f fn()i32) i32, x i32) i32:
error: test.ura:7:18 expected ':' after the function header
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
help: use fn apply(f fn(i32:) i32, x i32) i32:
error: test.ura:7:18 unexpected ')'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
error: test.ura:7:23 unexpected ','
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                       ^
error: test.ura:7:30 unexpected ')'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                              ^
error: test.ura:7:35 unexpected ':'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                                   ^
error: aborting due to 10 errors
1
```
