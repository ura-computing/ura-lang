# m06 llvm ir

## 002 — multi-line string: a block string, its first and last newline add nothing

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
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

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %text = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 12, i32* %a, align 4
  store %String { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 11, i32 0 }, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %"$a" = load i32, i32* %a, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i32 %"$a")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i32 %"$a")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %3 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 003 — builtin: output picks the spec from each argument

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%d %c %s %s\0A\00", align 1

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
  %c = alloca i8, align 1
  %s = alloca i8*, align 8
  %arr = alloca [3 x i8], align 1
  %flag = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store i32 42, i32* %n, align 4
  store i8 122, i8* %c, align 1
  store [3 x i8] c"hi\00", [3 x i8]* %arr, align 1
  %0 = getelementptr inbounds [3 x i8], [3 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 1
  store i1 %GT, i1* %flag, align 1
  store i32 0, i32* %format.size, align 4
  %"$n1" = load i32, i32* %n, align 4
  %"$c" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c" to i32
  %"$s" = load i8*, i8** %s, align 8
  %"$flag" = load i1, i1* %flag, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$flag", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"$n1", i32 %AS, i8* %"$s", i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS2 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS2, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text3" = load i8*, i8** %bool.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS2, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"$n1", i32 %AS, i8* %"$s", i8* %"$bool.text3")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %2 = insertvalue %String %1, i32 %"$format.size8", 1
  %3 = insertvalue %String %2, i32 %ADD6, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 004 — builtin: output with literals and escapes

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"n=%d\09tab\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"no newline\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 3, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$n")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$n")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  call void @"io::out"(%String { i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 10, i32 0 })
  call void @"io::out"(%String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 1, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 012 — output: basic output: strings, ints, floats, bools

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [35 x i8] c"=== Aldric enters the dungeon ===\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"Floor: %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1
@str.5 = private unnamed_addr constant [11 x i8] c"Speed: %f\0A\00", align 1
@str.6 = private unnamed_addr constant [17 x i8] c"Ready to fight!\0A\00", align 1

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text11 = alloca i8*, align 8
  %format.size20 = alloca i32, align 4
  %format.text25 = alloca i8*, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str, i32 0, i32 0), i32 34, i32 0 })
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 1)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 1)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store i32 0, i32* %format.size6, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 true, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %"$bool.text14" = load i8*, i8** %bool.text, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text14")
  %"$format.size16" = load i32, i32* %format.size6, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text11, align 8
  %"$format.size19" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %4 = insertvalue %String %3, i32 %"$format.size19", 1
  %5 = insertvalue %String %4, i32 %ADD17, 2
  call void @"io::out"(%String %5)
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), double 1.500000e+00)
  store i32 %call.snprintf21, i32* %format.size20, align 4
  %"$format.size22" = load i32, i32* %format.size20, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %AS24 = sext i32 %ADD23 to i64
  %call.calloc26 = call i8* @calloc(i64 %AS24, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc26, i8** %format.text25, align 8
  %"$format.text27" = load i8*, i8** %format.text25, align 8
  %call.snprintf28 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text27", i64 %AS24, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), double 1.500000e+00)
  %"$format.size29" = load i32, i32* %format.size20, align 4
  %ADD30 = add i32 %"$format.size29", 1
  %"$format.text31" = load i8*, i8** %format.text25, align 8
  %"$format.size32" = load i32, i32* %format.size20, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text31", 0
  %7 = insertvalue %String %6, i32 %"$format.size32", 1
  %8 = insertvalue %String %7, i32 %ADD30, 2
  call void @"io::out"(%String %8)
  call void @"io::out"(%String { i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.6, i32 0, i32 0), i32 16, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 013 — output: output with expressions: arithmetic inline

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [18 x i8] c"Damage dealt: %d\0A\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"XP gained:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"Gold split:   %d\0A\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Bonus:        %d\0A\00", align 1

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 17)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 17)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 150)
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 150)
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i32 0, i32 0), i32 30)
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i32 0, i32 0), i32 30)
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %7 = insertvalue %String %6, i32 %"$format.size31", 1
  %8 = insertvalue %String %7, i32 %ADD29, 2
  call void @"io::out"(%String %8)
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.3, i32 0, i32 0), i32 37)
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.3, i32 0, i32 0), i32 37)
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %10 = insertvalue %String %9, i32 %"$format.size44", 1
  %11 = insertvalue %String %10, i32 %ADD42, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 014 — output: output all supported types in one block

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"Name:  %s\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"HP:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"Grade: %c\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1
@str.6 = private unnamed_addr constant [11 x i8] c"Speed: %f\0A\00", align 1

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
  %grade = alloca i8, align 1
  %alive = alloca i1, align 1
  %speed = alloca float, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text25 = alloca i8*, align 8
  %format.size33 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text38 = alloca i8*, align 8
  %format.size47 = alloca i32, align 4
  %format.text53 = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i8 83, i8* %grade, align 1
  store i1 true, i1* %alive, align 1
  store float 1.750000e+00, float* %speed, align 4
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
  %"$grade" = load i8, i8* %grade, align 1
  %AS20 = sext i8 %"$grade" to i32
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i32 %AS20)
  store i32 %call.snprintf21, i32* %format.size19, align 4
  %"$format.size22" = load i32, i32* %format.size19, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %AS24 = sext i32 %ADD23 to i64
  %call.calloc26 = call i8* @calloc(i64 %AS24, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc26, i8** %format.text25, align 8
  %"$format.text27" = load i8*, i8** %format.text25, align 8
  %call.snprintf28 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text27", i64 %AS24, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i32 %AS20)
  %"$format.size29" = load i32, i32* %format.size19, align 4
  %ADD30 = add i32 %"$format.size29", 1
  %"$format.text31" = load i8*, i8** %format.text25, align 8
  %"$format.size32" = load i32, i32* %format.size19, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text31", 0
  %8 = insertvalue %String %7, i32 %"$format.size32", 1
  %9 = insertvalue %String %8, i32 %ADD30, 2
  call void @"io::out"(%String %9)
  store i32 0, i32* %format.size33, align 4
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf34 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf34, i32* %format.size33, align 4
  %"$format.size35" = load i32, i32* %format.size33, align 4
  %ADD36 = add i32 %"$format.size35", 1
  %AS37 = sext i32 %ADD36 to i64
  %call.calloc39 = call i8* @calloc(i64 %AS37, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc39, i8** %format.text38, align 8
  %"$format.text40" = load i8*, i8** %format.text38, align 8
  %"$bool.text41" = load i8*, i8** %bool.text, align 8
  %call.snprintf42 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text40", i64 %AS37, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text41")
  %"$format.size43" = load i32, i32* %format.size33, align 4
  %ADD44 = add i32 %"$format.size43", 1
  %"$format.text45" = load i8*, i8** %format.text38, align 8
  %"$format.size46" = load i32, i32* %format.size33, align 4
  %10 = insertvalue %String zeroinitializer, i8* %"$format.text45", 0
  %11 = insertvalue %String %10, i32 %"$format.size46", 1
  %12 = insertvalue %String %11, i32 %ADD44, 2
  call void @"io::out"(%String %12)
  %"$speed" = load float, float* %speed, align 4
  %AS48 = fpext float %"$speed" to double
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.6, i32 0, i32 0), double %AS48)
  store i32 %call.snprintf49, i32* %format.size47, align 4
  %"$format.size50" = load i32, i32* %format.size47, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %AS52 = sext i32 %ADD51 to i64
  %call.calloc54 = call i8* @calloc(i64 %AS52, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc54, i8** %format.text53, align 8
  %"$format.text55" = load i8*, i8** %format.text53, align 8
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text55", i64 %AS52, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.6, i32 0, i32 0), double %AS48)
  %"$format.size57" = load i32, i32* %format.size47, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %"$format.text59" = load i8*, i8** %format.text53, align 8
  %"$format.size60" = load i32, i32* %format.size47, align 4
  %13 = insertvalue %String zeroinitializer, i8* %"$format.text59", 0
  %14 = insertvalue %String %13, i32 %"$format.size60", 1
  %15 = insertvalue %String %14, i32 %ADD58, 2
  call void @"io::out"(%String %15)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 015 — output: output() auto-formats each argument by its type

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"hello %s %d %s\0A\00", align 1

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
  %arr = alloca [4 x i8], align 1
  %n = alloca i32, align 4
  %ok = alloca i1, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  store [4 x i8] c"ura\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 42, i32* %n, align 4
  store i1 true, i1* %ok, align 1
  store i32 0, i32* %format.size, align 4
  %"$name" = load i8*, i8** %name, align 8
  %"$n" = load i32, i32* %n, align 4
  %"$ok" = load i1, i1* %ok, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$ok", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i8* %"$name", i32 %"$n", i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text1" = load i8*, i8** %bool.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i8* %"$name", i32 %"$n", i8* %"$bool.text1")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 016 — string: String constructors and accessors

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
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [10 x i8] c"%s %d %s\0A\00", align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.16 = private unnamed_addr constant [13 x i8] c"%c %c %d %d\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 0, i8* %value, align 1
  br label %if11

if11:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %GE = icmp sge i32 %"$index", 0
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$index1", %"$*self.count"
  %AND = and i1 %GE, %LT
  br i1 %AND, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index3" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index3"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
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

define i1 @String.is_empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
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

define %String @String.create.1(i32 %0) {
entry:
  %number = alloca i32, align 4
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  store i32 %0, i32* %number, align 4
  store %String zeroinitializer, %String* %res, align 8
  %"$number" = load i32, i32* %number, align 4
  store i32 %"$number", i32* %n, align 4
  br label %if12

if12:                                             ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"$n3" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n3"
  store i32 %SUB, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if12.then, %if12
  br label %while13

while13:                                          ; preds = %endif1, %endif
  br i1 true, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"$n4" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n4", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  call void @String.append(%String* %res, i8 %AS)
  %"$n5" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n5", 10
  store i32 %DIV, i32* %n, align 4
  br label %if14

if14:                                             ; preds = %while13.then
  %"$n6" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n6", 0
  br i1 %EQ, label %if14.then, label %endif1

if14.then:                                        ; preds = %if14
  br label %endloop

endif1:                                           ; preds = %if14
  br label %while13

endloop:                                          ; preds = %if14.then, %while13
  br label %if15

if15:                                             ; preds = %endloop
  %"$number7" = load i32, i32* %number, align 4
  %LT8 = icmp slt i32 %"$number7", 0
  br i1 %LT8, label %if15.then, label %endif2

if15.then:                                        ; preds = %if15
  call void @String.append(%String* %res, i8 45)
  br label %endif2

endif2:                                           ; preds = %if15.then, %if15
  %call.String.reverse = call %String @String.reverse(%String* %res)
  %1 = bitcast %String* %res to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret %String %call.String.reverse
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while16

while16:                                          ; preds = %while16.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
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
  br label %while17

while17:                                          ; preds = %while17.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
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
  br label %while17

endloop:                                          ; preds = %while17
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
  %s1 = alloca %String, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %format.size13 = alloca i32, align 4
  %bool.text14 = alloca i8*, align 8
  %format.text20 = alloca i8*, align 8
  %s3 = alloca %String, align 8
  %format.size32 = alloca i32, align 4
  %bool.text33 = alloca i8*, align 8
  %format.text39 = alloca i8*, align 8
  %s4 = alloca %String, align 8
  %format.size50 = alloca i32, align 4
  %format.text55 = alloca i8*, align 8
  %s5 = alloca %String, align 8
  %format.size65 = alloca i32, align 4
  %format.text70 = alloca i8*, align 8
  %s6 = alloca %String, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %format.size98 = alloca i32, align 4
  %format.text105 = alloca i8*, align 8
  %format.size114 = alloca i32, align 4
  %format.text119 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try18

try18:                                            ; preds = %entry
  br i1 true, label %try18.then, label %endtry

try18.then:                                       ; preds = %try18
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s1, align 8
  %call.String.size = call i32 @String.size(%String* %s1)
  %call.String.is_empty = call i1 @String.is_empty(%String* %s1)
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if19

if19:                                             ; preds = %try18.then
  br i1 %call.String.is_empty, label %if19.then, label %else20

if19.then:                                        ; preds = %if19
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else20:                                           ; preds = %if19
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else20, %if19.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %call.String.size, i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text4" = load i8*, i8** %bool.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %call.String.size, i8* %"$bool.text4")
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  %call.String.create10 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  store %String %call.String.create10, %String* %s2, align 8
  %call.String.chars = call i8* @String.chars(%String* %s2)
  %call.String.size11 = call i32 @String.size(%String* %s2)
  %call.String.is_empty12 = call i1 @String.is_empty(%String* %s2)
  store i32 0, i32* %format.size13, align 4
  store i8* null, i8** %bool.text14, align 8
  br label %if21

if21:                                             ; preds = %endif
  br i1 %call.String.is_empty12, label %if21.then, label %else22

if21.then:                                        ; preds = %if21
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif1

else22:                                           ; preds = %if21
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif1

endif1:                                           ; preds = %else22, %if21.then
  %"$bool.text15" = load i8*, i8** %bool.text14, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size11, i8* %"$bool.text15")
  store i32 %call.snprintf16, i32* %format.size13, align 4
  %"$format.size17" = load i32, i32* %format.size13, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %AS19 = sext i32 %ADD18 to i64
  %call.calloc21 = call i8* @calloc(i64 %AS19, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc21, i8** %format.text20, align 8
  %"$format.text22" = load i8*, i8** %format.text20, align 8
  %"$bool.text23" = load i8*, i8** %bool.text14, align 8
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text22", i64 %AS19, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size11, i8* %"$bool.text23")
  %"$format.size25" = load i32, i32* %format.size13, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %"$format.text27" = load i8*, i8** %format.text20, align 8
  %"$format.size28" = load i32, i32* %format.size13, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text27", 0
  %4 = insertvalue %String %3, i32 %"$format.size28", 1
  %5 = insertvalue %String %4, i32 %ADD26, 2
  call void @"io::out"(%String %5)
  %call.String.create29 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0))
  store %String %call.String.create29, %String* %s3, align 8
  %call.String.size30 = call i32 @String.size(%String* %s3)
  %call.String.is_empty31 = call i1 @String.is_empty(%String* %s3)
  store i32 0, i32* %format.size32, align 4
  store i8* null, i8** %bool.text33, align 8
  br label %if23

if23:                                             ; preds = %endif1
  br i1 %call.String.is_empty31, label %if23.then, label %else24

if23.then:                                        ; preds = %if23
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.10, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif2

else24:                                           ; preds = %if23
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif2

endif2:                                           ; preds = %else24, %if23.then
  %"$bool.text34" = load i8*, i8** %bool.text33, align 8
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size30, i8* %"$bool.text34")
  store i32 %call.snprintf35, i32* %format.size32, align 4
  %"$format.size36" = load i32, i32* %format.size32, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %"$bool.text42" = load i8*, i8** %bool.text33, align 8
  %call.snprintf43 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size30, i8* %"$bool.text42")
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %ADD45 = add i32 %"$format.size44", 1
  %"$format.text46" = load i8*, i8** %format.text39, align 8
  %"$format.size47" = load i32, i32* %format.size32, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text46", 0
  %7 = insertvalue %String %6, i32 %"$format.size47", 1
  %8 = insertvalue %String %7, i32 %ADD45, 2
  call void @"io::out"(%String %8)
  %call.String.create48 = call %String @String.create.1(i32 42)
  store %String %call.String.create48, %String* %s4, align 8
  %call.String.chars49 = call i8* @String.chars(%String* %s4)
  %call.snprintf51 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars49)
  store i32 %call.snprintf51, i32* %format.size50, align 4
  %"$format.size52" = load i32, i32* %format.size50, align 4
  %ADD53 = add i32 %"$format.size52", 1
  %AS54 = sext i32 %ADD53 to i64
  %call.calloc56 = call i8* @calloc(i64 %AS54, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc56, i8** %format.text55, align 8
  %"$format.text57" = load i8*, i8** %format.text55, align 8
  %call.snprintf58 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text57", i64 %AS54, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars49)
  %"$format.size59" = load i32, i32* %format.size50, align 4
  %ADD60 = add i32 %"$format.size59", 1
  %"$format.text61" = load i8*, i8** %format.text55, align 8
  %"$format.size62" = load i32, i32* %format.size50, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text61", 0
  %10 = insertvalue %String %9, i32 %"$format.size62", 1
  %11 = insertvalue %String %10, i32 %ADD60, 2
  call void @"io::out"(%String %11)
  %call.String.create63 = call %String @String.create.1(i32 -123)
  store %String %call.String.create63, %String* %s5, align 8
  %call.String.chars64 = call i8* @String.chars(%String* %s5)
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %call.String.chars64)
  store i32 %call.snprintf66, i32* %format.size65, align 4
  %"$format.size67" = load i32, i32* %format.size65, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %call.snprintf73 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %call.String.chars64)
  %"$format.size74" = load i32, i32* %format.size65, align 4
  %ADD75 = add i32 %"$format.size74", 1
  %"$format.text76" = load i8*, i8** %format.text70, align 8
  %"$format.size77" = load i32, i32* %format.size65, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text76", 0
  %13 = insertvalue %String %12, i32 %"$format.size77", 1
  %14 = insertvalue %String %13, i32 %ADD75, 2
  call void @"io::out"(%String %14)
  %call.String.create78 = call %String @String.create.1(i32 0)
  store %String %call.String.create78, %String* %s6, align 8
  %call.String.chars79 = call i8* @String.chars(%String* %s6)
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i8* %call.String.chars79)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i8* %call.String.chars79)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %16 = insertvalue %String %15, i32 %"$format.size92", 1
  %17 = insertvalue %String %16, i32 %ADD90, 2
  call void @"io::out"(%String %17)
  %call.String.at = call i8 @String.at(%String* %s2, i32 0)
  %call.String.at93 = call i8 @String.at(%String* %s2, i32 4)
  %call.String.at94 = call i8 @String.at(%String* %s2, i32 -1)
  %AS95 = sext i8 %call.String.at94 to i32
  %call.String.at96 = call i8 @String.at(%String* %s2, i32 99)
  %AS97 = sext i8 %call.String.at96 to i32
  %AS99 = sext i8 %call.String.at to i32
  %AS100 = sext i8 %call.String.at93 to i32
  %call.snprintf101 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.16, i32 0, i32 0), i32 %AS99, i32 %AS100, i32 %AS95, i32 %AS97)
  store i32 %call.snprintf101, i32* %format.size98, align 4
  %"$format.size102" = load i32, i32* %format.size98, align 4
  %ADD103 = add i32 %"$format.size102", 1
  %AS104 = sext i32 %ADD103 to i64
  %call.calloc106 = call i8* @calloc(i64 %AS104, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc106, i8** %format.text105, align 8
  %"$format.text107" = load i8*, i8** %format.text105, align 8
  %call.snprintf108 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text107", i64 %AS104, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.16, i32 0, i32 0), i32 %AS99, i32 %AS100, i32 %AS95, i32 %AS97)
  %"$format.size109" = load i32, i32* %format.size98, align 4
  %ADD110 = add i32 %"$format.size109", 1
  %"$format.text111" = load i8*, i8** %format.text105, align 8
  %"$format.size112" = load i32, i32* %format.size98, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text111", 0
  %19 = insertvalue %String %18, i32 %"$format.size112", 1
  %20 = insertvalue %String %19, i32 %ADD110, 2
  call void @"io::out"(%String %20)
  %call.String.chars113 = call i8* @String.chars(%String* %s2)
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars113)
  store i32 %call.snprintf115, i32* %format.size114, align 4
  %"$format.size116" = load i32, i32* %format.size114, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %AS118 = sext i32 %ADD117 to i64
  %call.calloc120 = call i8* @calloc(i64 %AS118, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc120, i8** %format.text119, align 8
  %"$format.text121" = load i8*, i8** %format.text119, align 8
  %call.snprintf122 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text121", i64 %AS118, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars113)
  %"$format.size123" = load i32, i32* %format.size114, align 4
  %ADD124 = add i32 %"$format.size123", 1
  %"$format.text125" = load i8*, i8** %format.text119, align 8
  %"$format.size126" = load i32, i32* %format.size114, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text125", 0
  %22 = insertvalue %String %21, i32 %"$format.size126", 1
  %23 = insertvalue %String %22, i32 %ADD124, 2
  call void @"io::out"(%String %23)
  %24 = bitcast %String* %s6 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %24)
  %25 = bitcast %String* %s5 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %25)
  %26 = bitcast %String* %s4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %26)
  %27 = bitcast %String* %s3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %27)
  %28 = bitcast %String* %s2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %28)
  %29 = bitcast %String* %s1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %29)
  ret i32 0

