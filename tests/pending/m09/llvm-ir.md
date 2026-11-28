# m09 llvm ir

## 001 — method: a pub fn is static: no self, called with ::

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

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.Room.create = call %Room @Room.create(i32 7)
  store %Room %call.Room.create, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
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

## 002 — method: calling a non-pub method with ::

```stderr
error: 'Room' has no pub fn 'depth'
at test.ura:11:22
   |
11 |     io::out($"{Room::depth()}\n")
   |                      ^^^^^
error: aborting due to 1 error
1
```

## 003 — method: :: on a type that does not exist

```stderr
error: unknown type 'Nope'
at test.ura:5:16
  |
5 |     io::out($"{Nope::create()}\n")
  |                ^^^^
error: aborting due to 1 error
1
```

## 004 — method: two structs declare the same pub fn, and statics compose

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [18 x i8] c"room %d tower %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"zero %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"level %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"method %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"mixed %d\0A\00", align 1

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

define %Room @Room.zero() {
entry:
  %call.Room.create = call %Room @Room.create(i32 0)
  ret %Room %call.Room.create
}

define i32 @Room.level(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %"$f" = load i32, i32* %f, align 4
  %MUL = mul i32 %"$f", 10
  ret i32 %MUL
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

define %Tower @Tower.create(i32 %0) {
entry:
  %h = alloca i32, align 4
  %t = alloca %Tower, align 8
  store i32 %0, i32* %h, align 4
  store %Tower zeroinitializer, %Tower* %t, align 4
  %t.height = getelementptr inbounds %Tower, %Tower* %t, i32 0, i32 0
  %"$h" = load i32, i32* %h, align 4
  store i32 %"$h", i32* %t.height, align 4
  %"$t" = load %Tower, %Tower* %t, align 4
  ret %Tower %"$t"
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %b = alloca %Tower, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp = alloca %Room, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %tmp33 = alloca %Room, align 8
  %format.size34 = alloca i32, align 4
  %format.text39 = alloca i8*, align 8
  %n = alloca i32, align 4
  %tmp48 = alloca %Room, align 8
  %tmp51 = alloca %Tower, align 8
  %format.size53 = alloca i32, align 4
  %format.text58 = alloca i8*, align 8
  %call.Room.create = call %Room @Room.create(i32 3)
  store %Room %call.Room.create, %Room* %a, align 4
  %call.Tower.create = call %Tower @Tower.create(i32 9)
  store %Tower %call.Tower.create, %Tower* %b, align 4
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  %b.height = getelementptr inbounds %Tower, %Tower* %b, i32 0, i32 0
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %"$b.height" = load i32, i32* %b.height, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$a.floor", i32 %"$b.height")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$a.floor", i32 %"$b.height")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.Room.zero = call %Room @Room.zero()
  store %Room %call.Room.zero, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %call.Room.level = call i32 @Room.level(i32 4)
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %call.Room.level)
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %call.Room.level)
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %7 = insertvalue %String %6, i32 %"$format.size31", 1
  %8 = insertvalue %String %7, i32 %ADD29, 2
  call void @"io::out"(%String %8)
  %call.Room.create32 = call %Room @Room.create(i32 5)
  store %Room %call.Room.create32, %Room* %tmp33, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %tmp33)
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %call.Room.depth)
  store i32 %call.snprintf35, i32* %format.size34, align 4
  %"$format.size36" = load i32, i32* %format.size34, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %call.snprintf42 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %call.Room.depth)
  %"$format.size43" = load i32, i32* %format.size34, align 4
  %ADD44 = add i32 %"$format.size43", 1
  %"$format.text45" = load i8*, i8** %format.text39, align 8
  %"$format.size46" = load i32, i32* %format.size34, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text45", 0
  %10 = insertvalue %String %9, i32 %"$format.size46", 1
  %11 = insertvalue %String %10, i32 %ADD44, 2
  call void @"io::out"(%String %11)
  %call.Room.create47 = call %Room @Room.create(i32 2)
  store %Room %call.Room.create47, %Room* %tmp48, align 4
  %call.Room.depth49 = call i32 @Room.depth(%Room* %tmp48)
  %call.Tower.create50 = call %Tower @Tower.create(i32 1)
  store %Tower %call.Tower.create50, %Tower* %tmp51, align 4
  %tmp.height = getelementptr inbounds %Tower, %Tower* %tmp51, i32 0, i32 0
  %"$tmp.height" = load i32, i32* %tmp.height, align 4
  %ADD52 = add i32 %call.Room.depth49, %"$tmp.height"
  store i32 %ADD52, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$n")
  store i32 %call.snprintf54, i32* %format.size53, align 4
  %"$format.size55" = load i32, i32* %format.size53, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %AS57 = sext i32 %ADD56 to i64
  %call.calloc59 = call i8* @calloc(i64 %AS57, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc59, i8** %format.text58, align 8
  %"$format.text60" = load i8*, i8** %format.text58, align 8
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text60", i64 %AS57, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$n")
  %"$format.size62" = load i32, i32* %format.size53, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %"$format.text64" = load i8*, i8** %format.text58, align 8
  %"$format.size65" = load i32, i32* %format.size53, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text64", 0
  %13 = insertvalue %String %12, i32 %"$format.size65", 1
  %14 = insertvalue %String %13, i32 %ADD63, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 005 — operator: an operator inside a struct is an error

```stderr
error: 'operator' must be declared outside the struct
at test.ura:6:5
  |
6 |     operator +(other &Vec) Vec:
  |     ^^^^^^^^
help: move it after the struct, the left side is the first parameter
error: aborting due to 1 error
1
```

## 006 — operator: operator missing operator symbol

```stderr
error: expected '(' after the function name
at test.ura:9:11
  |
9 | operator (left &Vec) Vec:
  |           ^^^^
help: use operator (()left &Vec) Vec:
error: expected ':' after the function header
at test.ura:9:16
  |
9 | operator (left &Vec) Vec:
  |                ^
help: use operator (left: &Vec) Vec:
error: unexpected ')'
at test.ura:9:20
  |
9 | operator (left &Vec) Vec:
  |                    ^
error: unexpected ':'
at test.ura:9:25
  |
9 | operator (left &Vec) Vec:
  |                         ^
error: aborting due to 4 errors
1
```

## 007 — operator: adding two structs without an operator + overload

```stderr
error: 'Vec' has no operator '+' that takes 'Vec'
at test.ura:12:15
   |
12 |     c Vec = a + b
   |               ^
error: aborting due to 1 error
1
```

## 008 — operator: operator + on two structs

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*left.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  %c.y = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.y" = load i32, i32* %c.y, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$c.x", i32 %"$c.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$c.x", i32 %"$c.y")
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

## 009 — operator: two overloads of + with different right-hand types

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32 }

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

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD)
  ret %Vec %call.Vec.create
}

define %Vec @"Vec.+.1"(%Vec* %0, i32 %1) {
entry:
  %left = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*left.x", %"$n"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD)
  ret %Vec %call.Vec.create
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %tmp3 = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %tmp, align 4
  %tmp.x = getelementptr inbounds %Vec, %Vec* %tmp, i32 0, i32 0
  %"call.Vec.+2" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+2", %Vec* %tmp3, align 4
  %tmp.x4 = getelementptr inbounds %Vec, %Vec* %tmp3, i32 0, i32 0
  %"$tmp.x" = load i32, i32* %tmp.x, align 4
  %"$tmp.x5" = load i32, i32* %tmp.x4, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$tmp.x", i32 %"$tmp.x5")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$tmp.x", i32 %"$tmp.x5")
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

## 010 — operator: operator == returning bool

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32 }

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

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*left.x", %"$other.x"
  ret i1 %EQ
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text5 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %call.Vec.create3 = call %Vec @Vec.create(i32 9)
  store %Vec %call.Vec.create3, %Vec* %c, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b")
  %"$c" = load %Vec, %Vec* %c, align 4
  %"call.Vec.==4" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$c")
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  store i8* null, i8** %bool.text5, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==4", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text5, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text6")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text7" = load i8*, i8** %bool.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text5, align 8
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text7", i8* %"$bool.text8")
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

## 011 — operator: no overload for that operator

```stderr
error: 'Vec' has no operator '-' that takes 'Vec'
at test.ura:10:15
   |
10 |     c Vec = a - b
   |               ^
error: aborting due to 1 error
1
```

## 012 — operator: assigning a different struct

```stderr
error: expected 'Vec', found 'Point'
at test.ura:13:9
   |
13 |     v = p
   |         ^
error: aborting due to 1 error
1
```

## 013 — operator: indexing an array literal directly

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
  %arr = alloca [3 x i32], align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  %"VAL_CREATE[]" = getelementptr inbounds i32, i32* %0, i32 1
  %"$VAL_CREATE[]" = load i32, i32* %"VAL_CREATE[]", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$VAL_CREATE[]")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$VAL_CREATE[]")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 014 — operator: operators and a method on the same struct

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"(%d, %d)\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
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

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*left.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*left.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size11 = alloca i32, align 4
  %bool.text12 = alloca i8*, align 8
  %format.text18 = alloca i8*, align 8
  %tmp = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %"$a" = load %Vec, %Vec* %a, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$a")
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text3" = load i8*, i8** %bool.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text3")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  %"$b9" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==10" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b9")
  store i32 0, i32* %format.size11, align 4
  store i8* null, i8** %bool.text12, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==10", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text13" = load i8*, i8** %bool.text12, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text13")
  store i32 %call.snprintf14, i32* %format.size11, align 4
  %"$format.size15" = load i32, i32* %format.size11, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %AS17 = sext i32 %ADD16 to i64
  %call.calloc19 = call i8* @calloc(i64 %AS17, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc19, i8** %format.text18, align 8
  %"$format.text20" = load i8*, i8** %format.text18, align 8
  %"$bool.text21" = load i8*, i8** %bool.text12, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text20", i64 %AS17, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text21")
  %"$format.size23" = load i32, i32* %format.size11, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text18, align 8
  %"$format.size26" = load i32, i32* %format.size11, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %4 = insertvalue %String %3, i32 %"$format.size26", 1
  %5 = insertvalue %String %4, i32 %ADD24, 2
  call void @"io::out"(%String %5)
  %"$b27" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+28" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b27")
  store %Vec %"call.Vec.+28", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 015 — operator: assigning a void result

```stderr
error: expected 'Vec', found 'void'
at test.ura:12:14
   |
12 |     e Vec = a.show()
   |              ^
error: aborting due to 1 error
1
```

## 016 — operator: a binary operator with mismatched scalar types

```stderr
error: can't apply '+' to 'i32' and 'i8[]'
at test.ura:7:18
  |
7 |     io::out($"{n + s}\n")
  |                  ^
error: aborting due to 1 error
1
```

## 017 — operator: operator = copies on declaration and on reassignment

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"decl   %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"assign %d %d\0A\00", align 1

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

define void @"Vec.="(%Vec* %0, %Vec* %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec* %1, %Vec** %other, align 8
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*other" = load %Vec*, %Vec** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec, %Vec* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*left.x", align 4
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.tag" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  store i32 99, i32* %"*left.tag", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %copy = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %c = alloca %Vec, align 8
  %copy6 = alloca %Vec, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %a.x, align 4
  store %Vec zeroinitializer, %Vec* %copy, align 4
  call void @"Vec.="(%Vec* %copy, %Vec* %a)
  %"$copy" = load %Vec, %Vec* %copy, align 4
  store %Vec %"$copy", %Vec* %b, align 4
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  %b.tag = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 1
  %"$b.x" = load i32, i32* %b.x, align 4
  %"$b.tag" = load i32, i32* %b.tag, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$b.x", i32 %"$b.tag")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$b.x", i32 %"$b.tag")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store %Vec zeroinitializer, %Vec* %copy6, align 4
  call void @"Vec.="(%Vec* %copy6, %Vec* %a)
  %"$copy7" = load %Vec, %Vec* %copy6, align 4
  store %Vec %"$copy7", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  %c.tag = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.tag" = load i32, i32* %c.tag, align 4
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %"$c.x", i32 %"$c.tag")
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %"$c.x", i32 %"$c.tag")
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
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

## 018 — operator: compound assignment operators

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32 }

@str = private unnamed_addr constant [13 x i8] c"after += %d\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"after -= %d\0A\00", align 1

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

define void @"Vec.+="(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  store i32 %ADD, i32* %"*left.x", align 4
  ret void
}

define void @"Vec.-="(%Vec* %0, i32 %1) {
entry:
  %left = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*left.x", %"$n"
  store i32 %SUB, i32* %"*left.x", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 10, i32* %a.x, align 4
  store %Vec zeroinitializer, %Vec* %b, align 4
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  store i32 4, i32* %b.x, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  call void @"Vec.+="(%Vec* %a, %Vec %"$b")
  %a.x1 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  %"$a.x" = load i32, i32* %a.x1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a.x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a.x")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  call void @"Vec.-="(%Vec* %a, i32 3)
  %a.x7 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  %"$a.x9" = load i32, i32* %a.x7, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %"$a.x9")
  store i32 %call.snprintf10, i32* %format.size8, align 4
  %"$format.size11" = load i32, i32* %format.size8, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %"$a.x9")
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

## 019 — operator: a compound operator with no overload

```stderr
error: 'Vec' has no operator '*=' that takes 'Vec'
at test.ura:10:7
   |
10 |     a *= b
   |       ^^
error: aborting due to 1 error
1
```

## 020 — operator: return unwinds every scope, and keeps the returned local

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Tag = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"early %d\0A\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"g.id %d\0A\00", align 1

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

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %t = alloca %Tag, align 8
  store i32 %0, i32* %n, align 4
  store %Tag zeroinitializer, %Tag* %t, align 4
  %t.id = getelementptr inbounds %Tag, %Tag* %t, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %t.id, align 4
  %"$t" = load %Tag, %Tag* %t, align 4
  ret %Tag %"$t"
}

define void @Tag.drop(%Tag* %0) {
entry:
  %value = alloca %Tag*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Tag* %0, %Tag** %value, align 8
  %"*value" = load %Tag*, %Tag** %value, align 8
  %"*value.id" = getelementptr inbounds %Tag, %Tag* %"*value", i32 0, i32 0
  %"$*value.id" = load i32, i32* %"*value.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*value.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*value.id")
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

define i32 @early() {
entry:
  %a = alloca %Tag, align 8
  %b = alloca %Tag, align 8
  %call.Tag.create = call %Tag @Tag.create(i32 1)
  store %Tag %call.Tag.create, %Tag* %a, align 4
  br label %if6

if6:                                              ; preds = %entry
  br i1 true, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %call.Tag.create1 = call %Tag @Tag.create(i32 10)
  store %Tag %call.Tag.create1, %Tag* %b, align 4
  call void @Tag.drop(%Tag* %b)
  call void @Tag.drop(%Tag* %a)
  ret i32 7

endif:                                            ; preds = %if6
  call void @Tag.drop(%Tag* %a)
  ret i32 0
}

define %Tag @giveback() {
entry:
  %t = alloca %Tag, align 8
  %call.Tag.create = call %Tag @Tag.create(i32 99)
  store %Tag %call.Tag.create, %Tag* %t, align 4
  %"$t" = load %Tag, %Tag* %t, align 4
  ret %Tag %"$t"
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %g = alloca %Tag, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %call.early = call i32 @early()
  store i32 %call.early, i32* %r, align 4
  %"$r" = load i32, i32* %r, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$r")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$r")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.giveback = call %Tag @giveback()
  store %Tag %call.giveback, %Tag* %g, align 4
  %g.id = getelementptr inbounds %Tag, %Tag* %g, i32 0, i32 0
  %"$g.id" = load i32, i32* %g.id, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %"$g.id")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %"$g.id")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  call void @Tag.drop(%Tag* %g)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 021 — operator: every operator on one struct, and = from an i32 overload

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec = type { i32, i32, i32 }