endtry:                                           ; preds = %try18
  br label %catch25

catch25:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch25.then, label %endcatch

catch25.then:                                     ; preds = %catch25
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch25
  br label %if26

if26:                                             ; preds = %endcatch
  %"$ucg.thrown127" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown127", label %if26.then, label %endif3

if26.then:                                        ; preds = %if26
  ret i32 1

endif3:                                           ; preds = %if26
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 017 — string: assign, push, pop, join and clear

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
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@str.4 = private unnamed_addr constant [36 x i8] c"hello world this is a longer string\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@str.12 = private unnamed_addr constant [10 x i8] c"%c %s %d\0A\00", align 1
@str.13 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.15 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.18 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.19 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

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

define void @String.own(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %while12

while12:                                          ; preds = %while12.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while12

endloop:                                          ; preds = %while12
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if13.then, %if13
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

define i8 @String.pop(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 0, i8* %value, align 1
  br label %if14

if14:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"

endif:                                            ; preds = %if14
  %"*self2" = load %String*, %String** %self, align 8
  call void @String.own(%String* %"*self2")
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while15

while15:                                          ; preds = %while15.then, %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.count7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.data11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 0
  %"$*self.data12" = load i8*, i8** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds i8, i8* %"$*self.data12", i32 %"$i13"
  %"*self15" = load %String*, %String** %self, align 8
  %"*self.data16" = getelementptr inbounds %String, %String* %"*self15", i32 0, i32 0
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  %"$*self.data18" = load i8*, i8** %"*self.data16", align 8
  %"*self.data[]19" = getelementptr inbounds i8, i8* %"$*self.data18", i32 %ADD
  %"$*self.data[]20" = load i8, i8* %"*self.data[]19", align 1
  store i8 %"$*self.data[]20", i8* %"*self.data[]14", align 1
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.count24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %SUB26 = sub i32 %"$*self.count25", 1
  store i32 %SUB26, i32* %"*self.count24", align 4
  %"*self27" = load %String*, %String** %self, align 8
  %"*self.data28" = getelementptr inbounds %String, %String* %"*self27", i32 0, i32 0
  %"*self29" = load %String*, %String** %self, align 8
  %"*self.count30" = getelementptr inbounds %String, %String* %"*self29", i32 0, i32 1
  %"$*self.data31" = load i8*, i8** %"*self.data28", align 8
  %"$*self.count32" = load i32, i32* %"*self.count30", align 4
  %"*self.data[]33" = getelementptr inbounds i8, i8* %"$*self.data31", i32 %"$*self.count32"
  store i8 0, i8* %"*self.data[]33", align 1
  %"$value34" = load i8, i8* %value, align 1
  ret i8 %"$value34"
}

define i8 @String.pop.1(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %call.String.pop = call i8 @String.pop(%String* %"*self", i32 %SUB)
  ret i8 %call.String.pop
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

define i1 @String.is_empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
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
  br label %while16

while16:                                          ; preds = %while16.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  %format.size23 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text28 = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %format.size40 = alloca i32, align 4
  %format.text45 = alloca i8*, align 8
  %c = alloca i8, align 1
  %format.size55 = alloca i32, align 4
  %format.text61 = alloca i8*, align 8
  %c2 = alloca i8, align 1
  %format.size74 = alloca i32, align 4
  %format.text79 = alloca i8*, align 8
  %s3 = alloca %String, align 8
  %format.size89 = alloca i32, align 4
  %format.text94 = alloca i8*, align 8
  %format.size104 = alloca i32, align 4
  %bool.text105 = alloca i8*, align 8
  %format.text111 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try17

try17:                                            ; preds = %entry
  br i1 true, label %try17.then, label %endtry

try17.then:                                       ; preds = %try17
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %0 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %0)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 3, i32 0 }, %String* %s, align 8
  %call.String.chars = call i8* @String.chars(%String* %s)
  %call.String.size = call i32 @String.size(%String* %s)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %2 = insertvalue %String %1, i32 %"$format.size7", 1
  %3 = insertvalue %String %2, i32 %ADD5, 2
  call void @"io::out"(%String %3)
  %4 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  store %String { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.4, i32 0, i32 0), i32 35, i32 0 }, %String* %s, align 8
  %call.String.chars8 = call i8* @String.chars(%String* %s)
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars8)
  store i32 %call.snprintf10, i32* %format.size9, align 4
  %"$format.size11" = load i32, i32* %format.size9, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars8)
  %"$format.size18" = load i32, i32* %format.size9, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size9, align 4
  %5 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %6 = insertvalue %String %5, i32 %"$format.size21", 1
  %7 = insertvalue %String %6, i32 %ADD19, 2
  call void @"io::out"(%String %7)
  %8 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %8)
  store %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i32 0, i32 0 }, %String* %s, align 8
  %call.String.size22 = call i32 @String.size(%String* %s)
  %call.String.is_empty = call i1 @String.is_empty(%String* %s)
  store i32 0, i32* %format.size23, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if18

if18:                                             ; preds = %try17.then
  br i1 %call.String.is_empty, label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else19:                                           ; preds = %if18
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.8, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else19, %if18.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.9, i32 0, i32 0), i32 %call.String.size22, i8* %"$bool.text")
  store i32 %call.snprintf24, i32* %format.size23, align 4
  %"$format.size25" = load i32, i32* %format.size23, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %AS27 = sext i32 %ADD26 to i64
  %call.calloc29 = call i8* @calloc(i64 %AS27, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc29, i8** %format.text28, align 8
  %"$format.text30" = load i8*, i8** %format.text28, align 8
  %"$bool.text31" = load i8*, i8** %bool.text, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text30", i64 %AS27, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.9, i32 0, i32 0), i32 %call.String.size22, i8* %"$bool.text31")
  %"$format.size33" = load i32, i32* %format.size23, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text28, align 8
  %"$format.size36" = load i32, i32* %format.size23, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %10 = insertvalue %String %9, i32 %"$format.size36", 1
  %11 = insertvalue %String %10, i32 %ADD34, 2
  call void @"io::out"(%String %11)
  %call.String.create37 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0))
  store %String %call.String.create37, %String* %s2, align 8
  call void @String.append(%String* %s2, i8 88)
  call void @String.append(%String* %s2, i8 89)
  call void @String.append(%String* %s2, i8 90)
  %call.String.chars38 = call i8* @String.chars(%String* %s2)
  %call.String.size39 = call i32 @String.size(%String* %s2)
  %call.snprintf41 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.11, i32 0, i32 0), i8* %call.String.chars38, i32 %call.String.size39)
  store i32 %call.snprintf41, i32* %format.size40, align 4
  %"$format.size42" = load i32, i32* %format.size40, align 4
  %ADD43 = add i32 %"$format.size42", 1
  %AS44 = sext i32 %ADD43 to i64
  %call.calloc46 = call i8* @calloc(i64 %AS44, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc46, i8** %format.text45, align 8
  %"$format.text47" = load i8*, i8** %format.text45, align 8
  %call.snprintf48 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text47", i64 %AS44, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.11, i32 0, i32 0), i8* %call.String.chars38, i32 %call.String.size39)
  %"$format.size49" = load i32, i32* %format.size40, align 4
  %ADD50 = add i32 %"$format.size49", 1
  %"$format.text51" = load i8*, i8** %format.text45, align 8
  %"$format.size52" = load i32, i32* %format.size40, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text51", 0
  %13 = insertvalue %String %12, i32 %"$format.size52", 1
  %14 = insertvalue %String %13, i32 %ADD50, 2
  call void @"io::out"(%String %14)
  %call.String.pop = call i8 @String.pop.1(%String* %s2)
  store i8 %call.String.pop, i8* %c, align 1
  %call.String.chars53 = call i8* @String.chars(%String* %s2)
  %call.String.size54 = call i32 @String.size(%String* %s2)
  %"$c" = load i8, i8* %c, align 1
  %AS56 = sext i8 %"$c" to i32
  %call.snprintf57 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), i32 %AS56, i8* %call.String.chars53, i32 %call.String.size54)
  store i32 %call.snprintf57, i32* %format.size55, align 4
  %"$format.size58" = load i32, i32* %format.size55, align 4
  %ADD59 = add i32 %"$format.size58", 1
  %AS60 = sext i32 %ADD59 to i64
  %call.calloc62 = call i8* @calloc(i64 %AS60, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc62, i8** %format.text61, align 8
  %"$format.text63" = load i8*, i8** %format.text61, align 8
  %call.snprintf64 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text63", i64 %AS60, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), i32 %AS56, i8* %call.String.chars53, i32 %call.String.size54)
  %"$format.size65" = load i32, i32* %format.size55, align 4
  %ADD66 = add i32 %"$format.size65", 1
  %"$format.text67" = load i8*, i8** %format.text61, align 8
  %"$format.size68" = load i32, i32* %format.size55, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text67", 0
  %16 = insertvalue %String %15, i32 %"$format.size68", 1
  %17 = insertvalue %String %16, i32 %ADD66, 2
  call void @"io::out"(%String %17)
  %call.String.pop69 = call i8 @String.pop.1(%String* %s2)
  %call.String.pop70 = call i8 @String.pop.1(%String* %s2)
  %call.String.pop71 = call i8 @String.pop.1(%String* %s2)
  store i8 %call.String.pop71, i8* %c2, align 1
  %"$c2" = load i8, i8* %c2, align 1
  %AS72 = sext i8 %"$c2" to i32
  %call.String.size73 = call i32 @String.size(%String* %s2)
  %call.snprintf75 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.13, i32 0, i32 0), i32 %AS72, i32 %call.String.size73)
  store i32 %call.snprintf75, i32* %format.size74, align 4
  %"$format.size76" = load i32, i32* %format.size74, align 4
  %ADD77 = add i32 %"$format.size76", 1
  %AS78 = sext i32 %ADD77 to i64
  %call.calloc80 = call i8* @calloc(i64 %AS78, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc80, i8** %format.text79, align 8
  %"$format.text81" = load i8*, i8** %format.text79, align 8
  %call.snprintf82 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text81", i64 %AS78, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.13, i32 0, i32 0), i32 %AS72, i32 %call.String.size73)
  %"$format.size83" = load i32, i32* %format.size74, align 4
  %ADD84 = add i32 %"$format.size83", 1
  %"$format.text85" = load i8*, i8** %format.text79, align 8
  %"$format.size86" = load i32, i32* %format.size74, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text85", 0
  %19 = insertvalue %String %18, i32 %"$format.size86", 1
  %20 = insertvalue %String %19, i32 %ADD84, 2
  call void @"io::out"(%String %20)
  %call.String.create87 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0))
  store %String %call.String.create87, %String* %s3, align 8
  call void @String.join(%String* %s3, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.15, i32 0, i32 0))
  %call.String.chars88 = call i8* @String.chars(%String* %s3)
  %call.snprintf90 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars88)
  store i32 %call.snprintf90, i32* %format.size89, align 4
  %"$format.size91" = load i32, i32* %format.size89, align 4
  %ADD92 = add i32 %"$format.size91", 1
  %AS93 = sext i32 %ADD92 to i64
  %call.calloc95 = call i8* @calloc(i64 %AS93, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc95, i8** %format.text94, align 8
  %"$format.text96" = load i8*, i8** %format.text94, align 8
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text96", i64 %AS93, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars88)
  %"$format.size98" = load i32, i32* %format.size89, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %"$format.text100" = load i8*, i8** %format.text94, align 8
  %"$format.size101" = load i32, i32* %format.size89, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text100", 0
  %22 = insertvalue %String %21, i32 %"$format.size101", 1
  %23 = insertvalue %String %22, i32 %ADD99, 2
  call void @"io::out"(%String %23)
  call void @String.clear(%String* %s3)
  %call.String.size102 = call i32 @String.size(%String* %s3)
  %call.String.is_empty103 = call i1 @String.is_empty(%String* %s3)
  store i32 0, i32* %format.size104, align 4
  store i8* null, i8** %bool.text105, align 8
  br label %if20

if20:                                             ; preds = %endif
  br i1 %call.String.is_empty103, label %if20.then, label %else21

if20.then:                                        ; preds = %if20
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i8** %bool.text105, align 8
  br label %endif1

else21:                                           ; preds = %if20
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i8** %bool.text105, align 8
  br label %endif1

endif1:                                           ; preds = %else21, %if20.then
  %"$bool.text106" = load i8*, i8** %bool.text105, align 8
  %call.snprintf107 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.19, i32 0, i32 0), i32 %call.String.size102, i8* %"$bool.text106")
  store i32 %call.snprintf107, i32* %format.size104, align 4
  %"$format.size108" = load i32, i32* %format.size104, align 4
  %ADD109 = add i32 %"$format.size108", 1
  %AS110 = sext i32 %ADD109 to i64
  %call.calloc112 = call i8* @calloc(i64 %AS110, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc112, i8** %format.text111, align 8
  %"$format.text113" = load i8*, i8** %format.text111, align 8
  %"$bool.text114" = load i8*, i8** %bool.text105, align 8
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text113", i64 %AS110, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.19, i32 0, i32 0), i32 %call.String.size102, i8* %"$bool.text114")
  %"$format.size116" = load i32, i32* %format.size104, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %"$format.text118" = load i8*, i8** %format.text111, align 8
  %"$format.size119" = load i32, i32* %format.size104, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text118", 0
  %25 = insertvalue %String %24, i32 %"$format.size119", 1
  %26 = insertvalue %String %25, i32 %ADD117, 2
  call void @"io::out"(%String %26)
  %27 = bitcast %String* %s3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %27)
  %28 = bitcast %String* %s2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %28)
  %29 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %29)
  ret i32 0

endtry:                                           ; preds = %try17
  br label %catch22

catch22:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch22.then, label %endcatch

catch22.then:                                     ; preds = %catch22
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch22
  br label %if23

if23:                                             ; preds = %endcatch
  %"$ucg.thrown120" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown120", label %if23.then, label %endif2

if23.then:                                        ; preds = %if23
  ret i32 1

endif2:                                           ; preds = %if23
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 018 — string: find, contains, starts_with and ends_with

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
@str = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.24 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.26 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.27 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.28 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.29 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.30 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.31 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.32 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.33 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.34 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.35 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.36 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.37 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.38 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.39 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@str.40 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.41 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.42 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.43 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.44 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.45 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.47 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.48 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.49 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.50 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.51 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.52 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.53 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.54 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.55 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.56 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.57 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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
  br label %while13

while13:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  br label %if14

if14:                                             ; preds = %while13.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  ret i1 false

endif:                                            ; preds = %if14
  br label %if15

if15:                                             ; preds = %endif
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
  br i1 %NQ11, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret i1 false

endif1:                                           ; preds = %if15
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %while16

while16:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  br label %if17

if17:                                             ; preds = %while16.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if17
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
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
  br label %if18

if18:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  ret i1 false

endif:                                            ; preds = %if18
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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size18 = alloca i32, align 4
  %format.text23 = alloca i8*, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  %format.size46 = alloca i32, align 4
  %format.text51 = alloca i8*, align 8
  %format.size60 = alloca i32, align 4
  %format.text65 = alloca i8*, align 8
  %format.size73 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text78 = alloca i8*, align 8
  %format.size88 = alloca i32, align 4
  %bool.text89 = alloca i8*, align 8
  %format.text95 = alloca i8*, align 8
  %format.size104 = alloca i32, align 4
  %bool.text105 = alloca i8*, align 8
  %format.text111 = alloca i8*, align 8
  %format.size121 = alloca i32, align 4
  %bool.text122 = alloca i8*, align 8
  %format.text128 = alloca i8*, align 8
  %format.size138 = alloca i32, align 4
  %bool.text139 = alloca i8*, align 8
  %format.text145 = alloca i8*, align 8
  %format.size154 = alloca i32, align 4
  %bool.text155 = alloca i8*, align 8
  %format.text161 = alloca i8*, align 8
  %format.size171 = alloca i32, align 4
  %bool.text172 = alloca i8*, align 8
  %format.text178 = alloca i8*, align 8
  %format.size188 = alloca i32, align 4
  %bool.text189 = alloca i8*, align 8
  %format.text195 = alloca i8*, align 8
  %empty = alloca %String, align 8
  %format.size206 = alloca i32, align 4
  %format.text211 = alloca i8*, align 8
  %format.size220 = alloca i32, align 4
  %bool.text221 = alloca i8*, align 8
  %format.text227 = alloca i8*, align 8
  %format.size237 = alloca i32, align 4
  %bool.text238 = alloca i8*, align 8
  %format.text244 = alloca i8*, align 8
  %format.size254 = alloca i32, align 4
  %bool.text255 = alloca i8*, align 8
  %format.text261 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try19

try19:                                            ; preds = %entry
  br i1 true, label %try19.then, label %endtry

try19.then:                                       ; preds = %try19
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %call.String.find = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.find)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf12 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.find)
  %"$format.size13" = load i32, i32* %format.size, align 4
  %ADD14 = add i32 %"$format.size13", 1
  %"$format.text15" = load i8*, i8** %format.text, align 8
  %"$format.size16" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text15", 0
  %1 = insertvalue %String %0, i32 %"$format.size16", 1
  %2 = insertvalue %String %1, i32 %ADD14, 2
  call void @"io::out"(%String %2)
  %call.String.find17 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  %call.snprintf19 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.find17)
  store i32 %call.snprintf19, i32* %format.size18, align 4
  %"$format.size20" = load i32, i32* %format.size18, align 4
  %ADD21 = add i32 %"$format.size20", 1
  %AS22 = sext i32 %ADD21 to i64
  %call.calloc24 = call i8* @calloc(i64 %AS22, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc24, i8** %format.text23, align 8
  %"$format.text25" = load i8*, i8** %format.text23, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text25", i64 %AS22, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.find17)
  %"$format.size27" = load i32, i32* %format.size18, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text23, align 8
  %"$format.size30" = load i32, i32* %format.size18, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  %call.String.find31 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0))
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.find31)
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.find31)
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %7 = insertvalue %String %6, i32 %"$format.size44", 1
  %8 = insertvalue %String %7, i32 %ADD42, 2
  call void @"io::out"(%String %8)
  %call.String.find45 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0))
  %call.snprintf47 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i32 %call.String.find45)
  store i32 %call.snprintf47, i32* %format.size46, align 4
  %"$format.size48" = load i32, i32* %format.size46, align 4
  %ADD49 = add i32 %"$format.size48", 1
  %AS50 = sext i32 %ADD49 to i64
  %call.calloc52 = call i8* @calloc(i64 %AS50, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc52, i8** %format.text51, align 8
  %"$format.text53" = load i8*, i8** %format.text51, align 8
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text53", i64 %AS50, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i32 %call.String.find45)
  %"$format.size55" = load i32, i32* %format.size46, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %"$format.text57" = load i8*, i8** %format.text51, align 8
  %"$format.size58" = load i32, i32* %format.size46, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text57", 0
  %10 = insertvalue %String %9, i32 %"$format.size58", 1
  %11 = insertvalue %String %10, i32 %ADD56, 2
  call void @"io::out"(%String %11)
  %call.String.find59 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.find59)
  store i32 %call.snprintf61, i32* %format.size60, align 4
  %"$format.size62" = load i32, i32* %format.size60, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %AS64 = sext i32 %ADD63 to i64
  %call.calloc66 = call i8* @calloc(i64 %AS64, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc66, i8** %format.text65, align 8
  %"$format.text67" = load i8*, i8** %format.text65, align 8
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text67", i64 %AS64, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.find59)
  %"$format.size69" = load i32, i32* %format.size60, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %"$format.text71" = load i8*, i8** %format.text65, align 8
  %"$format.size72" = load i32, i32* %format.size60, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text71", 0
  %13 = insertvalue %String %12, i32 %"$format.size72", 1
  %14 = insertvalue %String %13, i32 %ADD70, 2
  call void @"io::out"(%String %14)
  %call.String.contains = call i1 @String.contains(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0))
  store i32 0, i32* %format.size73, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if20

if20:                                             ; preds = %try19.then
  br i1 %call.String.contains, label %if20.then, label %else21

if20.then:                                        ; preds = %if20
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else21:                                           ; preds = %if20
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else21, %if20.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf74, i32* %format.size73, align 4
  %"$format.size75" = load i32, i32* %format.size73, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %AS77 = sext i32 %ADD76 to i64
  %call.calloc79 = call i8* @calloc(i64 %AS77, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc79, i8** %format.text78, align 8
  %"$format.text80" = load i8*, i8** %format.text78, align 8
  %"$bool.text81" = load i8*, i8** %bool.text, align 8
  %call.snprintf82 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text80", i64 %AS77, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text81")
  %"$format.size83" = load i32, i32* %format.size73, align 4
  %ADD84 = add i32 %"$format.size83", 1
  %"$format.text85" = load i8*, i8** %format.text78, align 8
  %"$format.size86" = load i32, i32* %format.size73, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text85", 0
  %16 = insertvalue %String %15, i32 %"$format.size86", 1
  %17 = insertvalue %String %16, i32 %ADD84, 2
  call void @"io::out"(%String %17)
  %call.String.contains87 = call i1 @String.contains(%String* %s, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0))
  store i32 0, i32* %format.size88, align 4
  store i8* null, i8** %bool.text89, align 8
  br label %if22

if22:                                             ; preds = %endif
  br i1 %call.String.contains87, label %if22.then, label %else23

if22.then:                                        ; preds = %if22
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text89, align 8
  br label %endif1

else23:                                           ; preds = %if22
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text89, align 8
  br label %endif1

endif1:                                           ; preds = %else23, %if22.then
  %"$bool.text90" = load i8*, i8** %bool.text89, align 8
  %call.snprintf91 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text90")
  store i32 %call.snprintf91, i32* %format.size88, align 4
  %"$format.size92" = load i32, i32* %format.size88, align 4
  %ADD93 = add i32 %"$format.size92", 1
  %AS94 = sext i32 %ADD93 to i64
  %call.calloc96 = call i8* @calloc(i64 %AS94, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc96, i8** %format.text95, align 8
  %"$format.text97" = load i8*, i8** %format.text95, align 8
  %"$bool.text98" = load i8*, i8** %bool.text89, align 8
  %call.snprintf99 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text97", i64 %AS94, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text98")
  %"$format.size100" = load i32, i32* %format.size88, align 4
  %ADD101 = add i32 %"$format.size100", 1
  %"$format.text102" = load i8*, i8** %format.text95, align 8
  %"$format.size103" = load i32, i32* %format.size88, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text102", 0
  %19 = insertvalue %String %18, i32 %"$format.size103", 1
  %20 = insertvalue %String %19, i32 %ADD101, 2
  call void @"io::out"(%String %20)
  %call.String.starts_with = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0))
  store i32 0, i32* %format.size104, align 4
  store i8* null, i8** %bool.text105, align 8
  br label %if24

if24:                                             ; preds = %endif1
  br i1 %call.String.starts_with, label %if24.then, label %else25

if24.then:                                        ; preds = %if24
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.20, i32 0, i32 0), i8** %bool.text105, align 8
  br label %endif2

else25:                                           ; preds = %if24
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.21, i32 0, i32 0), i8** %bool.text105, align 8
  br label %endif2

endif2:                                           ; preds = %else25, %if24.then
  %"$bool.text106" = load i8*, i8** %bool.text105, align 8
  %call.snprintf107 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text106")
  store i32 %call.snprintf107, i32* %format.size104, align 4
  %"$format.size108" = load i32, i32* %format.size104, align 4
  %ADD109 = add i32 %"$format.size108", 1
  %AS110 = sext i32 %ADD109 to i64
  %call.calloc112 = call i8* @calloc(i64 %AS110, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc112, i8** %format.text111, align 8
  %"$format.text113" = load i8*, i8** %format.text111, align 8
  %"$bool.text114" = load i8*, i8** %bool.text105, align 8
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text113", i64 %AS110, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text114")
  %"$format.size116" = load i32, i32* %format.size104, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %"$format.text118" = load i8*, i8** %format.text111, align 8
  %"$format.size119" = load i32, i32* %format.size104, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text118", 0
  %22 = insertvalue %String %21, i32 %"$format.size119", 1
  %23 = insertvalue %String %22, i32 %ADD117, 2
  call void @"io::out"(%String %23)
  %call.String.starts_with120 = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0))
  store i32 0, i32* %format.size121, align 4
  store i8* null, i8** %bool.text122, align 8
  br label %if26

if26:                                             ; preds = %endif2
  br i1 %call.String.starts_with120, label %if26.then, label %else27

if26.then:                                        ; preds = %if26
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.24, i32 0, i32 0), i8** %bool.text122, align 8
  br label %endif3

else27:                                           ; preds = %if26
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0), i8** %bool.text122, align 8
  br label %endif3

endif3:                                           ; preds = %else27, %if26.then
  %"$bool.text123" = load i8*, i8** %bool.text122, align 8
  %call.snprintf124 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text123")
  store i32 %call.snprintf124, i32* %format.size121, align 4
  %"$format.size125" = load i32, i32* %format.size121, align 4
  %ADD126 = add i32 %"$format.size125", 1
  %AS127 = sext i32 %ADD126 to i64
  %call.calloc129 = call i8* @calloc(i64 %AS127, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc129, i8** %format.text128, align 8
  %"$format.text130" = load i8*, i8** %format.text128, align 8
  %"$bool.text131" = load i8*, i8** %bool.text122, align 8
  %call.snprintf132 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text130", i64 %AS127, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text131")
  %"$format.size133" = load i32, i32* %format.size121, align 4
  %ADD134 = add i32 %"$format.size133", 1
  %"$format.text135" = load i8*, i8** %format.text128, align 8
  %"$format.size136" = load i32, i32* %format.size121, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text135", 0
  %25 = insertvalue %String %24, i32 %"$format.size136", 1
  %26 = insertvalue %String %25, i32 %ADD134, 2
  call void @"io::out"(%String %26)
  %call.String.starts_with137 = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.27, i32 0, i32 0))
  store i32 0, i32* %format.size138, align 4
  store i8* null, i8** %bool.text139, align 8
  br label %if28

if28:                                             ; preds = %endif3
  br i1 %call.String.starts_with137, label %if28.then, label %else29

if28.then:                                        ; preds = %if28
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.28, i32 0, i32 0), i8** %bool.text139, align 8
  br label %endif4

else29:                                           ; preds = %if28
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.29, i32 0, i32 0), i8** %bool.text139, align 8
  br label %endif4

endif4:                                           ; preds = %else29, %if28.then
  %"$bool.text140" = load i8*, i8** %bool.text139, align 8
  %call.snprintf141 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.30, i32 0, i32 0), i8* %"$bool.text140")
  store i32 %call.snprintf141, i32* %format.size138, align 4
  %"$format.size142" = load i32, i32* %format.size138, align 4
  %ADD143 = add i32 %"$format.size142", 1
  %AS144 = sext i32 %ADD143 to i64
  %call.calloc146 = call i8* @calloc(i64 %AS144, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc146, i8** %format.text145, align 8
  %"$format.text147" = load i8*, i8** %format.text145, align 8
  %"$bool.text148" = load i8*, i8** %bool.text139, align 8
  %call.snprintf149 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text147", i64 %AS144, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.30, i32 0, i32 0), i8* %"$bool.text148")
  %"$format.size150" = load i32, i32* %format.size138, align 4
  %ADD151 = add i32 %"$format.size150", 1
  %"$format.text152" = load i8*, i8** %format.text145, align 8
  %"$format.size153" = load i32, i32* %format.size138, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text152", 0
  %28 = insertvalue %String %27, i32 %"$format.size153", 1
  %29 = insertvalue %String %28, i32 %ADD151, 2
  call void @"io::out"(%String %29)
  %call.String.ends_with = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.31, i32 0, i32 0))
  store i32 0, i32* %format.size154, align 4
  store i8* null, i8** %bool.text155, align 8
  br label %if30

if30:                                             ; preds = %endif4
  br i1 %call.String.ends_with, label %if30.then, label %else31

if30.then:                                        ; preds = %if30
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i8** %bool.text155, align 8
  br label %endif5

else31:                                           ; preds = %if30
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.33, i32 0, i32 0), i8** %bool.text155, align 8
  br label %endif5

endif5:                                           ; preds = %else31, %if30.then
  %"$bool.text156" = load i8*, i8** %bool.text155, align 8
  %call.snprintf157 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text156")
  store i32 %call.snprintf157, i32* %format.size154, align 4
  %"$format.size158" = load i32, i32* %format.size154, align 4
  %ADD159 = add i32 %"$format.size158", 1
  %AS160 = sext i32 %ADD159 to i64
  %call.calloc162 = call i8* @calloc(i64 %AS160, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc162, i8** %format.text161, align 8
  %"$format.text163" = load i8*, i8** %format.text161, align 8
  %"$bool.text164" = load i8*, i8** %bool.text155, align 8
  %call.snprintf165 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text163", i64 %AS160, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text164")
  %"$format.size166" = load i32, i32* %format.size154, align 4
  %ADD167 = add i32 %"$format.size166", 1
  %"$format.text168" = load i8*, i8** %format.text161, align 8
  %"$format.size169" = load i32, i32* %format.size154, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text168", 0
  %31 = insertvalue %String %30, i32 %"$format.size169", 1
  %32 = insertvalue %String %31, i32 %ADD167, 2
  call void @"io::out"(%String %32)
  %call.String.ends_with170 = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.35, i32 0, i32 0))
  store i32 0, i32* %format.size171, align 4
  store i8* null, i8** %bool.text172, align 8
  br label %if32

if32:                                             ; preds = %endif5
  br i1 %call.String.ends_with170, label %if32.then, label %else33

if32.then:                                        ; preds = %if32
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.36, i32 0, i32 0), i8** %bool.text172, align 8
  br label %endif6

else33:                                           ; preds = %if32
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.37, i32 0, i32 0), i8** %bool.text172, align 8
  br label %endif6

endif6:                                           ; preds = %else33, %if32.then
  %"$bool.text173" = load i8*, i8** %bool.text172, align 8
  %call.snprintf174 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i8* %"$bool.text173")
  store i32 %call.snprintf174, i32* %format.size171, align 4
  %"$format.size175" = load i32, i32* %format.size171, align 4
  %ADD176 = add i32 %"$format.size175", 1
  %AS177 = sext i32 %ADD176 to i64
  %call.calloc179 = call i8* @calloc(i64 %AS177, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc179, i8** %format.text178, align 8
  %"$format.text180" = load i8*, i8** %format.text178, align 8
  %"$bool.text181" = load i8*, i8** %bool.text172, align 8
  %call.snprintf182 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text180", i64 %AS177, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i8* %"$bool.text181")
  %"$format.size183" = load i32, i32* %format.size171, align 4
  %ADD184 = add i32 %"$format.size183", 1
  %"$format.text185" = load i8*, i8** %format.text178, align 8
  %"$format.size186" = load i32, i32* %format.size171, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text185", 0
  %34 = insertvalue %String %33, i32 %"$format.size186", 1
  %35 = insertvalue %String %34, i32 %ADD184, 2
  call void @"io::out"(%String %35)
  %call.String.ends_with187 = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.39, i32 0, i32 0))
  store i32 0, i32* %format.size188, align 4
  store i8* null, i8** %bool.text189, align 8
  br label %if34

if34:                                             ; preds = %endif6
  br i1 %call.String.ends_with187, label %if34.then, label %else35

if34.then:                                        ; preds = %if34
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.40, i32 0, i32 0), i8** %bool.text189, align 8
  br label %endif7

else35:                                           ; preds = %if34
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.41, i32 0, i32 0), i8** %bool.text189, align 8
  br label %endif7

endif7:                                           ; preds = %else35, %if34.then
  %"$bool.text190" = load i8*, i8** %bool.text189, align 8
  %call.snprintf191 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i8* %"$bool.text190")
  store i32 %call.snprintf191, i32* %format.size188, align 4
  %"$format.size192" = load i32, i32* %format.size188, align 4
  %ADD193 = add i32 %"$format.size192", 1
  %AS194 = sext i32 %ADD193 to i64
  %call.calloc196 = call i8* @calloc(i64 %AS194, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc196, i8** %format.text195, align 8
  %"$format.text197" = load i8*, i8** %format.text195, align 8
  %"$bool.text198" = load i8*, i8** %bool.text189, align 8
  %call.snprintf199 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text197", i64 %AS194, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i8* %"$bool.text198")
  %"$format.size200" = load i32, i32* %format.size188, align 4
  %ADD201 = add i32 %"$format.size200", 1
  %"$format.text202" = load i8*, i8** %format.text195, align 8
  %"$format.size203" = load i32, i32* %format.size188, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text202", 0
  %37 = insertvalue %String %36, i32 %"$format.size203", 1
  %38 = insertvalue %String %37, i32 %ADD201, 2
  call void @"io::out"(%String %38)
  %call.String.create204 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.43, i32 0, i32 0))
  store %String %call.String.create204, %String* %empty, align 8
  %call.String.find205 = call i32 @String.find(%String* %empty, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.44, i32 0, i32 0))
  %call.snprintf207 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i32 %call.String.find205)
  store i32 %call.snprintf207, i32* %format.size206, align 4
  %"$format.size208" = load i32, i32* %format.size206, align 4
  %ADD209 = add i32 %"$format.size208", 1
  %AS210 = sext i32 %ADD209 to i64
  %call.calloc212 = call i8* @calloc(i64 %AS210, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc212, i8** %format.text211, align 8
  %"$format.text213" = load i8*, i8** %format.text211, align 8
  %call.snprintf214 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text213", i64 %AS210, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i32 %call.String.find205)
  %"$format.size215" = load i32, i32* %format.size206, align 4
  %ADD216 = add i32 %"$format.size215", 1
  %"$format.text217" = load i8*, i8** %format.text211, align 8
  %"$format.size218" = load i32, i32* %format.size206, align 4
  %39 = insertvalue %String zeroinitializer, i8* %"$format.text217", 0
  %40 = insertvalue %String %39, i32 %"$format.size218", 1
  %41 = insertvalue %String %40, i32 %ADD216, 2
  call void @"io::out"(%String %41)
  %call.String.contains219 = call i1 @String.contains(%String* %empty, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  store i32 0, i32* %format.size220, align 4
  store i8* null, i8** %bool.text221, align 8
  br label %if36

if36:                                             ; preds = %endif7
  br i1 %call.String.contains219, label %if36.then, label %else37

if36.then:                                        ; preds = %if36
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.47, i32 0, i32 0), i8** %bool.text221, align 8
  br label %endif8

else37:                                           ; preds = %if36
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.48, i32 0, i32 0), i8** %bool.text221, align 8
  br label %endif8