@str = private unnamed_addr constant [17 x i8] c"(%d, %d) tag %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"*self2" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self2", i32 0, i32 2
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$*self.tag" = load i32, i32* %"*self.tag", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %2 = insertvalue %String %1, i32 %"$format.size7", 1
  %3 = insertvalue %String %2, i32 %ADD5, 2
  call void @"io::out"(%String %3)
  ret void
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*left.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define %Vec @"Vec.+.1"(%Vec* %0, i32 %1) {
entry:
  %left = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*left.x", %"$n"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %ADD3 = add i32 %"$*left.y", %"$n2"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD3)
  ret %Vec %call.Vec.create
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*left.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define void @"Vec.="(%Vec* %0, %Vec* %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec* %1, %Vec** %other, align 8
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*other" = load %Vec*, %Vec** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec, %Vec* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*left.x", align 4
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 1
  %"*other2" = load %Vec*, %Vec** %other, align 8
  %"*other.y" = getelementptr inbounds %Vec, %Vec* %"*other2", i32 0, i32 1
  %"$*other.y" = load i32, i32* %"*other.y", align 4
  store i32 %"$*other.y", i32* %"*left.y", align 4
  %"*left3" = load %Vec*, %Vec** %left, align 8
  %"*left.tag" = getelementptr inbounds %Vec, %Vec* %"*left3", i32 0, i32 2
  store i32 99, i32* %"*left.tag", align 4
  ret void
}

define void @"Vec.+="(%Vec* %0, %Vec %1) {
entry:
  %left = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %left, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  store i32 %ADD, i32* %"*left.x", align 4
  %"*left3" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec*, %Vec** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec, %Vec* %"*left4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD6 = add i32 %"$*left.y", %"$other.y"
  store i32 %ADD6, i32* %"*left.y", align 4
  ret void
}

define void @"Vec.-="(%Vec* %0, i32 %1) {
entry:
  %left = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec*, %Vec** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec, %Vec* %"*left", i32 0, i32 0
  %"*left1" = load %Vec*, %Vec** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec, %Vec* %"*left1", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*left.x", %"$n"
  store i32 %SUB, i32* %"*left.x", align 4
  %"*left3" = load %Vec*, %Vec** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec, %Vec* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec*, %Vec** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec, %Vec* %"*left4", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %SUB7 = sub i32 %"$*left.y", %"$n6"
  store i32 %SUB7, i32* %"*left.y", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %copy = alloca %Vec, align 8
  %d = alloca %Vec, align 8
  %copy4 = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %copy12 = alloca %Vec, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %copy31 = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %e = alloca %Vec, align 8
  %copy34 = alloca %Vec, align 8
  %f = alloca %Vec, align 8
  %copy36 = alloca %Vec, align 8
  %g = alloca %Vec, align 8
  %copy39 = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  store %Vec zeroinitializer, %Vec* %copy, align 4
  call void @"Vec.="(%Vec* %copy, %Vec* %b)
  %"$copy" = load %Vec, %Vec* %copy, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$copy")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %"call.Vec.+3" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+3", %Vec* %d, align 4
  call void @Vec.show(%Vec* %d)
  store %Vec zeroinitializer, %Vec* %copy4, align 4
  call void @"Vec.="(%Vec* %copy4, %Vec* %a)
  %"$copy5" = load %Vec, %Vec* %copy4, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$copy5")
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text6")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  store %Vec zeroinitializer, %Vec* %copy12, align 4
  call void @"Vec.="(%Vec* %copy12, %Vec* %b)
  %"$copy13" = load %Vec, %Vec* %copy12, align 4
  %"call.Vec.==14" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$copy13")
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==14", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  store %Vec zeroinitializer, %Vec* %copy31, align 4
  call void @"Vec.="(%Vec* %copy31, %Vec* %b)
  %"$copy32" = load %Vec, %Vec* %copy31, align 4
  %"call.Vec.+33" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$copy32")
  store %Vec %"call.Vec.+33", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  store %Vec zeroinitializer, %Vec* %copy34, align 4
  call void @"Vec.="(%Vec* %copy34, %Vec* %a)
  %"$copy35" = load %Vec, %Vec* %copy34, align 4
  store %Vec %"$copy35", %Vec* %e, align 4
  call void @Vec.show(%Vec* %e)
  store %Vec zeroinitializer, %Vec* %copy36, align 4
  call void @"Vec.="(%Vec* %copy36, %Vec* %a)
  %"$copy37" = load %Vec, %Vec* %copy36, align 4
  store %Vec %"$copy37", %Vec* %f, align 4
  call void @Vec.show(%Vec* %f)
  %call.Vec.create38 = call %Vec @Vec.create(i32 10, i32 10)
  store %Vec %call.Vec.create38, %Vec* %g, align 4
  store %Vec zeroinitializer, %Vec* %copy39, align 4
  call void @"Vec.="(%Vec* %copy39, %Vec* %b)
  %"$copy40" = load %Vec, %Vec* %copy39, align 4
  call void @"Vec.+="(%Vec* %g, %Vec %"$copy40")
  call void @Vec.show(%Vec* %g)
  call void @"Vec.-="(%Vec* %g, i32 3)
  call void @Vec.show(%Vec* %g)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 022 — operator: a temporary is destroyed at the end of its statement

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Plain = type { i32 }
%R = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"show %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"DROP %d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"end\0A\00", align 1

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

define %Plain @Plain.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %p = alloca %Plain, align 8
  store i32 %0, i32* %n, align 4
  store %Plain zeroinitializer, %Plain* %p, align 4
  %p.x = getelementptr inbounds %Plain, %Plain* %p, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %p.x, align 4
  %"$p" = load %Plain, %Plain* %p, align 4
  ret %Plain %"$p"
}

define %R @R.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %R, align 8
  store i32 %0, i32* %n, align 4
  store %R zeroinitializer, %R* %r, align 4
  %r.id = getelementptr inbounds %R, %R* %r, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %r.id, align 4
  %"$r" = load %R, %R* %r, align 4
  ret %R %"$r"
}

define void @R.show(%R* %0) {
entry:
  %self = alloca %R*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %R* %0, %R** %self, align 8
  %"*self" = load %R*, %R** %self, align 8
  %"*self.id" = getelementptr inbounds %R, %R* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
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

define void @"R.="(%R* %0, %R* %1) {
entry:
  %left = alloca %R*, align 8
  %other = alloca %R*, align 8
  store %R* %0, %R** %left, align 8
  store %R* %1, %R** %other, align 8
  %"*left" = load %R*, %R** %left, align 8
  %"*left.id" = getelementptr inbounds %R, %R* %"*left", i32 0, i32 0
  %"*other" = load %R*, %R** %other, align 8
  %"*other.id" = getelementptr inbounds %R, %R* %"*other", i32 0, i32 0
  %"$*other.id" = load i32, i32* %"*other.id", align 4
  store i32 %"$*other.id", i32* %"*left.id", align 4
  ret void
}

define %R @"R.+"(%R* %0, %R %1) {
entry:
  %left = alloca %R*, align 8
  %other = alloca %R, align 8
  store %R* %0, %R** %left, align 8
  store %R %1, %R* %other, align 4
  %"*left" = load %R*, %R** %left, align 8
  %"*left.id" = getelementptr inbounds %R, %R* %"*left", i32 0, i32 0
  %other.id = getelementptr inbounds %R, %R* %other, i32 0, i32 0
  %"$*left.id" = load i32, i32* %"*left.id", align 4
  %"$other.id" = load i32, i32* %other.id, align 4
  %ADD = add i32 %"$*left.id", %"$other.id"
  %call.R.create = call %R @R.create(i32 %ADD)
  call void @R.drop(%R* %other)
  ret %R %call.R.create
}

define void @R.drop(%R* %0) {
entry:
  %value = alloca %R*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %R* %0, %R** %value, align 8
  %"*value" = load %R*, %R** %value, align 8
  %"*value.id" = getelementptr inbounds %R, %R* %"*value", i32 0, i32 0
  %"$*value.id" = load i32, i32* %"*value.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$*value.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$*value.id")
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

define i32 @via_return(%R %0, %R %1) {
entry:
  %a = alloca %R, align 8
  %b = alloca %R, align 8
  %copy = alloca %R, align 8
  %tmp = alloca %R, align 8
  store %R %0, %R* %a, align 4
  store %R %1, %R* %b, align 4
  store %R zeroinitializer, %R* %copy, align 4
  call void @"R.="(%R* %copy, %R* %b)
  %"$copy" = load %R, %R* %copy, align 4
  %"call.R.+" = call %R @"R.+"(%R* %a, %R %"$copy")
  store %R %"call.R.+", %R* %tmp, align 4
  %tmp.id = getelementptr inbounds %R, %R* %tmp, i32 0, i32 0
  %"$tmp.id" = load i32, i32* %tmp.id, align 4
  call void @R.drop(%R* %tmp)
  call void @R.drop(%R* %b)
  call void @R.drop(%R* %a)
  ret i32 %"$tmp.id"
}

define i32 @main() {
entry:
  %a = alloca %R, align 8
  %b = alloca %R, align 8
  %copy = alloca %R, align 8
  %tmp = alloca %R, align 8
  %copy2 = alloca %R, align 8
  %tmp5 = alloca %R, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %copy11 = alloca %R, align 8
  %tmp14 = alloca %R, align 8
  %copy15 = alloca %R, align 8
  %tmp18 = alloca %R, align 8
  %i = alloca i32, align 4
  %copy19 = alloca %R, align 8
  %tmp22 = alloca %R, align 8
  %copy25 = alloca %R, align 8
  %copy26 = alloca %R, align 8
  %format.size29 = alloca i32, align 4
  %format.text34 = alloca i8*, align 8
  %tmp43 = alloca %R, align 8
  %format.size45 = alloca i32, align 4
  %format.text51 = alloca i8*, align 8
  %tmp59 = alloca %Plain, align 8
  %format.size60 = alloca i32, align 4
  %format.text65 = alloca i8*, align 8
  %call.R.create = call %R @R.create(i32 1)
  store %R %call.R.create, %R* %a, align 4
  %call.R.create1 = call %R @R.create(i32 2)
  store %R %call.R.create1, %R* %b, align 4
  store %R zeroinitializer, %R* %copy, align 4
  call void @"R.="(%R* %copy, %R* %b)
  %"$copy" = load %R, %R* %copy, align 4
  %"call.R.+" = call %R @"R.+"(%R* %a, %R %"$copy")
  store %R %"call.R.+", %R* %tmp, align 4
  call void @R.show(%R* %tmp)
  call void @R.drop(%R* %tmp)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 3, i32 0 })
  store %R zeroinitializer, %R* %copy2, align 4
  call void @"R.="(%R* %copy2, %R* %b)
  %"$copy3" = load %R, %R* %copy2, align 4
  %"call.R.+4" = call %R @"R.+"(%R* %a, %R %"$copy3")
  store %R %"call.R.+4", %R* %tmp5, align 4
  %tmp.id = getelementptr inbounds %R, %R* %tmp5, i32 0, i32 0
  %"$tmp.id" = load i32, i32* %tmp.id, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$tmp.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$tmp.id")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  call void @R.drop(%R* %tmp5)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 3, i32 0 })
  store %R zeroinitializer, %R* %copy11, align 4
  call void @"R.="(%R* %copy11, %R* %b)
  %"$copy12" = load %R, %R* %copy11, align 4
  %"call.R.+13" = call %R @"R.+"(%R* %a, %R %"$copy12")
  store %R %"call.R.+13", %R* %tmp14, align 4
  store %R zeroinitializer, %R* %copy15, align 4
  call void @"R.="(%R* %copy15, %R* %a)
  %"$copy16" = load %R, %R* %copy15, align 4
  %"call.R.+17" = call %R @"R.+"(%R* %tmp14, %R %"$copy16")
  store %R %"call.R.+17", %R* %tmp18, align 4
  call void @R.show(%R* %tmp18)
  call void @R.drop(%R* %tmp18)
  call void @R.drop(%R* %tmp14)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i32 3, i32 0 })
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  store %R zeroinitializer, %R* %copy19, align 4
  call void @"R.="(%R* %copy19, %R* %b)
  %"$copy20" = load %R, %R* %copy19, align 4
  %"call.R.+21" = call %R @"R.+"(%R* %a, %R %"$copy20")
  store %R %"call.R.+21", %R* %tmp22, align 4
  call void @R.show(%R* %tmp22)
  call void @R.drop(%R* %tmp22)
  %"$i23" = load i32, i32* %i, align 4
  %ADD24 = add i32 %"$i23", 1
  store i32 %ADD24, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 3, i32 0 })
  store %R zeroinitializer, %R* %copy25, align 4
  call void @"R.="(%R* %copy25, %R* %a)
  store %R zeroinitializer, %R* %copy26, align 4
  call void @"R.="(%R* %copy26, %R* %b)
  %"$copy27" = load %R, %R* %copy25, align 4
  %"$copy28" = load %R, %R* %copy26, align 4
  %call.via_return = call i32 @via_return(%R %"$copy27", %R %"$copy28")
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i32 %call.via_return)
  store i32 %call.snprintf30, i32* %format.size29, align 4
  %"$format.size31" = load i32, i32* %format.size29, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %AS33 = sext i32 %ADD32 to i64
  %call.calloc35 = call i8* @calloc(i64 %AS33, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc35, i8** %format.text34, align 8
  %"$format.text36" = load i8*, i8** %format.text34, align 8
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text36", i64 %AS33, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i32 %call.via_return)
  %"$format.size38" = load i32, i32* %format.size29, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %"$format.text40" = load i8*, i8** %format.text34, align 8
  %"$format.size41" = load i32, i32* %format.size29, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text40", 0
  %4 = insertvalue %String %3, i32 %"$format.size41", 1
  %5 = insertvalue %String %4, i32 %ADD39, 2
  call void @"io::out"(%String %5)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i32 3, i32 0 })
  %call.R.create42 = call %R @R.create(i32 9)
  store %R %call.R.create42, %R* %tmp43, align 4
  %tmp.id44 = getelementptr inbounds %R, %R* %tmp43, i32 0, i32 0
  %"$tmp.id46" = load i32, i32* %tmp.id44, align 4
  %call.snprintf47 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %"$tmp.id46")
  store i32 %call.snprintf47, i32* %format.size45, align 4
  %"$format.size48" = load i32, i32* %format.size45, align 4
  %ADD49 = add i32 %"$format.size48", 1
  %AS50 = sext i32 %ADD49 to i64
  %call.calloc52 = call i8* @calloc(i64 %AS50, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc52, i8** %format.text51, align 8
  %"$format.text53" = load i8*, i8** %format.text51, align 8
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text53", i64 %AS50, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %"$tmp.id46")
  %"$format.size55" = load i32, i32* %format.size45, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %"$format.text57" = load i8*, i8** %format.text51, align 8
  %"$format.size58" = load i32, i32* %format.size45, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text57", 0
  %7 = insertvalue %String %6, i32 %"$format.size58", 1
  %8 = insertvalue %String %7, i32 %ADD56, 2
  call void @"io::out"(%String %8)
  call void @R.drop(%R* %tmp43)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 3, i32 0 })
  %call.Plain.create = call %Plain @Plain.create(i32 7)
  store %Plain %call.Plain.create, %Plain* %tmp59, align 4
  %tmp.x = getelementptr inbounds %Plain, %Plain* %tmp59, i32 0, i32 0
  %"$tmp.x" = load i32, i32* %tmp.x, align 4
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %"$tmp.x")
  store i32 %call.snprintf61, i32* %format.size60, align 4
  %"$format.size62" = load i32, i32* %format.size60, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %AS64 = sext i32 %ADD63 to i64
  %call.calloc66 = call i8* @calloc(i64 %AS64, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc66, i8** %format.text65, align 8
  %"$format.text67" = load i8*, i8** %format.text65, align 8
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text67", i64 %AS64, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %"$tmp.x")
  %"$format.size69" = load i32, i32* %format.size60, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %"$format.text71" = load i8*, i8** %format.text65, align 8
  %"$format.size72" = load i32, i32* %format.size60, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text71", 0
  %10 = insertvalue %String %9, i32 %"$format.size72", 1
  %11 = insertvalue %String %10, i32 %ADD70, 2
  call void @"io::out"(%String %11)
  call void @"io::out"(%String { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i32 4, i32 0 })
  call void @R.drop(%R* %b)
  call void @R.drop(%R* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 023 — operator: every overloadable operator on one struct

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Vec2 = type { i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"%s(%d, %d)\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"a + b   \00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"a - b   \00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"a * 3   \00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"a / 2   \00", align 1
@str.5 = private unnamed_addr constant [9 x i8] c"a mod 4 \00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [14 x i8] c"== %s  != %s\0A\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.12 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [14 x i8] c"<  %s  >  %s\0A\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [14 x i8] c"<= %s  >= %s\0A\00", align 1
@str.21 = private unnamed_addr constant [10 x i8] c"c = a    \00", align 1
@str.22 = private unnamed_addr constant [10 x i8] c"c += b   \00", align 1
@str.23 = private unnamed_addr constant [10 x i8] c"c -= b   \00", align 1
@str.24 = private unnamed_addr constant [10 x i8] c"c *= 2   \00", align 1
@str.25 = private unnamed_addr constant [10 x i8] c"c /= 3   \00", align 1
@str.26 = private unnamed_addr constant [10 x i8] c"c %= 5   \00", align 1
@str.27 = private unnamed_addr constant [13 x i8] c"a is longer\0A\00", align 1
@str.28 = private unnamed_addr constant [21 x i8] c"steps to pass a: %d\0A\00", align 1

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

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec2, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %v.x = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %"$v"
}

define i32 @Vec2.taxicab(%Vec2* %0) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %call.Vec2.abs = call i32 @Vec2.abs(%Vec2* %"*self", i32 %"$*self.x")
  %"*self2" = load %Vec2*, %Vec2** %self, align 8
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.Vec2.abs4 = call i32 @Vec2.abs(%Vec2* %"*self2", i32 %"$*self.y")
  %ADD = add i32 %call.Vec2.abs, %call.Vec2.abs4
  ret i32 %ADD
}

define i32 @Vec2.abs(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n1"
  ret i32 %SUB

endif:                                            ; preds = %if6
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @Vec2.show(%Vec2* %0, i8* %1) {
entry:
  %self = alloca %Vec2*, align 8
  %label = alloca i8*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store i8* %1, i8** %label, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$label" = load i8*, i8** %label, align 8
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %3 = insertvalue %String %2, i32 %"$format.size6", 1
  %4 = insertvalue %String %3, i32 %ADD4, 2
  call void @"io::out"(%String %4)
  ret void
}

define %Vec2 @"Vec2.+"(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*left.y", %"$other.y"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %ADD, i32 %ADD2)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @Vec2.-(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %SUB = sub i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %SUB2 = sub i32 %"$*left.y", %"$other.y"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %SUB, i32 %SUB2)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2.*"(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$*left.x", %"$n"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %MUL3 = mul i32 %"$*left.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %MUL, i32 %MUL3)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2./"(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$*left.x", %"$n"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %DIV3 = sdiv i32 %"$*left.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %DIV, i32 %DIV3)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2.%"(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$*left.x", %"$n"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %MOD3 = srem i32 %"$*left.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %MOD, i32 %MOD3)
  ret %Vec2 %call.Vec2.create
}

define i1 @"Vec2.=="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*left.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define i1 @"Vec2.!="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %NQ = icmp ne i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %NQ2 = icmp ne i32 %"$*left.y", %"$other.y"
  %OR = or i1 %NQ, %NQ2
  ret i1 %OR
}

define i1 @"Vec2.<"(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*left")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %LT = icmp slt i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %LT
}

define i1 @"Vec2.>"(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*left")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %GT = icmp sgt i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %GT
}

define i1 @"Vec2.<="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*left")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %LE = icmp sle i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %LE
}