endif8:                                           ; preds = %else37, %if36.then
  %"$bool.text222" = load i8*, i8** %bool.text221, align 8
  %call.snprintf223 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text222")
  store i32 %call.snprintf223, i32* %format.size220, align 4
  %"$format.size224" = load i32, i32* %format.size220, align 4
  %ADD225 = add i32 %"$format.size224", 1
  %AS226 = sext i32 %ADD225 to i64
  %call.calloc228 = call i8* @calloc(i64 %AS226, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc228, i8** %format.text227, align 8
  %"$format.text229" = load i8*, i8** %format.text227, align 8
  %"$bool.text230" = load i8*, i8** %bool.text221, align 8
  %call.snprintf231 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text229", i64 %AS226, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text230")
  %"$format.size232" = load i32, i32* %format.size220, align 4
  %ADD233 = add i32 %"$format.size232", 1
  %"$format.text234" = load i8*, i8** %format.text227, align 8
  %"$format.size235" = load i32, i32* %format.size220, align 4
  %42 = insertvalue %String zeroinitializer, i8* %"$format.text234", 0
  %43 = insertvalue %String %42, i32 %"$format.size235", 1
  %44 = insertvalue %String %43, i32 %ADD233, 2
  call void @"io::out"(%String %44)
  %call.String.starts_with236 = call i1 @String.starts_with(%String* %empty, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.50, i32 0, i32 0))
  store i32 0, i32* %format.size237, align 4
  store i8* null, i8** %bool.text238, align 8
  br label %if38

if38:                                             ; preds = %endif8
  br i1 %call.String.starts_with236, label %if38.then, label %else39

if38.then:                                        ; preds = %if38
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.51, i32 0, i32 0), i8** %bool.text238, align 8
  br label %endif9

else39:                                           ; preds = %if38
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.52, i32 0, i32 0), i8** %bool.text238, align 8
  br label %endif9

endif9:                                           ; preds = %else39, %if38.then
  %"$bool.text239" = load i8*, i8** %bool.text238, align 8
  %call.snprintf240 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text239")
  store i32 %call.snprintf240, i32* %format.size237, align 4
  %"$format.size241" = load i32, i32* %format.size237, align 4
  %ADD242 = add i32 %"$format.size241", 1
  %AS243 = sext i32 %ADD242 to i64
  %call.calloc245 = call i8* @calloc(i64 %AS243, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc245, i8** %format.text244, align 8
  %"$format.text246" = load i8*, i8** %format.text244, align 8
  %"$bool.text247" = load i8*, i8** %bool.text238, align 8
  %call.snprintf248 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text246", i64 %AS243, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text247")
  %"$format.size249" = load i32, i32* %format.size237, align 4
  %ADD250 = add i32 %"$format.size249", 1
  %"$format.text251" = load i8*, i8** %format.text244, align 8
  %"$format.size252" = load i32, i32* %format.size237, align 4
  %45 = insertvalue %String zeroinitializer, i8* %"$format.text251", 0
  %46 = insertvalue %String %45, i32 %"$format.size252", 1
  %47 = insertvalue %String %46, i32 %ADD250, 2
  call void @"io::out"(%String %47)
  %call.String.ends_with253 = call i1 @String.ends_with(%String* %empty, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.54, i32 0, i32 0))
  store i32 0, i32* %format.size254, align 4
  store i8* null, i8** %bool.text255, align 8
  br label %if40

if40:                                             ; preds = %endif9
  br i1 %call.String.ends_with253, label %if40.then, label %else41

if40.then:                                        ; preds = %if40
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.55, i32 0, i32 0), i8** %bool.text255, align 8
  br label %endif10

else41:                                           ; preds = %if40
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.56, i32 0, i32 0), i8** %bool.text255, align 8
  br label %endif10

endif10:                                          ; preds = %else41, %if40.then
  %"$bool.text256" = load i8*, i8** %bool.text255, align 8
  %call.snprintf257 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.57, i32 0, i32 0), i8* %"$bool.text256")
  store i32 %call.snprintf257, i32* %format.size254, align 4
  %"$format.size258" = load i32, i32* %format.size254, align 4
  %ADD259 = add i32 %"$format.size258", 1
  %AS260 = sext i32 %ADD259 to i64
  %call.calloc262 = call i8* @calloc(i64 %AS260, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc262, i8** %format.text261, align 8
  %"$format.text263" = load i8*, i8** %format.text261, align 8
  %"$bool.text264" = load i8*, i8** %bool.text255, align 8
  %call.snprintf265 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text263", i64 %AS260, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.57, i32 0, i32 0), i8* %"$bool.text264")
  %"$format.size266" = load i32, i32* %format.size254, align 4
  %ADD267 = add i32 %"$format.size266", 1
  %"$format.text268" = load i8*, i8** %format.text261, align 8
  %"$format.size269" = load i32, i32* %format.size254, align 4
  %48 = insertvalue %String zeroinitializer, i8* %"$format.text268", 0
  %49 = insertvalue %String %48, i32 %"$format.size269", 1
  %50 = insertvalue %String %49, i32 %ADD267, 2
  call void @"io::out"(%String %50)
  %51 = bitcast %String* %empty to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %51)
  %52 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %52)
  ret i32 0

endtry:                                           ; preds = %try19
  br label %catch42

catch42:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch42.then, label %endcatch

catch42.then:                                     ; preds = %catch42
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch42
  br label %if43

if43:                                             ; preds = %endcatch
  %"$ucg.thrown270" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown270", label %if43.then, label %endif11

if43.then:                                        ; preds = %if43
  ret i32 1

endif11:                                          ; preds = %if43
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse

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
@str = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [12 x i8] c"   hello   \00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.13 = private unnamed_addr constant [16 x i8] c"foo bar foo baz\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.19 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.20 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.21 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.25 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.26 = private unnamed_addr constant [6 x i8] c"abcde\00", align 1
@str.27 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.29 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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
  br label %while13

while13:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  br label %if14

if14:                                             ; preds = %while13.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  ret i1 false

endif:                                            ; preds = %if14
  br label %if15

if15:                                             ; preds = %endif
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
  br i1 %NQ11, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret i1 false

endif1:                                           ; preds = %if15
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while13

endloop:                                          ; preds = %while13
  ret i1 true
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
  br label %if16

if16:                                             ; preds = %entry
  %"$start" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$start", 0
  br i1 %LT, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"

endif:                                            ; preds = %if16
  %"$start2" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start2", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if17

if17:                                             ; preds = %endif
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %stop, align 4
  br label %endif1

endif1:                                           ; preds = %if17.then, %if17
  %"$start6" = load i32, i32* %start, align 4
  store i32 %"$start6", i32* %i, align 4
  br label %while18

while18:                                          ; preds = %while18.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %"$stop7" = load i32, i32* %stop, align 4
  %LT8 = icmp slt i32 %"$i", %"$stop7"
  br i1 %LT8, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
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
  br label %while18

endloop:                                          ; preds = %while18
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
  br label %while19

while19:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if20

if20:                                             ; preds = %while19.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while19

endloop:                                          ; preds = %while19
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
  br label %while21

while21:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if22

if22:                                             ; preds = %while21.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 65
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 90
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %ADD = add i32 %AS, 32
  %AS5 = trunc i32 %ADD to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
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
  br label %while23

while23:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$start" = load i32, i32* %start, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$start", %"$*self.count"
  br i1 %LT, label %while23.then, label %endloop

while23.then:                                     ; preds = %while23
  br label %if24

if24:                                             ; preds = %while23.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$start5" = load i32, i32* %start, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$start5"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %call.String.is_space = call i1 @String.is_space(%String* %"*self3", i8 %"$*self.data[]")
  %NOT = xor i1 %call.String.is_space, true
  br i1 %NOT, label %if24.then, label %endif

if24.then:                                        ; preds = %if24
  br label %endloop

endif:                                            ; preds = %if24
  %"$start6" = load i32, i32* %start, align 4
  %ADD = add i32 %"$start6", 1
  store i32 %ADD, i32* %start, align 4
  br label %while23

endloop:                                          ; preds = %if24.then, %while23
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  store i32 %"$*self.count9", i32* %stop, align 4
  br label %while25

while25:                                          ; preds = %endif1, %endloop
  %"$stop" = load i32, i32* %stop, align 4
  %"$start10" = load i32, i32* %start, align 4
  %GT = icmp sgt i32 %"$stop", %"$start10"
  br i1 %GT, label %while25.then, label %endloop2

while25.then:                                     ; preds = %while25
  br label %if26

if26:                                             ; preds = %while25.then
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
  br i1 %NOT19, label %if26.then, label %endif1

if26.then:                                        ; preds = %if26
  br label %endloop2

endif1:                                           ; preds = %if26
  %"$stop20" = load i32, i32* %stop, align 4
  %SUB21 = sub i32 %"$stop20", 1
  store i32 %SUB21, i32* %stop, align 4
  br label %while25

endloop2:                                         ; preds = %if26.then, %while25
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
  br label %while27

while27:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while27.then, label %endloop

while27.then:                                     ; preds = %while27
  br label %if28

if28:                                             ; preds = %while27.then
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$old3" = load i8*, i8** %old, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$old3")
  %AND = and i1 %GT, %call.String.matches_at
  br i1 %AND, label %if28.then, label %else29

if28.then:                                        ; preds = %if28
  %"$fresh" = load i8*, i8** %fresh, align 8
  call void @String.join(%String* %res, i8* %"$fresh")
  %"$n4" = load i32, i32* %n, align 4
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", %"$n4"
  store i32 %ADD, i32* %i, align 4
  br label %endif

else29:                                           ; preds = %if28
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

endif:                                            ; preds = %else29, %if28.then
  br label %while27

endloop:                                          ; preds = %while27
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
  br label %while30

while30:                                          ; preds = %endloop, %entry
  %"$k" = load i32, i32* %k, align 4
  %"$times" = load i32, i32* %times, align 4
  %LT = icmp slt i32 %"$k", %"$times"
  br i1 %LT, label %while30.then, label %endloop1

while30.then:                                     ; preds = %while30
  store i32 0, i32* %i, align 4
  br label %while31

while31:                                          ; preds = %while31.then, %while30.then
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT2 = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT2, label %while31.then, label %endloop

while31.then:                                     ; preds = %while31
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
  br label %while31

endloop:                                          ; preds = %while31
  %"$k6" = load i32, i32* %k, align 4
  %ADD7 = add i32 %"$k6", 1
  store i32 %ADD7, i32* %k, align 4
  br label %while30

endloop1:                                         ; preds = %while30
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
  br label %while32

while32:                                          ; preds = %while32.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while32.then, label %endloop

while32.then:                                     ; preds = %while32
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
  br label %while32

endloop:                                          ; preds = %while32
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
  %u = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %l = alloca %String, align 8
  %format.size7 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  %sub = alloca %String, align 8
  %format.size21 = alloca i32, align 4
  %format.text26 = alloca i8*, align 8
  %sub2 = alloca %String, align 8
  %format.size36 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %sub3 = alloca %String, align 8
  %format.size51 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  %sub4 = alloca %String, align 8
  %format.size65 = alloca i32, align 4
  %format.text70 = alloca i8*, align 8
  %t1 = alloca %String, align 8
  %t2 = alloca %String, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %t3 = alloca %String, align 8
  %t4 = alloca %String, align 8
  %format.size96 = alloca i32, align 4
  %format.text101 = alloca i8*, align 8
  %t5 = alloca %String, align 8
  %t6 = alloca %String, align 8
  %format.size112 = alloca i32, align 4
  %format.text117 = alloca i8*, align 8
  %r1 = alloca %String, align 8
  %r2 = alloca %String, align 8
  %format.size127 = alloca i32, align 4
  %format.text132 = alloca i8*, align 8
  %r3 = alloca %String, align 8
  %format.size142 = alloca i32, align 4
  %format.text147 = alloca i8*, align 8
  %r4 = alloca %String, align 8
  %format.size157 = alloca i32, align 4
  %format.text162 = alloca i8*, align 8
  %rep = alloca %String, align 8
  %rep2 = alloca %String, align 8
  %format.size172 = alloca i32, align 4
  %format.text177 = alloca i8*, align 8
  %rep3 = alloca %String, align 8
  %format.size187 = alloca i32, align 4
  %format.text192 = alloca i8*, align 8
  %rev = alloca %String, align 8
  %rev2 = alloca %String, align 8
  %format.size202 = alloca i32, align 4
  %format.text207 = alloca i8*, align 8
  %rev3 = alloca %String, align 8
  %rev4 = alloca %String, align 8
  %format.size218 = alloca i32, align 4
  %format.text223 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try33

try33:                                            ; preds = %entry
  br i1 true, label %try33.then, label %endtry