define i1 @"Vec2.>="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*left")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %GE = icmp sge i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %GE
}

define void @"Vec2.="(%Vec2* %0, %Vec2* %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2* %1, %Vec2** %other, align 8
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*other" = load %Vec2*, %Vec2** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec2, %Vec2* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*left.x", align 4
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %"*other2" = load %Vec2*, %Vec2** %other, align 8
  %"*other.y" = getelementptr inbounds %Vec2, %Vec2* %"*other2", i32 0, i32 1
  %"$*other.y" = load i32, i32* %"*other.y", align 4
  store i32 %"$*other.y", i32* %"*left.y", align 4
  ret void
}

define void @"Vec2.+="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*left.x", %"$other.x"
  store i32 %ADD, i32* %"*left.x", align 4
  %"*left3" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec2, %Vec2* %"*left4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD6 = add i32 %"$*left.y", %"$other.y"
  store i32 %ADD6, i32* %"*left.y", align 4
  ret void
}

define void @"Vec2.-="(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %SUB = sub i32 %"$*left.x", %"$other.x"
  store i32 %SUB, i32* %"*left.x", align 4
  %"*left3" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec2, %Vec2* %"*left4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %SUB6 = sub i32 %"$*left.y", %"$other.y"
  store i32 %SUB6, i32* %"*left.y", align 4
  ret void
}

define void @"Vec2.*="(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$*left.x", %"$n"
  store i32 %MUL, i32* %"*left.x", align 4
  %"*left3" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec2, %Vec2* %"*left4", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %MUL7 = mul i32 %"$*left.y", %"$n6"
  store i32 %MUL7, i32* %"*left.y", align 4
  ret void
}

define void @"Vec2./="(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$*left.x", %"$n"
  store i32 %DIV, i32* %"*left.x", align 4
  %"*left3" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec2, %Vec2* %"*left4", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %DIV7 = sdiv i32 %"$*left.y", %"$n6"
  store i32 %DIV7, i32* %"*left.y", align 4
  ret void
}

define void @"Vec2.%="(%Vec2* %0, i32 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x2" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$*left.x", %"$n"
  store i32 %MOD, i32* %"*left.x", align 4
  %"*left3" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left3", i32 0, i32 1
  %"*left4" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y5" = getelementptr inbounds %Vec2, %Vec2* %"*left4", i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %MOD7 = srem i32 %"$*left.y", %"$n6"
  store i32 %MOD7, i32* %"*left.y", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec2, align 8
  %b = alloca %Vec2, align 8
  %copy = alloca %Vec2, align 8
  %tmp = alloca %Vec2, align 8
  %copy8 = alloca %Vec2, align 8
  %tmp10 = alloca %Vec2, align 8
  %tmp11 = alloca %Vec2, align 8
  %tmp12 = alloca %Vec2, align 8
  %tmp13 = alloca %Vec2, align 8
  %copy14 = alloca %Vec2, align 8
  %copy16 = alloca %Vec2, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text18 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %copy27 = alloca %Vec2, align 8
  %copy29 = alloca %Vec2, align 8
  %format.size31 = alloca i32, align 4
  %bool.text32 = alloca i8*, align 8
  %bool.text33 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %copy50 = alloca %Vec2, align 8
  %copy52 = alloca %Vec2, align 8
  %format.size54 = alloca i32, align 4
  %bool.text55 = alloca i8*, align 8
  %bool.text56 = alloca i8*, align 8
  %format.text63 = alloca i8*, align 8
  %c = alloca %Vec2, align 8
  %copy74 = alloca %Vec2, align 8
  %copy76 = alloca %Vec2, align 8
  %copy78 = alloca %Vec2, align 8
  %copy80 = alloca %Vec2, align 8
  %n = alloca i32, align 4
  %d = alloca %Vec2, align 8
  %copy84 = alloca %Vec2, align 8
  %copy87 = alloca %Vec2, align 8
  %format.size90 = alloca i32, align 4
  %format.text96 = alloca i8*, align 8
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 9, i32 12)
  store %Vec2 %call.Vec2.create, %Vec2* %a, align 4
  %call.Vec2.create7 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create7, %Vec2* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %copy, align 4
  call void @"Vec2.="(%Vec2* %copy, %Vec2* %b)
  %"$copy" = load %Vec2, %Vec2* %copy, align 4
  %"call.Vec2.+" = call %Vec2 @"Vec2.+"(%Vec2* %a, %Vec2 %"$copy")
  store %Vec2 %"call.Vec2.+", %Vec2* %tmp, align 4
  call void @Vec2.show(%Vec2* %tmp, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy8, align 4
  call void @"Vec2.="(%Vec2* %copy8, %Vec2* %b)
  %"$copy9" = load %Vec2, %Vec2* %copy8, align 4
  %call.Vec2.- = call %Vec2 @Vec2.-(%Vec2* %a, %Vec2 %"$copy9")
  store %Vec2 %call.Vec2.-, %Vec2* %tmp10, align 4
  call void @Vec2.show(%Vec2* %tmp10, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0))
  %"call.Vec2.*" = call %Vec2 @"Vec2.*"(%Vec2* %a, i32 3)
  store %Vec2 %"call.Vec2.*", %Vec2* %tmp11, align 4
  call void @Vec2.show(%Vec2* %tmp11, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0))
  %"call.Vec2./" = call %Vec2 @"Vec2./"(%Vec2* %a, i32 2)
  store %Vec2 %"call.Vec2./", %Vec2* %tmp12, align 4
  call void @Vec2.show(%Vec2* %tmp12, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0))
  %"call.Vec2.%" = call %Vec2 @"Vec2.%"(%Vec2* %a, i32 4)
  store %Vec2 %"call.Vec2.%", %Vec2* %tmp13, align 4
  call void @Vec2.show(%Vec2* %tmp13, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.5, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy14, align 4
  call void @"Vec2.="(%Vec2* %copy14, %Vec2* %b)
  %"$copy15" = load %Vec2, %Vec2* %copy14, align 4
  %"call.Vec2.==" = call i1 @"Vec2.=="(%Vec2* %a, %Vec2 %"$copy15")
  store %Vec2 zeroinitializer, %Vec2* %copy16, align 4
  call void @"Vec2.="(%Vec2* %copy16, %Vec2* %b)
  %"$copy17" = load %Vec2, %Vec2* %copy16, align 4
  %"call.Vec2.!=" = call i1 @"Vec2.!="(%Vec2* %a, %Vec2 %"$copy17")
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if7

if7:                                              ; preds = %entry
  br i1 %"call.Vec2.==", label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else8, %if7.then
  store i8* null, i8** %bool.text18, align 8
  br label %if9

if9:                                              ; preds = %endif
  br i1 %"call.Vec2.!=", label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

endif1:                                           ; preds = %else10, %if9.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text19" = load i8*, i8** %bool.text18, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text19")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text20" = load i8*, i8** %bool.text, align 8
  %"$bool.text21" = load i8*, i8** %bool.text18, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text20", i8* %"$bool.text21")
  %"$format.size23" = load i32, i32* %format.size, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text, align 8
  %"$format.size26" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %1 = insertvalue %String %0, i32 %"$format.size26", 1
  %2 = insertvalue %String %1, i32 %ADD24, 2
  call void @"io::out"(%String %2)
  store %Vec2 zeroinitializer, %Vec2* %copy27, align 4
  call void @"Vec2.="(%Vec2* %copy27, %Vec2* %b)
  %"$copy28" = load %Vec2, %Vec2* %copy27, align 4
  %"call.Vec2.<" = call i1 @"Vec2.<"(%Vec2* %a, %Vec2 %"$copy28")
  store %Vec2 zeroinitializer, %Vec2* %copy29, align 4
  call void @"Vec2.="(%Vec2* %copy29, %Vec2* %b)
  %"$copy30" = load %Vec2, %Vec2* %copy29, align 4
  %"call.Vec2.>" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy30")
  store i32 0, i32* %format.size31, align 4
  store i8* null, i8** %bool.text32, align 8
  br label %if11

if11:                                             ; preds = %endif1
  br i1 %"call.Vec2.<", label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

endif2:                                           ; preds = %else12, %if11.then
  store i8* null, i8** %bool.text33, align 8
  br label %if13

if13:                                             ; preds = %endif2
  br i1 %"call.Vec2.>", label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif3

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif3

endif3:                                           ; preds = %else14, %if13.then
  %"$bool.text34" = load i8*, i8** %bool.text32, align 8
  %"$bool.text35" = load i8*, i8** %bool.text33, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text34", i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size31, align 4
  %"$format.size37" = load i32, i32* %format.size31, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text32, align 8
  %"$bool.text44" = load i8*, i8** %bool.text33, align 8
  %call.snprintf45 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text43", i8* %"$bool.text44")
  %"$format.size46" = load i32, i32* %format.size31, align 4
  %ADD47 = add i32 %"$format.size46", 1
  %"$format.text48" = load i8*, i8** %format.text40, align 8
  %"$format.size49" = load i32, i32* %format.size31, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text48", 0
  %4 = insertvalue %String %3, i32 %"$format.size49", 1
  %5 = insertvalue %String %4, i32 %ADD47, 2
  call void @"io::out"(%String %5)
  store %Vec2 zeroinitializer, %Vec2* %copy50, align 4
  call void @"Vec2.="(%Vec2* %copy50, %Vec2* %b)
  %"$copy51" = load %Vec2, %Vec2* %copy50, align 4
  %"call.Vec2.<=" = call i1 @"Vec2.<="(%Vec2* %a, %Vec2 %"$copy51")
  store %Vec2 zeroinitializer, %Vec2* %copy52, align 4
  call void @"Vec2.="(%Vec2* %copy52, %Vec2* %b)
  %"$copy53" = load %Vec2, %Vec2* %copy52, align 4
  %"call.Vec2.>=" = call i1 @"Vec2.>="(%Vec2* %a, %Vec2 %"$copy53")
  store i32 0, i32* %format.size54, align 4
  store i8* null, i8** %bool.text55, align 8
  br label %if15

if15:                                             ; preds = %endif3
  br i1 %"call.Vec2.<=", label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text55, align 8
  br label %endif4

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text55, align 8
  br label %endif4

endif4:                                           ; preds = %else16, %if15.then
  store i8* null, i8** %bool.text56, align 8
  br label %if17

if17:                                             ; preds = %endif4
  br i1 %"call.Vec2.>=", label %if17.then, label %else18

if17.then:                                        ; preds = %if17
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.18, i32 0, i32 0), i8** %bool.text56, align 8
  br label %endif5

else18:                                           ; preds = %if17
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i8** %bool.text56, align 8
  br label %endif5

endif5:                                           ; preds = %else18, %if17.then
  %"$bool.text57" = load i8*, i8** %bool.text55, align 8
  %"$bool.text58" = load i8*, i8** %bool.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text57", i8* %"$bool.text58")
  store i32 %call.snprintf59, i32* %format.size54, align 4
  %"$format.size60" = load i32, i32* %format.size54, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %AS62 = sext i32 %ADD61 to i64
  %call.calloc64 = call i8* @calloc(i64 %AS62, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc64, i8** %format.text63, align 8
  %"$format.text65" = load i8*, i8** %format.text63, align 8
  %"$bool.text66" = load i8*, i8** %bool.text55, align 8
  %"$bool.text67" = load i8*, i8** %bool.text56, align 8
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text65", i64 %AS62, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text66", i8* %"$bool.text67")
  %"$format.size69" = load i32, i32* %format.size54, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %"$format.text71" = load i8*, i8** %format.text63, align 8
  %"$format.size72" = load i32, i32* %format.size54, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text71", 0
  %7 = insertvalue %String %6, i32 %"$format.size72", 1
  %8 = insertvalue %String %7, i32 %ADD70, 2
  call void @"io::out"(%String %8)
  %call.Vec2.create73 = call %Vec2 @Vec2.create(i32 0, i32 0)
  store %Vec2 %call.Vec2.create73, %Vec2* %c, align 4
  store %Vec2 zeroinitializer, %Vec2* %copy74, align 4
  call void @"Vec2.="(%Vec2* %copy74, %Vec2* %a)
  %"$copy75" = load %Vec2, %Vec2* %copy74, align 4
  store %Vec2 %"$copy75", %Vec2* %c, align 4
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy76, align 4
  call void @"Vec2.="(%Vec2* %copy76, %Vec2* %b)
  %"$copy77" = load %Vec2, %Vec2* %copy76, align 4
  call void @"Vec2.+="(%Vec2* %c, %Vec2 %"$copy77")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.22, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy78, align 4
  call void @"Vec2.="(%Vec2* %copy78, %Vec2* %b)
  %"$copy79" = load %Vec2, %Vec2* %copy78, align 4
  call void @"Vec2.-="(%Vec2* %c, %Vec2 %"$copy79")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.23, i32 0, i32 0))
  call void @"Vec2.*="(%Vec2* %c, i32 2)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0))
  call void @"Vec2./="(%Vec2* %c, i32 3)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.25, i32 0, i32 0))
  call void @"Vec2.%="(%Vec2* %c, i32 5)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.26, i32 0, i32 0))
  br label %if19

if19:                                             ; preds = %endif5
  store %Vec2 zeroinitializer, %Vec2* %copy80, align 4
  call void @"Vec2.="(%Vec2* %copy80, %Vec2* %b)
  %"$copy81" = load %Vec2, %Vec2* %copy80, align 4
  %"call.Vec2.>82" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy81")
  br i1 %"call.Vec2.>82", label %if19.then, label %endif6

if19.then:                                        ; preds = %if19
  call void @"io::out"(%String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.27, i32 0, i32 0), i32 12, i32 0 })
  br label %endif6

endif6:                                           ; preds = %if19.then, %if19
  store i32 0, i32* %n, align 4
  %call.Vec2.create83 = call %Vec2 @Vec2.create(i32 1, i32 1)
  store %Vec2 %call.Vec2.create83, %Vec2* %d, align 4
  br label %while20

while20:                                          ; preds = %while20.then, %endif6
  store %Vec2 zeroinitializer, %Vec2* %copy84, align 4
  call void @"Vec2.="(%Vec2* %copy84, %Vec2* %a)
  %"$copy85" = load %Vec2, %Vec2* %copy84, align 4
  %"call.Vec2.<86" = call i1 @"Vec2.<"(%Vec2* %d, %Vec2 %"$copy85")
  br i1 %"call.Vec2.<86", label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
  store %Vec2 zeroinitializer, %Vec2* %copy87, align 4
  call void @"Vec2.="(%Vec2* %copy87, %Vec2* %b)
  %"$copy88" = load %Vec2, %Vec2* %copy87, align 4
  call void @"Vec2.+="(%Vec2* %d, %Vec2 %"$copy88")
  %"$n" = load i32, i32* %n, align 4
  %ADD89 = add i32 %"$n", 1
  store i32 %ADD89, i32* %n, align 4
  br label %while20