try33.then:                                       ; preds = %try33
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %u, align 8
  %call.String.chars = call i8* @String.chars(%String* %u)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.String.lower = call %String @String.lower(%String* %s)
  store %String %call.String.lower, %String* %l, align 8
  %call.String.chars6 = call i8* @String.chars(%String* %l)
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars6)
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars6)
  %"$format.size16" = load i32, i32* %format.size7, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %4 = insertvalue %String %3, i32 %"$format.size19", 1
  %5 = insertvalue %String %4, i32 %ADD17, 2
  call void @"io::out"(%String %5)
  %call.String.substr = call %String @String.substr(%String* %s, i32 6, i32 5)
  store %String %call.String.substr, %String* %sub, align 8
  %call.String.chars20 = call i8* @String.chars(%String* %sub)
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars20)
  store i32 %call.snprintf22, i32* %format.size21, align 4
  %"$format.size23" = load i32, i32* %format.size21, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %AS25 = sext i32 %ADD24 to i64
  %call.calloc27 = call i8* @calloc(i64 %AS25, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc27, i8** %format.text26, align 8
  %"$format.text28" = load i8*, i8** %format.text26, align 8
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text28", i64 %AS25, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars20)
  %"$format.size30" = load i32, i32* %format.size21, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %"$format.text32" = load i8*, i8** %format.text26, align 8
  %"$format.size33" = load i32, i32* %format.size21, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text32", 0
  %7 = insertvalue %String %6, i32 %"$format.size33", 1
  %8 = insertvalue %String %7, i32 %ADD31, 2
  call void @"io::out"(%String %8)
  %call.String.substr34 = call %String @String.substr(%String* %s, i32 0, i32 5)
  store %String %call.String.substr34, %String* %sub2, align 8
  %call.String.chars35 = call i8* @String.chars(%String* %sub2)
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars35)
  store i32 %call.snprintf37, i32* %format.size36, align 4
  %"$format.size38" = load i32, i32* %format.size36, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars35)
  %"$format.size45" = load i32, i32* %format.size36, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size36, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %10 = insertvalue %String %9, i32 %"$format.size48", 1
  %11 = insertvalue %String %10, i32 %ADD46, 2
  call void @"io::out"(%String %11)
  %call.String.substr49 = call %String @String.substr(%String* %s, i32 0, i32 100)
  store %String %call.String.substr49, %String* %sub3, align 8
  %call.String.chars50 = call i8* @String.chars(%String* %sub3)
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars50)
  store i32 %call.snprintf52, i32* %format.size51, align 4
  %"$format.size53" = load i32, i32* %format.size51, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars50)
  %"$format.size60" = load i32, i32* %format.size51, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size51, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %13 = insertvalue %String %12, i32 %"$format.size63", 1
  %14 = insertvalue %String %13, i32 %ADD61, 2
  call void @"io::out"(%String %14)
  %call.String.substr64 = call %String @String.substr(%String* %s, i32 -1, i32 5)
  store %String %call.String.substr64, %String* %sub4, align 8
  %call.String.size = call i32 @String.size(%String* %sub4)
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.size)
  store i32 %call.snprintf66, i32* %format.size65, align 4
  %"$format.size67" = load i32, i32* %format.size65, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %call.snprintf73 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.size)
  %"$format.size74" = load i32, i32* %format.size65, align 4
  %ADD75 = add i32 %"$format.size74", 1
  %"$format.text76" = load i8*, i8** %format.text70, align 8
  %"$format.size77" = load i32, i32* %format.size65, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text76", 0
  %16 = insertvalue %String %15, i32 %"$format.size77", 1
  %17 = insertvalue %String %16, i32 %ADD75, 2
  call void @"io::out"(%String %17)
  %call.String.create78 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create78, %String* %t1, align 8
  %call.String.trim = call %String @String.trim(%String* %t1)
  store %String %call.String.trim, %String* %t2, align 8
  %call.String.chars79 = call i8* @String.chars(%String* %t2)
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars79)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars79)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.String.create93 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0))
  store %String %call.String.create93, %String* %t3, align 8
  %call.String.trim94 = call %String @String.trim(%String* %t3)
  store %String %call.String.trim94, %String* %t4, align 8
  %call.String.chars95 = call i8* @String.chars(%String* %t4)
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars95)
  store i32 %call.snprintf97, i32* %format.size96, align 4
  %"$format.size98" = load i32, i32* %format.size96, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %AS100 = sext i32 %ADD99 to i64
  %call.calloc102 = call i8* @calloc(i64 %AS100, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc102, i8** %format.text101, align 8
  %"$format.text103" = load i8*, i8** %format.text101, align 8
  %call.snprintf104 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text103", i64 %AS100, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars95)
  %"$format.size105" = load i32, i32* %format.size96, align 4
  %ADD106 = add i32 %"$format.size105", 1
  %"$format.text107" = load i8*, i8** %format.text101, align 8
  %"$format.size108" = load i32, i32* %format.size96, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text107", 0
  %22 = insertvalue %String %21, i32 %"$format.size108", 1
  %23 = insertvalue %String %22, i32 %ADD106, 2
  call void @"io::out"(%String %23)
  %call.String.create109 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create109, %String* %t5, align 8
  %call.String.trim110 = call %String @String.trim(%String* %t5)
  store %String %call.String.trim110, %String* %t6, align 8
  %call.String.size111 = call i32 @String.size(%String* %t6)
  %call.snprintf113 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size111)
  store i32 %call.snprintf113, i32* %format.size112, align 4
  %"$format.size114" = load i32, i32* %format.size112, align 4
  %ADD115 = add i32 %"$format.size114", 1
  %AS116 = sext i32 %ADD115 to i64
  %call.calloc118 = call i8* @calloc(i64 %AS116, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc118, i8** %format.text117, align 8
  %"$format.text119" = load i8*, i8** %format.text117, align 8
  %call.snprintf120 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text119", i64 %AS116, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size111)
  %"$format.size121" = load i32, i32* %format.size112, align 4
  %ADD122 = add i32 %"$format.size121", 1
  %"$format.text123" = load i8*, i8** %format.text117, align 8
  %"$format.size124" = load i32, i32* %format.size112, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text123", 0
  %25 = insertvalue %String %24, i32 %"$format.size124", 1
  %26 = insertvalue %String %25, i32 %ADD122, 2
  call void @"io::out"(%String %26)
  %call.String.create125 = call %String @String.create(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create125, %String* %r1, align 8
  %call.String.replace = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0))
  store %String %call.String.replace, %String* %r2, align 8
  %call.String.chars126 = call i8* @String.chars(%String* %r2)
  %call.snprintf128 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars126)
  store i32 %call.snprintf128, i32* %format.size127, align 4
  %"$format.size129" = load i32, i32* %format.size127, align 4
  %ADD130 = add i32 %"$format.size129", 1
  %AS131 = sext i32 %ADD130 to i64
  %call.calloc133 = call i8* @calloc(i64 %AS131, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc133, i8** %format.text132, align 8
  %"$format.text134" = load i8*, i8** %format.text132, align 8
  %call.snprintf135 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text134", i64 %AS131, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars126)
  %"$format.size136" = load i32, i32* %format.size127, align 4
  %ADD137 = add i32 %"$format.size136", 1
  %"$format.text138" = load i8*, i8** %format.text132, align 8
  %"$format.size139" = load i32, i32* %format.size127, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text138", 0
  %28 = insertvalue %String %27, i32 %"$format.size139", 1
  %29 = insertvalue %String %28, i32 %ADD137, 2
  call void @"io::out"(%String %29)
  %call.String.replace140 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace140, %String* %r3, align 8
  %call.String.chars141 = call i8* @String.chars(%String* %r3)
  %call.snprintf143 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars141)
  store i32 %call.snprintf143, i32* %format.size142, align 4
  %"$format.size144" = load i32, i32* %format.size142, align 4
  %ADD145 = add i32 %"$format.size144", 1
  %AS146 = sext i32 %ADD145 to i64
  %call.calloc148 = call i8* @calloc(i64 %AS146, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc148, i8** %format.text147, align 8
  %"$format.text149" = load i8*, i8** %format.text147, align 8
  %call.snprintf150 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text149", i64 %AS146, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars141)
  %"$format.size151" = load i32, i32* %format.size142, align 4
  %ADD152 = add i32 %"$format.size151", 1
  %"$format.text153" = load i8*, i8** %format.text147, align 8
  %"$format.size154" = load i32, i32* %format.size142, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text153", 0
  %31 = insertvalue %String %30, i32 %"$format.size154", 1
  %32 = insertvalue %String %31, i32 %ADD152, 2
  call void @"io::out"(%String %32)
  %call.String.replace155 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.20, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  store %String %call.String.replace155, %String* %r4, align 8
  %call.String.chars156 = call i8* @String.chars(%String* %r4)
  %call.snprintf158 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %call.String.chars156)
  store i32 %call.snprintf158, i32* %format.size157, align 4
  %"$format.size159" = load i32, i32* %format.size157, align 4
  %ADD160 = add i32 %"$format.size159", 1
  %AS161 = sext i32 %ADD160 to i64
  %call.calloc163 = call i8* @calloc(i64 %AS161, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc163, i8** %format.text162, align 8
  %"$format.text164" = load i8*, i8** %format.text162, align 8
  %call.snprintf165 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text164", i64 %AS161, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %call.String.chars156)
  %"$format.size166" = load i32, i32* %format.size157, align 4
  %ADD167 = add i32 %"$format.size166", 1
  %"$format.text168" = load i8*, i8** %format.text162, align 8
  %"$format.size169" = load i32, i32* %format.size157, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text168", 0
  %34 = insertvalue %String %33, i32 %"$format.size169", 1
  %35 = insertvalue %String %34, i32 %ADD167, 2
  call void @"io::out"(%String %35)
  %call.String.create170 = call %String @String.create(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.23, i32 0, i32 0))
  store %String %call.String.create170, %String* %rep, align 8
  %call.String.repeat = call %String @String.repeat(%String* %rep, i32 3)
  store %String %call.String.repeat, %String* %rep2, align 8
  %call.String.chars171 = call i8* @String.chars(%String* %rep2)
  %call.snprintf173 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i8* %call.String.chars171)
  store i32 %call.snprintf173, i32* %format.size172, align 4
  %"$format.size174" = load i32, i32* %format.size172, align 4
  %ADD175 = add i32 %"$format.size174", 1
  %AS176 = sext i32 %ADD175 to i64
  %call.calloc178 = call i8* @calloc(i64 %AS176, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc178, i8** %format.text177, align 8
  %"$format.text179" = load i8*, i8** %format.text177, align 8
  %call.snprintf180 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text179", i64 %AS176, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i8* %call.String.chars171)
  %"$format.size181" = load i32, i32* %format.size172, align 4
  %ADD182 = add i32 %"$format.size181", 1
  %"$format.text183" = load i8*, i8** %format.text177, align 8
  %"$format.size184" = load i32, i32* %format.size172, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text183", 0
  %37 = insertvalue %String %36, i32 %"$format.size184", 1
  %38 = insertvalue %String %37, i32 %ADD182, 2
  call void @"io::out"(%String %38)
  %call.String.repeat185 = call %String @String.repeat(%String* %rep, i32 0)
  store %String %call.String.repeat185, %String* %rep3, align 8
  %call.String.size186 = call i32 @String.size(%String* %rep3)
  %call.snprintf188 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i32 %call.String.size186)
  store i32 %call.snprintf188, i32* %format.size187, align 4
  %"$format.size189" = load i32, i32* %format.size187, align 4
  %ADD190 = add i32 %"$format.size189", 1
  %AS191 = sext i32 %ADD190 to i64
  %call.calloc193 = call i8* @calloc(i64 %AS191, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc193, i8** %format.text192, align 8
  %"$format.text194" = load i8*, i8** %format.text192, align 8
  %call.snprintf195 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text194", i64 %AS191, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i32 %call.String.size186)
  %"$format.size196" = load i32, i32* %format.size187, align 4
  %ADD197 = add i32 %"$format.size196", 1
  %"$format.text198" = load i8*, i8** %format.text192, align 8
  %"$format.size199" = load i32, i32* %format.size187, align 4
  %39 = insertvalue %String zeroinitializer, i8* %"$format.text198", 0
  %40 = insertvalue %String %39, i32 %"$format.size199", 1
  %41 = insertvalue %String %40, i32 %ADD197, 2
  call void @"io::out"(%String %41)
  %call.String.create200 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0))
  store %String %call.String.create200, %String* %rev, align 8
  %call.String.reverse = call %String @String.reverse(%String* %rev)
  store %String %call.String.reverse, %String* %rev2, align 8
  %call.String.chars201 = call i8* @String.chars(%String* %rev2)
  %call.snprintf203 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i8* %call.String.chars201)
  store i32 %call.snprintf203, i32* %format.size202, align 4
  %"$format.size204" = load i32, i32* %format.size202, align 4
  %ADD205 = add i32 %"$format.size204", 1
  %AS206 = sext i32 %ADD205 to i64
  %call.calloc208 = call i8* @calloc(i64 %AS206, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc208, i8** %format.text207, align 8
  %"$format.text209" = load i8*, i8** %format.text207, align 8
  %call.snprintf210 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text209", i64 %AS206, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i8* %call.String.chars201)
  %"$format.size211" = load i32, i32* %format.size202, align 4
  %ADD212 = add i32 %"$format.size211", 1
  %"$format.text213" = load i8*, i8** %format.text207, align 8
  %"$format.size214" = load i32, i32* %format.size202, align 4
  %42 = insertvalue %String zeroinitializer, i8* %"$format.text213", 0
  %43 = insertvalue %String %42, i32 %"$format.size214", 1
  %44 = insertvalue %String %43, i32 %ADD212, 2
  call void @"io::out"(%String %44)
  %call.String.create215 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  store %String %call.String.create215, %String* %rev3, align 8
  %call.String.reverse216 = call %String @String.reverse(%String* %rev3)
  store %String %call.String.reverse216, %String* %rev4, align 8
  %call.String.chars217 = call i8* @String.chars(%String* %rev4)
  %call.snprintf219 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %call.String.chars217)
  store i32 %call.snprintf219, i32* %format.size218, align 4
  %"$format.size220" = load i32, i32* %format.size218, align 4
  %ADD221 = add i32 %"$format.size220", 1
  %AS222 = sext i32 %ADD221 to i64
  %call.calloc224 = call i8* @calloc(i64 %AS222, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc224, i8** %format.text223, align 8
  %"$format.text225" = load i8*, i8** %format.text223, align 8
  %call.snprintf226 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text225", i64 %AS222, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %call.String.chars217)
  %"$format.size227" = load i32, i32* %format.size218, align 4
  %ADD228 = add i32 %"$format.size227", 1
  %"$format.text229" = load i8*, i8** %format.text223, align 8
  %"$format.size230" = load i32, i32* %format.size218, align 4
  %45 = insertvalue %String zeroinitializer, i8* %"$format.text229", 0
  %46 = insertvalue %String %45, i32 %"$format.size230", 1
  %47 = insertvalue %String %46, i32 %ADD228, 2
  call void @"io::out"(%String %47)
  %48 = bitcast %String* %rev4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %48)
  %49 = bitcast %String* %rev3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %49)
  %50 = bitcast %String* %rev2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %50)
  %51 = bitcast %String* %rev to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %51)
  %52 = bitcast %String* %rep3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %52)
  %53 = bitcast %String* %rep2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %53)
  %54 = bitcast %String* %rep to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %54)
  %55 = bitcast %String* %r4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %55)
  %56 = bitcast %String* %r3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %56)
  %57 = bitcast %String* %r2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %57)
  %58 = bitcast %String* %r1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %58)
  %59 = bitcast %String* %t6 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %59)
  %60 = bitcast %String* %t5 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %60)
  %61 = bitcast %String* %t4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %61)
  %62 = bitcast %String* %t3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %62)
  %63 = bitcast %String* %t2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %63)
  %64 = bitcast %String* %t1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %64)
  %65 = bitcast %String* %sub4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %65)
  %66 = bitcast %String* %sub3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %66)
  %67 = bitcast %String* %sub2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %67)
  %68 = bitcast %String* %sub to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %68)
  %69 = bitcast %String* %l to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %69)
  %70 = bitcast %String* %u to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %70)
  %71 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %71)
  ret i32 0

endtry:                                           ; preds = %try33
  br label %catch34

catch34:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch34.then, label %endcatch

catch34.then:                                     ; preds = %catch34
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch34
  br label %if35

if35:                                             ; preds = %endcatch
  %"$ucg.thrown231" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown231", label %if35.then, label %endif

if35.then:                                        ; preds = %if35
  ret i32 1

endif:                                            ; preds = %if35
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 020 — string: operators: + += == != < > against String and against a literal

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
@str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c" there\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"ing\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.18 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.19 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.24 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.25 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.29 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.32 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.33 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.34 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.35 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.36 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.37 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.38 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.39 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.40 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.41 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.42 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.43 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.44 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.45 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.46 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.47 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.48 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.49 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.50 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.51 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.52 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.53 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.54 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.55 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.56 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.57 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.58 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

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

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %endif, %entry
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
  br i1 %AND, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  br label %if13

if13:                                             ; preds = %while12.then
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
  br i1 %NQ, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
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

endif:                                            ; preds = %if13
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
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

define i1 @"String.>"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %GT = icmp sgt i32 %call.String.compare, 0
  ret i1 %GT
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
  br label %while14

while14:                                          ; preds = %while14.then, %entry
  %"*right" = load %String*, %String** %right, align 8
  %"*right.count" = getelementptr inbounds %String, %String* %"*right", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count"
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
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
  br label %while14

endloop:                                          ; preds = %while14
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
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %d = alloca %String, align 8
  %format.size22 = alloca i32, align 4
  %format.text27 = alloca i8*, align 8
  %format.size36 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %e = alloca %String, align 8
  %format.size51 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  %x = alloca %String, align 8
  %y = alloca %String, align 8
  %z = alloca %String, align 8
  %format.size67 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text72 = alloca i8*, align 8
  %format.size82 = alloca i32, align 4
  %bool.text83 = alloca i8*, align 8
  %format.text89 = alloca i8*, align 8
  %format.size98 = alloca i32, align 4
  %bool.text99 = alloca i8*, align 8
  %format.text105 = alloca i8*, align 8
  %format.size115 = alloca i32, align 4
  %bool.text116 = alloca i8*, align 8
  %format.text122 = alloca i8*, align 8
  %format.size131 = alloca i32, align 4
  %bool.text132 = alloca i8*, align 8
  %format.text138 = alloca i8*, align 8
  %format.size148 = alloca i32, align 4
  %bool.text149 = alloca i8*, align 8
  %format.text155 = alloca i8*, align 8
  %format.size164 = alloca i32, align 4
  %bool.text165 = alloca i8*, align 8
  %format.text171 = alloca i8*, align 8
  %format.size181 = alloca i32, align 4
  %bool.text182 = alloca i8*, align 8
  %format.text188 = alloca i8*, align 8
  %tmp = alloca %String, align 8
  %format.size198 = alloca i32, align 4
  %bool.text199 = alloca i8*, align 8
  %format.text205 = alloca i8*, align 8
  %tmp214 = alloca %String, align 8
  %format.size216 = alloca i32, align 4
  %bool.text217 = alloca i8*, align 8
  %format.text223 = alloca i8*, align 8
  %tmp232 = alloca %String, align 8
  %format.size234 = alloca i32, align 4
  %bool.text235 = alloca i8*, align 8
  %format.text241 = alloca i8*, align 8
  %tmp250 = alloca %String, align 8
  %format.size252 = alloca i32, align 4
  %bool.text253 = alloca i8*, align 8
  %format.text259 = alloca i8*, align 8
  %f = alloca %String, align 8
  %g = alloca %String, align 8
  %copy = alloca %String, align 8
  %format.size272 = alloca i32, align 4
  %bool.text273 = alloca i8*, align 8
  %format.text279 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try15

try15:                                            ; preds = %entry
  br i1 true, label %try15.then, label %endtry

try15.then:                                       ; preds = %try15
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  %call.String.create14 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create14, %String* %b, align 8
  %"call.String.+" = call %String @"String.+.2"(%String* %a, %String* %b)
  store %String %"call.String.+", %String* %c, align 8
  %call.String.chars = call i8* @String.chars(%String* %c)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size16" = load i32, i32* %format.size, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text, align 8
  %"$format.size19" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %1 = insertvalue %String %0, i32 %"$format.size19", 1
  %2 = insertvalue %String %1, i32 %ADD17, 2
  call void @"io::out"(%String %2)
  %"call.String.+20" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0))
  store %String %"call.String.+20", %String* %d, align 8
  %call.String.chars21 = call i8* @String.chars(%String* %d)
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars21)
  store i32 %call.snprintf23, i32* %format.size22, align 4
  %"$format.size24" = load i32, i32* %format.size22, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc28 = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc28, i8** %format.text27, align 8
  %"$format.text29" = load i8*, i8** %format.text27, align 8
  %call.snprintf30 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text29", i64 %AS26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars21)
  %"$format.size31" = load i32, i32* %format.size22, align 4
  %ADD32 = add i32 %"$format.size31", 1
  %"$format.text33" = load i8*, i8** %format.text27, align 8
  %"$format.size34" = load i32, i32* %format.size22, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text33", 0
  %4 = insertvalue %String %3, i32 %"$format.size34", 1
  %5 = insertvalue %String %4, i32 %ADD32, 2
  call void @"io::out"(%String %5)
  call void @"String.+=.1"(%String* %a, %String* %b)
  %call.String.chars35 = call i8* @String.chars(%String* %a)
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars35)
  store i32 %call.snprintf37, i32* %format.size36, align 4
  %"$format.size38" = load i32, i32* %format.size36, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars35)
  %"$format.size45" = load i32, i32* %format.size36, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size36, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  %call.String.create49 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create49, %String* %e, align 8
  call void @"String.+="(%String* %e, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  %call.String.chars50 = call i8* @String.chars(%String* %e)
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars50)
  store i32 %call.snprintf52, i32* %format.size51, align 4
  %"$format.size53" = load i32, i32* %format.size51, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars50)
  %"$format.size60" = load i32, i32* %format.size51, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size51, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %10 = insertvalue %String %9, i32 %"$format.size63", 1
  %11 = insertvalue %String %10, i32 %ADD61, 2
  call void @"io::out"(%String %11)
  %call.String.create64 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create64, %String* %x, align 8
  %call.String.create65 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create65, %String* %y, align 8
  %call.String.create66 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create66, %String* %z, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %x, %String* %y)
  store i32 0, i32* %format.size67, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if16

if16:                                             ; preds = %try15.then
  br i1 %"call.String.==", label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else17, %if16.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf68, i32* %format.size67, align 4
  %"$format.size69" = load i32, i32* %format.size67, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %AS71 = sext i32 %ADD70 to i64
  %call.calloc73 = call i8* @calloc(i64 %AS71, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc73, i8** %format.text72, align 8
  %"$format.text74" = load i8*, i8** %format.text72, align 8
  %"$bool.text75" = load i8*, i8** %bool.text, align 8
  %call.snprintf76 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text74", i64 %AS71, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %"$bool.text75")
  %"$format.size77" = load i32, i32* %format.size67, align 4
  %ADD78 = add i32 %"$format.size77", 1
  %"$format.text79" = load i8*, i8** %format.text72, align 8
  %"$format.size80" = load i32, i32* %format.size67, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text79", 0
  %13 = insertvalue %String %12, i32 %"$format.size80", 1
  %14 = insertvalue %String %13, i32 %ADD78, 2
  call void @"io::out"(%String %14)
  %"call.String.==81" = call i1 @"String.=="(%String* %x, %String* %z)
  store i32 0, i32* %format.size82, align 4
  store i8* null, i8** %bool.text83, align 8
  br label %if18

if18:                                             ; preds = %endif
  br i1 %"call.String.==81", label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

else19:                                           ; preds = %if18
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

endif1:                                           ; preds = %else19, %if18.then
  %"$bool.text84" = load i8*, i8** %bool.text83, align 8
  %call.snprintf85 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %"$bool.text84")
  store i32 %call.snprintf85, i32* %format.size82, align 4
  %"$format.size86" = load i32, i32* %format.size82, align 4
  %ADD87 = add i32 %"$format.size86", 1
  %AS88 = sext i32 %ADD87 to i64
  %call.calloc90 = call i8* @calloc(i64 %AS88, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc90, i8** %format.text89, align 8
  %"$format.text91" = load i8*, i8** %format.text89, align 8
  %"$bool.text92" = load i8*, i8** %bool.text83, align 8
  %call.snprintf93 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text91", i64 %AS88, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %"$bool.text92")
  %"$format.size94" = load i32, i32* %format.size82, align 4
  %ADD95 = add i32 %"$format.size94", 1
  %"$format.text96" = load i8*, i8** %format.text89, align 8
  %"$format.size97" = load i32, i32* %format.size82, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text96", 0
  %16 = insertvalue %String %15, i32 %"$format.size97", 1
  %17 = insertvalue %String %16, i32 %ADD95, 2
  call void @"io::out"(%String %17)
  %"call.String.!=" = call i1 @"String.!="(%String* %x, %String* %z)
  store i32 0, i32* %format.size98, align 4
  store i8* null, i8** %bool.text99, align 8
  br label %if20

if20:                                             ; preds = %endif1
  br i1 %"call.String.!=", label %if20.then, label %else21

if20.then:                                        ; preds = %if20
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.20, i32 0, i32 0), i8** %bool.text99, align 8
  br label %endif2