endloop:                                          ; preds = %while20
  %"$n91" = load i32, i32* %n, align 4
  %call.snprintf92 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.28, i32 0, i32 0), i32 %"$n91")
  store i32 %call.snprintf92, i32* %format.size90, align 4
  %"$format.size93" = load i32, i32* %format.size90, align 4
  %ADD94 = add i32 %"$format.size93", 1
  %AS95 = sext i32 %ADD94 to i64
  %call.calloc97 = call i8* @calloc(i64 %AS95, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc97, i8** %format.text96, align 8
  %"$format.text98" = load i8*, i8** %format.text96, align 8
  %call.snprintf99 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text98", i64 %AS95, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.28, i32 0, i32 0), i32 %"$n91")
  %"$format.size100" = load i32, i32* %format.size90, align 4
  %ADD101 = add i32 %"$format.size100", 1
  %"$format.text102" = load i8*, i8** %format.text96, align 8
  %"$format.size103" = load i32, i32* %format.size90, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text102", 0
  %10 = insertvalue %String %9, i32 %"$format.size103", 1
  %11 = insertvalue %String %10, i32 %ADD101, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 024 — operator: bitwise operator overloads, and one returning a scalar

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Flags = type { i32 }
%Vec2 = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"and %d  or %d\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"  xor %d\0A\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"shl %d  shr %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"dot %d\0A\00", align 1

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

define %Flags @Flags.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %f = alloca %Flags, align 8
  store i32 %0, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %f, align 4
  %f.bits = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %f.bits, align 4
  %"$f" = load %Flags, %Flags* %f, align 4
  ret %Flags %"$f"
}

define %Flags @"Flags.&"(%Flags* %0, %Flags %1) {
entry:
  %left = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BAND = and i32 %"$*left.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BAND)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.|"(%Flags* %0, %Flags %1) {
entry:
  %left = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BOR = or i32 %"$*left.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BOR)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.^"(%Flags* %0, %Flags %1) {
entry:
  %left = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BXOR = xor i32 %"$*left.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BXOR)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.<<"(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*left.bits", %"$n"
  %call.Flags.create = call %Flags @Flags.create(i32 %LSHIFT)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.>>"(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*left.bits", %"$n"
  %call.Flags.create = call %Flags @Flags.create(i32 %RSHIFT)
  ret %Flags %call.Flags.create
}

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec2, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %v.x = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %"$v"
}

define i32 @"Vec2.*"(%Vec2* %0, %Vec2 %1) {
entry:
  %left = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %left, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*left" = load %Vec2*, %Vec2** %left, align 8
  %"*left.x" = getelementptr inbounds %Vec2, %Vec2* %"*left", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %MUL = mul i32 %"$*left.x", %"$other.x"
  %"*left1" = load %Vec2*, %Vec2** %left, align 8
  %"*left.y" = getelementptr inbounds %Vec2, %Vec2* %"*left1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %MUL2 = mul i32 %"$*left.y", %"$other.y"
  %ADD = add i32 %MUL, %MUL2
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Flags, align 8
  %b = alloca %Flags, align 8
  %tmp = alloca %Flags, align 8
  %tmp3 = alloca %Flags, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp12 = alloca %Flags, align 8
  %format.size14 = alloca i32, align 4
  %format.text20 = alloca i8*, align 8
  %tmp28 = alloca %Flags, align 8
  %tmp30 = alloca %Flags, align 8
  %format.size32 = alloca i32, align 4
  %format.text39 = alloca i8*, align 8
  %p = alloca %Vec2, align 8
  %q = alloca %Vec2, align 8
  %format.size48 = alloca i32, align 4
  %format.text53 = alloca i8*, align 8
  %call.Flags.create = call %Flags @Flags.create(i32 12)
  store %Flags %call.Flags.create, %Flags* %a, align 4
  %call.Flags.create1 = call %Flags @Flags.create(i32 10)
  store %Flags %call.Flags.create1, %Flags* %b, align 4
  %"$b" = load %Flags, %Flags* %b, align 4
  %"call.Flags.&" = call %Flags @"Flags.&"(%Flags* %a, %Flags %"$b")
  store %Flags %"call.Flags.&", %Flags* %tmp, align 4
  %tmp.bits = getelementptr inbounds %Flags, %Flags* %tmp, i32 0, i32 0
  %"$b2" = load %Flags, %Flags* %b, align 4
  %"call.Flags.|" = call %Flags @"Flags.|"(%Flags* %a, %Flags %"$b2")
  store %Flags %"call.Flags.|", %Flags* %tmp3, align 4
  %tmp.bits4 = getelementptr inbounds %Flags, %Flags* %tmp3, i32 0, i32 0
  %"$tmp.bits" = load i32, i32* %tmp.bits, align 4
  %"$tmp.bits5" = load i32, i32* %tmp.bits4, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$tmp.bits", i32 %"$tmp.bits5")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$tmp.bits", i32 %"$tmp.bits5")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  %"$b11" = load %Flags, %Flags* %b, align 4
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags %"$b11")
  store %Flags %"call.Flags.^", %Flags* %tmp12, align 4
  %tmp.bits13 = getelementptr inbounds %Flags, %Flags* %tmp12, i32 0, i32 0
  %"$tmp.bits15" = load i32, i32* %tmp.bits13, align 4
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.bits15")
  store i32 %call.snprintf16, i32* %format.size14, align 4
  %"$format.size17" = load i32, i32* %format.size14, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %AS19 = sext i32 %ADD18 to i64
  %call.calloc21 = call i8* @calloc(i64 %AS19, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc21, i8** %format.text20, align 8
  %"$format.text22" = load i8*, i8** %format.text20, align 8
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text22", i64 %AS19, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.bits15")
  %"$format.size24" = load i32, i32* %format.size14, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %"$format.text26" = load i8*, i8** %format.text20, align 8
  %"$format.size27" = load i32, i32* %format.size14, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text26", 0
  %4 = insertvalue %String %3, i32 %"$format.size27", 1
  %5 = insertvalue %String %4, i32 %ADD25, 2
  call void @"io::out"(%String %5)
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %tmp28, align 4
  %tmp.bits29 = getelementptr inbounds %Flags, %Flags* %tmp28, i32 0, i32 0
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.>>", %Flags* %tmp30, align 4
  %tmp.bits31 = getelementptr inbounds %Flags, %Flags* %tmp30, i32 0, i32 0
  %"$tmp.bits33" = load i32, i32* %tmp.bits29, align 4
  %"$tmp.bits34" = load i32, i32* %tmp.bits31, align 4
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i32 %"$tmp.bits33", i32 %"$tmp.bits34")
  store i32 %call.snprintf35, i32* %format.size32, align 4
  %"$format.size36" = load i32, i32* %format.size32, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %call.snprintf42 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i32 %"$tmp.bits33", i32 %"$tmp.bits34")
  %"$format.size43" = load i32, i32* %format.size32, align 4
  %ADD44 = add i32 %"$format.size43", 1
  %"$format.text45" = load i8*, i8** %format.text39, align 8
  %"$format.size46" = load i32, i32* %format.size32, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text45", 0
  %7 = insertvalue %String %6, i32 %"$format.size46", 1
  %8 = insertvalue %String %7, i32 %ADD44, 2
  call void @"io::out"(%String %8)
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create, %Vec2* %p, align 4
  %call.Vec2.create47 = call %Vec2 @Vec2.create(i32 4, i32 5)
  store %Vec2 %call.Vec2.create47, %Vec2* %q, align 4
  %"$q" = load %Vec2, %Vec2* %q, align 4
  %"call.Vec2.*" = call i32 @"Vec2.*"(%Vec2* %p, %Vec2 %"$q")
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"call.Vec2.*")
  store i32 %call.snprintf49, i32* %format.size48, align 4
  %"$format.size50" = load i32, i32* %format.size48, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %AS52 = sext i32 %ADD51 to i64
  %call.calloc54 = call i8* @calloc(i64 %AS52, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc54, i8** %format.text53, align 8
  %"$format.text55" = load i8*, i8** %format.text53, align 8
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text55", i64 %AS52, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"call.Vec2.*")
  %"$format.size57" = load i32, i32* %format.size48, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %"$format.text59" = load i8*, i8** %format.text53, align 8
  %"$format.size60" = load i32, i32* %format.size48, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text59", 0
  %10 = insertvalue %String %9, i32 %"$format.size60", 1
  %11 = insertvalue %String %10, i32 %ADD58, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 025 — operator: compound bitwise assignment overloads

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Flags = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
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

define void @"Flags.&="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*left1" = load %Flags*, %Flags** %left, align 8
  %"*left.bits2" = getelementptr inbounds %Flags, %Flags* %"*left1", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BAND = and i32 %"$*left.bits", %"$n"
  store i32 %BAND, i32* %"*left.bits", align 4
  ret void
}

define void @"Flags.|="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*left1" = load %Flags*, %Flags** %left, align 8
  %"*left.bits2" = getelementptr inbounds %Flags, %Flags* %"*left1", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BOR = or i32 %"$*left.bits", %"$n"
  store i32 %BOR, i32* %"*left.bits", align 4
  ret void
}

define void @"Flags.^="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*left1" = load %Flags*, %Flags** %left, align 8
  %"*left.bits2" = getelementptr inbounds %Flags, %Flags* %"*left1", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BXOR = xor i32 %"$*left.bits", %"$n"
  store i32 %BXOR, i32* %"*left.bits", align 4
  ret void
}

define void @"Flags.<<="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*left1" = load %Flags*, %Flags** %left, align 8
  %"*left.bits2" = getelementptr inbounds %Flags, %Flags* %"*left1", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*left.bits", %"$n"
  store i32 %LSHIFT, i32* %"*left.bits", align 4
  ret void
}

define void @"Flags.>>="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*left1" = load %Flags*, %Flags** %left, align 8
  %"*left.bits2" = getelementptr inbounds %Flags, %Flags* %"*left1", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*left.bits", %"$n"
  store i32 %RSHIFT, i32* %"*left.bits", align 4
  ret void
}

define i32 @main() {
entry:
  %f = alloca %Flags, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  %format.size23 = alloca i32, align 4
  %format.text29 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %format.text44 = alloca i8*, align 8
  %format.size53 = alloca i32, align 4
  %format.text59 = alloca i8*, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %f.bits = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  store i32 12, i32* %f.bits, align 4
  call void @"Flags.&="(%Flags* %f, i32 10)
  %f.bits1 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits" = load i32, i32* %f.bits1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$f.bits")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$f.bits")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  call void @"Flags.|="(%Flags* %f, i32 3)
  %f.bits7 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits9" = load i32, i32* %f.bits7, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$f.bits9")
  store i32 %call.snprintf10, i32* %format.size8, align 4
  %"$format.size11" = load i32, i32* %format.size8, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$f.bits9")
  %"$format.size18" = load i32, i32* %format.size8, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %4 = insertvalue %String %3, i32 %"$format.size21", 1
  %5 = insertvalue %String %4, i32 %ADD19, 2
  call void @"io::out"(%String %5)
  call void @"Flags.^="(%Flags* %f, i32 5)
  %f.bits22 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits24" = load i32, i32* %f.bits22, align 4
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$f.bits24")
  store i32 %call.snprintf25, i32* %format.size23, align 4
  %"$format.size26" = load i32, i32* %format.size23, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %AS28 = sext i32 %ADD27 to i64
  %call.calloc30 = call i8* @calloc(i64 %AS28, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc30, i8** %format.text29, align 8
  %"$format.text31" = load i8*, i8** %format.text29, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text31", i64 %AS28, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$f.bits24")
  %"$format.size33" = load i32, i32* %format.size23, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text29, align 8
  %"$format.size36" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %7 = insertvalue %String %6, i32 %"$format.size36", 1
  %8 = insertvalue %String %7, i32 %ADD34, 2
  call void @"io::out"(%String %8)
  call void @"Flags.<<="(%Flags* %f, i32 2)
  %f.bits37 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits39" = load i32, i32* %f.bits37, align 4
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$f.bits39")
  store i32 %call.snprintf40, i32* %format.size38, align 4
  %"$format.size41" = load i32, i32* %format.size38, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %AS43 = sext i32 %ADD42 to i64
  %call.calloc45 = call i8* @calloc(i64 %AS43, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc45, i8** %format.text44, align 8
  %"$format.text46" = load i8*, i8** %format.text44, align 8
  %call.snprintf47 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text46", i64 %AS43, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$f.bits39")
  %"$format.size48" = load i32, i32* %format.size38, align 4
  %ADD49 = add i32 %"$format.size48", 1
  %"$format.text50" = load i8*, i8** %format.text44, align 8
  %"$format.size51" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text50", 0
  %10 = insertvalue %String %9, i32 %"$format.size51", 1
  %11 = insertvalue %String %10, i32 %ADD49, 2
  call void @"io::out"(%String %11)
  call void @"Flags.>>="(%Flags* %f, i32 1)
  %f.bits52 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits54" = load i32, i32* %f.bits52, align 4
  %call.snprintf55 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$f.bits54")
  store i32 %call.snprintf55, i32* %format.size53, align 4
  %"$format.size56" = load i32, i32* %format.size53, align 4
  %ADD57 = add i32 %"$format.size56", 1
  %AS58 = sext i32 %ADD57 to i64
  %call.calloc60 = call i8* @calloc(i64 %AS58, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc60, i8** %format.text59, align 8
  %"$format.text61" = load i8*, i8** %format.text59, align 8
  %call.snprintf62 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text61", i64 %AS58, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$f.bits54")
  %"$format.size63" = load i32, i32* %format.size53, align 4
  %ADD64 = add i32 %"$format.size63", 1
  %"$format.text65" = load i8*, i8** %format.text59, align 8
  %"$format.size66" = load i32, i32* %format.size53, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text65", 0
  %13 = insertvalue %String %12, i32 %"$format.size66", 1
  %14 = insertvalue %String %13, i32 %ADD64, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 026 — operator: bitwise and shift overloads taking a ref parameter

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Flags = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"a & b:  %d\0A\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"a | b:  %d\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"a ^ b:  %d\0A\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a << 2: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a >> 1: %d\0A\00", align 1

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

define void @"Flags.="(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*left.bits", align 4
  ret void
}

define %Flags @"Flags.&"(%Flags* %0, %Flags* %1) {
entry:
  %left = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BAND = and i32 %"$*left.bits", %"$*f.bits"
  store i32 %BAND, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.|"(%Flags* %0, %Flags* %1) {
entry:
  %left = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BOR = or i32 %"$*left.bits", %"$*f.bits"
  store i32 %BOR, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.^"(%Flags* %0, %Flags* %1) {
entry:
  %left = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BXOR = xor i32 %"$*left.bits", %"$*f.bits"
  store i32 %BXOR, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.<<"(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*left.bits", %"$n"
  store i32 %LSHIFT, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.>>"(%Flags* %0, i32 %1) {
entry:
  %left = alloca %Flags*, align 8
  %n = alloca i32, align 4
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %left, align 8
  store i32 %1, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*left" = load %Flags*, %Flags** %left, align 8
  %"*left.bits" = getelementptr inbounds %Flags, %Flags* %"*left", i32 0, i32 0
  %"$*left.bits" = load i32, i32* %"*left.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*left.bits", %"$n"
  store i32 %RSHIFT, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define i32 @main() {
entry:
  %a = alloca %Flags, align 8
  %b = alloca %Flags, align 8
  %band = alloca %Flags, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %bor = alloca %Flags, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %bxor = alloca %Flags, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %lsh = alloca %Flags, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  %rsh = alloca %Flags, align 8
  %format.size45 = alloca i32, align 4
  %format.text50 = alloca i8*, align 8
  store %Flags zeroinitializer, %Flags* %a, align 4
  call void @"Flags.="(%Flags* %a, i32 12)
  store %Flags zeroinitializer, %Flags* %b, align 4
  call void @"Flags.="(%Flags* %b, i32 10)
  %"call.Flags.&" = call %Flags @"Flags.&"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.&", %Flags* %band, align 4
  %band.bits = getelementptr inbounds %Flags, %Flags* %band, i32 0, i32 0
  %"$band.bits" = load i32, i32* %band.bits, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$band.bits")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$band.bits")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %"call.Flags.|" = call %Flags @"Flags.|"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.|", %Flags* %bor, align 4
  %bor.bits = getelementptr inbounds %Flags, %Flags* %bor, i32 0, i32 0
  %"$bor.bits" = load i32, i32* %bor.bits, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$bor.bits")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$bor.bits")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.^", %Flags* %bxor, align 4
  %bxor.bits = getelementptr inbounds %Flags, %Flags* %bxor, i32 0, i32 0
  %"$bxor.bits" = load i32, i32* %bxor.bits, align 4
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$bxor.bits")
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$bxor.bits")
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %7 = insertvalue %String %6, i32 %"$format.size31", 1
  %8 = insertvalue %String %7, i32 %ADD29, 2
  call void @"io::out"(%String %8)
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %lsh, align 4
  %lsh.bits = getelementptr inbounds %Flags, %Flags* %lsh, i32 0, i32 0
  %"$lsh.bits" = load i32, i32* %lsh.bits, align 4
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %"$lsh.bits")
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %"$lsh.bits")
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %10 = insertvalue %String %9, i32 %"$format.size44", 1
  %11 = insertvalue %String %10, i32 %ADD42, 2
  call void @"io::out"(%String %11)
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 1)
  store %Flags %"call.Flags.>>", %Flags* %rsh, align 4
  %rsh.bits = getelementptr inbounds %Flags, %Flags* %rsh, i32 0, i32 0
  %"$rsh.bits" = load i32, i32* %rsh.bits, align 4
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$rsh.bits")
  store i32 %call.snprintf46, i32* %format.size45, align 4
  %"$format.size47" = load i32, i32* %format.size45, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %AS49 = sext i32 %ADD48 to i64
  %call.calloc51 = call i8* @calloc(i64 %AS49, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc51, i8** %format.text50, align 8
  %"$format.text52" = load i8*, i8** %format.text50, align 8
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text52", i64 %AS49, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$rsh.bits")
  %"$format.size54" = load i32, i32* %format.size45, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %"$format.text56" = load i8*, i8** %format.text50, align 8
  %"$format.size57" = load i32, i32* %format.size45, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text56", 0
  %13 = insertvalue %String %12, i32 %"$format.size57", 1
  %14 = insertvalue %String %13, i32 %ADD55, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 028 — struct: pub inside struct must be followed by fn