else21:                                           ; preds = %if20
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.21, i32 0, i32 0), i8** %bool.text99, align 8
  br label %endif2

endif2:                                           ; preds = %else21, %if20.then
  %"$bool.text100" = load i8*, i8** %bool.text99, align 8
  %call.snprintf101 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text100")
  store i32 %call.snprintf101, i32* %format.size98, align 4
  %"$format.size102" = load i32, i32* %format.size98, align 4
  %ADD103 = add i32 %"$format.size102", 1
  %AS104 = sext i32 %ADD103 to i64
  %call.calloc106 = call i8* @calloc(i64 %AS104, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc106, i8** %format.text105, align 8
  %"$format.text107" = load i8*, i8** %format.text105, align 8
  %"$bool.text108" = load i8*, i8** %bool.text99, align 8
  %call.snprintf109 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text107", i64 %AS104, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text108")
  %"$format.size110" = load i32, i32* %format.size98, align 4
  %ADD111 = add i32 %"$format.size110", 1
  %"$format.text112" = load i8*, i8** %format.text105, align 8
  %"$format.size113" = load i32, i32* %format.size98, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text112", 0
  %19 = insertvalue %String %18, i32 %"$format.size113", 1
  %20 = insertvalue %String %19, i32 %ADD111, 2
  call void @"io::out"(%String %20)
  %"call.String.!=114" = call i1 @"String.!="(%String* %x, %String* %y)
  store i32 0, i32* %format.size115, align 4
  store i8* null, i8** %bool.text116, align 8
  br label %if22

if22:                                             ; preds = %endif2
  br i1 %"call.String.!=114", label %if22.then, label %else23

if22.then:                                        ; preds = %if22
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.23, i32 0, i32 0), i8** %bool.text116, align 8
  br label %endif3

else23:                                           ; preds = %if22
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i8** %bool.text116, align 8
  br label %endif3

endif3:                                           ; preds = %else23, %if22.then
  %"$bool.text117" = load i8*, i8** %bool.text116, align 8
  %call.snprintf118 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i8* %"$bool.text117")
  store i32 %call.snprintf118, i32* %format.size115, align 4
  %"$format.size119" = load i32, i32* %format.size115, align 4
  %ADD120 = add i32 %"$format.size119", 1
  %AS121 = sext i32 %ADD120 to i64
  %call.calloc123 = call i8* @calloc(i64 %AS121, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc123, i8** %format.text122, align 8
  %"$format.text124" = load i8*, i8** %format.text122, align 8
  %"$bool.text125" = load i8*, i8** %bool.text116, align 8
  %call.snprintf126 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text124", i64 %AS121, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i8* %"$bool.text125")
  %"$format.size127" = load i32, i32* %format.size115, align 4
  %ADD128 = add i32 %"$format.size127", 1
  %"$format.text129" = load i8*, i8** %format.text122, align 8
  %"$format.size130" = load i32, i32* %format.size115, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text129", 0
  %22 = insertvalue %String %21, i32 %"$format.size130", 1
  %23 = insertvalue %String %22, i32 %ADD128, 2
  call void @"io::out"(%String %23)
  %"call.String.<" = call i1 @"String.<"(%String* %x, %String* %z)
  store i32 0, i32* %format.size131, align 4
  store i8* null, i8** %bool.text132, align 8
  br label %if24

if24:                                             ; preds = %endif3
  br i1 %"call.String.<", label %if24.then, label %else25

if24.then:                                        ; preds = %if24
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.26, i32 0, i32 0), i8** %bool.text132, align 8
  br label %endif4

else25:                                           ; preds = %if24
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.27, i32 0, i32 0), i8** %bool.text132, align 8
  br label %endif4

endif4:                                           ; preds = %else25, %if24.then
  %"$bool.text133" = load i8*, i8** %bool.text132, align 8
  %call.snprintf134 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i8* %"$bool.text133")
  store i32 %call.snprintf134, i32* %format.size131, align 4
  %"$format.size135" = load i32, i32* %format.size131, align 4
  %ADD136 = add i32 %"$format.size135", 1
  %AS137 = sext i32 %ADD136 to i64
  %call.calloc139 = call i8* @calloc(i64 %AS137, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc139, i8** %format.text138, align 8
  %"$format.text140" = load i8*, i8** %format.text138, align 8
  %"$bool.text141" = load i8*, i8** %bool.text132, align 8
  %call.snprintf142 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text140", i64 %AS137, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i8* %"$bool.text141")
  %"$format.size143" = load i32, i32* %format.size131, align 4
  %ADD144 = add i32 %"$format.size143", 1
  %"$format.text145" = load i8*, i8** %format.text138, align 8
  %"$format.size146" = load i32, i32* %format.size131, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text145", 0
  %25 = insertvalue %String %24, i32 %"$format.size146", 1
  %26 = insertvalue %String %25, i32 %ADD144, 2
  call void @"io::out"(%String %26)
  %"call.String.<147" = call i1 @"String.<"(%String* %z, %String* %x)
  store i32 0, i32* %format.size148, align 4
  store i8* null, i8** %bool.text149, align 8
  br label %if26

if26:                                             ; preds = %endif4
  br i1 %"call.String.<147", label %if26.then, label %else27

if26.then:                                        ; preds = %if26
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text149, align 8
  br label %endif5

else27:                                           ; preds = %if26
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text149, align 8
  br label %endif5

endif5:                                           ; preds = %else27, %if26.then
  %"$bool.text150" = load i8*, i8** %bool.text149, align 8
  %call.snprintf151 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.31, i32 0, i32 0), i8* %"$bool.text150")
  store i32 %call.snprintf151, i32* %format.size148, align 4
  %"$format.size152" = load i32, i32* %format.size148, align 4
  %ADD153 = add i32 %"$format.size152", 1
  %AS154 = sext i32 %ADD153 to i64
  %call.calloc156 = call i8* @calloc(i64 %AS154, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc156, i8** %format.text155, align 8
  %"$format.text157" = load i8*, i8** %format.text155, align 8
  %"$bool.text158" = load i8*, i8** %bool.text149, align 8
  %call.snprintf159 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text157", i64 %AS154, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.31, i32 0, i32 0), i8* %"$bool.text158")
  %"$format.size160" = load i32, i32* %format.size148, align 4
  %ADD161 = add i32 %"$format.size160", 1
  %"$format.text162" = load i8*, i8** %format.text155, align 8
  %"$format.size163" = load i32, i32* %format.size148, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text162", 0
  %28 = insertvalue %String %27, i32 %"$format.size163", 1
  %29 = insertvalue %String %28, i32 %ADD161, 2
  call void @"io::out"(%String %29)
  %"call.String.>" = call i1 @"String.>"(%String* %z, %String* %x)
  store i32 0, i32* %format.size164, align 4
  store i8* null, i8** %bool.text165, align 8
  br label %if28

if28:                                             ; preds = %endif5
  br i1 %"call.String.>", label %if28.then, label %else29

if28.then:                                        ; preds = %if28
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i8** %bool.text165, align 8
  br label %endif6

else29:                                           ; preds = %if28
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.33, i32 0, i32 0), i8** %bool.text165, align 8
  br label %endif6

endif6:                                           ; preds = %else29, %if28.then
  %"$bool.text166" = load i8*, i8** %bool.text165, align 8
  %call.snprintf167 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text166")
  store i32 %call.snprintf167, i32* %format.size164, align 4
  %"$format.size168" = load i32, i32* %format.size164, align 4
  %ADD169 = add i32 %"$format.size168", 1
  %AS170 = sext i32 %ADD169 to i64
  %call.calloc172 = call i8* @calloc(i64 %AS170, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc172, i8** %format.text171, align 8
  %"$format.text173" = load i8*, i8** %format.text171, align 8
  %"$bool.text174" = load i8*, i8** %bool.text165, align 8
  %call.snprintf175 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text173", i64 %AS170, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text174")
  %"$format.size176" = load i32, i32* %format.size164, align 4
  %ADD177 = add i32 %"$format.size176", 1
  %"$format.text178" = load i8*, i8** %format.text171, align 8
  %"$format.size179" = load i32, i32* %format.size164, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text178", 0
  %31 = insertvalue %String %30, i32 %"$format.size179", 1
  %32 = insertvalue %String %31, i32 %ADD177, 2
  call void @"io::out"(%String %32)
  %"call.String.>180" = call i1 @"String.>"(%String* %x, %String* %z)
  store i32 0, i32* %format.size181, align 4
  store i8* null, i8** %bool.text182, align 8
  br label %if30

if30:                                             ; preds = %endif6
  br i1 %"call.String.>180", label %if30.then, label %else31

if30.then:                                        ; preds = %if30
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.35, i32 0, i32 0), i8** %bool.text182, align 8
  br label %endif7

else31:                                           ; preds = %if30
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.36, i32 0, i32 0), i8** %bool.text182, align 8
  br label %endif7

endif7:                                           ; preds = %else31, %if30.then
  %"$bool.text183" = load i8*, i8** %bool.text182, align 8
  %call.snprintf184 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i8* %"$bool.text183")
  store i32 %call.snprintf184, i32* %format.size181, align 4
  %"$format.size185" = load i32, i32* %format.size181, align 4
  %ADD186 = add i32 %"$format.size185", 1
  %AS187 = sext i32 %ADD186 to i64
  %call.calloc189 = call i8* @calloc(i64 %AS187, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc189, i8** %format.text188, align 8
  %"$format.text190" = load i8*, i8** %format.text188, align 8
  %"$bool.text191" = load i8*, i8** %bool.text182, align 8
  %call.snprintf192 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text190", i64 %AS187, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i8* %"$bool.text191")
  %"$format.size193" = load i32, i32* %format.size181, align 4
  %ADD194 = add i32 %"$format.size193", 1
  %"$format.text195" = load i8*, i8** %format.text188, align 8
  %"$format.size196" = load i32, i32* %format.size181, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text195", 0
  %34 = insertvalue %String %33, i32 %"$format.size196", 1
  %35 = insertvalue %String %34, i32 %ADD194, 2
  call void @"io::out"(%String %35)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp, align 8
  %"call.String.==197" = call i1 @"String.=="(%String* %x, %String* %tmp)
  store i32 0, i32* %format.size198, align 4
  store i8* null, i8** %bool.text199, align 8
  br label %if32

if32:                                             ; preds = %endif7
  br i1 %"call.String.==197", label %if32.then, label %else33

if32.then:                                        ; preds = %if32
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.39, i32 0, i32 0), i8** %bool.text199, align 8
  br label %endif8

else33:                                           ; preds = %if32
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.40, i32 0, i32 0), i8** %bool.text199, align 8
  br label %endif8

endif8:                                           ; preds = %else33, %if32.then
  %"$bool.text200" = load i8*, i8** %bool.text199, align 8
  %call.snprintf201 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i8* %"$bool.text200")
  store i32 %call.snprintf201, i32* %format.size198, align 4
  %"$format.size202" = load i32, i32* %format.size198, align 4
  %ADD203 = add i32 %"$format.size202", 1
  %AS204 = sext i32 %ADD203 to i64
  %call.calloc206 = call i8* @calloc(i64 %AS204, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc206, i8** %format.text205, align 8
  %"$format.text207" = load i8*, i8** %format.text205, align 8
  %"$bool.text208" = load i8*, i8** %bool.text199, align 8
  %call.snprintf209 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text207", i64 %AS204, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i8* %"$bool.text208")
  %"$format.size210" = load i32, i32* %format.size198, align 4
  %ADD211 = add i32 %"$format.size210", 1
  %"$format.text212" = load i8*, i8** %format.text205, align 8
  %"$format.size213" = load i32, i32* %format.size198, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text212", 0
  %37 = insertvalue %String %36, i32 %"$format.size213", 1
  %38 = insertvalue %String %37, i32 %ADD211, 2
  call void @"io::out"(%String %38)
  %39 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %39)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp214, align 8
  %"call.String.==215" = call i1 @"String.=="(%String* %x, %String* %tmp214)
  store i32 0, i32* %format.size216, align 4
  store i8* null, i8** %bool.text217, align 8
  br label %if34

if34:                                             ; preds = %endif8
  br i1 %"call.String.==215", label %if34.then, label %else35

if34.then:                                        ; preds = %if34
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.43, i32 0, i32 0), i8** %bool.text217, align 8
  br label %endif9

else35:                                           ; preds = %if34
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.44, i32 0, i32 0), i8** %bool.text217, align 8
  br label %endif9

endif9:                                           ; preds = %else35, %if34.then
  %"$bool.text218" = load i8*, i8** %bool.text217, align 8
  %call.snprintf219 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i8* %"$bool.text218")
  store i32 %call.snprintf219, i32* %format.size216, align 4
  %"$format.size220" = load i32, i32* %format.size216, align 4
  %ADD221 = add i32 %"$format.size220", 1
  %AS222 = sext i32 %ADD221 to i64
  %call.calloc224 = call i8* @calloc(i64 %AS222, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc224, i8** %format.text223, align 8
  %"$format.text225" = load i8*, i8** %format.text223, align 8
  %"$bool.text226" = load i8*, i8** %bool.text217, align 8
  %call.snprintf227 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text225", i64 %AS222, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i8* %"$bool.text226")
  %"$format.size228" = load i32, i32* %format.size216, align 4
  %ADD229 = add i32 %"$format.size228", 1
  %"$format.text230" = load i8*, i8** %format.text223, align 8
  %"$format.size231" = load i32, i32* %format.size216, align 4
  %40 = insertvalue %String zeroinitializer, i8* %"$format.text230", 0
  %41 = insertvalue %String %40, i32 %"$format.size231", 1
  %42 = insertvalue %String %41, i32 %ADD229, 2
  call void @"io::out"(%String %42)
  %43 = bitcast %String* %tmp214 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %43)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.46, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp232, align 8
  %"call.String.!=233" = call i1 @"String.!="(%String* %x, %String* %tmp232)
  store i32 0, i32* %format.size234, align 4
  store i8* null, i8** %bool.text235, align 8
  br label %if36

if36:                                             ; preds = %endif9
  br i1 %"call.String.!=233", label %if36.then, label %else37

if36.then:                                        ; preds = %if36
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.47, i32 0, i32 0), i8** %bool.text235, align 8
  br label %endif10

else37:                                           ; preds = %if36
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.48, i32 0, i32 0), i8** %bool.text235, align 8
  br label %endif10

endif10:                                          ; preds = %else37, %if36.then
  %"$bool.text236" = load i8*, i8** %bool.text235, align 8
  %call.snprintf237 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text236")
  store i32 %call.snprintf237, i32* %format.size234, align 4
  %"$format.size238" = load i32, i32* %format.size234, align 4
  %ADD239 = add i32 %"$format.size238", 1
  %AS240 = sext i32 %ADD239 to i64
  %call.calloc242 = call i8* @calloc(i64 %AS240, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc242, i8** %format.text241, align 8
  %"$format.text243" = load i8*, i8** %format.text241, align 8
  %"$bool.text244" = load i8*, i8** %bool.text235, align 8
  %call.snprintf245 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text243", i64 %AS240, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text244")
  %"$format.size246" = load i32, i32* %format.size234, align 4
  %ADD247 = add i32 %"$format.size246", 1
  %"$format.text248" = load i8*, i8** %format.text241, align 8
  %"$format.size249" = load i32, i32* %format.size234, align 4
  %44 = insertvalue %String zeroinitializer, i8* %"$format.text248", 0
  %45 = insertvalue %String %44, i32 %"$format.size249", 1
  %46 = insertvalue %String %45, i32 %ADD247, 2
  call void @"io::out"(%String %46)
  %47 = bitcast %String* %tmp232 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %47)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.50, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp250, align 8
  %"call.String.!=251" = call i1 @"String.!="(%String* %x, %String* %tmp250)
  store i32 0, i32* %format.size252, align 4
  store i8* null, i8** %bool.text253, align 8
  br label %if38

if38:                                             ; preds = %endif10
  br i1 %"call.String.!=251", label %if38.then, label %else39

if38.then:                                        ; preds = %if38
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.51, i32 0, i32 0), i8** %bool.text253, align 8
  br label %endif11

else39:                                           ; preds = %if38
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.52, i32 0, i32 0), i8** %bool.text253, align 8
  br label %endif11

endif11:                                          ; preds = %else39, %if38.then
  %"$bool.text254" = load i8*, i8** %bool.text253, align 8
  %call.snprintf255 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text254")
  store i32 %call.snprintf255, i32* %format.size252, align 4
  %"$format.size256" = load i32, i32* %format.size252, align 4
  %ADD257 = add i32 %"$format.size256", 1
  %AS258 = sext i32 %ADD257 to i64
  %call.calloc260 = call i8* @calloc(i64 %AS258, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc260, i8** %format.text259, align 8
  %"$format.text261" = load i8*, i8** %format.text259, align 8
  %"$bool.text262" = load i8*, i8** %bool.text253, align 8
  %call.snprintf263 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text261", i64 %AS258, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text262")
  %"$format.size264" = load i32, i32* %format.size252, align 4
  %ADD265 = add i32 %"$format.size264", 1
  %"$format.text266" = load i8*, i8** %format.text259, align 8
  %"$format.size267" = load i32, i32* %format.size252, align 4
  %48 = insertvalue %String zeroinitializer, i8* %"$format.text266", 0
  %49 = insertvalue %String %48, i32 %"$format.size267", 1
  %50 = insertvalue %String %49, i32 %ADD265, 2
  call void @"io::out"(%String %50)
  %51 = bitcast %String* %tmp250 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %51)
  %call.String.create268 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.54, i32 0, i32 0))
  store %String %call.String.create268, %String* %f, align 8
  %call.String.create269 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.55, i32 0, i32 0))
  store %String %call.String.create269, %String* %g, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %52 = bitcast %String* %copy to %"Array<i8>"*
  %53 = bitcast %String* %y to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %52, %"Array<i8>"* %53)
  %"$copy" = load %String, %String* %copy, align 8
  %54 = bitcast %String* %f to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %54)
  store %String %"$copy", %String* %f, align 8
  %call.String.chars270 = call i8* @String.chars(%String* %f)
  %"call.String.==271" = call i1 @"String.=="(%String* %f, %String* %y)
  store i32 0, i32* %format.size272, align 4
  store i8* null, i8** %bool.text273, align 8
  br label %if40

if40:                                             ; preds = %endif11
  br i1 %"call.String.==271", label %if40.then, label %else41

if40.then:                                        ; preds = %if40
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.56, i32 0, i32 0), i8** %bool.text273, align 8
  br label %endif12

else41:                                           ; preds = %if40
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.57, i32 0, i32 0), i8** %bool.text273, align 8
  br label %endif12

endif12:                                          ; preds = %else41, %if40.then
  %"$bool.text274" = load i8*, i8** %bool.text273, align 8
  %call.snprintf275 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.58, i32 0, i32 0), i8* %call.String.chars270, i8* %"$bool.text274")
  store i32 %call.snprintf275, i32* %format.size272, align 4
  %"$format.size276" = load i32, i32* %format.size272, align 4
  %ADD277 = add i32 %"$format.size276", 1
  %AS278 = sext i32 %ADD277 to i64
  %call.calloc280 = call i8* @calloc(i64 %AS278, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc280, i8** %format.text279, align 8
  %"$format.text281" = load i8*, i8** %format.text279, align 8
  %"$bool.text282" = load i8*, i8** %bool.text273, align 8
  %call.snprintf283 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text281", i64 %AS278, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.58, i32 0, i32 0), i8* %call.String.chars270, i8* %"$bool.text282")
  %"$format.size284" = load i32, i32* %format.size272, align 4
  %ADD285 = add i32 %"$format.size284", 1
  %"$format.text286" = load i8*, i8** %format.text279, align 8
  %"$format.size287" = load i32, i32* %format.size272, align 4
  %55 = insertvalue %String zeroinitializer, i8* %"$format.text286", 0
  %56 = insertvalue %String %55, i32 %"$format.size287", 1
  %57 = insertvalue %String %56, i32 %ADD285, 2
  call void @"io::out"(%String %57)
  %58 = bitcast %String* %g to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %58)
  %59 = bitcast %String* %f to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %59)
  %60 = bitcast %String* %z to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %60)
  %61 = bitcast %String* %y to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %61)
  %62 = bitcast %String* %x to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %62)
  %63 = bitcast %String* %e to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %63)
  %64 = bitcast %String* %d to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %64)
  %65 = bitcast %String* %c to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %65)
  %66 = bitcast %String* %b to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %66)
  %67 = bitcast %String* %a to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %67)
  ret i32 0

endtry:                                           ; preds = %try15
  br label %catch42

catch42:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch42.then, label %endcatch

catch42.then:                                     ; preds = %catch42
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch42
  br label %if43

if43:                                             ; preds = %endcatch
  %"$ucg.thrown288" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown288", label %if43.then, label %endif13

if43.then:                                        ; preds = %if43
  ret i32 1

endif13:                                          ; preds = %if43
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 021 — string: to_int, compare and chained transformations

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
@str = private unnamed_addr constant [6 x i8] c"12345\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"-99\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"apple\00", align 1
@str.8 = private unnamed_addr constant [7 x i8] c"banana\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.12 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"aaaa\00", align 1
@str.15 = private unnamed_addr constant [5 x i8] c"bbbb\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define %String @String.create.1(i32 %0) {
entry:
  %number = alloca i32, align 4
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  store i32 %0, i32* %number, align 4
  store %String zeroinitializer, %String* %res, align 8
  %"$number" = load i32, i32* %number, align 4
  store i32 %"$number", i32* %n, align 4
  br label %if11

if11:                                             ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"$n3" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n3"
  store i32 %SUB, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %while12

while12:                                          ; preds = %endif1, %endif
  br i1 true, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"$n4" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n4", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  call void @String.append(%String* %res, i8 %AS)
  %"$n5" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n5", 10
  store i32 %DIV, i32* %n, align 4
  br label %if13

if13:                                             ; preds = %while12.then
  %"$n6" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n6", 0
  br i1 %EQ, label %if13.then, label %endif1

if13.then:                                        ; preds = %if13
  br label %endloop

endif1:                                           ; preds = %if13
  br label %while12

endloop:                                          ; preds = %if13.then, %while12
  br label %if14

if14:                                             ; preds = %endloop
  %"$number7" = load i32, i32* %number, align 4
  %LT8 = icmp slt i32 %"$number7", 0
  br i1 %LT8, label %if14.then, label %endif2

if14.then:                                        ; preds = %if14
  call void @String.append(%String* %res, i8 45)
  br label %endif2

endif2:                                           ; preds = %if14.then, %if14
  %call.String.reverse = call %String @String.reverse(%String* %res)
  %1 = bitcast %String* %res to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret %String %call.String.reverse
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while15

while15:                                          ; preds = %while15.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
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
  br label %if16

if16:                                             ; preds = %entry
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
  br i1 %AND, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  store i32 -1, i32* %sign, align 4
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  store i32 0, i32* %n, align 4
  br label %while17

while17:                                          ; preds = %endif1, %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count5"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  %"$i9" = load i32, i32* %i, align 4
  %"*self.data[]10" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %"$i9"
  %"$*self.data[]11" = load i8, i8* %"*self.data[]10", align 1
  store i8 %"$*self.data[]11", i8* %c, align 1
  br label %if18

if18:                                             ; preds = %while17.then
  %"$c" = load i8, i8* %c, align 1
  %LT12 = icmp slt i8 %"$c", 48
  %"$c13" = load i8, i8* %c, align 1
  %GT14 = icmp sgt i8 %"$c13", 57
  %OR = or i1 %LT12, %GT14
  br i1 %OR, label %if18.then, label %endif1

if18.then:                                        ; preds = %if18
  br label %endloop

endif1:                                           ; preds = %if18
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
  br label %while17

endloop:                                          ; preds = %if18.then, %while17
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
  br label %while19

while19:                                          ; preds = %endif, %entry
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
  br i1 %AND, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  br label %if20

if20:                                             ; preds = %while19.then
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
  br i1 %NQ, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
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

endif:                                            ; preds = %if20
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while19

endloop:                                          ; preds = %while19
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
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
  br label %while21

while21:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if22

if22:                                             ; preds = %while21.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
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
  br label %while23

while23:                                          ; preds = %while23.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while23.then, label %endloop

while23.then:                                     ; preds = %while23
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
  br label %while23

endloop:                                          ; preds = %while23
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
  %s1 = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %s3 = alloca %String, align 8
  %format.size23 = alloca i32, align 4
  %format.text28 = alloca i8*, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %format.size38 = alloca i32, align 4
  %format.text43 = alloca i8*, align 8
  %format.size52 = alloca i32, align 4
  %format.text57 = alloca i8*, align 8
  %format.size66 = alloca i32, align 4
  %format.text71 = alloca i8*, align 8
  %s = alloca %String, align 8
  %r = alloca %String, align 8
  %r2 = alloca %String, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %big = alloca %String, align 8
  %i = alloca i32, align 4
  %format.size96 = alloca i32, align 4
  %format.text101 = alloca i8*, align 8
  %n1 = alloca %String, align 8
  %format.size111 = alloca i32, align 4
  %format.text116 = alloca i8*, align 8
  %n2 = alloca %String, align 8
  %format.size126 = alloca i32, align 4
  %format.text131 = alloca i8*, align 8
  %error.at = alloca %Error*, align 8
  br label %try24

try24:                                            ; preds = %entry
  br i1 true, label %try24.then, label %endtry

try24.then:                                       ; preds = %try24
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
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s1, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %s1)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.to_int)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.to_int)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.String.create6 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create6, %String* %s2, align 8
  %call.String.to_int7 = call i32 @String.to_int(%String* %s2)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.to_int7)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.to_int7)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  %call.String.create21 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  store %String %call.String.create21, %String* %s3, align 8
  %call.String.to_int22 = call i32 @String.to_int(%String* %s3)
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.to_int22)
  store i32 %call.snprintf24, i32* %format.size23, align 4
  %"$format.size25" = load i32, i32* %format.size23, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %AS27 = sext i32 %ADD26 to i64
  %call.calloc29 = call i8* @calloc(i64 %AS27, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc29, i8** %format.text28, align 8
  %"$format.text30" = load i8*, i8** %format.text28, align 8
  %call.snprintf31 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text30", i64 %AS27, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.to_int22)
  %"$format.size32" = load i32, i32* %format.size23, align 4
  %ADD33 = add i32 %"$format.size32", 1
  %"$format.text34" = load i8*, i8** %format.text28, align 8
  %"$format.size35" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text34", 0
  %7 = insertvalue %String %6, i32 %"$format.size35", 1
  %8 = insertvalue %String %7, i32 %ADD33, 2
  call void @"io::out"(%String %8)
  %call.String.create36 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create36, %String* %a, align 8
  %call.String.create37 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create37, %String* %b, align 8
  %call.String.compare = call i32 @String.compare(%String* %a, %String* %b)
  %call.snprintf39 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %call.String.compare)
  store i32 %call.snprintf39, i32* %format.size38, align 4
  %"$format.size40" = load i32, i32* %format.size38, align 4
  %ADD41 = add i32 %"$format.size40", 1
  %AS42 = sext i32 %ADD41 to i64
  %call.calloc44 = call i8* @calloc(i64 %AS42, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc44, i8** %format.text43, align 8
  %"$format.text45" = load i8*, i8** %format.text43, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text45", i64 %AS42, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %call.String.compare)
  %"$format.size47" = load i32, i32* %format.size38, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text43, align 8
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %10 = insertvalue %String %9, i32 %"$format.size50", 1
  %11 = insertvalue %String %10, i32 %ADD48, 2
  call void @"io::out"(%String %11)
  %call.String.compare51 = call i32 @String.compare(%String* %b, %String* %a)
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.compare51)
  store i32 %call.snprintf53, i32* %format.size52, align 4
  %"$format.size54" = load i32, i32* %format.size52, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %AS56 = sext i32 %ADD55 to i64
  %call.calloc58 = call i8* @calloc(i64 %AS56, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc58, i8** %format.text57, align 8
  %"$format.text59" = load i8*, i8** %format.text57, align 8
  %call.snprintf60 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text59", i64 %AS56, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.compare51)
  %"$format.size61" = load i32, i32* %format.size52, align 4
  %ADD62 = add i32 %"$format.size61", 1
  %"$format.text63" = load i8*, i8** %format.text57, align 8
  %"$format.size64" = load i32, i32* %format.size52, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text63", 0
  %13 = insertvalue %String %12, i32 %"$format.size64", 1
  %14 = insertvalue %String %13, i32 %ADD62, 2
  call void @"io::out"(%String %14)
  %call.String.compare65 = call i32 @String.compare(%String* %a, %String* %a)
  %call.snprintf67 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %call.String.compare65)
  store i32 %call.snprintf67, i32* %format.size66, align 4
  %"$format.size68" = load i32, i32* %format.size66, align 4
  %ADD69 = add i32 %"$format.size68", 1
  %AS70 = sext i32 %ADD69 to i64
  %call.calloc72 = call i8* @calloc(i64 %AS70, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc72, i8** %format.text71, align 8
  %"$format.text73" = load i8*, i8** %format.text71, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text73", i64 %AS70, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %call.String.compare65)
  %"$format.size75" = load i32, i32* %format.size66, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %"$format.text77" = load i8*, i8** %format.text71, align 8
  %"$format.size78" = load i32, i32* %format.size66, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text77", 0
  %16 = insertvalue %String %15, i32 %"$format.size78", 1
  %17 = insertvalue %String %16, i32 %ADD76, 2
  call void @"io::out"(%String %17)
  %call.String.create79 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create79, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %r, align 8
  %call.String.reverse = call %String @String.reverse(%String* %r)
  store %String %call.String.reverse, %String* %r2, align 8
  %call.String.chars = call i8* @String.chars(%String* %r2)
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.String.create93 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0))
  store %String %call.String.create93, %String* %big, align 8
  store i32 0, i32* %i, align 4
  br label %while25

while25:                                          ; preds = %while25.then, %try24.then
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while25.then, label %endloop

while25.then:                                     ; preds = %while25
  call void @String.join(%String* %big, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0))
  %"$i94" = load i32, i32* %i, align 4
  %ADD95 = add i32 %"$i94", 1
  store i32 %ADD95, i32* %i, align 4
  br label %while25

endloop:                                          ; preds = %while25
  %call.String.size = call i32 @String.size(%String* %big)
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %call.String.size)
  store i32 %call.snprintf97, i32* %format.size96, align 4
  %"$format.size98" = load i32, i32* %format.size96, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %AS100 = sext i32 %ADD99 to i64
  %call.calloc102 = call i8* @calloc(i64 %AS100, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc102, i8** %format.text101, align 8
  %"$format.text103" = load i8*, i8** %format.text101, align 8
  %call.snprintf104 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text103", i64 %AS100, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %call.String.size)
  %"$format.size105" = load i32, i32* %format.size96, align 4
  %ADD106 = add i32 %"$format.size105", 1
  %"$format.text107" = load i8*, i8** %format.text101, align 8
  %"$format.size108" = load i32, i32* %format.size96, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text107", 0
  %22 = insertvalue %String %21, i32 %"$format.size108", 1
  %23 = insertvalue %String %22, i32 %ADD106, 2
  call void @"io::out"(%String %23)
  %call.String.create109 = call %String @String.create.1(i32 1000000)
  store %String %call.String.create109, %String* %n1, align 8
  %call.String.chars110 = call i8* @String.chars(%String* %n1)
  %call.snprintf112 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars110)
  store i32 %call.snprintf112, i32* %format.size111, align 4
  %"$format.size113" = load i32, i32* %format.size111, align 4
  %ADD114 = add i32 %"$format.size113", 1
  %AS115 = sext i32 %ADD114 to i64
  %call.calloc117 = call i8* @calloc(i64 %AS115, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc117, i8** %format.text116, align 8
  %"$format.text118" = load i8*, i8** %format.text116, align 8
  %call.snprintf119 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text118", i64 %AS115, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars110)
  %"$format.size120" = load i32, i32* %format.size111, align 4
  %ADD121 = add i32 %"$format.size120", 1
  %"$format.text122" = load i8*, i8** %format.text116, align 8
  %"$format.size123" = load i32, i32* %format.size111, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text122", 0
  %25 = insertvalue %String %24, i32 %"$format.size123", 1
  %26 = insertvalue %String %25, i32 %ADD121, 2
  call void @"io::out"(%String %26)
  %call.String.create124 = call %String @String.create.1(i32 -2000000)
  store %String %call.String.create124, %String* %n2, align 8
  %call.String.chars125 = call i8* @String.chars(%String* %n2)
  %call.snprintf127 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars125)
  store i32 %call.snprintf127, i32* %format.size126, align 4
  %"$format.size128" = load i32, i32* %format.size126, align 4
  %ADD129 = add i32 %"$format.size128", 1
  %AS130 = sext i32 %ADD129 to i64
  %call.calloc132 = call i8* @calloc(i64 %AS130, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc132, i8** %format.text131, align 8
  %"$format.text133" = load i8*, i8** %format.text131, align 8
  %call.snprintf134 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text133", i64 %AS130, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars125)
  %"$format.size135" = load i32, i32* %format.size126, align 4
  %ADD136 = add i32 %"$format.size135", 1
  %"$format.text137" = load i8*, i8** %format.text131, align 8
  %"$format.size138" = load i32, i32* %format.size126, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text137", 0
  %28 = insertvalue %String %27, i32 %"$format.size138", 1
  %29 = insertvalue %String %28, i32 %ADD136, 2
  call void @"io::out"(%String %29)
  %30 = bitcast %String* %n2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %30)
  %31 = bitcast %String* %n1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %31)
  %32 = bitcast %String* %big to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %32)
  %33 = bitcast %String* %r2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %33)
  %34 = bitcast %String* %r to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %34)
  %35 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %35)
  %36 = bitcast %String* %b to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %36)
  %37 = bitcast %String* %a to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %37)
  %38 = bitcast %String* %s3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %38)
  %39 = bitcast %String* %s2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %39)
  %40 = bitcast %String* %s1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %40)
  ret i32 0

endtry:                                           ; preds = %try24
  br label %catch26

catch26:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch26.then, label %endcatch

catch26.then:                                     ; preds = %catch26
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch26
  br label %if27

if27:                                             ; preds = %endcatch
  %"$ucg.thrown139" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown139", label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  ret i32 1

endif:                                            ; preds = %if27
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 026 — output: output and errput print every scalar type in one call

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [44 x i8] c"hp: %d speed: %f alive: %s grade: %c 100%%\0A\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"bad value: %d\0A\00", align 1

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
  %hp = alloca i32, align 4
  %speed = alloca float, align 4
  %alive = alloca i1, align 1
  %grade = alloca i8, align 1
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size9 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  store i32 100, i32* %hp, align 4
  store float 1.500000e+00, float* %speed, align 4
  store i1 true, i1* %alive, align 1
  store i8 83, i8* %grade, align 1
  store i32 0, i32* %format.size, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %"$speed" = load float, float* %speed, align 4
  %AS = fpext float %"$speed" to double
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"$alive", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$grade" = load i8, i8* %grade, align 1
  %AS1 = sext i8 %"$grade" to i32
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([44 x i8], [44 x i8]* @str.2, i32 0, i32 0), i32 %"$hp", double %AS, i8* %"$bool.text", i32 %AS1)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS2 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS2, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text3" = load i8*, i8** %bool.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS2, i8* getelementptr inbounds ([44 x i8], [44 x i8]* @str.2, i32 0, i32 0), i32 %"$hp", double %AS, i8* %"$bool.text3", i32 %AS1)
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 7)
  store i32 %call.snprintf10, i32* %format.size9, align 4
  %"$format.size11" = load i32, i32* %format.size9, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 7)
  %"$format.size18" = load i32, i32* %format.size9, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size9, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %4 = insertvalue %String %3, i32 %"$format.size21", 1
  %5 = insertvalue %String %4, i32 %ADD19, 2
  call void @"io::err"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 027 — input: read two lines from stdin

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"name? \00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"hello %s\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"age? \00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"you said %s\0A\00", align 1

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