```stderr
error: attribute 'n' needs a type
at test.ura:8:9
  |
8 |     pub n i32
  |         ^
help: like: n i32
error: a struct holds only attributes and methods
at test.ura:8:11
  |
8 |     pub n i32
  |           ^^^
error: aborting due to 2 errors
1
```

## 029 — struct: instance method called via :: instead of .

```stderr
error: 'Counter' has no pub fn 'bump'
at test.ura:14:14
   |
14 |     Counter::bump()
   |              ^^^^
error: aborting due to 1 error
1
```

## 030 — struct: pub fn called via . instead of ::

```stderr
error: 'Maker' has no method 'make'
at test.ura:16:7
   |
16 |     m.make()
   |       ^^^^
error: aborting due to 1 error
1
```

## 031 — struct: pub may only be used inside a struct

```stderr
error: 'pub' may only be used inside a struct
at test.ura:5:5
  |
5 | pub fn helper():
  |     ^^
error: aborting due to 1 error
1
```

## 032 — operator: [] returns &i32, so b[0] reads and writes through

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Bag = type { i32* }

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

declare i32* @malloc(i32)

define i32* @"Bag.[]"(%Bag* %0, i32 %1) {
entry:
  %left = alloca %Bag*, align 8
  %index = alloca i32, align 4
  store %Bag* %0, %Bag** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %Bag*, %Bag** %left, align 8
  %"*left.data" = getelementptr inbounds %Bag, %Bag* %"*left", i32 0, i32 0
  %"$*left.data" = load i32*, i32** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds i32, i32* %"$*left.data", i32 %"$index"
  ret i32* %"*left.data[]"
}

define i32 @main() {
entry:
  %b = alloca %Bag, align 8
  %ref = alloca i32*, align 8
  %ref2 = alloca i32*, align 8
  %ref5 = alloca i32*, align 8
  %ref8 = alloca i32*, align 8
  %ref13 = alloca i32*, align 8
  %ref16 = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %ref27 = alloca i32*, align 8
  store %Bag zeroinitializer, %Bag* %b, align 8
  %b.data = getelementptr inbounds %Bag, %Bag* %b, i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 40)
  store i32* %call.malloc, i32** %b.data, align 8
  %"call.Bag.[]" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32 5, i32* %"*ref", align 4
  %"call.Bag.[]1" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]1", i32** %ref2, align 8
  %"*ref3" = load i32*, i32** %ref2, align 8
  %"call.Bag.[]4" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]4", i32** %ref5, align 8
  %"*ref6" = load i32*, i32** %ref5, align 8
  %"$*ref" = load i32, i32* %"*ref6", align 4
  %ADD = add i32 %"$*ref", 2
  store i32 %ADD, i32* %"*ref3", align 4
  %"call.Bag.[]7" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]7", i32** %ref8, align 8
  %"*ref9" = load i32*, i32** %ref8, align 8
  %"$*ref10" = load i32, i32* %"*ref9", align 4
  %ADD11 = add i32 %"$*ref10", 1
  store i32 %ADD11, i32* %"*ref9", align 4
  %"call.Bag.[]12" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]12", i32** %ref13, align 8
  %"*ref14" = load i32*, i32** %ref13, align 8
  %"call.Bag.[]15" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]15", i32** %ref16, align 8
  %"*ref17" = load i32*, i32** %ref16, align 8
  %"$*ref18" = load i32, i32* %"*ref14", align 4
  %"$*ref19" = load i32, i32* %"*ref17", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*ref18", i32 %"$*ref19")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD20 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD20 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*ref18", i32 %"$*ref19")
  %"$format.size22" = load i32, i32* %format.size, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %"$format.text24" = load i8*, i8** %format.text, align 8
  %"$format.size25" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text24", 0
  %1 = insertvalue %String %0, i32 %"$format.size25", 1
  %2 = insertvalue %String %1, i32 %ADD23, 2
  call void @"io::out"(%String %2)
  %"call.Bag.[]26" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]26", i32** %ref27, align 8
  %"*ref28" = load i32*, i32** %ref27, align 8
  %"$*ref29" = load i32, i32* %"*ref28", align 4
  ret i32 %"$*ref29"
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Noisy = type { i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"loop %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

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

define void @Noisy.drop(%Noisy* %0) {
entry:
  %value = alloca %Noisy*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Noisy* %0, %Noisy** %value, align 8
  %"*value" = load %Noisy*, %Noisy** %value, align 8
  %"*value.id" = getelementptr inbounds %Noisy, %Noisy* %"*value", i32 0, i32 0
  %"$*value.id" = load i32, i32* %"*value.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*value.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*value.id")
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

define %Noisy @make(i32 %0) {
entry:
  %id = alloca i32, align 4
  %n = alloca %Noisy, align 8
  store i32 %0, i32* %id, align 4
  store %Noisy zeroinitializer, %Noisy* %n, align 4
  %n.id = getelementptr inbounds %Noisy, %Noisy* %n, i32 0, i32 0
  %"$id" = load i32, i32* %id, align 4
  store i32 %"$id", i32* %n.id, align 4
  %"$n" = load %Noisy, %Noisy* %n, align 4
  ret %Noisy %"$n"
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
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  ret void

endif1:                                           ; preds = %if7
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
  br label %if8

if8:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
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
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while11

while11:                                          ; preds = %while11.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
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
  br label %while11

endloop:                                          ; preds = %while11
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if12

if12:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if12.then, %if12
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

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %left = alloca %"Array<i32>"*, align 8
  %right = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %left, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %right, align 8
  %"*left" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*left")
  %"*left1" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*right" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right", i32 0, i32 1
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*left1", i32 %"$*right.count")
  store i32 0, i32* %i, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %"*right2" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*left5" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*left5", i32 0, i32 0
  %"$*left.data" = load i32*, i32** %"*left.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*left.data[]" = getelementptr inbounds i32, i32* %"$*left.data", i32 %"$i6"
  %"*right7" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right7", i32 0, i32 0
  %"$*right.data" = load i32*, i32** %"*right.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i32, i32* %"$*right.data", i32 %"$i8"
  %"$*right.data[]" = load i32, i32* %"*right.data[]", align 4
  store i32 %"$*right.data[]", i32* %"*left.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
  %"*left10" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*left10", i32 0, i32 1
  %"*right11" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right11", i32 0, i32 1
  %"$*right.count13" = load i32, i32* %"*right.count12", align 4
  store i32 %"$*right.count13", i32* %"*left.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %value = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %value, align 8
  %"*value" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*value")
  br label %if14

if14:                                             ; preds = %entry
  %"*value1" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*value2" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<i32>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Noisy, align 8
  %i = alloca i32, align 4
  %b = alloca %Noisy, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %c = alloca %Noisy, align 8
  %x = alloca %"Array<i32>", align 8
  %y = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca i32*, align 8
  %format.size15 = alloca i32, align 4
  %format.text20 = alloca i8*, align 8
  %call.make = call %Noisy @make(i32 1)
  store %Noisy %call.make, %Noisy* %a, align 4
  store i32 0, i32* %i, align 4
  br label %while15

while15:                                          ; preds = %endif, %if16.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  store %Noisy zeroinitializer, %Noisy* %b, align 4
  %b.id = getelementptr inbounds %Noisy, %Noisy* %b, i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 10, %"$i2"
  store i32 %ADD, i32* %b.id, align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %if16

if16:                                             ; preds = %while15.then
  %"$i5" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i5", 2
  br i1 %EQ, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  call void @Noisy.drop(%Noisy* %b)
  br label %while15

endif:                                            ; preds = %if16
  %"$i6" = load i32, i32* %i, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$i6")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD7 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$i6")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  call void @Noisy.drop(%Noisy* %b)
  br label %while15

endloop:                                          ; preds = %while15
  br label %if17

if17:                                             ; preds = %endloop
  %a.id = getelementptr inbounds %Noisy, %Noisy* %a, i32 0, i32 0
  %"$a.id" = load i32, i32* %a.id, align 4
  %EQ13 = icmp eq i32 %"$a.id", 1
  br i1 %EQ13, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  store %Noisy zeroinitializer, %Noisy* %c, align 4
  %c.id = getelementptr inbounds %Noisy, %Noisy* %c, i32 0, i32 0
  store i32 5, i32* %c.id, align 4
  call void @Noisy.drop(%Noisy* %c)
  br label %endif1

endif1:                                           ; preds = %if17.then, %if17
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %x, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %x, i32 4)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %x)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  store %"Array<i32>" %"$copy", %"Array<i32>"* %y, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %y, i32 5)
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %x)
  %"call.Array<i32>.size14" = call i32 @"Array<i32>.size"(%"Array<i32>"* %y)
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %y, i32 1)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"$*ref")
  store i32 %call.snprintf16, i32* %format.size15, align 4
  %"$format.size17" = load i32, i32* %format.size15, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %AS19 = sext i32 %ADD18 to i64
  %call.calloc21 = call i8* @calloc(i64 %AS19, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc21, i8** %format.text20, align 8
  %"$format.text22" = load i8*, i8** %format.text20, align 8
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text22", i64 %AS19, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"$*ref")
  %"$format.size24" = load i32, i32* %format.size15, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %"$format.text26" = load i8*, i8** %format.text20, align 8
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text26", 0
  %4 = insertvalue %String %3, i32 %"$format.size27", 1
  %5 = insertvalue %String %4, i32 %ADD25, 2
  call void @"io::out"(%String %5)
  call void @"Array<i32>.drop"(%"Array<i32>"* %y)
  call void @"Array<i32>.drop"(%"Array<i32>"* %x)
  call void @Noisy.drop(%Noisy* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 034 — operator: copying a struct with drop needs operator =

```stderr
error: 'Buffer' can't be copied: it needs a drop and has no operator =
at test.ura:11:16
   |
11 |     b Buffer = a
   |                ^
help: define operator =(left &Buffer, right &Buffer), or take it as &Buffer
error: aborting due to 1 error
1
```

## 035 — operator: passing a struct with drop by value

```stderr
error: 'Buffer' can't be copied: it needs a drop and has no operator =
at test.ura:14:17
   |
14 |     return take(a)
   |                 ^
help: define operator =(left &Buffer, right &Buffer), or take it as &Buffer
error: aborting due to 1 error
1
```

## 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Text = type { i8*, i32, i32, i8*, i32 }
%Title = type { i8*, i32, i32, i8*, i32 }

@str = private unnamed_addr constant [9 x i8] c"greeting\00", align 1
@TYPEOF = private unnamed_addr constant [5 x i8] c"Text\00", align 1
@str.1 = private unnamed_addr constant [23 x i8] c"%d %d %c%c%c %s %c %s\0A\00", align 1

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
  br label %if6

if6:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if8.then, label %endif1

if8.then:                                         ; preds = %if8
  ret void

endif1:                                           ; preds = %if8
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
  br label %if9

if9:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if9.then, label %endif2

if9.then:                                         ; preds = %if9
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if9.then, %if9
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

define void @Text.reserve(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %if11

if11:                                             ; preds = %endif
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if11.then, label %endif1

if11.then:                                        ; preds = %if11
  ret void

endif1:                                           ; preds = %if11
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self9", i32 0, i32 0
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if12

if12:                                             ; preds = %endif1
  %"*self13" = load %Text*, %Text** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Text, %Text* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if12.then, label %endif2

if12.then:                                        ; preds = %if12
  %"*self16" = load %Text*, %Text** %self, align 8
  %"*self.data17" = getelementptr inbounds %Text, %Text* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if12.then, %if12
  %"*self19" = load %Text*, %Text** %self, align 8
  %"*self.data20" = getelementptr inbounds %Text, %Text* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Text*, %Text** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Text, %Text* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Text.append(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %value = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Text.reserve(%Text* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 0
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.count8" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @Text.size(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @Text.name(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.label" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 3
  %"$*self.label" = load i8*, i8** %"*self.label", align 8
  ret i8* %"$*self.label"
}

define %Text @Text.shout(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  %res = alloca %Text, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store %Text zeroinitializer, %Text* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while14

while14:                                          ; preds = %endif, %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.size = call i32 @Text.size(%Text* %"*self")
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.Text.size
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  %"*self1" = load %Text*, %Text** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %1 = bitcast %Text* %"*self1" to %"Array<i8>"*
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %1, i32 %"$i2")
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  br label %if15

if15:                                             ; preds = %while14.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if15.then, %if15
  %"$c6" = load i8, i8* %c, align 1
  call void @Text.append(%Text* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while14

endloop:                                          ; preds = %while14
  %"$res" = load %Text, %Text* %res, align 8
  ret %Text %"$res"
}

define void @Title.reserve(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  br label %if17

if17:                                             ; preds = %endif
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  ret void

endif1:                                           ; preds = %if17
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self9", i32 0, i32 0
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if18

if18:                                             ; preds = %endif1
  %"*self13" = load %Title*, %Title** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Title, %Title* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if18.then, label %endif2

if18.then:                                        ; preds = %if18
  %"*self16" = load %Title*, %Title** %self, align 8
  %"*self.data17" = getelementptr inbounds %Title, %Title* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if18.then, %if18
  %"*self19" = load %Title*, %Title** %self, align 8
  %"*self.data20" = getelementptr inbounds %Title, %Title* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Title*, %Title** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Title, %Title* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Title.append(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %value = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Title.reserve(%Title* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 0
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.count8" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8 @Title.first(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %ref = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %1 = bitcast %Title* %"*self" to %"Array<i8>"*
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %1, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  ret i8 %"$*ref"
}

define i32 @main() {
entry:
  %t = alloca %Text, align 8
  %loud = alloca %Text, align 8
  %title = alloca %Title, align 8
  %ref = alloca i8*, align 8
  %ref2 = alloca i8*, align 8
  %ref5 = alloca i8*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Text zeroinitializer, %Text* %t, align 8
  %t.label = getelementptr inbounds %Text, %Text* %t, i32 0, i32 3
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i8** %t.label, align 8
  call void @Text.append(%Text* %t, i8 104)
  call void @Text.append(%Text* %t, i8 105)
  call void @Text.append(%Text* %t, i8 32)
  call void @Text.append(%Text* %t, i8 117)
  %call.Text.shout = call %Text @Text.shout(%Text* %t)
  store %Text %call.Text.shout, %Text* %loud, align 8
  store %Title zeroinitializer, %Title* %title, align 8
  call void @Title.append(%Title* %title, i8 122)
  %call.Text.size = call i32 @Text.size(%Text* %t)
  %t.words = getelementptr inbounds %Text, %Text* %t, i32 0, i32 4
  %0 = bitcast %Text* %loud to %"Array<i8>"*
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %1 = bitcast %Text* %loud to %"Array<i8>"*
  %"call.Array<i8>.[]1" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %1, i32 1)
  store i8* %"call.Array<i8>.[]1", i8** %ref2, align 8
  %"*ref3" = load i8*, i8** %ref2, align 8
  %2 = bitcast %Text* %loud to %"Array<i8>"*
  %"call.Array<i8>.[]4" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %2, i32 3)
  store i8* %"call.Array<i8>.[]4", i8** %ref5, align 8
  %"*ref6" = load i8*, i8** %ref5, align 8
  %call.Text.name = call i8* @Text.name(%Text* %t)
  %call.Title.first = call i8 @Title.first(%Title* %title)
  %"$t.words" = load i32, i32* %t.words, align 4
  %"$*ref" = load i8, i8* %"*ref", align 1
  %AS = sext i8 %"$*ref" to i32
  %"$*ref7" = load i8, i8* %"*ref3", align 1
  %AS8 = sext i8 %"$*ref7" to i32
  %"$*ref9" = load i8, i8* %"*ref6", align 1
  %AS10 = sext i8 %"$*ref9" to i32
  %AS11 = sext i8 %call.Title.first to i32
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.1, i32 0, i32 0), i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS8, i32 %AS10, i8* %call.Text.name, i32 %AS11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS12 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf13 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS12, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.1, i32 0, i32 0), i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS8, i32 %AS10, i8* %call.Text.name, i32 %AS11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size14" = load i32, i32* %format.size, align 4
  %ADD15 = add i32 %"$format.size14", 1
  %"$format.text16" = load i8*, i8** %format.text, align 8
  %"$format.size17" = load i32, i32* %format.size, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text16", 0
  %4 = insertvalue %String %3, i32 %"$format.size17", 1
  %5 = insertvalue %String %4, i32 %ADD15, 2
  call void @"io::out"(%String %5)
  %6 = bitcast %Title* %title to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  %7 = bitcast %Text* %loud to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  %8 = bitcast %Text* %t to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %8)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 037 — struct: from a struct that doesn't exist

```stderr
error: no struct named 'Nothing' to take from
at test.ura:2:18
  |
2 | struct Text from Nothing:
  |                  ^^^^^^^
error: aborting due to 1 error
1
```

## 038 — struct: String from Array<char> in "@/posix/string", temporaries are dropped

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@"cstd::F_OK" = global i32 0
@"cstd::O_RDONLY" = global i32 0
@"cstd::O_WRONLY" = global i32 0
@"cstd::O_RDWR" = global i32 0
@"cstd::O_CREAT" = global i32 0
@"cstd::O_TRUNC" = global i32 0
@"cstd::O_APPEND" = global i32 0
@"cstd::CLOCK_MONOTONIC" = global i32 0
@"cstd::S_IFMT" = global i32 0
@"cstd::S_IFREG" = global i32 0
@"cstd::S_IFDIR" = global i32 0
@"cstd::S_IRWXU" = global i32 0
@"cstd::S_IRUSR" = global i32 0
@"cstd::S_IWUSR" = global i32 0
@"cstd::AF_INET" = global i32 0
@"cstd::SOCK_STREAM" = global i32 0
@"cstd::SOL_SOCKET" = global i32 0
@"cstd::SO_REUSEADDR" = global i32 0
@"cstd::INADDR_ANY" = global i32 0
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [16 x i8] c"  Hello World  \00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"[%s] %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"lo W\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"Hell\00", align 1
@str.8 = private unnamed_addr constant [3 x i8] c"ld\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [16 x i8] c"%d %s %s %s %d\0A\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.19 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.20 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.25 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.26 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.27 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.28 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.29 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [16 x i8] c"%s %s %s %s %d\0A\00", align 1
@str.32 = private unnamed_addr constant [3 x i8] c"42\00", align 1
@TYPEOF = private unnamed_addr constant [7 x i8] c"String\00", align 1
@str.33 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

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

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
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
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1) {
entry:
  %left = alloca %"Array<i8>"*, align 8
  %right = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %left, align 8
  store %"Array<i8>"* %1, %"Array<i8>"** %right, align 8
  %"*left" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*left")
  %"*left1" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*right" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right", i32 0, i32 1
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*left1", i32 %"$*right.count")
  store i32 0, i32* %i, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"*right2" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*left5" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left5", i32 0, i32 0
  %"$*left.data" = load i8*, i8** %"*left.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*left.data[]" = getelementptr inbounds i8, i8* %"$*left.data", i32 %"$i6"
  %"*right7" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right7", i32 0, i32 0
  %"$*right.data" = load i8*, i8** %"*right.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i8, i8* %"$*right.data", i32 %"$i8"
  %"$*right.data[]" = load i8, i8* %"*right.data[]", align 1
  store i8 %"$*right.data[]", i8* %"*left.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while5

endloop:                                          ; preds = %while5
  %"*left10" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left10", i32 0, i32 1
  %"*right11" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count12" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right11", i32 0, i32 1
  %"$*right.count13" = load i32, i32* %"*right.count12", align 4
  store i32 %"$*right.count13", i32* %"*left.count", align 4
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if6

if6:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if8.then, label %endif1

if8.then:                                         ; preds = %if8
  ret void

endif1:                                           ; preds = %if8
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
  br label %if9

if9:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if9.then, label %endif2

if9.then:                                         ; preds = %if9
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if9.then, %if9
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
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if10.then, %if10
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

define i32 @String.size(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.length_of(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %text, align 8
  store i32 0, i32* %n, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$n" = load i32, i32* %n, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$n"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while11

endloop:                                          ; preds = %while11
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  ret void
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

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %i = alloca i32, align 4
  %sign = alloca i32, align 4
  %n = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %i, align 4
  store i32 1, i32* %sign, align 4
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %EQ = icmp eq i8 %"$*self.data[]", 45
  %AND = and i1 %GT, %EQ
  br i1 %AND, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  store i32 -1, i32* %sign, align 4
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  store i32 0, i32* %n, align 4
  br label %while14

while14:                                          ; preds = %endif1, %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count5"
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  %"$i9" = load i32, i32* %i, align 4
  %"*self.data[]10" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %"$i9"
  %"$*self.data[]11" = load i8, i8* %"*self.data[]10", align 1
  store i8 %"$*self.data[]11", i8* %c, align 1
  br label %if15

if15:                                             ; preds = %while14.then
  %"$c" = load i8, i8* %c, align 1
  %LT12 = icmp slt i8 %"$c", 48
  %"$c13" = load i8, i8* %c, align 1
  %GT14 = icmp sgt i8 %"$c13", 57
  %OR = or i1 %LT12, %GT14
  br i1 %OR, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  br label %endloop

endif1:                                           ; preds = %if15
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$c15" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c15" to i32
  %SUB = sub i32 %AS, 48
  %ADD = add i32 %MUL, %SUB
  store i32 %ADD, i32* %n, align 4
  %"$i16" = load i32, i32* %i, align 4
  %ADD17 = add i32 %"$i16", 1
  store i32 %ADD17, i32* %i, align 4
  br label %while14

endloop:                                          ; preds = %if15.then, %while14
  %"$n18" = load i32, i32* %n, align 4
  %"$sign" = load i32, i32* %sign, align 4
  %MUL19 = mul i32 %"$n18", %"$sign"
  ret i32 %MUL19
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while16

while16:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i1" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT2 = icmp slt i32 %"$i1", %"$*other.count"
  %AND = and i1 %LT, %LT2
  br i1 %AND, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  br label %if17

if17:                                             ; preds = %while16.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"*other5" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other5", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i6"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  %NQ = icmp ne i8 %"$*self.data[]", %"$*other.data[]"
  br i1 %NQ, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]11" = getelementptr inbounds i8, i8* %"$*self.data9", i32 %"$i10"
  %"$*self.data[]12" = load i8, i8* %"*self.data[]11", align 1
  %AS = sext i8 %"$*self.data[]12" to i32
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.data14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 0
  %"$*other.data15" = load i8*, i8** %"*other.data14", align 8
  %"$i16" = load i32, i32* %i, align 4
  %"*other.data[]17" = getelementptr inbounds i8, i8* %"$*other.data15", i32 %"$i16"
  %"$*other.data[]18" = load i8, i8* %"*other.data[]17", align 1
  %AS19 = sext i8 %"$*other.data[]18" to i32
  %SUB = sub i32 %AS, %AS19
  ret i32 %SUB

endif:                                            ; preds = %if17
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
}

define i1 @String.matches_at(%String* %0, i32 %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %text = alloca i8*, align 8
  %j = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8* %2, i8** %text, align 8
  store i32 0, i32* %j, align 4
  br label %while18

while18:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  br label %if19

if19:                                             ; preds = %while18.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  ret i1 false

endif:                                            ; preds = %if19
  br label %if20

if20:                                             ; preds = %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$index4" = load i32, i32* %index, align 4
  %"$j5" = load i32, i32* %j, align 4
  %ADD6 = add i32 %"$index4", %"$j5"
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %ADD6
  %"$text7" = load i8*, i8** %text, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"text[]9" = getelementptr inbounds i8, i8* %"$text7", i32 %"$j8"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$text[]10" = load i8, i8* %"text[]9", align 1
  %NQ11 = icmp ne i8 %"$*self.data[]", %"$text[]10"
  br i1 %NQ11, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  ret i1 false

endif1:                                           ; preds = %if20
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while18

endloop:                                          ; preds = %while18
  ret i1 true
}

define i32 @String.find(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while21

while21:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  br label %if22

if22:                                             ; preds = %while21.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if22
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
  ret i32 -1
}

define i1 @String.contains(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.find = call i32 @String.find(%String* %"*self", i8* %"$text")
  %NQ = icmp ne i32 %call.String.find, -1
  ret i1 %NQ
}

define i1 @String.starts_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self", i32 0, i8* %"$text")
  ret i1 %call.String.matches_at
}

define i1 @String.ends_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$text")
  store i32 %call.String.length_of, i32* %n, align 4
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  ret i1 false

endif:                                            ; preds = %if23
  %"*self1" = load %String*, %String** %self, align 8
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"$n5" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.count4", %"$n5"
  %"$text6" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %SUB, i8* %"$text6")
  ret i1 %call.String.matches_at
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %length = alloca i32, align 4
  %res = alloca %String, align 8
  %stop = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %start, align 4
  store i32 %2, i32* %length, align 4
  store %String zeroinitializer, %String* %res, align 8
  br label %if24

if24:                                             ; preds = %entry
  %"$start" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$start", 0
  br i1 %LT, label %if24.then, label %endif

if24.then:                                        ; preds = %if24
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"

endif:                                            ; preds = %if24
  %"$start2" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start2", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if25

if25:                                             ; preds = %endif
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if25.then, label %endif1

if25.then:                                        ; preds = %if25
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %stop, align 4
  br label %endif1

endif1:                                           ; preds = %if25.then, %if25
  %"$start6" = load i32, i32* %start, align 4
  store i32 %"$start6", i32* %i, align 4
  br label %while26

while26:                                          ; preds = %while26.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %"$stop7" = load i32, i32* %stop, align 4
  %LT8 = icmp slt i32 %"$i", %"$stop7"
  br i1 %LT8, label %while26.then, label %endloop

while26.then:                                     ; preds = %while26
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i10"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i11" = load i32, i32* %i, align 4
  %ADD12 = add i32 %"$i11", 1
  store i32 %ADD12, i32* %i, align 4
  br label %while26

endloop:                                          ; preds = %while26
  %"$res13" = load %String, %String* %res, align 8
  ret %String %"$res13"
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while27

while27:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while27.then, label %endloop

while27.then:                                     ; preds = %while27
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if28

if28:                                             ; preds = %while27.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if28.then, label %endif

if28.then:                                        ; preds = %if28
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if28.then, %if28
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while27

endloop:                                          ; preds = %while27
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while29

while29:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while29.then, label %endloop

while29.then:                                     ; preds = %while29
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if30

if30:                                             ; preds = %while29.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 65
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 90
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if30.then, label %endif

if30.then:                                        ; preds = %if30
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %ADD = add i32 %AS, 32
  %AS5 = trunc i32 %ADD to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if30.then, %if30
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while29

endloop:                                          ; preds = %while29
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i1 @String.is_space(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 32
  %"$c1" = load i8, i8* %c, align 1
  %EQ2 = icmp eq i8 %"$c1", 9
  %OR = or i1 %EQ, %EQ2
  %"$c3" = load i8, i8* %c, align 1
  %EQ4 = icmp eq i8 %"$c3", 10
  %OR5 = or i1 %OR, %EQ4
  %"$c6" = load i8, i8* %c, align 1
  %EQ7 = icmp eq i8 %"$c6", 13
  %OR8 = or i1 %OR5, %EQ7
  ret i1 %OR8
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %start, align 4
  br label %while31

while31:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$start" = load i32, i32* %start, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$start", %"$*self.count"
  br i1 %LT, label %while31.then, label %endloop

while31.then:                                     ; preds = %while31
  br label %if32

if32:                                             ; preds = %while31.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$start5" = load i32, i32* %start, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$start5"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %call.String.is_space = call i1 @String.is_space(%String* %"*self3", i8 %"$*self.data[]")
  %NOT = xor i1 %call.String.is_space, true
  br i1 %NOT, label %if32.then, label %endif

if32.then:                                        ; preds = %if32
  br label %endloop

endif:                                            ; preds = %if32
  %"$start6" = load i32, i32* %start, align 4
  %ADD = add i32 %"$start6", 1
  store i32 %ADD, i32* %start, align 4
  br label %while31

endloop:                                          ; preds = %if32.then, %while31
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  store i32 %"$*self.count9", i32* %stop, align 4
  br label %while33

while33:                                          ; preds = %endif1, %endloop
  %"$stop" = load i32, i32* %stop, align 4
  %"$start10" = load i32, i32* %start, align 4
  %GT = icmp sgt i32 %"$stop", %"$start10"
  br i1 %GT, label %while33.then, label %endloop2

while33.then:                                     ; preds = %while33
  br label %if34

if34:                                             ; preds = %while33.then
  %"*self11" = load %String*, %String** %self, align 8
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.data13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 0
  %"$stop14" = load i32, i32* %stop, align 4
  %SUB = sub i32 %"$stop14", 1
  %"$*self.data15" = load i8*, i8** %"*self.data13", align 8
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data15", i32 %SUB
  %"$*self.data[]17" = load i8, i8* %"*self.data[]16", align 1
  %call.String.is_space18 = call i1 @String.is_space(%String* %"*self11", i8 %"$*self.data[]17")
  %NOT19 = xor i1 %call.String.is_space18, true
  br i1 %NOT19, label %if34.then, label %endif1

if34.then:                                        ; preds = %if34
  br label %endloop2

endif1:                                           ; preds = %if34
  %"$stop20" = load i32, i32* %stop, align 4
  %SUB21 = sub i32 %"$stop20", 1
  store i32 %SUB21, i32* %stop, align 4
  br label %while33

endloop2:                                         ; preds = %if34.then, %while33
  %"*self22" = load %String*, %String** %self, align 8
  %"$stop23" = load i32, i32* %stop, align 4
  %"$start24" = load i32, i32* %start, align 4
  %SUB25 = sub i32 %"$stop23", %"$start24"
  %"$start26" = load i32, i32* %start, align 4
  %call.String.substr = call %String @String.substr(%String* %"*self22", i32 %"$start26", i32 %SUB25)
  ret %String %call.String.substr
}

define %String @String.replace(%String* %0, i8* %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %old = alloca i8*, align 8
  %fresh = alloca i8*, align 8
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %old, align 8
  store i8* %2, i8** %fresh, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$old" = load i8*, i8** %old, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$old")
  store i32 %call.String.length_of, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while35

while35:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while35.then, label %endloop

while35.then:                                     ; preds = %while35
  br label %if36

if36:                                             ; preds = %while35.then
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$old3" = load i8*, i8** %old, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$old3")
  %AND = and i1 %GT, %call.String.matches_at
  br i1 %AND, label %if36.then, label %else37

if36.then:                                        ; preds = %if36
  %"$fresh" = load i8*, i8** %fresh, align 8
  call void @String.join(%String* %res, i8* %"$fresh")
  %"$n4" = load i32, i32* %n, align 4
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", %"$n4"
  store i32 %ADD, i32* %i, align 4
  br label %endif

else37:                                           ; preds = %if36
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i7"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  store i32 %ADD9, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %else37, %if36.then
  br label %while35

endloop:                                          ; preds = %while35
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %times = alloca i32, align 4
  %res = alloca %String, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %times, align 4
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %k, align 4
  br label %while38

while38:                                          ; preds = %endloop, %entry
  %"$k" = load i32, i32* %k, align 4
  %"$times" = load i32, i32* %times, align 4
  %LT = icmp slt i32 %"$k", %"$times"
  br i1 %LT, label %while38.then, label %endloop1

while38.then:                                     ; preds = %while38
  store i32 0, i32* %i, align 4
  br label %while39

while39:                                          ; preds = %while39.then, %while38.then
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT2 = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT2, label %while39.then, label %endloop

while39.then:                                     ; preds = %while39
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while39

endloop:                                          ; preds = %while39
  %"$k6" = load i32, i32* %k, align 4
  %ADD7 = add i32 %"$k6", 1
  store i32 %ADD7, i32* %k, align 4
  br label %while38

endloop1:                                         ; preds = %while38
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while40

while40:                                          ; preds = %while40.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while40.then, label %endloop

while40.then:                                     ; preds = %while40
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %SUB4 = sub i32 %"$i3", 1
  store i32 %SUB4, i32* %i, align 4
  br label %while40

endloop:                                          ; preds = %while40
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i1 @"String.=="(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %EQ = icmp eq i32 %call.String.compare, 0
  ret i1 %EQ
}

define i1 @"String.!="(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %NQ = icmp ne i32 %call.String.compare, 0
  ret i1 %NQ
}

define i1 @"String.<"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %LT = icmp slt i32 %call.String.compare, 0
  ret i1 %LT
}

define void @"String.+="(%String* %0, i8* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca i8*, align 8
  store %String* %0, %String** %left, align 8
  store i8* %1, i8** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"$right" = load i8*, i8** %right, align 8
  call void @String.join(%String* %"*left", i8* %"$right")
  ret void
}

define void @"String.+=.1"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store i32 0, i32* %i, align 4
  br label %while41

while41:                                          ; preds = %while41.then, %entry
  %"*right" = load %String*, %String** %right, align 8
  %"*right.count" = getelementptr inbounds %String, %String* %"*right", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count"
  br i1 %LT, label %while41.then, label %endloop