define %String @"io::input"() {
entry:
  %line = alloca %String, align 8
  %c = alloca i8, align 1
  store %String zeroinitializer, %String* %line, align 8
  store i8 0, i8* %c, align 1
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
  %"$line" = load %String, %String* %line, align 8
  ret %String %"$line"
}

define i32 @main() {
entry:
  %name = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %age = alloca %String, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 6, i32 0 })
  %"call.io::input" = call %String @"io::input"()
  store %String %"call.io::input", %String* %name, align 8
  %call.String.chars = call i8* @String.chars(%String* %name)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i32 5, i32 0 })
  %"call.io::input6" = call %String @"io::input"()
  store %String %"call.io::input6", %String* %age, align 8
  %call.String.chars7 = call i8* @String.chars(%String* %age)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars7)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars7)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  %6 = bitcast %String* %age to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  %7 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 028 — $ strings: values inside {} formatted into a String

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [27 x i8] c"test.ura:%d hp %d %f %s %c\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [14 x i8] c"name: %s, %s\0A\00", align 1

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
  %name = alloca %String, align 8
  %s = alloca %String, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %bay = alloca i21, align 4
  %format.size22 = alloca i32, align 4
  %arr = alloca [5 x i8], align 1
  %format.text28 = alloca i8*, align 8
  store i32 10, i32* %hp, align 4
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 3, i32 0 }, %String* %name, align 8
  store %String zeroinitializer, %String* %s, align 8
  %"$hp" = load i32, i32* %hp, align 4
  %EQ = icmp eq i32 %"$hp", 10
  store i32 0, i32* %format.size, align 4
  %"$hp1" = load i32, i32* %hp, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %EQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.3, i32 0, i32 0), i32 8, i32 %"$hp1", double 2.500000e+00, i8* %"$bool.text", i32 99)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text2" = load i8*, i8** %bool.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.3, i32 0, i32 0), i32 8, i32 %"$hp1", double 2.500000e+00, i8* %"$bool.text2", i32 99)
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  store %String %2, %String* %s, align 8
  %call.String.chars = call i8* @String.chars(%String* %s)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  store i21 28006, i21* %bay, align 4
  %call.String.chars21 = call i8* @String.chars(%String* %name)
  store i32 0, i32* %format.size22, align 4
  %"$bay" = load i21, i21* %bay, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr, align 1
  %6 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  %AS23 = sext i21 %"$bay" to i32
  %call.ura.unic = call i8* @ura.unic(i32 %AS23, i8* %6)
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars21, i8* %call.ura.unic)
  store i32 %call.snprintf24, i32* %format.size22, align 4
  %"$format.size25" = load i32, i32* %format.size22, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %AS27 = sext i32 %ADD26 to i64
  %call.calloc29 = call i8* @calloc(i64 %AS27, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc29, i8** %format.text28, align 8
  %"$format.text30" = load i8*, i8** %format.text28, align 8
  %call.snprintf31 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text30", i64 %AS27, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars21, i8* %call.ura.unic)
  %"$format.size32" = load i32, i32* %format.size22, align 4
  %ADD33 = add i32 %"$format.size32", 1
  %"$format.text34" = load i8*, i8** %format.text28, align 8
  %"$format.size35" = load i32, i32* %format.size22, align 4
  %7 = insertvalue %String zeroinitializer, i8* %"$format.text34", 0
  %8 = insertvalue %String %7, i32 %"$format.size35", 1
  %9 = insertvalue %String %8, i32 %ADD33, 2
  call void @"io::out"(%String %9)
  %10 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %10)
  %11 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)

define i8* @ura.unic(i32 %0, i8* %1) {
entry:
  %code = alloca i32, align 4
  %text = alloca i8*, align 8
  store i32 %0, i32* %code, align 4
  store i8* %1, i8** %text, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"$code" = load i32, i32* %code, align 4
  %LT = icmp slt i32 %"$code", 128
  br i1 %LT, label %if8.then, label %elif9

if8.then:                                         ; preds = %if8
  %"$code1" = load i32, i32* %code, align 4
  %DIV = sdiv i32 %"$code1", 1
  %ADD = add i32 %DIV, 0
  %MOD = srem i32 %DIV, 64
  %ADD2 = add i32 %MOD, 128
  %AS = trunc i32 %ADD to i8
  %"$text" = load i8*, i8** %text, align 8
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 0
  store i8 %AS, i8* %"text[]", align 1
  %"$text3" = load i8*, i8** %text, align 8
  %"text[]4" = getelementptr inbounds i8, i8* %"$text3", i32 1
  store i8 0, i8* %"text[]4", align 1
  br label %endif

elif9:                                            ; preds = %if8
  %"$code5" = load i32, i32* %code, align 4
  %LT6 = icmp slt i32 %"$code5", 2048
  br i1 %LT6, label %elif9.then, label %elif10

elif9.then:                                       ; preds = %elif9
  %"$code7" = load i32, i32* %code, align 4
  %DIV8 = sdiv i32 %"$code7", 1
  %ADD9 = add i32 %DIV8, 192
  %MOD10 = srem i32 %DIV8, 64
  %ADD11 = add i32 %MOD10, 128
  %AS12 = trunc i32 %ADD11 to i8
  %"$text13" = load i8*, i8** %text, align 8
  %"text[]14" = getelementptr inbounds i8, i8* %"$text13", i32 1
  store i8 %AS12, i8* %"text[]14", align 1
  %"$code15" = load i32, i32* %code, align 4
  %DIV16 = sdiv i32 %"$code15", 64
  %ADD17 = add i32 %DIV16, 192
  %MOD18 = srem i32 %DIV16, 64
  %ADD19 = add i32 %MOD18, 128
  %AS20 = trunc i32 %ADD17 to i8
  %"$text21" = load i8*, i8** %text, align 8
  %"text[]22" = getelementptr inbounds i8, i8* %"$text21", i32 0
  store i8 %AS20, i8* %"text[]22", align 1
  %"$text23" = load i8*, i8** %text, align 8
  %"text[]24" = getelementptr inbounds i8, i8* %"$text23", i32 2
  store i8 0, i8* %"text[]24", align 1
  br label %endif

elif10:                                           ; preds = %elif9
  %"$code25" = load i32, i32* %code, align 4
  %LT26 = icmp slt i32 %"$code25", 65536
  br i1 %LT26, label %elif10.then, label %elif11

elif10.then:                                      ; preds = %elif10
  %"$code27" = load i32, i32* %code, align 4
  %DIV28 = sdiv i32 %"$code27", 1
  %ADD29 = add i32 %DIV28, 224
  %MOD30 = srem i32 %DIV28, 64
  %ADD31 = add i32 %MOD30, 128
  %AS32 = trunc i32 %ADD31 to i8
  %"$text33" = load i8*, i8** %text, align 8
  %"text[]34" = getelementptr inbounds i8, i8* %"$text33", i32 2
  store i8 %AS32, i8* %"text[]34", align 1
  %"$code35" = load i32, i32* %code, align 4
  %DIV36 = sdiv i32 %"$code35", 64
  %ADD37 = add i32 %DIV36, 224
  %MOD38 = srem i32 %DIV36, 64
  %ADD39 = add i32 %MOD38, 128
  %AS40 = trunc i32 %ADD39 to i8
  %"$text41" = load i8*, i8** %text, align 8
  %"text[]42" = getelementptr inbounds i8, i8* %"$text41", i32 1
  store i8 %AS40, i8* %"text[]42", align 1
  %"$code43" = load i32, i32* %code, align 4
  %DIV44 = sdiv i32 %"$code43", 4096
  %ADD45 = add i32 %DIV44, 224
  %MOD46 = srem i32 %DIV44, 64
  %ADD47 = add i32 %MOD46, 128
  %AS48 = trunc i32 %ADD45 to i8
  %"$text49" = load i8*, i8** %text, align 8
  %"text[]50" = getelementptr inbounds i8, i8* %"$text49", i32 0
  store i8 %AS48, i8* %"text[]50", align 1
  %"$text51" = load i8*, i8** %text, align 8
  %"text[]52" = getelementptr inbounds i8, i8* %"$text51", i32 3
  store i8 0, i8* %"text[]52", align 1
  br label %endif

elif11:                                           ; preds = %elif10
  %"$code53" = load i32, i32* %code, align 4
  %LT54 = icmp slt i32 %"$code53", 1114112
  br i1 %LT54, label %elif11.then, label %endif

elif11.then:                                      ; preds = %elif11
  %"$code55" = load i32, i32* %code, align 4
  %DIV56 = sdiv i32 %"$code55", 1
  %ADD57 = add i32 %DIV56, 240
  %MOD58 = srem i32 %DIV56, 64
  %ADD59 = add i32 %MOD58, 128
  %AS60 = trunc i32 %ADD59 to i8
  %"$text61" = load i8*, i8** %text, align 8
  %"text[]62" = getelementptr inbounds i8, i8* %"$text61", i32 3
  store i8 %AS60, i8* %"text[]62", align 1
  %"$code63" = load i32, i32* %code, align 4
  %DIV64 = sdiv i32 %"$code63", 64
  %ADD65 = add i32 %DIV64, 240
  %MOD66 = srem i32 %DIV64, 64
  %ADD67 = add i32 %MOD66, 128
  %AS68 = trunc i32 %ADD67 to i8
  %"$text69" = load i8*, i8** %text, align 8
  %"text[]70" = getelementptr inbounds i8, i8* %"$text69", i32 2
  store i8 %AS68, i8* %"text[]70", align 1
  %"$code71" = load i32, i32* %code, align 4
  %DIV72 = sdiv i32 %"$code71", 4096
  %ADD73 = add i32 %DIV72, 240
  %MOD74 = srem i32 %DIV72, 64
  %ADD75 = add i32 %MOD74, 128
  %AS76 = trunc i32 %ADD75 to i8
  %"$text77" = load i8*, i8** %text, align 8
  %"text[]78" = getelementptr inbounds i8, i8* %"$text77", i32 1
  store i8 %AS76, i8* %"text[]78", align 1
  %"$code79" = load i32, i32* %code, align 4
  %DIV80 = sdiv i32 %"$code79", 262144
  %ADD81 = add i32 %DIV80, 240
  %MOD82 = srem i32 %DIV80, 64
  %ADD83 = add i32 %MOD82, 128
  %AS84 = trunc i32 %ADD81 to i8
  %"$text85" = load i8*, i8** %text, align 8
  %"text[]86" = getelementptr inbounds i8, i8* %"$text85", i32 0
  store i8 %AS84, i8* %"text[]86", align 1
  %"$text87" = load i8*, i8** %text, align 8
  %"text[]88" = getelementptr inbounds i8, i8* %"$text87", i32 4
  store i8 0, i8* %"text[]88", align 1
  br label %endif

endif:                                            ; preds = %elif11.then, %elif11, %elif10.then, %elif9.then, %if8.then
  %"$text89" = load i8*, i8** %text, align 8
  ret i8* %"$text89"
}
```

## 029 — $ strings: \{ and \} are literal braces

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [15 x i8] c"set { x: %d }\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"{}\0A\00", align 1

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
  %"$x" = load i32, i32* %x, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$x")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  call void @"io::out"(%String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 3, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 030 — operator $: a struct formats itself inside {}

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Point = type { i32, i32 }
%Line = type { %Point, %Point }

@str = private unnamed_addr constant [9 x i8] c"(%d, %d)\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"%s -> %s\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"p = %s\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"line = %s\0A\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

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

define %String @"Point.$"(%Point* %0) {
entry:
  %left = alloca %Point*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Point* %0, %Point** %left, align 8
  %"*left" = load %Point*, %Point** %left, align 8
  %"*left.x" = getelementptr inbounds %Point, %Point* %"*left", i32 0, i32 0
  %"*left1" = load %Point*, %Point** %left, align 8
  %"*left.y" = getelementptr inbounds %Point, %Point* %"*left1", i32 0, i32 1
  %"$*left.x" = load i32, i32* %"*left.x", align 4
  %"$*left.y" = load i32, i32* %"*left.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*left.x", i32 %"$*left.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*left.x", i32 %"$*left.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  ret %String %3
}

define %String @"Line.$"(%Line* %0) {
entry:
  %left = alloca %Line*, align 8
  %tmp = alloca %String, align 8
  %tmp3 = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Line* %0, %Line** %left, align 8
  %"*left" = load %Line*, %Line** %left, align 8
  %"*left.a" = getelementptr inbounds %Line, %Line* %"*left", i32 0, i32 0
  %"call.Point.$" = call %String @"Point.$"(%Point* %"*left.a")
  store %String %"call.Point.$", %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %"*left1" = load %Line*, %Line** %left, align 8
  %"*left.b" = getelementptr inbounds %Line, %Line* %"*left1", i32 0, i32 1
  %"call.Point.$2" = call %String @"Point.$"(%Point* %"*left.b")
  store %String %"call.Point.$2", %String* %tmp3, align 8
  %call.String.chars4 = call i8* @String.chars(%String* %tmp3)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %2 = insertvalue %String %1, i32 %"$format.size9", 1
  %3 = insertvalue %String %2, i32 %ADD7, 2
  %4 = bitcast %String* %tmp3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  %5 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  ret %String %3
}

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %l = alloca %Line, align 8
  %tmp7 = alloca %String, align 8
  %format.size9 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  %i = alloca i32, align 4
  %q = alloca %Point, align 8
  %tmp25 = alloca %String, align 8
  %format.size27 = alloca i32, align 4
  %format.text33 = alloca i8*, align 8
  store %Point zeroinitializer, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %p.x, align 4
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  store i32 2, i32* %p.y, align 4
  %"call.Point.$" = call %String @"Point.$"(%Point* %p)
  store %String %"call.Point.$", %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %3 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  store %Line zeroinitializer, %Line* %l, align 4
  %l.a = getelementptr inbounds %Line, %Line* %l, i32 0, i32 0
  %"$p" = load %Point, %Point* %p, align 4
  store %Point %"$p", %Point* %l.a, align 4
  %l.b = getelementptr inbounds %Line, %Line* %l, i32 0, i32 1
  %l.b.x = getelementptr inbounds %Point, %Point* %l.b, i32 0, i32 0
  store i32 3, i32* %l.b.x, align 4
  %l.b6 = getelementptr inbounds %Line, %Line* %l, i32 0, i32 1
  %l.b.y = getelementptr inbounds %Point, %Point* %l.b6, i32 0, i32 1
  store i32 4, i32* %l.b.y, align 4
  %"call.Line.$" = call %String @"Line.$"(%Line* %l)
  store %String %"call.Line.$", %String* %tmp7, align 8
  %call.String.chars8 = call i8* @String.chars(%String* %tmp7)
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars8)
  store i32 %call.snprintf10, i32* %format.size9, align 4
  %"$format.size11" = load i32, i32* %format.size9, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars8)
  %"$format.size18" = load i32, i32* %format.size9, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size9, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %5 = insertvalue %String %4, i32 %"$format.size21", 1
  %6 = insertvalue %String %5, i32 %ADD19, 2
  call void @"io::out"(%String %6)
  %7 = bitcast %String* %tmp7 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  store %Point zeroinitializer, %Point* %q, align 4
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$i22" = load i32, i32* %i, align 4
  store i32 %"$i22", i32* %q.x, align 4
  %q.y = getelementptr inbounds %Point, %Point* %q, i32 0, i32 1
  %"$i23" = load i32, i32* %i, align 4
  store i32 %"$i23", i32* %q.y, align 4
  %"call.Point.$24" = call %String @"Point.$"(%Point* %q)
  store %String %"call.Point.$24", %String* %tmp25, align 8
  %call.String.chars26 = call i8* @String.chars(%String* %tmp25)
  %"$i28" = load i32, i32* %i, align 4
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %"$i28", i8* %call.String.chars26)
  store i32 %call.snprintf29, i32* %format.size27, align 4
  %"$format.size30" = load i32, i32* %format.size27, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %AS32 = sext i32 %ADD31 to i64
  %call.calloc34 = call i8* @calloc(i64 %AS32, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc34, i8** %format.text33, align 8
  %"$format.text35" = load i8*, i8** %format.text33, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text35", i64 %AS32, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %"$i28", i8* %call.String.chars26)
  %"$format.size37" = load i32, i32* %format.size27, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %"$format.text39" = load i8*, i8** %format.text33, align 8
  %"$format.size40" = load i32, i32* %format.size27, align 4
  %8 = insertvalue %String zeroinitializer, i8* %"$format.text39", 0
  %9 = insertvalue %String %8, i32 %"$format.size40", 1
  %10 = insertvalue %String %9, i32 %ADD38, 2
  call void @"io::out"(%String %10)
  %11 = bitcast %String* %tmp25 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %11)
  %"$i41" = load i32, i32* %i, align 4
  %ADD42 = add i32 %"$i41", 1
  store i32 %ADD42, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 031 — operator $: a struct without it can't go inside {}

```stderr
error: 'Point' has no operator '$'
at test.ura:8:20
  |
8 |     io::out($"p = {p}\n")
  |                    ^
error: aborting due to 1 error
1
```

## 032 — io: io::out, io::err and io::input from ura-core

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"name? \00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"hello %s\0A\00", align 1
@str.2 = private unnamed_addr constant [21 x i8] c"this goes to stderr\0A\00", align 1
@str.3 = private unnamed_addr constant [19 x i8] c"3 letters? no, %d\0A\00", align 1

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

define %String @"io::input"() {
entry:
  %line = alloca %String, align 8
  %c = alloca i8, align 1
  store %String zeroinitializer, %String* %line, align 8
  store i8 0, i8* %c, align 1
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
  %"$line" = load %String, %String* %line, align 8
  ret %String %"$line"
}

define i32 @main() {
entry:
  %name = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 6, i32 0 })
  %"call.io::input" = call %String @"io::input"()
  store %String %"call.io::input", %String* %name, align 8
  %call.String.chars = call i8* @String.chars(%String* %name)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  call void @"io::err"(%String { i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.2, i32 0, i32 0), i32 20, i32 0 })
  %call.String.len = call i32 @String.len(%String* %name)
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i32 %call.String.len)
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.3, i32 0, i32 0), i32 %call.String.len)
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %6 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