while41.then:                                     ; preds = %while41
  %"*left" = load %String*, %String** %left, align 8
  %"*right1" = load %String*, %String** %right, align 8
  %"*right.data" = getelementptr inbounds %String, %String* %"*right1", i32 0, i32 0
  %"$*right.data" = load i8*, i8** %"*right.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i8, i8* %"$*right.data", i32 %"$i2"
  %"$*right.data[]" = load i8, i8* %"*right.data[]", align 1
  call void @String.append(%String* %"*left", i8 %"$*right.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while41

endloop:                                          ; preds = %while41
  ret void
}

define %String @"String.+"(%String* %0, i8* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca i8*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %left, align 8
  store i8* %1, i8** %right, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*left" = load %String*, %String** %left, align 8
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %"*left" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"$right" = load i8*, i8** %right, align 8
  call void @String.join(%String* %res, i8* %"$right")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @"String.+.2"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*left" = load %String*, %String** %left, align 8
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %"*left" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"*right" = load %String*, %String** %right, align 8
  call void @"String.+=.1"(%String* %res, %String* %"*right")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  %copy = alloca %String, align 8
  store %Error* %0, %Error** %error, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %1 = bitcast %String* %copy to %"Array<i8>"*
  %2 = bitcast %String* %"*error.message" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %1, %"Array<i8>"* %2)
  %"$copy" = load %String, %String* %copy, align 8
  call void @"io::err"(%String %"$copy")
  ret i32 1
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

define void @"io::err"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 2, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %t = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp = alloca %String, align 8
  %tmp13 = alloca %String, align 8
  %tmp15 = alloca %String, align 8
  %format.size17 = alloca i32, align 4
  %format.text22 = alloca i8*, align 8
  %format.size31 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text32 = alloca i8*, align 8
  %bool.text33 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %r = alloca %String, align 8
  %tmp52 = alloca %String, align 8
  %tmp54 = alloca %String, align 8
  %format.size56 = alloca i32, align 4
  %format.text61 = alloca i8*, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %tmp69 = alloca %String, align 8
  %tmp72 = alloca %String, align 8
  %format.size74 = alloca i32, align 4
  %bool.text75 = alloca i8*, align 8
  %bool.text76 = alloca i8*, align 8
  %bool.text77 = alloca i8*, align 8
  %format.text85 = alloca i8*, align 8
  %n = alloca %String, align 8
  %format.size97 = alloca i32, align 4
  %format.text102 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try42

try42:                                            ; preds = %entry
  br i1 true, label %try42.then, label %endtry

try42.then:                                       ; preds = %try42
  store i32 0, i32* @"cstd::F_OK", align 4
  store i32 0, i32* @"cstd::O_RDONLY", align 4
  store i32 1, i32* @"cstd::O_WRONLY", align 4
  store i32 2, i32* @"cstd::O_RDWR", align 4
  store i32 64, i32* @"cstd::O_CREAT", align 4
  store i32 512, i32* @"cstd::O_TRUNC", align 4
  store i32 1024, i32* @"cstd::O_APPEND", align 4
  store i32 1, i32* @"cstd::CLOCK_MONOTONIC", align 4
  store i32 61440, i32* @"cstd::S_IFMT", align 4
  store i32 32768, i32* @"cstd::S_IFREG", align 4
  store i32 16384, i32* @"cstd::S_IFDIR", align 4
  store i32 448, i32* @"cstd::S_IRWXU", align 4
  store i32 256, i32* @"cstd::S_IRUSR", align 4
  store i32 128, i32* @"cstd::S_IWUSR", align 4
  store i32 2, i32* @"cstd::AF_INET", align 4
  store i32 1, i32* @"cstd::SOCK_STREAM", align 4
  store i32 1, i32* @"cstd::SOL_SOCKET", align 4
  store i32 2, i32* @"cstd::SO_REUSEADDR", align 4
  store i32 0, i32* @"cstd::INADDR_ANY", align 4
  store %String { i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 15, i32 0 }, %String* %s, align 8
  %call.String.trim = call %String @String.trim(%String* %s)
  store %String %call.String.trim, %String* %t, align 8
  %call.String.chars = call i8* @String.chars(%String* %t)
  %call.String.size = call i32 @String.size(%String* %t)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  %call.String.upper = call %String @String.upper(%String* %t)
  store %String %call.String.upper, %String* %tmp, align 8
  %call.String.chars12 = call i8* @String.chars(%String* %tmp)
  %call.String.lower = call %String @String.lower(%String* %t)
  store %String %call.String.lower, %String* %tmp13, align 8
  %call.String.chars14 = call i8* @String.chars(%String* %tmp13)
  %call.String.reverse = call %String @String.reverse(%String* %t)
  store %String %call.String.reverse, %String* %tmp15, align 8
  %call.String.chars16 = call i8* @String.chars(%String* %tmp15)
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars12, i8* %call.String.chars14, i8* %call.String.chars16)
  store i32 %call.snprintf18, i32* %format.size17, align 4
  %"$format.size19" = load i32, i32* %format.size17, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars12, i8* %call.String.chars14, i8* %call.String.chars16)
  %"$format.size26" = load i32, i32* %format.size17, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %"$format.text28" = load i8*, i8** %format.text22, align 8
  %"$format.size29" = load i32, i32* %format.size17, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text28", 0
  %4 = insertvalue %String %3, i32 %"$format.size29", 1
  %5 = insertvalue %String %4, i32 %ADD27, 2
  call void @"io::out"(%String %5)
  %6 = bitcast %String* %tmp15 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  %7 = bitcast %String* %tmp13 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  %8 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %8)
  %call.String.find = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  %call.String.contains = call i1 @String.contains(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0))
  %call.String.starts_with = call i1 @String.starts_with(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0))
  %call.String.ends_with = call i1 @String.ends_with(%String* %t, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.8, i32 0, i32 0))
  %call.String.find30 = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  store i32 0, i32* %format.size31, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if43

if43:                                             ; preds = %try42.then
  br i1 %call.String.contains, label %if43.then, label %else44

if43.then:                                        ; preds = %if43
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.10, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else44:                                           ; preds = %if43
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else44, %if43.then
  store i8* null, i8** %bool.text32, align 8
  br label %if45

if45:                                             ; preds = %endif
  br i1 %call.String.starts_with, label %if45.then, label %else46

if45.then:                                        ; preds = %if45
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif1

else46:                                           ; preds = %if45
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif1

endif1:                                           ; preds = %else46, %if45.then
  store i8* null, i8** %bool.text33, align 8
  br label %if47

if47:                                             ; preds = %endif1
  br i1 %call.String.ends_with, label %if47.then, label %else48

if47.then:                                        ; preds = %if47
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif2

else48:                                           ; preds = %if47
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif2

endif2:                                           ; preds = %else48, %if47.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text34" = load i8*, i8** %bool.text32, align 8
  %"$bool.text35" = load i8*, i8** %bool.text33, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.16, i32 0, i32 0), i32 %call.String.find, i8* %"$bool.text", i8* %"$bool.text34", i8* %"$bool.text35", i32 %call.String.find30)
  store i32 %call.snprintf36, i32* %format.size31, align 4
  %"$format.size37" = load i32, i32* %format.size31, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text, align 8
  %"$bool.text44" = load i8*, i8** %bool.text32, align 8
  %"$bool.text45" = load i8*, i8** %bool.text33, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.16, i32 0, i32 0), i32 %call.String.find, i8* %"$bool.text43", i8* %"$bool.text44", i8* %"$bool.text45", i32 %call.String.find30)
  %"$format.size47" = load i32, i32* %format.size31, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text40, align 8
  %"$format.size50" = load i32, i32* %format.size31, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %10 = insertvalue %String %9, i32 %"$format.size50", 1
  %11 = insertvalue %String %10, i32 %ADD48, 2
  call void @"io::out"(%String %11)
  %call.String.replace = call %String @String.replace(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace, %String* %r, align 8
  %call.String.chars51 = call i8* @String.chars(%String* %r)
  %call.String.substr = call %String @String.substr(%String* %r, i32 1, i32 3)
  store %String %call.String.substr, %String* %tmp52, align 8
  %call.String.chars53 = call i8* @String.chars(%String* %tmp52)
  %call.String.repeat = call %String @String.repeat(%String* %r, i32 2)
  store %String %call.String.repeat, %String* %tmp54, align 8
  %call.String.chars55 = call i8* @String.chars(%String* %tmp54)
  %call.snprintf57 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars51, i8* %call.String.chars53, i8* %call.String.chars55)
  store i32 %call.snprintf57, i32* %format.size56, align 4
  %"$format.size58" = load i32, i32* %format.size56, align 4
  %ADD59 = add i32 %"$format.size58", 1
  %AS60 = sext i32 %ADD59 to i64
  %call.calloc62 = call i8* @calloc(i64 %AS60, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc62, i8** %format.text61, align 8
  %"$format.text63" = load i8*, i8** %format.text61, align 8
  %call.snprintf64 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text63", i64 %AS60, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars51, i8* %call.String.chars53, i8* %call.String.chars55)
  %"$format.size65" = load i32, i32* %format.size56, align 4
  %ADD66 = add i32 %"$format.size65", 1
  %"$format.text67" = load i8*, i8** %format.text61, align 8
  %"$format.size68" = load i32, i32* %format.size56, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text67", 0
  %13 = insertvalue %String %12, i32 %"$format.size68", 1
  %14 = insertvalue %String %13, i32 %ADD66, 2
  call void @"io::out"(%String %14)
  %15 = bitcast %String* %tmp54 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %15)
  %16 = bitcast %String* %tmp52 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %16)
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i32 3, i32 0 }, %String* %b, align 8
  %"call.String.+" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  store %String %"call.String.+", %String* %tmp69, align 8
  %"call.String.+70" = call %String @"String.+.2"(%String* %tmp69, %String* %b)
  store %String %"call.String.+70", %String* %c, align 8
  %17 = bitcast %String* %tmp69 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %17)
  call void @"String.+="(%String* %c, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %call.String.chars71 = call i8* @String.chars(%String* %c)
  %"call.String.<" = call i1 @"String.<"(%String* %a, %String* %b)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp72, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %a, %String* %tmp72)
  %"call.String.!=" = call i1 @"String.!="(%String* %a, %String* %b)
  %call.String.size73 = call i32 @String.size(%String* %c)
  store i32 0, i32* %format.size74, align 4
  store i8* null, i8** %bool.text75, align 8
  br label %if49

if49:                                             ; preds = %endif2
  br i1 %"call.String.<", label %if49.then, label %else50

if49.then:                                        ; preds = %if49
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0), i8** %bool.text75, align 8
  br label %endif3

else50:                                           ; preds = %if49
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0), i8** %bool.text75, align 8
  br label %endif3

endif3:                                           ; preds = %else50, %if49.then
  store i8* null, i8** %bool.text76, align 8
  br label %if51

if51:                                             ; preds = %endif3
  br i1 %"call.String.==", label %if51.then, label %else52

if51.then:                                        ; preds = %if51
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.27, i32 0, i32 0), i8** %bool.text76, align 8
  br label %endif4

else52:                                           ; preds = %if51
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.28, i32 0, i32 0), i8** %bool.text76, align 8
  br label %endif4

endif4:                                           ; preds = %else52, %if51.then
  store i8* null, i8** %bool.text77, align 8
  br label %if53

if53:                                             ; preds = %endif4
  br i1 %"call.String.!=", label %if53.then, label %else54

if53.then:                                        ; preds = %if53
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text77, align 8
  br label %endif5

else54:                                           ; preds = %if53
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text77, align 8
  br label %endif5

endif5:                                           ; preds = %else54, %if53.then
  %"$bool.text78" = load i8*, i8** %bool.text75, align 8
  %"$bool.text79" = load i8*, i8** %bool.text76, align 8
  %"$bool.text80" = load i8*, i8** %bool.text77, align 8
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.31, i32 0, i32 0), i8* %call.String.chars71, i8* %"$bool.text78", i8* %"$bool.text79", i8* %"$bool.text80", i32 %call.String.size73)
  store i32 %call.snprintf81, i32* %format.size74, align 4
  %"$format.size82" = load i32, i32* %format.size74, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %"$bool.text88" = load i8*, i8** %bool.text75, align 8
  %"$bool.text89" = load i8*, i8** %bool.text76, align 8
  %"$bool.text90" = load i8*, i8** %bool.text77, align 8
  %call.snprintf91 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.31, i32 0, i32 0), i8* %call.String.chars71, i8* %"$bool.text88", i8* %"$bool.text89", i8* %"$bool.text90", i32 %call.String.size73)
  %"$format.size92" = load i32, i32* %format.size74, align 4
  %ADD93 = add i32 %"$format.size92", 1
  %"$format.text94" = load i8*, i8** %format.text85, align 8
  %"$format.size95" = load i32, i32* %format.size74, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text94", 0
  %19 = insertvalue %String %18, i32 %"$format.size95", 1
  %20 = insertvalue %String %19, i32 %ADD93, 2
  call void @"io::out"(%String %20)
  %21 = bitcast %String* %tmp72 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %21)
  store %String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.32, i32 0, i32 0), i32 2, i32 0 }, %String* %n, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %n)
  %ADD96 = add i32 %call.String.to_int, 1
  %call.snprintf98 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), i32 %ADD96, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf98, i32* %format.size97, align 4
  %"$format.size99" = load i32, i32* %format.size97, align 4
  %ADD100 = add i32 %"$format.size99", 1
  %AS101 = sext i32 %ADD100 to i64
  %call.calloc103 = call i8* @calloc(i64 %AS101, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc103, i8** %format.text102, align 8
  %"$format.text104" = load i8*, i8** %format.text102, align 8
  %call.snprintf105 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text104", i64 %AS101, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), i32 %ADD96, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size106" = load i32, i32* %format.size97, align 4
  %ADD107 = add i32 %"$format.size106", 1
  %"$format.text108" = load i8*, i8** %format.text102, align 8
  %"$format.size109" = load i32, i32* %format.size97, align 4
  %22 = insertvalue %String zeroinitializer, i8* %"$format.text108", 0
  %23 = insertvalue %String %22, i32 %"$format.size109", 1
  %24 = insertvalue %String %23, i32 %ADD107, 2
  call void @"io::out"(%String %24)
  %25 = bitcast %String* %n to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %25)
  %26 = bitcast %String* %c to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %26)
  %27 = bitcast %String* %b to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %27)
  %28 = bitcast %String* %a to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %28)
  %29 = bitcast %String* %r to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %29)
  %30 = bitcast %String* %t to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %30)
  %31 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %31)
  ret i32 0

endtry:                                           ; preds = %try42
  br label %catch55

catch55:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch55.then, label %endcatch

catch55.then:                                     ; preds = %catch55
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch55
  br label %if56

if56:                                             ; preds = %endcatch
  %"$ucg.thrown110" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown110", label %if56.then, label %endif6

if56.then:                                        ; preds = %if56
  ret i32 1

endif6:                                           ; preds = %if56
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Plain = type { i32 }
%Owned = type { i32 }

@str = private unnamed_addr constant [15 x i8] c"plain drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"owned drop %d\0A\00", align 1

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

define void @Plain.drop(%Plain* %0) {
entry:
  %self = alloca %Plain*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Plain* %0, %Plain** %self, align 8
  %"*self" = load %Plain*, %Plain** %self, align 8
  %"*self.id" = getelementptr inbounds %Plain, %Plain* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
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

define void @Owned.drop(%Owned* %0) {
entry:
  %value = alloca %Owned*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Owned* %0, %Owned** %value, align 8
  %"*value" = load %Owned*, %Owned** %value, align 8
  %"*value.id" = getelementptr inbounds %Owned, %Owned* %"*value", i32 0, i32 0
  %"$*value.id" = load i32, i32* %"*value.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i32 %"$*value.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i32 %"$*value.id")
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
  %p = alloca %Plain, align 8
  %o = alloca %Owned, align 8
  store %Plain zeroinitializer, %Plain* %p, align 4
  %p.id = getelementptr inbounds %Plain, %Plain* %p, i32 0, i32 0
  store i32 1, i32* %p.id, align 4
  store %Owned zeroinitializer, %Owned* %o, align 4
  %o.id = getelementptr inbounds %Owned, %Owned* %o, i32 0, i32 0
  store i32 2, i32* %o.id, align 4
  call void @Plain.drop(%Plain* %p)
  call void @Owned.drop(%Owned* %o)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }
%Bag = type { %"Array<i32>", i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

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
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  ret void

endif1:                                           ; preds = %if7
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
  br label %if8

if8:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
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
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while11

while11:                                          ; preds = %while11.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
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
  br label %while11

endloop:                                          ; preds = %while11
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if12

if12:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if12.then, %if12
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

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %left = alloca %"Array<i32>"*, align 8
  %right = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %left, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %right, align 8
  %"*left" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*left")
  %"*left1" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*right" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right", i32 0, i32 1
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*left1", i32 %"$*right.count")
  store i32 0, i32* %i, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %"*right2" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*left5" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*left5", i32 0, i32 0
  %"$*left.data" = load i32*, i32** %"*left.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*left.data[]" = getelementptr inbounds i32, i32* %"$*left.data", i32 %"$i6"
  %"*right7" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right7", i32 0, i32 0
  %"$*right.data" = load i32*, i32** %"*right.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i32, i32* %"$*right.data", i32 %"$i8"
  %"$*right.data[]" = load i32, i32* %"*right.data[]", align 4
  store i32 %"$*right.data[]", i32* %"*left.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
  %"*left10" = load %"Array<i32>"*, %"Array<i32>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*left10", i32 0, i32 1
  %"*right11" = load %"Array<i32>"*, %"Array<i32>"** %right, align 8
  %"*right.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*right11", i32 0, i32 1
  %"$*right.count13" = load i32, i32* %"*right.count12", align 4
  store i32 %"$*right.count13", i32* %"*left.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %value = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %value, align 8
  %"*value" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*value")
  br label %if14

if14:                                             ; preds = %entry
  %"*value1" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*value2" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<i32>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define %"Array<i32>" @Bag.first_items(%Bag* %0) {
entry:
  %self = alloca %Bag*, align 8
  %copy = alloca %"Array<i32>", align 8
  store %Bag* %0, %Bag** %self, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self" = load %Bag*, %Bag** %self, align 8
  %"*self.items" = getelementptr inbounds %Bag, %Bag* %"*self", i32 0, i32 0
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.items")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  ret %"Array<i32>" %"$copy"
}

define %"Array<i32>" @make(i32 %0) {
entry:
  %n = alloca i32, align 4
  %a = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  store i32 0, i32* %i, align 4
  br label %while15

while15:                                          ; preds = %while15.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"$i1" = load i32, i32* %i, align 4
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 %"$i1")
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
  %"$a" = load %"Array<i32>", %"Array<i32>"* %a, align 8
  ret %"Array<i32>" %"$a"
}

define i32 @total(%"Array<i32>" %0) {
entry:
  %a = alloca %"Array<i32>", align 8
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %ref = alloca i32*, align 8
  store %"Array<i32>" %0, %"Array<i32>"* %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %while16

while16:                                          ; preds = %while16.then, %entry
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %"call.Array<i32>.size"
  br i1 %LT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  %"$i1" = load i32, i32* %i, align 4
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 %"$i1")
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*ref"
  store i32 %ADD, i32* %sum, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
  %"$sum4" = load i32, i32* %sum, align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 %"$sum4"
}

define %"Array<i32>"* @"new<Array<i32>>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %"Array<i32>"*
  ret %"Array<i32>"* %AS1
}

define void @"copy<Array<i32>>"(%"Array<i32>"* %0, %"Array<i32>"* %1, i32 %2) {
entry:
  %dst = alloca %"Array<i32>"*, align 8
  %src = alloca %"Array<i32>"*, align 8
  %count = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %dst, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %"Array<i32>"*, %"Array<i32>"** %dst, align 8
  %AS = bitcast %"Array<i32>"* %"$dst" to i8*
  %"$src" = load %"Array<i32>"*, %"Array<i32>"** %src, align 8
  %AS1 = bitcast %"Array<i32>"* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
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

define void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %"Array<i32>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  br label %if18

if18:                                             ; preds = %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if18.then, label %endif1

if18.then:                                        ; preds = %if18
  ret void

endif1:                                           ; preds = %if18
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Array<i32>>" = call %"Array<i32>"* @"new<Array<i32>>"(i32 %"$cap8")
  store %"Array<i32>"* %"call.new<Array<i32>>", %"Array<i32>"** %bigger, align 8
  %"*self9" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Array<i32>>"(%"Array<i32>"* %"$bigger", %"Array<i32>"* %"$*self.data", i32 %"$*self.count12")
  br label %if19

if19:                                             ; preds = %endif1
  %"*self13" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if19.then, label %endif2

if19.then:                                        ; preds = %if19
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if19.then, %if19
  %"*self19" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  store %"Array<i32>"* %"$bigger21", %"Array<i32>"** %"*self.data20", align 8
  %"*self22" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if21

if21:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if21.then, label %endif

if21.then:                                        ; preds = %if21
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if21.then, %if21
  br label %while22

while22:                                          ; preds = %while22.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while22.then, label %endloop

while22.then:                                     ; preds = %while22
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
  br label %while22

endloop:                                          ; preds = %while22
  ret void
}

define void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %0, %"Array<i32>" %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store %"Array<i32>" %1, %"Array<i32>"* %value, align 8
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count9"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %value)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" %"$copy", %"Array<i32>"* %"*self.data[]", align 8
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  ret void
}

define %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  %copy15 = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %value, align 8
  br label %if24

if24:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if24.then, label %endif

if24.then:                                        ; preds = %if24
  %"$value" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value"

endif:                                            ; preds = %if24
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*self2")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$index4"
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.data[]")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  store %"Array<i32>" %"$copy", %"Array<i32>"* %value, align 8
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while25

while25:                                          ; preds = %while25.then, %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while25.then, label %endloop

while25.then:                                     ; preds = %while25
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data12", i32 %"$i13"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy15, align 8
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$i18" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i18", 1
  %"$*self.data19" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  %"*self.data[]20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data19", i32 %ADD
  call void @"Array<i32>.="(%"Array<i32>"* %copy15, %"Array<i32>"* %"*self.data[]20")
  %"$copy21" = load %"Array<i32>", %"Array<i32>"* %copy15, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]14")
  store %"Array<i32>" %"$copy21", %"Array<i32>"* %"*self.data[]14", align 8
  %"$i22" = load i32, i32* %i, align 4
  %ADD23 = add i32 %"$i22", 1
  store i32 %ADD23, i32* %i, align 4
  br label %while25

endloop:                                          ; preds = %while25
  %"*self24" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count25" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self24", i32 0, i32 1
  %"$*self.count26" = load i32, i32* %"*self.count25", align 4
  %SUB27 = sub i32 %"$*self.count26", 1
  store i32 %SUB27, i32* %"*self.count25", align 4
  %"*self28" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data29" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self28", i32 0, i32 0
  %"*self30" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count31" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self30", i32 0, i32 1
  %"$*self.data32" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data29", align 8
  %"$*self.count33" = load i32, i32* %"*self.count31", align 4
  %"*self.data[]34" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data32", i32 %"$*self.count33"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]34")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]34", align 8
  %"$value35" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value35"
}

define %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %"*self", i32 %SUB)
  ret %"Array<i32>" %"call.Array<Array<i32>>.pop"
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

define %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %left, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*left")
  %"*left1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %"Array<i32>"*, %"Array<i32>"** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*left.data", i32 %"$index"
  ret %"Array<i32>"* %"*left.data[]"
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %value = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %value, align 8
  %"*value" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*value")
  br label %if26

if26:                                             ; preds = %entry
  %"*value1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"*value2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %"Array<i32>"*, %"Array<i32>"** %"*value.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  ret void
}

define i32 @main() {
entry:
  %grid = alloca %"Array<Array<i32>>", align 8
  %row = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca %"Array<i32>"*, align 8
  %copy2 = alloca %"Array<i32>", align 8
  %copy3 = alloca %"Array<i32>", align 8
  %ref5 = alloca %"Array<i32>"*, align 8
  %ref9 = alloca %"Array<i32>"*, align 8
  %ref12 = alloca %"Array<i32>"*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %bag = alloca %Bag, align 8
  %items = alloca %"Array<i32>", align 8
  %copy28 = alloca %"Array<i32>", align 8
  %copy30 = alloca %"Array<i32>", align 8
  %ref32 = alloca %"Array<i32>"*, align 8
  %format.size36 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %popped = alloca %"Array<i32>", align 8
  %format.size52 = alloca i32, align 4
  %format.text57 = alloca i8*, align 8
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %call.make = call %"Array<i32>" @make(i32 3)
  store %"Array<i32>" %call.make, %"Array<i32>"* %row, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %row)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %"$copy")
  %call.make1 = call %"Array<i32>" @make(i32 2)
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %call.make1)
  %"call.Array<Array<i32>>.[]" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]", %"Array<i32>"** %ref, align 8
  %"*ref" = load %"Array<i32>"*, %"Array<i32>"** %ref, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %"*ref", i32 9)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy3, align 8
  %"call.Array<Array<i32>>.[]4" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]4", %"Array<i32>"** %ref5, align 8
  %"*ref6" = load %"Array<i32>"*, %"Array<i32>"** %ref5, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy3, %"Array<i32>"* %"*ref6")
  %"$copy7" = load %"Array<i32>", %"Array<i32>"* %copy3, align 8
  store %"Array<i32>" %"$copy7", %"Array<i32>"* %copy2, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %copy2, i32 5)
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %"call.Array<Array<i32>>.[]8" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]8", %"Array<i32>"** %ref9, align 8
  %"*ref10" = load %"Array<i32>"*, %"Array<i32>"** %ref9, align 8
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref10")
  %"call.Array<Array<i32>>.[]11" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 1)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]11", %"Array<i32>"** %ref12, align 8
  %"*ref13" = load %"Array<i32>"*, %"Array<i32>"** %ref12, align 8
  %"call.Array<i32>.size14" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref13")
  %"call.Array<i32>.size15" = call i32 @"Array<i32>.size"(%"Array<i32>"* %copy2)
  %"call.Array<i32>.size16" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"call.Array<Array<i32>>.size", i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"call.Array<i32>.size15", i32 %"call.Array<i32>.size16")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"call.Array<Array<i32>>.size", i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"call.Array<i32>.size15", i32 %"call.Array<i32>.size16")
  %"$format.size18" = load i32, i32* %format.size, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text, align 8
  %"$format.size21" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %1 = insertvalue %String %0, i32 %"$format.size21", 1
  %2 = insertvalue %String %1, i32 %ADD19, 2
  call void @"io::out"(%String %2)
  store %Bag zeroinitializer, %Bag* %bag, align 8
  %bag.items = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make22 = call %"Array<i32>" @make(i32 4)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items)
  store %"Array<i32>" %call.make22, %"Array<i32>"* %bag.items, align 8
  %bag.items23 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make24 = call %"Array<i32>" @make(i32 2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items23)
  store %"Array<i32>" %call.make24, %"Array<i32>"* %bag.items23, align 8
  %call.Bag.first_items = call %"Array<i32>" @Bag.first_items(%Bag* %bag)
  store %"Array<i32>" %call.Bag.first_items, %"Array<i32>"* %items, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %items, i32 7)
  %bag.items25 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %"call.Array<i32>.size26" = call i32 @"Array<i32>.size"(%"Array<i32>"* %bag.items25)
  %"call.Array<i32>.size27" = call i32 @"Array<i32>.size"(%"Array<i32>"* %items)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy28, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy28, %"Array<i32>"* %items)
  %"$copy29" = load %"Array<i32>", %"Array<i32>"* %copy28, align 8
  %call.total = call i32 @total(%"Array<i32>" %"$copy29")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy30, align 8
  %"call.Array<Array<i32>>.[]31" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]31", %"Array<i32>"** %ref32, align 8
  %"*ref33" = load %"Array<i32>"*, %"Array<i32>"** %ref32, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy30, %"Array<i32>"* %"*ref33")
  %"$copy34" = load %"Array<i32>", %"Array<i32>"* %copy30, align 8
  %call.total35 = call i32 @total(%"Array<i32>" %"$copy34")
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size26", i32 %"call.Array<i32>.size27", i32 %call.total, i32 %call.total35)
  store i32 %call.snprintf37, i32* %format.size36, align 4
  %"$format.size38" = load i32, i32* %format.size36, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size26", i32 %"call.Array<i32>.size27", i32 %call.total, i32 %call.total35)
  %"$format.size45" = load i32, i32* %format.size36, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size36, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %4 = insertvalue %String %3, i32 %"$format.size48", 1
  %5 = insertvalue %String %4, i32 %ADD46, 2
  call void @"io::out"(%String %5)
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %grid)
  store %"Array<i32>" %"call.Array<Array<i32>>.pop", %"Array<i32>"* %popped, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %row, align 8
  %"call.Array<i32>.size49" = call i32 @"Array<i32>.size"(%"Array<i32>"* %popped)
  %"call.Array<i32>.size50" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %"call.Array<Array<i32>>.size51" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 %"call.Array<i32>.size49", i32 %"call.Array<i32>.size50", i32 %"call.Array<Array<i32>>.size51")
  store i32 %call.snprintf53, i32* %format.size52, align 4
  %"$format.size54" = load i32, i32* %format.size52, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %AS56 = sext i32 %ADD55 to i64
  %call.calloc58 = call i8* @calloc(i64 %AS56, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc58, i8** %format.text57, align 8
  %"$format.text59" = load i8*, i8** %format.text57, align 8
  %call.snprintf60 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text59", i64 %AS56, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 %"call.Array<i32>.size49", i32 %"call.Array<i32>.size50", i32 %"call.Array<Array<i32>>.size51")
  %"$format.size61" = load i32, i32* %format.size52, align 4
  %ADD62 = add i32 %"$format.size61", 1
  %"$format.text63" = load i8*, i8** %format.text57, align 8
  %"$format.size64" = load i32, i32* %format.size52, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text63", 0
  %7 = insertvalue %String %6, i32 %"$format.size64", 1
  %8 = insertvalue %String %7, i32 %ADD62, 2
  call void @"io::out"(%String %8)
  call void @"Array<i32>.drop"(%"Array<i32>"* %popped)
  call void @"Array<i32>.drop"(%"Array<i32>"* %items)
  %bag.items65 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items65)
  call void @"Array<i32>.drop"(%"Array<i32>"* %copy2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 041 — ownership: operator = taking the other side by value

```stderr
error: operator = takes the right side by reference, a copy would need itself
at test.ura:6:1
  |
6 | operator =(left &Vec, other Vec):
  | ^^^^^^^^
help: use operator =(left &Vec, right &Vec)
error: aborting due to 1 error
1
```

## 042 — ownership: a method called before its definition still copies a by-value String

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Greeter = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"two\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"kept %s\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"hi %s\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"one\00", align 1

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

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
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
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1) {
entry:
  %left = alloca %"Array<i8>"*, align 8
  %right = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %left, align 8
  store %"Array<i8>"* %1, %"Array<i8>"** %right, align 8
  %"*left" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*left")
  %"*left1" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*right" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right", i32 0, i32 1
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*left1", i32 %"$*right.count")
  store i32 0, i32* %i, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"*right2" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*left5" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left5", i32 0, i32 0
  %"$*left.data" = load i8*, i8** %"*left.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*left.data[]" = getelementptr inbounds i8, i8* %"$*left.data", i32 %"$i6"
  %"*right7" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right7", i32 0, i32 0
  %"$*right.data" = load i8*, i8** %"*right.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i8, i8* %"$*right.data", i32 %"$i8"
  %"$*right.data[]" = load i8, i8* %"*right.data[]", align 1
  store i8 %"$*right.data[]", i8* %"*left.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while5

endloop:                                          ; preds = %while5
  %"*left10" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left10", i32 0, i32 1
  %"*right11" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count12" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right11", i32 0, i32 1
  %"$*right.count13" = load i32, i32* %"*right.count12", align 4
  store i32 %"$*right.count13", i32* %"*left.count", align 4
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if6

if6:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if8.then, label %endif1

if8.then:                                         ; preds = %if8
  ret void

endif1:                                           ; preds = %if8
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
  br label %if9

if9:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if9.then, label %endif2

if9.then:                                         ; preds = %if9
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if9.then, %if9
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
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if10.then, %if10
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

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %while11
  ret void
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

define void @Greeter.run(%Greeter* %0, %String %1) {
entry:
  %self = alloca %Greeter*, align 8
  %name = alloca %String, align 8
  %copy = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Greeter* %0, %Greeter** %self, align 8
  store %String %1, %String* %name, align 8
  %"*self" = load %Greeter*, %Greeter** %self, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  call void @Greeter.greet(%Greeter* %"*self", %String %"$copy")
  %"*self1" = load %Greeter*, %Greeter** %self, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  call void @Greeter.greet(%Greeter* %"*self1", %String %call.String.create)
  %call.String.chars = call i8* @String.chars(%String* %name)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %5 = insertvalue %String %4, i32 %"$format.size6", 1
  %6 = insertvalue %String %5, i32 %ADD4, 2
  call void @"io::out"(%String %6)
  %7 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  ret void
}

define void @Greeter.greet(%Greeter* %0, %String %1) {
entry:
  %self = alloca %Greeter*, align 8
  %name = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Greeter* %0, %Greeter** %self, align 8
  store %String %1, %String* %name, align 8
  %call.String.chars = call i8* @String.chars(%String* %name)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %3 = insertvalue %String %2, i32 %"$format.size5", 1
  %4 = insertvalue %String %3, i32 %ADD3, 2
  call void @"io::out"(%String %4)
  %5 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  ret void
}

define i32 @main() {
entry:
  %g = alloca %Greeter, align 8
  store %Greeter zeroinitializer, %Greeter* %g, align 4
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0))
  call void @Greeter.run(%Greeter* %g, %String %call.String.create)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

