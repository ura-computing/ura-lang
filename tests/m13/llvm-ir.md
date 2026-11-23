# m13 llvm ir

## 001 — builtin: default argument promotion in a user variadic call

```llvm
@str = private unnamed_addr constant [18 x i8] c"%.2f %c %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"%.2f %c %d\0A\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"%.2f %d\0A\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"none\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %f = alloca float, align 4
  %c = alloca i8, align 1
  %s = alloca i16, align 2
  %b = alloca i1, align 1
  %l = alloca i64, align 8
  %i = alloca i32, align 4
  store float 3.500000e+00, float* %f, align 4
  store i8 65, i8* %c, align 1
  store i16 7, i16* %s, align 2
  store i1 true, i1* %b, align 1
  store i64 123456789, i64* %l, align 8
  store i32 42, i32* %i, align 4
  %"$f" = load float, float* %f, align 4
  %"$c" = load i8, i8* %c, align 1
  %"$s" = load i16, i16* %s, align 2
  %"$b" = load i1, i1* %b, align 1
  %"$i" = load i32, i32* %i, align 4
  %0 = fpext float %"$f" to double
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), double %0, i8 %"$c", i16 %"$s", i1 %"$b", i32 %"$i")
  %"$l" = load i64, i64* %l, align 8
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i64 %"$l")
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), double 2.500000e+00, i8 90, i32 9)
  %"$f3" = load float, float* %f, align 4
  %"$f4" = load float, float* %f, align 4
  %ADD = fadd float %"$f3", %"$f4"
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  %1 = fpext float %ADD to double
  %call.printf7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), double %1, i32 %ADD6)
  %call.printf8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i1 true, i1 false)
  %call.printf9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}
```

## 002 — directive: @if/@elif/@else selects the matching platform branch

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"unix\0A\00", align 1

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
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 003 — directive: @if unix matches on macOS and linux

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"posix\0A\00", align 1

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
  call void @"io::out"(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 6, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 004 — directive: a non-matching branch is dropped before parsing

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"kept\0A\00", align 1

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
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 005 — directive: a platform-gated top-level proto

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

declare i32 @getpid()

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %call.getpid = call i32 @getpid()
  %GT = icmp sgt i32 %call.getpid, 0
  store i32 0, i32* %format.size, align 4
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
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text1" = load i8*, i8** %bool.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text1")
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

## 006 — directive: @if selects a value inside a function

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

define i32 @tag() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.tag = call i32 @tag()
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %call.tag)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %call.tag)
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

## 014 — function: nested non-capturing helper function

```llvm
define i32 @main() {
entry:
  %call.dbl = call i32 @dbl(i32 21)
  ret i32 %call.dbl
}

define i32 @dbl(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %"$k" = load i32, i32* %k, align 4
  %"$k1" = load i32, i32* %k, align 4
  %ADD = add i32 %"$k", %"$k1"
  ret i32 %ADD
}
```

## 016 — function: proto then definition, and definition then proto

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

define i32 @tally(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  ret i32 %ADD
}

define i32 @double(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.tally = call i32 @tally(i32 41)
  %call.double = call i32 @double(i32 21)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call.tally, i32 %call.double)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call.tally, i32 %call.double)
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

## 018 — function: conflict between two of your own protos

```stderr
error: test.ura:6:7 'scan' is already declared with other parameters, on line 4
  |
6 | proto scan(a i32, b i8[], c i32) i32
  |       ^^^^
help: declare it once, with ... for the optional arguments
error: aborting due to 1 error
1
```

## 020 — function: infinite recursion overflows the stack

```llvm
define i32 @recurse(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.recurse = call i32 @recurse(i32 %"$n")
  ret i32 %call.recurse
}

define i32 @main() {
entry:
  %call.recurse = call i32 @recurse(i32 1)
  ret i32 %call.recurse
}
```

## 025 — project: calc

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Error = type { %String }
%Token = type { i32, i32 }
%"Array<Token>" = type { %Token*, i32, i32 }
%Node = type { %Token, i32, i32 }
%"Array<Node>" = type { %Node*, i32, i32 }
%Parser = type { %"Array<Token>", i32, %"Array<Node>" }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [2 x i8] c"+\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"?\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"op: %s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"\E2\94\9C\E2\94\80 \00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"\E2\94\82  \00", align 1
@str.8 = private unnamed_addr constant [8 x i8] c"\E2\94\94\E2\94\80 \00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.10 = private unnamed_addr constant [8 x i8] c"%s%s%s\0A\00", align 1
@str.11 = private unnamed_addr constant [17 x i8] c"division by zero\00", align 1
@str.12 = private unnamed_addr constant [26 x i8] c"calculate expr:  %s\0Aast:\0A\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.15 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.16 = private unnamed_addr constant [12 x i8] c"result: %d\0A\00", align 1
@str.17 = private unnamed_addr constant [51 x i8] c"ura-calc: type an expression, an empty line quits\0A\00", align 1
@str.18 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@str.19 = private unnamed_addr constant [11 x i8] c"error: %s\0A\00", align 1
@str.20 = private unnamed_addr constant [42 x i8] c"========================================\0A\00", align 1
@str.21 = private unnamed_addr constant [3 x i8] c"> \00", align 1

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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*right2" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
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
  br label %while6

endloop:                                          ; preds = %while6
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if11.then, %if11
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

define %String @String.create(i32 %0) {
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
  br label %while16

while16:                                          ; preds = %while16.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
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
  br label %while16

endloop:                                          ; preds = %while16
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define void @"String.+="(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store i32 0, i32* %i, align 4
  br label %while17

while17:                                          ; preds = %while17.then, %entry
  %"*right" = load %String*, %String** %right, align 8
  %"*right.count" = getelementptr inbounds %String, %String* %"*right", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
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
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define %String @"String.+"(%String* %0, %String* %1) {
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
  call void @"String.+="(%String* %res, %String* %"*right")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  %1 = bitcast %String* %copy to %"Array<i8>"*
  %2 = bitcast %String* %message to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %1, %"Array<i8>"* %2)
  %"$copy" = load %String, %String* %copy, align 8
  %3 = bitcast %String* %e.message to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  %4 = bitcast %String* %message to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  ret %Error %"$e"
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
  br label %while18

while18:                                          ; preds = %while18.then, %entry
  %call.read = call i64 @read(i32 0, i8* %c, i64 1)
  %EQ = icmp eq i64 %call.read, 1
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 10
  %AND = and i1 %EQ, %NQ
  br i1 %AND, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  %"$c1" = load i8, i8* %c, align 1
  call void @String.append(%String* %line, i8 %"$c1")
  br label %while18

endloop:                                          ; preds = %while18
  %"$line" = load %String, %String* %line, align 8
  ret %String %"$line"
}

define i1 @is_digit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 48
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define %Token* @"new<Token>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Token*
  ret %Token* %AS1
}

define void @"copy<Token>"(%Token* %0, %Token* %1, i32 %2) {
entry:
  %dst = alloca %Token*, align 8
  %src = alloca %Token*, align 8
  %count = alloca i32, align 4
  store %Token* %0, %Token** %dst, align 8
  store %Token* %1, %Token** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Token*, %Token** %dst, align 8
  %AS = bitcast %Token* %"$dst" to i8*
  %"$src" = load %Token*, %Token** %src, align 8
  %AS1 = bitcast %Token* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Token>"(%Token* %0) {
entry:
  %data = alloca %Token*, align 8
  store %Token* %0, %Token** %data, align 8
  %"$data" = load %Token*, %Token** %data, align 8
  %AS = bitcast %Token* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Token>.reserve"(%"Array<Token>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Token*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  br label %if20

if20:                                             ; preds = %endif
  %"*self6" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  ret void

endif1:                                           ; preds = %if20
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Token>" = call %Token* @"new<Token>"(i32 %"$cap8")
  store %Token* %"call.new<Token>", %Token** %bigger, align 8
  %"*self9" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Token*, %Token** %bigger, align 8
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Token>"(%Token* %"$bigger", %Token* %"$*self.data", i32 %"$*self.count12")
  br label %if21

if21:                                             ; preds = %endif1
  %"*self13" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if21.then, label %endif2

if21.then:                                        ; preds = %if21
  %"*self16" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Token*, %Token** %"*self.data17", align 8
  call void @"free<Token>"(%Token* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if21.then, %if21
  %"*self19" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Token*, %Token** %bigger, align 8
  store %Token* %"$bigger21", %Token** %"*self.data20", align 8
  %"*self22" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Token>.own"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Token>.reserve"(%"Array<Token>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  ret void
}

define void @"Array<Token>.clear"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  br label %while24

while24:                                          ; preds = %while24.then, %endif
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self4" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Token, %Token* %"$*self.data", i32 %"$*self.count10"
  store %Token zeroinitializer, %Token* %"*self.data[]", align 4
  br label %while24

endloop:                                          ; preds = %while24
  ret void
}

define void @"Array<Token>.append"(%"Array<Token>"* %0, %Token %1) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  %value = alloca %Token, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  store %Token %1, %Token* %value, align 4
  br label %if25

if25:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Token>.reserve"(%"Array<Token>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  %"*self6" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Token, %Token* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Token, %Token* %value, align 4
  store %Token %"$value", %Token* %"*self.data[]", align 4
  %"*self10" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Token>.len"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Token* @"Array<Token>.[]"(%"Array<Token>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Token>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Token>"* %0, %"Array<Token>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Token>"*, %"Array<Token>"** %left, align 8
  call void @"Array<Token>.own"(%"Array<Token>"* %"*left")
  %"*left1" = load %"Array<Token>"*, %"Array<Token>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Token*, %Token** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Token, %Token* %"$*left.data", i32 %"$index"
  ret %Token* %"*left.data[]"
}

define void @"Array<Token>.drop"(%"Array<Token>"* %0) {
entry:
  %value = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %value, align 8
  %"*value" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  call void @"Array<Token>.clear"(%"Array<Token>"* %"*value")
  br label %if26

if26:                                             ; preds = %entry
  %"*value1" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"*value2" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Token*, %Token** %"*value.data", align 8
  call void @"free<Token>"(%Token* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  ret void
}

define %"Array<Token>" @lex(%String %0) {
entry:
  %src = alloca %String, align 8
  %toks = alloca %"Array<Token>", align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  %t = alloca %Token, align 8
  %ref12 = alloca i8*, align 8
  %ref19 = alloca i8*, align 8
  %"match@1439" = alloca i8, align 1
  store %String %0, %String* %src, align 8
  store %"Array<Token>" zeroinitializer, %"Array<Token>"* %toks, align 8
  store i32 0, i32* %i, align 4
  br label %while27

while27:                                          ; preds = %endif2, %endloop, %if28.then, %entry
  %call.String.len = call i32 @String.len(%String* %src)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.String.len
  br i1 %LT, label %while27.then, label %endloop3

while27.then:                                     ; preds = %while27
  %"$i4" = load i32, i32* %i, align 4
  %1 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %1, i32 %"$i4")
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  store %Token zeroinitializer, %Token* %t, align 4
  br label %if28

if28:                                             ; preds = %while27.then
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 32
  br i1 %EQ, label %if28.then, label %endif

if28.then:                                        ; preds = %if28
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while27

endif:                                            ; preds = %if28
  br label %if29

if29:                                             ; preds = %endif
  %"$c6" = load i8, i8* %c, align 1
  %call.is_digit = call i1 @is_digit(i8 %"$c6")
  br i1 %call.is_digit, label %if29.then, label %endif1

if29.then:                                        ; preds = %if29
  %t.kind = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 1, i32* %t.kind, align 4
  br label %while30

while30:                                          ; preds = %while30.then, %if29.then
  %call.String.len7 = call i32 @String.len(%String* %src)
  %"$i8" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i8", %call.String.len7
  %"$i10" = load i32, i32* %i, align 4
  %2 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]11" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %2, i32 %"$i10")
  store i8* %"call.Array<i8>.[]11", i8** %ref12, align 8
  %"*ref13" = load i8*, i8** %ref12, align 8
  %"$*ref14" = load i8, i8* %"*ref13", align 1
  %call.is_digit15 = call i1 @is_digit(i8 %"$*ref14")
  %AND = and i1 %LT9, %call.is_digit15
  br i1 %AND, label %while30.then, label %endloop

while30.then:                                     ; preds = %while30
  %t.value = getelementptr inbounds %Token, %Token* %t, i32 0, i32 1
  %t.value16 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 1
  %"$t.value" = load i32, i32* %t.value16, align 4
  %MUL = mul i32 %"$t.value", 10
  %"$i17" = load i32, i32* %i, align 4
  %3 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]18" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %3, i32 %"$i17")
  store i8* %"call.Array<i8>.[]18", i8** %ref19, align 8
  %"*ref20" = load i8*, i8** %ref19, align 8
  %"$*ref21" = load i8, i8* %"*ref20", align 1
  %AS = sext i8 %"$*ref21" to i32
  %SUB = sub i32 %AS, 48
  %ADD22 = add i32 %MUL, %SUB
  store i32 %ADD22, i32* %t.value, align 4
  %"$i23" = load i32, i32* %i, align 4
  %ADD24 = add i32 %"$i23", 1
  store i32 %ADD24, i32* %i, align 4
  br label %while30

endloop:                                          ; preds = %while30
  %"$t" = load %Token, %Token* %t, align 4
  call void @"Array<Token>.append"(%"Array<Token>"* %toks, %Token %"$t")
  br label %while27

endif1:                                           ; preds = %if29
  %"$c25" = load i8, i8* %c, align 1
  store i8 %"$c25", i8* %"match@1439", align 1
  br label %if31

if31:                                             ; preds = %endif1
  %"$match@1439" = load i8, i8* %"match@1439", align 1
  %EQ26 = icmp eq i8 %"$match@1439", 43
  br i1 %EQ26, label %if31.then, label %elif32

if31.then:                                        ; preds = %if31
  %t.kind27 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 2, i32* %t.kind27, align 4
  br label %endif2

elif32:                                           ; preds = %if31
  %"$match@143928" = load i8, i8* %"match@1439", align 1
  %EQ29 = icmp eq i8 %"$match@143928", 45
  br i1 %EQ29, label %elif32.then, label %elif33

elif32.then:                                      ; preds = %elif32
  %t.kind30 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 3, i32* %t.kind30, align 4
  br label %endif2

elif33:                                           ; preds = %elif32
  %"$match@143931" = load i8, i8* %"match@1439", align 1
  %EQ32 = icmp eq i8 %"$match@143931", 42
  br i1 %EQ32, label %elif33.then, label %elif34

elif33.then:                                      ; preds = %elif33
  %t.kind33 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 4, i32* %t.kind33, align 4
  br label %endif2

elif34:                                           ; preds = %elif33
  %"$match@143934" = load i8, i8* %"match@1439", align 1
  %EQ35 = icmp eq i8 %"$match@143934", 47
  br i1 %EQ35, label %elif34.then, label %elif35

elif34.then:                                      ; preds = %elif34
  %t.kind36 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 5, i32* %t.kind36, align 4
  br label %endif2

elif35:                                           ; preds = %elif34
  %"$match@143937" = load i8, i8* %"match@1439", align 1
  %EQ38 = icmp eq i8 %"$match@143937", 40
  br i1 %EQ38, label %elif35.then, label %elif36

elif35.then:                                      ; preds = %elif35
  %t.kind39 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 6, i32* %t.kind39, align 4
  br label %endif2

elif36:                                           ; preds = %elif35
  %"$match@143940" = load i8, i8* %"match@1439", align 1
  %EQ41 = icmp eq i8 %"$match@143940", 41
  br i1 %EQ41, label %elif36.then, label %endif2

elif36.then:                                      ; preds = %elif36
  %t.kind42 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 7, i32* %t.kind42, align 4
  br label %endif2

endif2:                                           ; preds = %elif36.then, %elif36, %elif35.then, %elif34.then, %elif33.then, %elif32.then, %if31.then
  %"$t43" = load %Token, %Token* %t, align 4
  call void @"Array<Token>.append"(%"Array<Token>"* %toks, %Token %"$t43")
  %"$i44" = load i32, i32* %i, align 4
  %ADD45 = add i32 %"$i44", 1
  store i32 %ADD45, i32* %i, align 4
  br label %while27

endloop3:                                         ; preds = %while27
  %"$toks" = load %"Array<Token>", %"Array<Token>"* %toks, align 8
  %4 = bitcast %String* %src to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  ret %"Array<Token>" %"$toks"
}

define %String @sign(i32 %0) {
entry:
  %kind = alloca i32, align 4
  %"match@1786" = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  %"$kind" = load i32, i32* %kind, align 4
  store i32 %"$kind", i32* %"match@1786", align 4
  br label %if37

if37:                                             ; preds = %entry
  %"$match@1786" = load i32, i32* %"match@1786", align 4
  %EQ = icmp eq i32 %"$match@1786", 2
  br i1 %EQ, label %if37.then, label %elif38

if37.then:                                        ; preds = %if37
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 1, i32 0 }

elif38:                                           ; preds = %if37
  %"$match@17861" = load i32, i32* %"match@1786", align 4
  %EQ2 = icmp eq i32 %"$match@17861", 3
  br i1 %EQ2, label %elif38.then, label %elif39

elif38.then:                                      ; preds = %elif38
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 1, i32 0 }

elif39:                                           ; preds = %elif38
  %"$match@17863" = load i32, i32* %"match@1786", align 4
  %EQ4 = icmp eq i32 %"$match@17863", 4
  br i1 %EQ4, label %elif39.then, label %elif40

elif39.then:                                      ; preds = %elif39
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 1, i32 0 }

elif40:                                           ; preds = %elif39
  %"$match@17865" = load i32, i32* %"match@1786", align 4
  %EQ6 = icmp eq i32 %"$match@17865", 5
  br i1 %EQ6, label %elif40.then, label %endif

elif40.then:                                      ; preds = %elif40
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 1, i32 0 }

endif:                                            ; preds = %elif40
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 1, i32 0 }
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define void @"copy<Node>"(%Node* %0, %Node* %1, i32 %2) {
entry:
  %dst = alloca %Node*, align 8
  %src = alloca %Node*, align 8
  %count = alloca i32, align 4
  store %Node* %0, %Node** %dst, align 8
  store %Node* %1, %Node** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Node*, %Node** %dst, align 8
  %AS = bitcast %Node* %"$dst" to i8*
  %"$src" = load %Node*, %Node** %src, align 8
  %AS1 = bitcast %Node* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Node>"(%Node* %0) {
entry:
  %data = alloca %Node*, align 8
  store %Node* %0, %Node** %data, align 8
  %"$data" = load %Node*, %Node** %data, align 8
  %AS = bitcast %Node* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Node>.reserve"(%"Array<Node>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Node*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if41

if41:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if41.then, label %endif

if41.then:                                        ; preds = %if41
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if41.then, %if41
  br label %if42

if42:                                             ; preds = %endif
  %"*self6" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if42.then, label %endif1

if42.then:                                        ; preds = %if42
  ret void

endif1:                                           ; preds = %if42
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 %"$cap8")
  store %Node* %"call.new<Node>", %Node** %bigger, align 8
  %"*self9" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Node*, %Node** %bigger, align 8
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Node>"(%Node* %"$bigger", %Node* %"$*self.data", i32 %"$*self.count12")
  br label %if43

if43:                                             ; preds = %endif1
  %"*self13" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if43.then, label %endif2

if43.then:                                        ; preds = %if43
  %"*self16" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Node*, %Node** %"*self.data17", align 8
  call void @"free<Node>"(%Node* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if43.then, %if43
  %"*self19" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Node*, %Node** %bigger, align 8
  store %Node* %"$bigger21", %Node** %"*self.data20", align 8
  %"*self22" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Node>.own"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  br label %if44

if44:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if44.then, label %endif

if44.then:                                        ; preds = %if44
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Node>.reserve"(%"Array<Node>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if44.then, %if44
  ret void
}

define void @"Array<Node>.clear"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  br label %if45

if45:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if45.then, label %endif

if45.then:                                        ; preds = %if45
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if45.then, %if45
  br label %while46

while46:                                          ; preds = %while46.then, %endif
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while46.then, label %endloop

while46.then:                                     ; preds = %while46
  %"*self4" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Node, %Node* %"$*self.data", i32 %"$*self.count10"
  store %Node zeroinitializer, %Node* %"*self.data[]", align 4
  br label %while46

endloop:                                          ; preds = %while46
  ret void
}

define void @"Array<Node>.append"(%"Array<Node>"* %0, %Node %1) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  %value = alloca %Node, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  store %Node %1, %Node* %value, align 4
  br label %if47

if47:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if47.then, label %endif

if47.then:                                        ; preds = %if47
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Node>.reserve"(%"Array<Node>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if47.then, %if47
  %"*self6" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Node, %Node* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Node, %Node* %value, align 4
  store %Node %"$value", %Node* %"*self.data[]", align 4
  %"*self10" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Node>.len"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Node* @"Array<Node>.[]"(%"Array<Node>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Node>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Node>"* %0, %"Array<Node>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Node>"*, %"Array<Node>"** %left, align 8
  call void @"Array<Node>.own"(%"Array<Node>"* %"*left")
  %"*left1" = load %"Array<Node>"*, %"Array<Node>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Node*, %Node** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Node, %Node* %"$*left.data", i32 %"$index"
  ret %Node* %"*left.data[]"
}

define void @"Array<Node>.drop"(%"Array<Node>"* %0) {
entry:
  %value = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %value, align 8
  %"*value" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  call void @"Array<Node>.clear"(%"Array<Node>"* %"*value")
  br label %if48

if48:                                             ; preds = %entry
  %"*value1" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if48.then, label %endif

if48.then:                                        ; preds = %if48
  %"*value2" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Node*, %Node** %"*value.data", align 8
  call void @"free<Node>"(%Node* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if48.then, %if48
  ret void
}

define i32 @Parser.peek(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %ref = alloca %Token*, align 8
  store %Parser* %0, %Parser** %self, align 8
  br label %if49

if49:                                             ; preds = %entry
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 1
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 0
  %"call.Array<Token>.len" = call i32 @"Array<Token>.len"(%"Array<Token>"* %"*self.toks")
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %LT = icmp slt i32 %"$*self.pos", %"call.Array<Token>.len"
  br i1 %LT, label %if49.then, label %endif

if49.then:                                        ; preds = %if49
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %"*self.toks3" = getelementptr inbounds %Parser, %Parser* %"*self2", i32 0, i32 0
  %"*self4" = load %Parser*, %Parser** %self, align 8
  %"*self.pos5" = getelementptr inbounds %Parser, %Parser* %"*self4", i32 0, i32 1
  %"$*self.pos6" = load i32, i32* %"*self.pos5", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks3", i32 %"$*self.pos6")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"*ref.kind" = getelementptr inbounds %Token, %Token* %"*ref", i32 0, i32 0
  %"$*ref.kind" = load i32, i32* %"*ref.kind", align 4
  ret i32 %"$*ref.kind"

endif:                                            ; preds = %if49
  ret i32 0
}

define i32 @Parser.add(%Parser* %0, %Token %1, i32 %2, i32 %3) {
entry:
  %self = alloca %Parser*, align 8
  %token = alloca %Token, align 8
  %left = alloca i32, align 4
  %right = alloca i32, align 4
  %n = alloca %Node, align 8
  store %Parser* %0, %Parser** %self, align 8
  store %Token %1, %Token* %token, align 4
  store i32 %2, i32* %left, align 4
  store i32 %3, i32* %right, align 4
  store %Node zeroinitializer, %Node* %n, align 4
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %"$token" = load %Token, %Token* %token, align 4
  store %Token %"$token", %Token* %n.token, align 4
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$left" = load i32, i32* %left, align 4
  store i32 %"$left", i32* %n.left, align 4
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$right" = load i32, i32* %right, align 4
  store i32 %"$right", i32* %n.right, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$n" = load %Node, %Node* %n, align 4
  call void @"Array<Node>.append"(%"Array<Node>"* %"*self.nodes", %Node %"$n")
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes2" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 2
  %"call.Array<Node>.len" = call i32 @"Array<Node>.len"(%"Array<Node>"* %"*self.nodes2")
  %SUB = sub i32 %"call.Array<Node>.len", 1
  ret i32 %SUB
}

define i32 @Parser.factor(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %inner = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 0
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %"*self.pos3" = getelementptr inbounds %Parser, %Parser* %"*self2", i32 0, i32 1
  %"$*self.pos4" = load i32, i32* %"*self.pos3", align 4
  %ADD = add i32 %"$*self.pos4", 1
  store i32 %ADD, i32* %"*self.pos3", align 4
  br label %if50

if50:                                             ; preds = %entry
  %t.kind = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  %"$t.kind" = load i32, i32* %t.kind, align 4
  %EQ = icmp eq i32 %"$t.kind", 6
  br i1 %EQ, label %if50.then, label %endif

if50.then:                                        ; preds = %if50
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %call.Parser.expr = call i32 @Parser.expr(%Parser* %"*self5")
  store i32 %call.Parser.expr, i32* %inner, align 4
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos7" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos8" = load i32, i32* %"*self.pos7", align 4
  %ADD9 = add i32 %"$*self.pos8", 1
  store i32 %ADD9, i32* %"*self.pos7", align 4
  %"$inner" = load i32, i32* %inner, align 4
  ret i32 %"$inner"

endif:                                            ; preds = %if50
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self10", %Token %"$t", i32 -1, i32 -1)
  ret i32 %call.Parser.add
}

define i32 @Parser.term(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %left = alloca i32, align 4
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %right = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %call.Parser.factor = call i32 @Parser.factor(%Parser* %"*self")
  store i32 %call.Parser.factor, i32* %left, align 4
  br label %while51

while51:                                          ; preds = %while51.then, %entry
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek = call i32 @Parser.peek(%Parser* %"*self1")
  %EQ = icmp eq i32 %call.Parser.peek, 4
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek3 = call i32 @Parser.peek(%Parser* %"*self2")
  %EQ4 = icmp eq i32 %call.Parser.peek3, 5
  %OR = or i1 %EQ, %EQ4
  br i1 %OR, label %while51.then, label %endloop

while51.then:                                     ; preds = %while51
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self5", i32 0, i32 0
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self7" = load %Parser*, %Parser** %self, align 8
  %"*self.pos8" = getelementptr inbounds %Parser, %Parser* %"*self7", i32 0, i32 1
  %"$*self.pos9" = load i32, i32* %"*self.pos8", align 4
  %ADD = add i32 %"$*self.pos9", 1
  store i32 %ADD, i32* %"*self.pos8", align 4
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %call.Parser.factor11 = call i32 @Parser.factor(%Parser* %"*self10")
  store i32 %call.Parser.factor11, i32* %right, align 4
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %"$left" = load i32, i32* %left, align 4
  %"$right" = load i32, i32* %right, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self12", %Token %"$t", i32 %"$left", i32 %"$right")
  store i32 %call.Parser.add, i32* %left, align 4
  br label %while51

endloop:                                          ; preds = %while51
  %"$left13" = load i32, i32* %left, align 4
  ret i32 %"$left13"
}

define i32 @Parser.expr(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %left = alloca i32, align 4
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %right = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %call.Parser.term = call i32 @Parser.term(%Parser* %"*self")
  store i32 %call.Parser.term, i32* %left, align 4
  br label %while52

while52:                                          ; preds = %while52.then, %entry
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek = call i32 @Parser.peek(%Parser* %"*self1")
  %EQ = icmp eq i32 %call.Parser.peek, 2
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek3 = call i32 @Parser.peek(%Parser* %"*self2")
  %EQ4 = icmp eq i32 %call.Parser.peek3, 3
  %OR = or i1 %EQ, %EQ4
  br i1 %OR, label %while52.then, label %endloop

while52.then:                                     ; preds = %while52
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self5", i32 0, i32 0
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self7" = load %Parser*, %Parser** %self, align 8
  %"*self.pos8" = getelementptr inbounds %Parser, %Parser* %"*self7", i32 0, i32 1
  %"$*self.pos9" = load i32, i32* %"*self.pos8", align 4
  %ADD = add i32 %"$*self.pos9", 1
  store i32 %ADD, i32* %"*self.pos8", align 4
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %call.Parser.term11 = call i32 @Parser.term(%Parser* %"*self10")
  store i32 %call.Parser.term11, i32* %right, align 4
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %"$left" = load i32, i32* %left, align 4
  %"$right" = load i32, i32* %right, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self12", %Token %"$t", i32 %"$left", i32 %"$right")
  store i32 %call.Parser.add, i32* %left, align 4
  br label %while52

endloop:                                          ; preds = %while52
  %"$left13" = load i32, i32* %left, align 4
  ret i32 %"$left13"
}

define %String @Parser.label(%Parser* %0, i32 %1) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$i" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  br label %if53

if53:                                             ; preds = %entry
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind = getelementptr inbounds %Token, %Token* %n.token, i32 0, i32 0
  %"$n.token.kind" = load i32, i32* %n.token.kind, align 4
  %EQ = icmp eq i32 %"$n.token.kind", 1
  br i1 %EQ, label %if53.then, label %endif

if53.then:                                        ; preds = %if53
  %n.token1 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.value = getelementptr inbounds %Token, %Token* %n.token1, i32 0, i32 1
  %"$n.token.value" = load i32, i32* %n.token.value, align 4
  %call.String.create = call %String @String.create(i32 %"$n.token.value")
  ret %String %call.String.create

endif:                                            ; preds = %if53
  %n.token2 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind3 = getelementptr inbounds %Token, %Token* %n.token2, i32 0, i32 0
  %"$n.token.kind4" = load i32, i32* %n.token.kind3, align 4
  %call.sign = call %String @sign(i32 %"$n.token.kind4")
  store %String %call.sign, %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %3 = insertvalue %String %2, i32 %"$format.size9", 1
  %4 = insertvalue %String %3, i32 %ADD7, 2
  %5 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  ret %String %4
}

define void @Parser.show(%Parser* %0, i32 %1, %String %2, i1 %3) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %pre = alloca %String, align 8
  %last = alloca i1, align 1
  %branch = alloca %String, align 8
  %more = alloca %String, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %kid = alloca %String, align 8
  %copy = alloca %String, align 8
  %copy20 = alloca %String, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  store %String %2, %String* %pre, align 8
  store i1 %3, i1* %last, align 1
  store %String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 7, i32 0 }, %String* %branch, align 8
  store %String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i32 5, i32 0 }, %String* %more, align 8
  br label %if54

if54:                                             ; preds = %entry
  %"$last" = load i1, i1* %last, align 1
  br i1 %"$last", label %if54.then, label %endif

if54.then:                                        ; preds = %if54
  %4 = bitcast %String* %branch to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  store %String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.8, i32 0, i32 0), i32 7, i32 0 }, %String* %branch, align 8
  %5 = bitcast %String* %more to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 3, i32 0 }, %String* %more, align 8
  br label %endif

endif:                                            ; preds = %if54.then, %if54
  %call.String.chars = call i8* @String.chars(%String* %pre)
  %call.String.chars3 = call i8* @String.chars(%String* %branch)
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"$i" = load i32, i32* %i, align 4
  %call.Parser.label = call %String @Parser.label(%Parser* %"*self", i32 %"$i")
  store %String %call.Parser.label, %String* %tmp, align 8
  %call.String.chars4 = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars3, i8* %call.String.chars4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars3, i8* %call.String.chars4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %7 = insertvalue %String %6, i32 %"$format.size9", 1
  %8 = insertvalue %String %7, i32 %ADD7, 2
  call void @"io::out"(%String %8)
  %9 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %9)
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self10", i32 0, i32 2
  %"$i11" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i11")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  %"call.String.+" = call %String @"String.+"(%String* %pre, %String* %more)
  store %String %"call.String.+", %String* %kid, align 8
  br label %if55

if55:                                             ; preds = %endif
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %GE = icmp sge i32 %"$n.left", 0
  br i1 %GE, label %if55.then, label %endif1

if55.then:                                        ; preds = %if55
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %n.left13 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  store %String zeroinitializer, %String* %copy, align 8
  %10 = bitcast %String* %copy to %"Array<i8>"*
  %11 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %10, %"Array<i8>"* %11)
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %LT = icmp slt i32 %"$n.right", 0
  %"$n.left14" = load i32, i32* %n.left13, align 4
  %"$copy" = load %String, %String* %copy, align 8
  call void @Parser.show(%Parser* %"*self12", i32 %"$n.left14", %String %"$copy", i1 %LT)
  br label %endif1

endif1:                                           ; preds = %if55.then, %if55
  br label %if56

if56:                                             ; preds = %endif1
  %n.right15 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right16" = load i32, i32* %n.right15, align 4
  %GE17 = icmp sge i32 %"$n.right16", 0
  br i1 %GE17, label %if56.then, label %endif2

if56.then:                                        ; preds = %if56
  %"*self18" = load %Parser*, %Parser** %self, align 8
  %n.right19 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  store %String zeroinitializer, %String* %copy20, align 8
  %12 = bitcast %String* %copy20 to %"Array<i8>"*
  %13 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %12, %"Array<i8>"* %13)
  %"$n.right21" = load i32, i32* %n.right19, align 4
  %"$copy22" = load %String, %String* %copy20, align 8
  call void @Parser.show(%Parser* %"*self18", i32 %"$n.right21", %String %"$copy22", i1 true)
  br label %endif2

endif2:                                           ; preds = %if56.then, %if56
  %14 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %14)
  %15 = bitcast %String* %more to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %15)
  %16 = bitcast %String* %branch to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %16)
  %17 = bitcast %String* %pre to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %17)
  ret void
}

define i32 @Parser.eval(%Parser* %0, i32 %1) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %"match@3864" = alloca i32, align 4
  %d = alloca i32, align 4
  %error.at = alloca %Error*, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$i" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind = getelementptr inbounds %Token, %Token* %n.token, i32 0, i32 0
  %"$n.token.kind" = load i32, i32* %n.token.kind, align 4
  store i32 %"$n.token.kind", i32* %"match@3864", align 4
  br label %if57

if57:                                             ; preds = %entry
  %"$match@3864" = load i32, i32* %"match@3864", align 4
  %EQ = icmp eq i32 %"$match@3864", 1
  br i1 %EQ, label %if57.then, label %elif58

if57.then:                                        ; preds = %if57
  %n.token9 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.value = getelementptr inbounds %Token, %Token* %n.token9, i32 0, i32 1
  %"$n.token.value" = load i32, i32* %n.token.value, align 4
  ret i32 %"$n.token.value"

elif58:                                           ; preds = %if57
  %"$match@386410" = load i32, i32* %"match@3864", align 4
  %EQ11 = icmp eq i32 %"$match@386410", 2
  br i1 %EQ11, label %elif58.then, label %elif61

elif58.then:                                      ; preds = %elif58
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %call.Parser.eval = call i32 @Parser.eval(%Parser* %"*self12", i32 %"$n.left")
  br label %thrown59

thrown59:                                         ; preds = %elif58.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown59.then, label %endthrown

thrown59.then:                                    ; preds = %thrown59
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown59
  %"*self13" = load %Parser*, %Parser** %self, align 8
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %call.Parser.eval14 = call i32 @Parser.eval(%Parser* %"*self13", i32 %"$n.right")
  br label %thrown60

thrown60:                                         ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown60.then, label %endthrown1

thrown60.then:                                    ; preds = %thrown60
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown1:                                       ; preds = %thrown60
  %ADD = add i32 %call.Parser.eval, %call.Parser.eval14
  ret i32 %ADD

elif61:                                           ; preds = %elif58
  %"$match@386416" = load i32, i32* %"match@3864", align 4
  %EQ17 = icmp eq i32 %"$match@386416", 3
  br i1 %EQ17, label %elif61.then, label %elif64

elif61.then:                                      ; preds = %elif61
  %"*self18" = load %Parser*, %Parser** %self, align 8
  %n.left19 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left20" = load i32, i32* %n.left19, align 4
  %call.Parser.eval21 = call i32 @Parser.eval(%Parser* %"*self18", i32 %"$n.left20")
  br label %thrown62

thrown62:                                         ; preds = %elif61.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown62.then, label %endthrown2

thrown62.then:                                    ; preds = %thrown62
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown62
  %"*self23" = load %Parser*, %Parser** %self, align 8
  %n.right24 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right25" = load i32, i32* %n.right24, align 4
  %call.Parser.eval26 = call i32 @Parser.eval(%Parser* %"*self23", i32 %"$n.right25")
  br label %thrown63

thrown63:                                         ; preds = %endthrown2
  %"$ucg.thrown27" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown27", label %thrown63.then, label %endthrown3

thrown63.then:                                    ; preds = %thrown63
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown3:                                       ; preds = %thrown63
  %SUB = sub i32 %call.Parser.eval21, %call.Parser.eval26
  ret i32 %SUB

elif64:                                           ; preds = %elif61
  %"$match@386428" = load i32, i32* %"match@3864", align 4
  %EQ29 = icmp eq i32 %"$match@386428", 4
  br i1 %EQ29, label %elif64.then, label %elif67

elif64.then:                                      ; preds = %elif64
  %"*self30" = load %Parser*, %Parser** %self, align 8
  %n.left31 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left32" = load i32, i32* %n.left31, align 4
  %call.Parser.eval33 = call i32 @Parser.eval(%Parser* %"*self30", i32 %"$n.left32")
  br label %thrown65

thrown65:                                         ; preds = %elif64.then
  %"$ucg.thrown34" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown34", label %thrown65.then, label %endthrown4

thrown65.then:                                    ; preds = %thrown65
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown4:                                       ; preds = %thrown65
  %"*self35" = load %Parser*, %Parser** %self, align 8
  %n.right36 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right37" = load i32, i32* %n.right36, align 4
  %call.Parser.eval38 = call i32 @Parser.eval(%Parser* %"*self35", i32 %"$n.right37")
  br label %thrown66

thrown66:                                         ; preds = %endthrown4
  %"$ucg.thrown39" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown39", label %thrown66.then, label %endthrown5

thrown66.then:                                    ; preds = %thrown66
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown5:                                       ; preds = %thrown66
  %MUL = mul i32 %call.Parser.eval33, %call.Parser.eval38
  ret i32 %MUL

elif67:                                           ; preds = %elif64
  %"$match@386440" = load i32, i32* %"match@3864", align 4
  %EQ41 = icmp eq i32 %"$match@386440", 5
  br i1 %EQ41, label %elif67.then, label %endif8

elif67.then:                                      ; preds = %elif67
  %"*self42" = load %Parser*, %Parser** %self, align 8
  %n.right43 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right44" = load i32, i32* %n.right43, align 4
  %call.Parser.eval45 = call i32 @Parser.eval(%Parser* %"*self42", i32 %"$n.right44")
  br label %thrown68

thrown68:                                         ; preds = %elif67.then
  %"$ucg.thrown46" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown46", label %thrown68.then, label %endthrown6

thrown68.then:                                    ; preds = %thrown68
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown6:                                       ; preds = %thrown68
  store i32 %call.Parser.eval45, i32* %d, align 4
  br label %if69

if69:                                             ; preds = %endthrown6
  %"$d" = load i32, i32* %d, align 4
  %EQ47 = icmp eq i32 %"$d", 0
  br i1 %EQ47, label %if69.then, label %endif

if69.then:                                        ; preds = %if69
  %call.Error.create = call %Error @Error.create(%String { i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.11, i32 0, i32 0), i32 16, i32 0 })
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endif:                                            ; preds = %if69
  %"*self48" = load %Parser*, %Parser** %self, align 8
  %n.left49 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left50" = load i32, i32* %n.left49, align 4
  %call.Parser.eval51 = call i32 @Parser.eval(%Parser* %"*self48", i32 %"$n.left50")
  br label %thrown70

thrown70:                                         ; preds = %endif
  %"$ucg.thrown52" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown52", label %thrown70.then, label %endthrown7

thrown70.then:                                    ; preds = %thrown70
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown7:                                       ; preds = %thrown70
  %"$d53" = load i32, i32* %d, align 4
  %DIV = sdiv i32 %call.Parser.eval51, %"$d53"
  ret i32 %DIV

endif8:                                           ; preds = %elif67
  ret i32 0
}

define void @calc(%String %0) {
entry:
  %text = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %p = alloca %Parser, align 8
  %copy = alloca %String, align 8
  %root = alloca i32, align 4
  %tmp = alloca %String, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.12, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.12, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  call void @"io::out"(%String %3)
  store %Parser zeroinitializer, %Parser* %p, align 8
  %p.toks = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  %4 = bitcast %String* %copy to %"Array<i8>"*
  %5 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %4, %"Array<i8>"* %5)
  %"$copy" = load %String, %String* %copy, align 8
  %call.lex = call %"Array<Token>" @lex(%String %"$copy")
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks)
  store %"Array<Token>" %call.lex, %"Array<Token>"* %p.toks, align 8
  %call.Parser.expr = call i32 @Parser.expr(%Parser* %p)
  store i32 %call.Parser.expr, i32* %root, align 4
  %"$root" = load i32, i32* %root, align 4
  %call.Parser.label = call %String @Parser.label(%Parser* %p, i32 %"$root")
  store %String %call.Parser.label, %String* %tmp, align 8
  %call.String.chars7 = call i8* @String.chars(%String* %tmp)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars7)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars7)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %7 = insertvalue %String %6, i32 %"$format.size20", 1
  %8 = insertvalue %String %7, i32 %ADD18, 2
  call void @"io::out"(%String %8)
  %9 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %9)
  %p.nodes = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  %"$root21" = load i32, i32* %root, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %p.nodes, i32 %"$root21")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  br label %if71

if71:                                             ; preds = %entry
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %GE = icmp sge i32 %"$n.left", 0
  br i1 %GE, label %if71.then, label %endif

if71.then:                                        ; preds = %if71
  %n.left22 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %LT = icmp slt i32 %"$n.right", 0
  %"$n.left23" = load i32, i32* %n.left22, align 4
  call void @Parser.show(%Parser* %p, i32 %"$n.left23", %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.14, i32 0, i32 0), i32 0, i32 0 }, i1 %LT)
  br label %endif

endif:                                            ; preds = %if71.then, %if71
  br label %if72

if72:                                             ; preds = %endif
  %n.right24 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right25" = load i32, i32* %n.right24, align 4
  %GE26 = icmp sge i32 %"$n.right25", 0
  br i1 %GE26, label %if72.then, label %endif1

if72.then:                                        ; preds = %if72
  %n.right27 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right28" = load i32, i32* %n.right27, align 4
  call void @Parser.show(%Parser* %p, i32 %"$n.right28", %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.15, i32 0, i32 0), i32 0, i32 0 }, i1 true)
  br label %endif1

endif1:                                           ; preds = %if72.then, %if72
  %"$root29" = load i32, i32* %root, align 4
  %call.Parser.eval = call i32 @Parser.eval(%Parser* %p, i32 %"$root29")
  br label %thrown73

thrown73:                                         ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown73.then, label %endthrown

thrown73.then:                                    ; preds = %thrown73
  store i1 false, i1* @ucg.thrown, align 1
  %p.nodes30 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  call void @"Array<Node>.drop"(%"Array<Node>"* %p.nodes30)
  %p.toks31 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks31)
  %10 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %10)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown73
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.16, i32 0, i32 0), i32 %call.Parser.eval)
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.16, i32 0, i32 0), i32 %call.Parser.eval)
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %11 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %12 = insertvalue %String %11, i32 %"$format.size44", 1
  %13 = insertvalue %String %12, i32 %ADD42, 2
  call void @"io::out"(%String %13)
  %p.nodes45 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  call void @"Array<Node>.drop"(%"Array<Node>"* %p.nodes45)
  %p.toks46 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks46)
  %14 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %14)
  ret void
}

define i32 @main() {
entry:
  %line = alloca %String, align 8
  %copy = alloca %String, align 8
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %error.at16 = alloca %Error*, align 8
  br label %try74

try74:                                            ; preds = %entry
  br i1 true, label %try74.then, label %endtry1

try74.then:                                       ; preds = %try74
  call void @"io::out"(%String { i8* getelementptr inbounds ([51 x i8], [51 x i8]* @str.17, i32 0, i32 0), i32 50, i32 0 })
  call void @"io::out"(%String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.18, i32 0, i32 0), i32 2, i32 0 })
  %"call.io::input" = call %String @"io::input"()
  store %String %"call.io::input", %String* %line, align 8
  br label %while75

while75:                                          ; preds = %endrethrow, %try74.then
  %call.String.is_empty = call i1 @String.is_empty(%String* %line)
  %NOT = xor i1 %call.String.is_empty, true
  br i1 %NOT, label %while75.then, label %endloop

while75.then:                                     ; preds = %while75
  br label %try76

try76:                                            ; preds = %while75.then
  br i1 true, label %try76.then, label %endtry

try76.then:                                       ; preds = %try76
  store %String zeroinitializer, %String* %copy, align 8
  %0 = bitcast %String* %copy to %"Array<i8>"*
  %1 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1)
  %"$copy" = load %String, %String* %copy, align 8
  call void @calc(%String %"$copy")
  br label %thrown77

thrown77:                                         ; preds = %try76.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown77.then, label %endthrown

thrown77.then:                                    ; preds = %thrown77
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown77
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown77.then, %try76
  br label %catch78

catch78:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown3", %EQ
  br i1 %AND, label %catch78.then, label %endcatch

catch78.then:                                     ; preds = %catch78
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %call.String.chars = call i8* @String.chars(%String* %e.message)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %3 = insertvalue %String %2, i32 %"$format.size8", 1
  %4 = insertvalue %String %3, i32 %ADD6, 2
  call void @"io::out"(%String %4)
  %e.message9 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %5 = bitcast %String* %e.message9 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  br label %endcatch

endcatch:                                         ; preds = %catch78.then, %catch78
  br label %rethrow79

rethrow79:                                        ; preds = %endcatch
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %rethrow79.then, label %endrethrow

rethrow79.then:                                   ; preds = %rethrow79
  store i1 false, i1* @ucg.thrown, align 1
  %6 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow79
  call void @"io::out"(%String { i8* getelementptr inbounds ([42 x i8], [42 x i8]* @str.20, i32 0, i32 0), i32 41, i32 0 })
  call void @"io::out"(%String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.21, i32 0, i32 0), i32 2, i32 0 })
  %"call.io::input11" = call %String @"io::input"()
  %7 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  store %String %"call.io::input11", %String* %line, align 8
  br label %while75

endloop:                                          ; preds = %while75
  %8 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %8)
  ret i32 0

endtry1:                                          ; preds = %rethrow79.then, %try74
  br label %catch80

catch80:                                          ; preds = %endtry1
  %"$ucg.error_type12" = load i32, i32* @ucg.error_type, align 4
  %EQ13 = icmp eq i32 %"$ucg.error_type12", 1
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  %AND15 = and i1 %"$ucg.thrown14", %EQ13
  br i1 %AND15, label %catch80.then, label %endcatch2

catch80.then:                                     ; preds = %catch80
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at16, align 8
  %"*error.at17" = load %Error*, %Error** %error.at16, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at17")
  ret i32 %call.uncaught

endcatch2:                                        ; preds = %catch80
  br label %if81

if81:                                             ; preds = %endcatch2
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown18", label %if81.then, label %endif

if81.then:                                        ; preds = %if81
  ret i32 1

endif:                                            ; preds = %if81
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 027 — project: net server

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Range = type { i32, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }
%Pollfd = type { i32, i16, i16 }
%"Array<Pollfd>" = type { %Pollfd*, i32, i32 }

@POLLIN = global i32 0
@PORT = global i32 0
@str = private unnamed_addr constant [36 x i8] c"[server] can't listen on port 8080\0A\00", align 1
@str.2 = private unnamed_addr constant [33 x i8] c"[server] listening on port 8080\0A\00", align 1
@str.3 = private unnamed_addr constant [27 x i8] c"[server] client connected\0A\00", align 1
@str.4 = private unnamed_addr constant [30 x i8] c"[server] client disconnected\0A\00", align 1
@str.5 = private unnamed_addr constant [10 x i8] c"[client] \00", align 1

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

define void @"Array<i8>.resize"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %size = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self", i32 %"$size")
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %SUB = sub i32 %"$*self.count5", 1
  store i32 %SUB, i32* %"*self.count4", align 4
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while3

endloop:                                          ; preds = %while3
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
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
  br label %if11

if11:                                             ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
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
  br label %if12

if12:                                             ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  ret i1 false

endif:                                            ; preds = %if12
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
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

declare i32 @socket(i32, i32, i32)

declare i32 @bind(i32, i8*, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, i8*, i8*)

declare i32 @setsockopt(i32, i32, i32, i8*, i32)

declare i32 @poll(i8*, i32, i32)

declare i32 @close(i32)

define %"Array<i8>" @address(i32 %0) {
entry:
  %port = alloca i32, align 4
  %data = alloca %"Array<i8>", align 8
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i3 = alloca i32, align 4
  store i32 %0, i32* %port, align 4
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %data, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 2)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  %"$port" = load i32, i32* %port, align 4
  %RSHIFT = ashr i32 %"$port", 8
  %AS = trunc i32 %RSHIFT to i8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 %AS)
  %"$port1" = load i32, i32* %port, align 4
  %BAND = and i32 %"$port1", 255
  %AS2 = trunc i32 %BAND to i8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 %AS2)
  %call.Range.create = call %Range @Range.create(i32 0, i32 12)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i3, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i3)
  br i1 %call.Range.in, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  br label %while13

endloop:                                          ; preds = %while13
  %"$data" = load %"Array<i8>", %"Array<i8>"* %data, align 8
  ret %"Array<i8>" %"$data"
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
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %if15

if15:                                             ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret void

endif1:                                           ; preds = %if15
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
  br label %if16

if16:                                             ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if16.then, label %endif2

if16.then:                                        ; preds = %if16
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if16.then, %if16
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

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  br label %while18

while18:                                          ; preds = %while18.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
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
  br label %while18

endloop:                                          ; preds = %while18
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

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %value = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %value, align 8
  %"*value" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*value")
  br label %if20

if20:                                             ; preds = %entry
  %"*value1" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*value2" = load %"Array<i32>"*, %"Array<i32>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<i32>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  ret void
}

define i32 @listener(i32 %0) {
entry:
  %port = alloca i32, align 4
  %fd = alloca i32, align 4
  %one = alloca %"Array<i32>", align 8
  %addr = alloca %"Array<i8>", align 8
  %is_bound = alloca i1, align 1
  store i32 %0, i32* %port, align 4
  %call.socket = call i32 @socket(i32 2, i32 1, i32 0)
  store i32 %call.socket, i32* %fd, align 4
  br label %if21

if21:                                             ; preds = %entry
  %"$fd" = load i32, i32* %fd, align 4
  %LT = icmp slt i32 %"$fd", 0
  br i1 %LT, label %if21.then, label %endif

if21.then:                                        ; preds = %if21
  ret i32 -1

endif:                                            ; preds = %if21
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %one, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %one, i32 1)
  %one.data = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %one, i32 0, i32 0
  %"$one.data" = load i32*, i32** %one.data, align 8
  %AS = bitcast i32* %"$one.data" to i8*
  %"$fd2" = load i32, i32* %fd, align 4
  %call.setsockopt = call i32 @setsockopt(i32 %"$fd2", i32 1, i32 2, i8* %AS, i32 4)
  %"$port" = load i32, i32* %port, align 4
  %call.address = call %"Array<i8>" @address(i32 %"$port")
  store %"Array<i8>" %call.address, %"Array<i8>"* %addr, align 8
  %addr.data = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %addr, i32 0, i32 0
  %"$fd3" = load i32, i32* %fd, align 4
  %"$addr.data" = load i8*, i8** %addr.data, align 8
  %call.bind = call i32 @bind(i32 %"$fd3", i8* %"$addr.data", i32 16)
  %EQ = icmp eq i32 %call.bind, 0
  store i1 %EQ, i1* %is_bound, align 1
  br label %if22

if22:                                             ; preds = %endif
  %"$is_bound" = load i1, i1* %is_bound, align 1
  %NOT = xor i1 %"$is_bound", true
  %"$fd4" = load i32, i32* %fd, align 4
  %call.listen = call i32 @listen(i32 %"$fd4", i32 16)
  %LT5 = icmp slt i32 %call.listen, 0
  %OR = or i1 %NOT, %LT5
  br i1 %OR, label %if22.then, label %endif1

if22.then:                                        ; preds = %if22
  %"$fd6" = load i32, i32* %fd, align 4
  %call.close = call i32 @close(i32 %"$fd6")
  call void @"Array<i8>.drop"(%"Array<i8>"* %addr)
  call void @"Array<i32>.drop"(%"Array<i32>"* %one)
  ret i32 -1

endif1:                                           ; preds = %if22
  %"$fd7" = load i32, i32* %fd, align 4
  call void @"Array<i8>.drop"(%"Array<i8>"* %addr)
  call void @"Array<i32>.drop"(%"Array<i32>"* %one)
  ret i32 %"$fd7"
}

define %Pollfd* @"new<Pollfd>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Pollfd* getelementptr (%Pollfd, %Pollfd* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Pollfd*
  ret %Pollfd* %AS1
}

define void @"copy<Pollfd>"(%Pollfd* %0, %Pollfd* %1, i32 %2) {
entry:
  %dst = alloca %Pollfd*, align 8
  %src = alloca %Pollfd*, align 8
  %count = alloca i32, align 4
  store %Pollfd* %0, %Pollfd** %dst, align 8
  store %Pollfd* %1, %Pollfd** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Pollfd*, %Pollfd** %dst, align 8
  %AS = bitcast %Pollfd* %"$dst" to i8*
  %"$src" = load %Pollfd*, %Pollfd** %src, align 8
  %AS1 = bitcast %Pollfd* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Pollfd* getelementptr (%Pollfd, %Pollfd* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Pollfd>"(%Pollfd* %0) {
entry:
  %data = alloca %Pollfd*, align 8
  store %Pollfd* %0, %Pollfd** %data, align 8
  %"$data" = load %Pollfd*, %Pollfd** %data, align 8
  %AS = bitcast %Pollfd* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Pollfd*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  br label %if24

if24:                                             ; preds = %endif
  %"*self6" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if24.then, label %endif1

if24.then:                                        ; preds = %if24
  ret void

endif1:                                           ; preds = %if24
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Pollfd>" = call %Pollfd* @"new<Pollfd>"(i32 %"$cap8")
  store %Pollfd* %"call.new<Pollfd>", %Pollfd** %bigger, align 8
  %"*self9" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Pollfd*, %Pollfd** %bigger, align 8
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Pollfd>"(%Pollfd* %"$bigger", %Pollfd* %"$*self.data", i32 %"$*self.count12")
  br label %if25

if25:                                             ; preds = %endif1
  %"*self13" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if25.then, label %endif2

if25.then:                                        ; preds = %if25
  %"*self16" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Pollfd*, %Pollfd** %"*self.data17", align 8
  call void @"free<Pollfd>"(%Pollfd* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if25.then, %if25
  %"*self19" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Pollfd*, %Pollfd** %bigger, align 8
  store %Pollfd* %"$bigger21", %Pollfd** %"*self.data20", align 8
  %"*self22" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Pollfd>.own"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  br label %if26

if26:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  ret void
}

define void @"Array<Pollfd>.clear"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  br label %if27

if27:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if27.then, %if27
  br label %while28

while28:                                          ; preds = %while28.then, %endif
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while28.then, label %endloop

while28.then:                                     ; preds = %while28
  %"*self4" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data", i32 %"$*self.count10"
  store %Pollfd zeroinitializer, %Pollfd* %"*self.data[]", align 4
  br label %while28

endloop:                                          ; preds = %while28
  ret void
}

define void @"Array<Pollfd>.append"(%"Array<Pollfd>"* %0, %Pollfd %1) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  %value = alloca %Pollfd, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  store %Pollfd %1, %Pollfd* %value, align 4
  br label %if29

if29:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if29.then, label %endif

if29.then:                                        ; preds = %if29
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if29.then, %if29
  %"*self6" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Pollfd, %Pollfd* %value, align 4
  store %Pollfd %"$value", %Pollfd* %"*self.data[]", align 4
  %"*self10" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define %Pollfd @"Array<Pollfd>.pop"(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  %index = alloca i32, align 4
  %value = alloca %Pollfd, align 8
  %i = alloca i32, align 4
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store %Pollfd zeroinitializer, %Pollfd* %value, align 4
  br label %if30

if30:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if30.then, label %endif

if30.then:                                        ; preds = %if30
  %"$value" = load %Pollfd, %Pollfd* %value, align 4
  ret %Pollfd %"$value"

endif:                                            ; preds = %if30
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  call void @"Array<Pollfd>.own"(%"Array<Pollfd>"* %"*self2")
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data", i32 %"$index4"
  %"$*self.data[]" = load %Pollfd, %Pollfd* %"*self.data[]", align 4
  store %Pollfd %"$*self.data[]", %Pollfd* %value, align 4
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while31

while31:                                          ; preds = %while31.then, %endif
  %"*self6" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while31.then, label %endloop

while31.then:                                     ; preds = %while31
  %"*self10" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load %Pollfd*, %Pollfd** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data12", i32 %"$i13"
  %"*self15" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data16" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self15", i32 0, i32 0
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  %"$*self.data18" = load %Pollfd*, %Pollfd** %"*self.data16", align 8
  %"*self.data[]19" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data18", i32 %ADD
  %"$*self.data[]20" = load %Pollfd, %Pollfd* %"*self.data[]19", align 4
  store %Pollfd %"$*self.data[]20", %Pollfd* %"*self.data[]14", align 4
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while31

endloop:                                          ; preds = %while31
  %"*self23" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count24" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %SUB26 = sub i32 %"$*self.count25", 1
  store i32 %SUB26, i32* %"*self.count24", align 4
  %"*self27" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data28" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self27", i32 0, i32 0
  %"*self29" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count30" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self29", i32 0, i32 1
  %"$*self.data31" = load %Pollfd*, %Pollfd** %"*self.data28", align 8
  %"$*self.count32" = load i32, i32* %"*self.count30", align 4
  %"*self.data[]33" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data31", i32 %"$*self.count32"
  store %Pollfd zeroinitializer, %Pollfd* %"*self.data[]33", align 4
  %"$value34" = load %Pollfd, %Pollfd* %value, align 4
  ret %Pollfd %"$value34"
}

define %Pollfd @"Array<Pollfd>.pop.1"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<Pollfd>.pop" = call %Pollfd @"Array<Pollfd>.pop"(%"Array<Pollfd>"* %"*self", i32 %SUB)
  ret %Pollfd %"call.Array<Pollfd>.pop"
}

define i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Pollfd>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %left, align 8
  call void @"Array<Pollfd>.own"(%"Array<Pollfd>"* %"*left")
  %"*left1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Pollfd*, %Pollfd** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*left.data", i32 %"$index"
  ret %Pollfd* %"*left.data[]"
}

define void @"Array<Pollfd>.drop"(%"Array<Pollfd>"* %0) {
entry:
  %value = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %value, align 8
  %"*value" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  call void @"Array<Pollfd>.clear"(%"Array<Pollfd>"* %"*value")
  br label %if32

if32:                                             ; preds = %entry
  %"*value1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if32.then, label %endif

if32.then:                                        ; preds = %if32
  %"*value2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Pollfd*, %Pollfd** %"*value.data", align 8
  call void @"free<Pollfd>"(%Pollfd* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if32.then, %if32
  ret void
}

define void @watch(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %fds = alloca %"Array<Pollfd>"*, align 8
  %fd = alloca i32, align 4
  %p = alloca %Pollfd, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %fds, align 8
  store i32 %1, i32* %fd, align 4
  store %Pollfd zeroinitializer, %Pollfd* %p, align 4
  %p.fd = getelementptr inbounds %Pollfd, %Pollfd* %p, i32 0, i32 0
  %"$fd" = load i32, i32* %fd, align 4
  store i32 %"$fd", i32* %p.fd, align 4
  %p.events = getelementptr inbounds %Pollfd, %Pollfd* %p, i32 0, i32 1
  %"$POLLIN" = load i32, i32* @POLLIN, align 4
  %AS = trunc i32 %"$POLLIN" to i16
  store i16 %AS, i16* %p.events, align 2
  %"*fds" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %fds, align 8
  %"$p" = load %Pollfd, %Pollfd* %p, align 4
  call void @"Array<Pollfd>.append"(%"Array<Pollfd>"* %"*fds", %Pollfd %"$p")
  ret void
}

define i32 @main() {
entry:
  %server = alloca i32, align 4
  %fds = alloca %"Array<Pollfd>", align 8
  %buf = alloca %"Array<i8>", align 8
  %ref = alloca %Pollfd*, align 8
  %r = alloca i64, align 8
  %i = alloca i32, align 4
  %ref15 = alloca %Pollfd*, align 8
  %ref22 = alloca %Pollfd*, align 8
  %client = alloca i32, align 4
  %j = alloca i32, align 4
  %ref36 = alloca %Pollfd*, align 8
  %r45 = alloca i64, align 8
  %ref48 = alloca %Pollfd*, align 8
  %ref59 = alloca %Pollfd*, align 8
  %ref65 = alloca %Pollfd*, align 8
  %ref69 = alloca %Pollfd*, align 8
  store i32 1, i32* @POLLIN, align 4
  store i32 8080, i32* @PORT, align 4
  %"$PORT" = load i32, i32* @PORT, align 4
  %call.listener = call i32 @listener(i32 %"$PORT")
  store i32 %call.listener, i32* %server, align 4
  br label %if33

if33:                                             ; preds = %entry
  %"$server" = load i32, i32* %server, align 4
  %LT = icmp slt i32 %"$server", 0
  br i1 %LT, label %if33.then, label %endif

if33.then:                                        ; preds = %if33
  call void @"io::err"(%String { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str, i32 0, i32 0), i32 35, i32 0 })
  ret i32 1

endif:                                            ; preds = %if33
  call void @"io::out"(%String { i8* getelementptr inbounds ([33 x i8], [33 x i8]* @str.2, i32 0, i32 0), i32 32, i32 0 })
  store %"Array<Pollfd>" zeroinitializer, %"Array<Pollfd>"* %fds, align 8
  call void @watch(%"Array<Pollfd>"* %fds, i32 0)
  %"$server9" = load i32, i32* %server, align 4
  call void @watch(%"Array<Pollfd>"* %fds, i32 %"$server9")
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %buf, align 8
  call void @"Array<i8>.resize"(%"Array<i8>"* %buf, i32 1024)
  br label %while34

while34:                                          ; preds = %endloop7, %endif
  br i1 true, label %while34.then, label %endloop8

while34.then:                                     ; preds = %while34
  %fds.data = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %fds, i32 0, i32 0
  %"$fds.data" = load %Pollfd*, %Pollfd** %fds.data, align 8
  %AS = bitcast %Pollfd* %"$fds.data" to i8*
  %"call.Array<Pollfd>.len" = call i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %fds)
  %call.poll = call i32 @poll(i8* %AS, i32 %"call.Array<Pollfd>.len", i32 -1)
  br label %if35

if35:                                             ; preds = %while34.then
  %"call.Array<Pollfd>.[]" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 0)
  store %Pollfd* %"call.Array<Pollfd>.[]", %Pollfd** %ref, align 8
  %"*ref" = load %Pollfd*, %Pollfd** %ref, align 8
  %"*ref.revents" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref", i32 0, i32 2
  %"$*ref.revents" = load i16, i16* %"*ref.revents", align 2
  %AS10 = sext i16 %"$*ref.revents" to i32
  %"$POLLIN" = load i32, i32* @POLLIN, align 4
  %BAND = and i32 %AS10, %"$POLLIN"
  %NQ = icmp ne i32 %BAND, 0
  br i1 %NQ, label %if35.then, label %endif2

if35.then:                                        ; preds = %if35
  %buf.data = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$buf.data" = load i8*, i8** %buf.data, align 8
  %call.read = call i64 @read(i32 0, i8* %"$buf.data", i64 1023)
  store i64 %call.read, i64* %r, align 8
  br label %if36

if36:                                             ; preds = %if35.then
  %"$r" = load i64, i64* %r, align 8
  %LE = icmp sle i64 %"$r", 0
  br i1 %LE, label %if36.then, label %endif1

if36.then:                                        ; preds = %if36
  br label %endloop8

endif1:                                           ; preds = %if36
  store i32 2, i32* %i, align 4
  br label %while37

while37:                                          ; preds = %while37.then, %endif1
  %"call.Array<Pollfd>.len11" = call i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %fds)
  %"$i" = load i32, i32* %i, align 4
  %LT12 = icmp slt i32 %"$i", %"call.Array<Pollfd>.len11"
  br i1 %LT12, label %while37.then, label %endloop

while37.then:                                     ; preds = %while37
  %"$i13" = load i32, i32* %i, align 4
  %"call.Array<Pollfd>.[]14" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %"$i13")
  store %Pollfd* %"call.Array<Pollfd>.[]14", %Pollfd** %ref15, align 8
  %"*ref16" = load %Pollfd*, %Pollfd** %ref15, align 8
  %"*ref.fd" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref16", i32 0, i32 0
  %buf.data17 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$*ref.fd" = load i32, i32* %"*ref.fd", align 4
  %"$buf.data18" = load i8*, i8** %buf.data17, align 8
  %"$r19" = load i64, i64* %r, align 8
  %call.write = call i64 @write(i32 %"$*ref.fd", i8* %"$buf.data18", i64 %"$r19")
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while37

endloop:                                          ; preds = %while37
  br label %endif2

endif2:                                           ; preds = %endloop, %if35
  br label %if38

if38:                                             ; preds = %endif2
  %"call.Array<Pollfd>.[]21" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 1)
  store %Pollfd* %"call.Array<Pollfd>.[]21", %Pollfd** %ref22, align 8
  %"*ref23" = load %Pollfd*, %Pollfd** %ref22, align 8
  %"*ref.revents24" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref23", i32 0, i32 2
  %"$*ref.revents25" = load i16, i16* %"*ref.revents24", align 2
  %AS26 = sext i16 %"$*ref.revents25" to i32
  %"$POLLIN27" = load i32, i32* @POLLIN, align 4
  %BAND28 = and i32 %AS26, %"$POLLIN27"
  %NQ29 = icmp ne i32 %BAND28, 0
  br i1 %NQ29, label %if38.then, label %endif4

if38.then:                                        ; preds = %if38
  %"$server30" = load i32, i32* %server, align 4
  %call.accept = call i32 @accept(i32 %"$server30", i8* null, i8* null)
  store i32 %call.accept, i32* %client, align 4
  br label %if39

if39:                                             ; preds = %if38.then
  %"$client" = load i32, i32* %client, align 4
  %GE = icmp sge i32 %"$client", 0
  br i1 %GE, label %if39.then, label %endif3

if39.then:                                        ; preds = %if39
  %"$client31" = load i32, i32* %client, align 4
  call void @watch(%"Array<Pollfd>"* %fds, i32 %"$client31")
  call void @"io::out"(%String { i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.3, i32 0, i32 0), i32 26, i32 0 })
  br label %endif3

endif3:                                           ; preds = %if39.then, %if39
  br label %endif4

endif4:                                           ; preds = %endif3, %if38
  store i32 2, i32* %j, align 4
  br label %while40

while40:                                          ; preds = %endif6, %if42.then, %if41.then, %endif4
  %"call.Array<Pollfd>.len32" = call i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %fds)
  %"$j" = load i32, i32* %j, align 4
  %LT33 = icmp slt i32 %"$j", %"call.Array<Pollfd>.len32"
  br i1 %LT33, label %while40.then, label %endloop7

while40.then:                                     ; preds = %while40
  br label %if41

if41:                                             ; preds = %while40.then
  %"$j34" = load i32, i32* %j, align 4
  %"call.Array<Pollfd>.[]35" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %"$j34")
  store %Pollfd* %"call.Array<Pollfd>.[]35", %Pollfd** %ref36, align 8
  %"*ref37" = load %Pollfd*, %Pollfd** %ref36, align 8
  %"*ref.revents38" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref37", i32 0, i32 2
  %"$*ref.revents39" = load i16, i16* %"*ref.revents38", align 2
  %AS40 = sext i16 %"$*ref.revents39" to i32
  %"$POLLIN41" = load i32, i32* @POLLIN, align 4
  %BAND42 = and i32 %AS40, %"$POLLIN41"
  %EQ = icmp eq i32 %BAND42, 0
  br i1 %EQ, label %if41.then, label %endif5

if41.then:                                        ; preds = %if41
  %"$j43" = load i32, i32* %j, align 4
  %ADD44 = add i32 %"$j43", 1
  store i32 %ADD44, i32* %j, align 4
  br label %while40

endif5:                                           ; preds = %if41
  %"$j46" = load i32, i32* %j, align 4
  %"call.Array<Pollfd>.[]47" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %"$j46")
  store %Pollfd* %"call.Array<Pollfd>.[]47", %Pollfd** %ref48, align 8
  %"*ref49" = load %Pollfd*, %Pollfd** %ref48, align 8
  %"*ref.fd50" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref49", i32 0, i32 0
  %buf.data51 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$*ref.fd52" = load i32, i32* %"*ref.fd50", align 4
  %"$buf.data53" = load i8*, i8** %buf.data51, align 8
  %call.read54 = call i64 @read(i32 %"$*ref.fd52", i8* %"$buf.data53", i64 1023)
  store i64 %call.read54, i64* %r45, align 8
  br label %if42

if42:                                             ; preds = %endif5
  %"$r55" = load i64, i64* %r45, align 8
  %LE56 = icmp sle i64 %"$r55", 0
  br i1 %LE56, label %if42.then, label %endif6

if42.then:                                        ; preds = %if42
  %"$j57" = load i32, i32* %j, align 4
  %"call.Array<Pollfd>.[]58" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %"$j57")
  store %Pollfd* %"call.Array<Pollfd>.[]58", %Pollfd** %ref59, align 8
  %"*ref60" = load %Pollfd*, %Pollfd** %ref59, align 8
  %"*ref.fd61" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref60", i32 0, i32 0
  %"$*ref.fd62" = load i32, i32* %"*ref.fd61", align 4
  %call.close = call i32 @close(i32 %"$*ref.fd62")
  %"$j63" = load i32, i32* %j, align 4
  %"call.Array<Pollfd>.[]64" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %"$j63")
  store %Pollfd* %"call.Array<Pollfd>.[]64", %Pollfd** %ref65, align 8
  %"*ref66" = load %Pollfd*, %Pollfd** %ref65, align 8
  %"call.Array<Pollfd>.len67" = call i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %fds)
  %SUB = sub i32 %"call.Array<Pollfd>.len67", 1
  %"call.Array<Pollfd>.[]68" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 %SUB)
  store %Pollfd* %"call.Array<Pollfd>.[]68", %Pollfd** %ref69, align 8
  %"*ref70" = load %Pollfd*, %Pollfd** %ref69, align 8
  %"$*ref" = load %Pollfd, %Pollfd* %"*ref70", align 4
  store %Pollfd %"$*ref", %Pollfd* %"*ref66", align 4
  %"call.Array<Pollfd>.pop" = call %Pollfd @"Array<Pollfd>.pop.1"(%"Array<Pollfd>"* %fds)
  call void @"io::out"(%String { i8* getelementptr inbounds ([30 x i8], [30 x i8]* @str.4, i32 0, i32 0), i32 29, i32 0 })
  br label %while40

endif6:                                           ; preds = %if42
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.5, i32 0, i32 0), i32 9, i32 0 })
  %buf.data71 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$buf.data72" = load i8*, i8** %buf.data71, align 8
  %"$r73" = load i64, i64* %r45, align 8
  %call.write74 = call i64 @write(i32 1, i8* %"$buf.data72", i64 %"$r73")
  %"$j75" = load i32, i32* %j, align 4
  %ADD76 = add i32 %"$j75", 1
  store i32 %ADD76, i32* %j, align 4
  br label %while40

endloop7:                                         ; preds = %while40
  br label %while34

endloop8:                                         ; preds = %if36.then, %while34
  %"$server77" = load i32, i32* %server, align 4
  %call.close78 = call i32 @close(i32 %"$server77")
  call void @"Array<i8>.drop"(%"Array<i8>"* %buf)
  call void @"Array<Pollfd>.drop"(%"Array<Pollfd>"* %fds)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 028 — project: net client

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Range = type { i32, i32, i32 }
%Pollfd = type { i32, i16, i16 }
%"Array<Pollfd>" = type { %Pollfd*, i32, i32 }

@POLLIN = global i32 0
@PORT = global i32 0
@str = private unnamed_addr constant [37 x i8] c"[client] can't reach 127.0.0.1:8080\0A\00", align 1
@str.1 = private unnamed_addr constant [38 x i8] c"[client] connected to 127.0.0.1:8080\0A\00", align 1
@str.2 = private unnamed_addr constant [43 x i8] c"[client] the server closed the connection\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"[server] \00", align 1

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

define void @"Array<i8>.resize"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %size = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self", i32 %"$size")
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %SUB = sub i32 %"$*self.count5", 1
  store i32 %SUB, i32* %"*self.count4", align 4
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while3

endloop:                                          ; preds = %while3
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
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
  br label %if11

if11:                                             ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
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
  br label %if12

if12:                                             ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  ret i1 false

endif:                                            ; preds = %if12
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
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

declare i32 @socket(i32, i32, i32)

declare i32 @connect(i32, i8*, i32)

declare i32 @poll(i8*, i32, i32)

declare i32 @close(i32)

define %"Array<i8>" @address(i32 %0) {
entry:
  %port = alloca i32, align 4
  %data = alloca %"Array<i8>", align 8
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i3 = alloca i32, align 4
  store i32 %0, i32* %port, align 4
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %data, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 2)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  %"$port" = load i32, i32* %port, align 4
  %RSHIFT = ashr i32 %"$port", 8
  %AS = trunc i32 %RSHIFT to i8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 %AS)
  %"$port1" = load i32, i32* %port, align 4
  %BAND = and i32 %"$port1", 255
  %AS2 = trunc i32 %BAND to i8
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 %AS2)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 127)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 1)
  %call.Range.create = call %Range @Range.create(i32 0, i32 8)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i3, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i3)
  br i1 %call.Range.in, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  call void @"Array<i8>.append"(%"Array<i8>"* %data, i8 0)
  br label %while13

endloop:                                          ; preds = %while13
  %"$data" = load %"Array<i8>", %"Array<i8>"* %data, align 8
  ret %"Array<i8>" %"$data"
}

define %Pollfd* @"new<Pollfd>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Pollfd* getelementptr (%Pollfd, %Pollfd* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Pollfd*
  ret %Pollfd* %AS1
}

define void @"copy<Pollfd>"(%Pollfd* %0, %Pollfd* %1, i32 %2) {
entry:
  %dst = alloca %Pollfd*, align 8
  %src = alloca %Pollfd*, align 8
  %count = alloca i32, align 4
  store %Pollfd* %0, %Pollfd** %dst, align 8
  store %Pollfd* %1, %Pollfd** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Pollfd*, %Pollfd** %dst, align 8
  %AS = bitcast %Pollfd* %"$dst" to i8*
  %"$src" = load %Pollfd*, %Pollfd** %src, align 8
  %AS1 = bitcast %Pollfd* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Pollfd* getelementptr (%Pollfd, %Pollfd* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Pollfd>"(%Pollfd* %0) {
entry:
  %data = alloca %Pollfd*, align 8
  store %Pollfd* %0, %Pollfd** %data, align 8
  %"$data" = load %Pollfd*, %Pollfd** %data, align 8
  %AS = bitcast %Pollfd* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Pollfd*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %if15

if15:                                             ; preds = %endif
  %"*self6" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret void

endif1:                                           ; preds = %if15
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Pollfd>" = call %Pollfd* @"new<Pollfd>"(i32 %"$cap8")
  store %Pollfd* %"call.new<Pollfd>", %Pollfd** %bigger, align 8
  %"*self9" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Pollfd*, %Pollfd** %bigger, align 8
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Pollfd>"(%Pollfd* %"$bigger", %Pollfd* %"$*self.data", i32 %"$*self.count12")
  br label %if16

if16:                                             ; preds = %endif1
  %"*self13" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if16.then, label %endif2

if16.then:                                        ; preds = %if16
  %"*self16" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Pollfd*, %Pollfd** %"*self.data17", align 8
  call void @"free<Pollfd>"(%Pollfd* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if16.then, %if16
  %"*self19" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Pollfd*, %Pollfd** %bigger, align 8
  store %Pollfd* %"$bigger21", %Pollfd** %"*self.data20", align 8
  %"*self22" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Pollfd>.own"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  ret void
}

define void @"Array<Pollfd>.clear"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  br label %if18

if18:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if18.then, %if18
  br label %while19

while19:                                          ; preds = %while19.then, %endif
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  %"*self4" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data", i32 %"$*self.count10"
  store %Pollfd zeroinitializer, %Pollfd* %"*self.data[]", align 4
  br label %while19

endloop:                                          ; preds = %while19
  ret void
}

define void @"Array<Pollfd>.append"(%"Array<Pollfd>"* %0, %Pollfd %1) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  %value = alloca %Pollfd, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  store %Pollfd %1, %Pollfd* %value, align 4
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self3" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Pollfd>.reserve"(%"Array<Pollfd>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  %"*self6" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Pollfd*, %Pollfd** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Pollfd, %Pollfd* %value, align 4
  store %Pollfd %"$value", %Pollfd* %"*self.data[]", align 4
  %"*self10" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %0) {
entry:
  %self = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %self, align 8
  %"*self" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Pollfd>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %left, align 8
  call void @"Array<Pollfd>.own"(%"Array<Pollfd>"* %"*left")
  %"*left1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Pollfd*, %Pollfd** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Pollfd, %Pollfd* %"$*left.data", i32 %"$index"
  ret %Pollfd* %"*left.data[]"
}

define void @"Array<Pollfd>.drop"(%"Array<Pollfd>"* %0) {
entry:
  %value = alloca %"Array<Pollfd>"*, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %value, align 8
  %"*value" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  call void @"Array<Pollfd>.clear"(%"Array<Pollfd>"* %"*value")
  br label %if21

if21:                                             ; preds = %entry
  %"*value1" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if21.then, label %endif

if21.then:                                        ; preds = %if21
  %"*value2" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Pollfd*, %Pollfd** %"*value.data", align 8
  call void @"free<Pollfd>"(%Pollfd* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if21.then, %if21
  ret void
}

define void @watch(%"Array<Pollfd>"* %0, i32 %1) {
entry:
  %fds = alloca %"Array<Pollfd>"*, align 8
  %fd = alloca i32, align 4
  %p = alloca %Pollfd, align 8
  store %"Array<Pollfd>"* %0, %"Array<Pollfd>"** %fds, align 8
  store i32 %1, i32* %fd, align 4
  store %Pollfd zeroinitializer, %Pollfd* %p, align 4
  %p.fd = getelementptr inbounds %Pollfd, %Pollfd* %p, i32 0, i32 0
  %"$fd" = load i32, i32* %fd, align 4
  store i32 %"$fd", i32* %p.fd, align 4
  %p.events = getelementptr inbounds %Pollfd, %Pollfd* %p, i32 0, i32 1
  %"$POLLIN" = load i32, i32* @POLLIN, align 4
  %AS = trunc i32 %"$POLLIN" to i16
  store i16 %AS, i16* %p.events, align 2
  %"*fds" = load %"Array<Pollfd>"*, %"Array<Pollfd>"** %fds, align 8
  %"$p" = load %Pollfd, %Pollfd* %p, align 4
  call void @"Array<Pollfd>.append"(%"Array<Pollfd>"* %"*fds", %Pollfd %"$p")
  ret void
}

define i32 @main() {
entry:
  %fd = alloca i32, align 4
  %addr = alloca %"Array<i8>", align 8
  %fds = alloca %"Array<Pollfd>", align 8
  %buf = alloca %"Array<i8>", align 8
  %ref = alloca %Pollfd*, align 8
  %r = alloca i64, align 8
  %ref14 = alloca %Pollfd*, align 8
  %r22 = alloca i64, align 8
  store i32 1, i32* @POLLIN, align 4
  store i32 8080, i32* @PORT, align 4
  %call.socket = call i32 @socket(i32 2, i32 1, i32 0)
  store i32 %call.socket, i32* %fd, align 4
  %"$PORT" = load i32, i32* @PORT, align 4
  %call.address = call %"Array<i8>" @address(i32 %"$PORT")
  store %"Array<i8>" %call.address, %"Array<i8>"* %addr, align 8
  br label %if22

if22:                                             ; preds = %entry
  %"$fd" = load i32, i32* %fd, align 4
  %LT = icmp slt i32 %"$fd", 0
  %addr.data = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %addr, i32 0, i32 0
  %"$fd5" = load i32, i32* %fd, align 4
  %"$addr.data" = load i8*, i8** %addr.data, align 8
  %call.connect = call i32 @connect(i32 %"$fd5", i8* %"$addr.data", i32 16)
  %NQ = icmp ne i32 %call.connect, 0
  %OR = or i1 %LT, %NQ
  br i1 %OR, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  call void @"io::err"(%String { i8* getelementptr inbounds ([37 x i8], [37 x i8]* @str, i32 0, i32 0), i32 36, i32 0 })
  call void @"Array<i8>.drop"(%"Array<i8>"* %addr)
  ret i32 1

endif:                                            ; preds = %if22
  call void @"io::out"(%String { i8* getelementptr inbounds ([38 x i8], [38 x i8]* @str.1, i32 0, i32 0), i32 37, i32 0 })
  store %"Array<Pollfd>" zeroinitializer, %"Array<Pollfd>"* %fds, align 8
  call void @watch(%"Array<Pollfd>"* %fds, i32 0)
  %"$fd6" = load i32, i32* %fd, align 4
  call void @watch(%"Array<Pollfd>"* %fds, i32 %"$fd6")
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %buf, align 8
  call void @"Array<i8>.resize"(%"Array<i8>"* %buf, i32 1024)
  br label %while23

while23:                                          ; preds = %endif4, %endif
  br i1 true, label %while23.then, label %endloop

while23.then:                                     ; preds = %while23
  %fds.data = getelementptr inbounds %"Array<Pollfd>", %"Array<Pollfd>"* %fds, i32 0, i32 0
  %"$fds.data" = load %Pollfd*, %Pollfd** %fds.data, align 8
  %AS = bitcast %Pollfd* %"$fds.data" to i8*
  %"call.Array<Pollfd>.len" = call i32 @"Array<Pollfd>.len"(%"Array<Pollfd>"* %fds)
  %call.poll = call i32 @poll(i8* %AS, i32 %"call.Array<Pollfd>.len", i32 -1)
  br label %if24

if24:                                             ; preds = %while23.then
  %"call.Array<Pollfd>.[]" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 0)
  store %Pollfd* %"call.Array<Pollfd>.[]", %Pollfd** %ref, align 8
  %"*ref" = load %Pollfd*, %Pollfd** %ref, align 8
  %"*ref.revents" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref", i32 0, i32 2
  %"$*ref.revents" = load i16, i16* %"*ref.revents", align 2
  %AS7 = sext i16 %"$*ref.revents" to i32
  %"$POLLIN" = load i32, i32* @POLLIN, align 4
  %BAND = and i32 %AS7, %"$POLLIN"
  %NQ8 = icmp ne i32 %BAND, 0
  br i1 %NQ8, label %if24.then, label %endif2

if24.then:                                        ; preds = %if24
  %buf.data = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$buf.data" = load i8*, i8** %buf.data, align 8
  %call.read = call i64 @read(i32 0, i8* %"$buf.data", i64 1023)
  store i64 %call.read, i64* %r, align 8
  br label %if25

if25:                                             ; preds = %if24.then
  %"$r" = load i64, i64* %r, align 8
  %LE = icmp sle i64 %"$r", 0
  br i1 %LE, label %if25.then, label %endif1

if25.then:                                        ; preds = %if25
  br label %endloop

endif1:                                           ; preds = %if25
  %buf.data9 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$fd10" = load i32, i32* %fd, align 4
  %"$buf.data11" = load i8*, i8** %buf.data9, align 8
  %"$r12" = load i64, i64* %r, align 8
  %call.write = call i64 @write(i32 %"$fd10", i8* %"$buf.data11", i64 %"$r12")
  br label %endif2

endif2:                                           ; preds = %endif1, %if24
  br label %if26

if26:                                             ; preds = %endif2
  %"call.Array<Pollfd>.[]13" = call %Pollfd* @"Array<Pollfd>.[]"(%"Array<Pollfd>"* %fds, i32 1)
  store %Pollfd* %"call.Array<Pollfd>.[]13", %Pollfd** %ref14, align 8
  %"*ref15" = load %Pollfd*, %Pollfd** %ref14, align 8
  %"*ref.revents16" = getelementptr inbounds %Pollfd, %Pollfd* %"*ref15", i32 0, i32 2
  %"$*ref.revents17" = load i16, i16* %"*ref.revents16", align 2
  %AS18 = sext i16 %"$*ref.revents17" to i32
  %"$POLLIN19" = load i32, i32* @POLLIN, align 4
  %BAND20 = and i32 %AS18, %"$POLLIN19"
  %NQ21 = icmp ne i32 %BAND20, 0
  br i1 %NQ21, label %if26.then, label %endif4

if26.then:                                        ; preds = %if26
  %buf.data23 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$fd24" = load i32, i32* %fd, align 4
  %"$buf.data25" = load i8*, i8** %buf.data23, align 8
  %call.read26 = call i64 @read(i32 %"$fd24", i8* %"$buf.data25", i64 1023)
  store i64 %call.read26, i64* %r22, align 8
  br label %if27

if27:                                             ; preds = %if26.then
  %"$r27" = load i64, i64* %r22, align 8
  %LE28 = icmp sle i64 %"$r27", 0
  br i1 %LE28, label %if27.then, label %endif3

if27.then:                                        ; preds = %if27
  call void @"io::out"(%String { i8* getelementptr inbounds ([43 x i8], [43 x i8]* @str.2, i32 0, i32 0), i32 42, i32 0 })
  br label %endloop

endif3:                                           ; preds = %if27
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 9, i32 0 })
  %buf.data29 = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %buf, i32 0, i32 0
  %"$buf.data30" = load i8*, i8** %buf.data29, align 8
  %"$r31" = load i64, i64* %r22, align 8
  %call.write32 = call i64 @write(i32 1, i8* %"$buf.data30", i64 %"$r31")
  br label %endif4

endif4:                                           ; preds = %endif3, %if26
  br label %while23

endloop:                                          ; preds = %if27.then, %if25.then, %while23
  %"$fd33" = load i32, i32* %fd, align 4
  %call.close = call i32 @close(i32 %"$fd33")
  call void @"Array<i8>.drop"(%"Array<i8>"* %buf)
  call void @"Array<Pollfd>.drop"(%"Array<Pollfd>"* %fds)
  call void @"Array<i8>.drop"(%"Array<i8>"* %addr)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 029 — dungeon: floor04

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [34 x i8] c"Orc takes %d damage \E2\80\94 dead: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"4 squared: %d\0A\00", align 1

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

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", %"$n1"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %call.damage = call i32 @damage(i32 25, i32 8)
  store i32 %call.damage, i32* %d, align 4
  %"$d" = load i32, i32* %d, align 4
  %SUB = sub i32 %"$d", 60
  %call.is_dead = call i1 @is_dead(i32 %SUB)
  store i32 0, i32* %format.size, align 4
  %"$d1" = load i32, i32* %d, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if8

if8:                                              ; preds = %entry
  br i1 %call.is_dead, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str.2, i32 0, i32 0), i32 %"$d1", i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text2" = load i8*, i8** %bool.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str.2, i32 0, i32 0), i32 %"$d1", i8* %"$bool.text2")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %call.square = call i32 @square(i32 4)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 %call.square)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 %call.square)
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

## 030 — dungeon: floor09b

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
  %hp = alloca i32, align 4
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 80, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 100, i32* %"*r", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$hp")
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

## 031 — dungeon: floor14a

```llvm
@str = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i64 @write(i32, i8*, i64)

declare i64 @strlen(i8*)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %call.strlen = call i64 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %n, align 4
  %call.write = call i64 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 3)
  %"$n" = load i32, i32* %n, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$n")
  ret i32 0
}
```

## 032 — dungeon: boss

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Range = type { i32, i32, i32 }
%Hero = type { %String, %Stats }
%Stats = type { i32, i32 }
%"Array<Hero>" = type { %Hero*, i32, i32 }
%"Array<EnemyType>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"Dragonfang\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"none\00", align 1
@str.2 = private unnamed_addr constant [15 x i8] c"== Dungeon ==\0A\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"Borin\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"Cass\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"Party:\0A\00", align 1
@str.7 = private unnamed_addr constant [13 x i8] c"  %s atk=%d\0A\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [25 x i8] c"Champion: %s (alive=%s)\0A\00", align 1
@str.11 = private unnamed_addr constant [19 x i8] c"Foe hp=%d loot=%s\0A\00", align 1
@str.12 = private unnamed_addr constant [16 x i8] c"Trap! %s hp=%d\0A\00", align 1
@TYPEOF = private unnamed_addr constant [5 x i8] c"Hero\00", align 1
@str.13 = private unnamed_addr constant [21 x i8] c"sizeof=%d typeof=%s\0A\00", align 1

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
  br label %if10

if10:                                             ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
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
  br label %if11

if11:                                             ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  ret i1 false

endif:                                            ; preds = %if11
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

define %Hero @Hero.create(%String %0, i32 %1, i32 %2) {
entry:
  %name = alloca %String, align 8
  %hp = alloca i32, align 4
  %atk = alloca i32, align 4
  %h = alloca %Hero, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %name, align 8
  store i32 %1, i32* %hp, align 4
  store i32 %2, i32* %atk, align 4
  store %Hero zeroinitializer, %Hero* %h, align 8
  %h.name = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  %3 = bitcast %String* %copy to %"Array<i8>"*
  %4 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %3, %"Array<i8>"* %4)
  %"$copy" = load %String, %String* %copy, align 8
  %5 = bitcast %String* %h.name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  store %String %"$copy", %String* %h.name, align 8
  %h.stats = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  %h.stats.hp = getelementptr inbounds %Stats, %Stats* %h.stats, i32 0, i32 0
  %"$hp" = load i32, i32* %hp, align 4
  store i32 %"$hp", i32* %h.stats.hp, align 4
  %h.stats1 = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  %h.stats.atk = getelementptr inbounds %Stats, %Stats* %h.stats1, i32 0, i32 1
  %"$atk" = load i32, i32* %atk, align 4
  store i32 %"$atk", i32* %h.stats.atk, align 4
  %"$h" = load %Hero, %Hero* %h, align 8
  %6 = bitcast %String* %name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  ret %Hero %"$h"
}

define i1 @Hero.alive(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %"*self" = load %Hero*, %Hero** %self, align 8
  %"*self.stats" = getelementptr inbounds %Hero, %Hero* %"*self", i32 0, i32 1
  %"*self.stats.hp" = getelementptr inbounds %Stats, %Stats* %"*self.stats", i32 0, i32 0
  %"$*self.stats.hp" = load i32, i32* %"*self.stats.hp", align 4
  %GT = icmp sgt i32 %"$*self.stats.hp", 0
  ret i1 %GT
}

define void @"Hero.="(%Hero* %0, %Hero* %1) {
entry:
  %left = alloca %Hero*, align 8
  %right = alloca %Hero*, align 8
  %copy = alloca %String, align 8
  store %Hero* %0, %Hero** %left, align 8
  store %Hero* %1, %Hero** %right, align 8
  %"*left" = load %Hero*, %Hero** %left, align 8
  %"*left.name" = getelementptr inbounds %Hero, %Hero* %"*left", i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  %"*right" = load %Hero*, %Hero** %right, align 8
  %"*right.name" = getelementptr inbounds %Hero, %Hero* %"*right", i32 0, i32 0
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %"*right.name" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  %4 = bitcast %String* %"*left.name" to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  store %String %"$copy", %String* %"*left.name", align 8
  %"*left1" = load %Hero*, %Hero** %left, align 8
  %"*left.stats" = getelementptr inbounds %Hero, %Hero* %"*left1", i32 0, i32 1
  %"*right2" = load %Hero*, %Hero** %right, align 8
  %"*right.stats" = getelementptr inbounds %Hero, %Hero* %"*right2", i32 0, i32 1
  %"$*right.stats" = load %Stats, %Stats* %"*right.stats", align 4
  store %Stats %"$*right.stats", %Stats* %"*left.stats", align 4
  ret void
}

define %String @"Hero.$"(%Hero* %0) {
entry:
  %hero = alloca %Hero*, align 8
  %copy = alloca %String, align 8
  store %Hero* %0, %Hero** %hero, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*hero" = load %Hero*, %Hero** %hero, align 8
  %"*hero.name" = getelementptr inbounds %Hero, %Hero* %"*hero", i32 0, i32 0
  %1 = bitcast %String* %copy to %"Array<i8>"*
  %2 = bitcast %String* %"*hero.name" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %1, %"Array<i8>"* %2)
  %"$copy" = load %String, %String* %copy, align 8
  ret %String %"$copy"
}

define i32 @enemy_hp(i32 %0) {
entry:
  %kind = alloca i32, align 4
  %"match@878" = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  %"$kind" = load i32, i32* %kind, align 4
  store i32 %"$kind", i32* %"match@878", align 4
  br label %if12

if12:                                             ; preds = %entry
  %"$match@878" = load i32, i32* %"match@878", align 4
  %EQ = icmp eq i32 %"$match@878", 0
  br i1 %EQ, label %if12.then, label %elif13

if12.then:                                        ; preds = %if12
  ret i32 20

elif13:                                           ; preds = %if12
  %"$match@8781" = load i32, i32* %"match@878", align 4
  %EQ2 = icmp eq i32 %"$match@8781", 1
  br i1 %EQ2, label %elif13.then, label %elif14

elif13.then:                                      ; preds = %elif13
  ret i32 45

elif14:                                           ; preds = %elif13
  %"$match@8783" = load i32, i32* %"match@878", align 4
  %EQ4 = icmp eq i32 %"$match@8783", 2
  br i1 %EQ4, label %elif14.then, label %endif

elif14.then:                                      ; preds = %elif14
  ret i32 120

endif:                                            ; preds = %elif14
  ret i32 0
}

define %String @loot_for(i32 %0) {
entry:
  %kind = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  br label %if15

if15:                                             ; preds = %entry
  %"$kind" = load i32, i32* %kind, align 4
  %EQ = icmp eq i32 %"$kind", 2
  br i1 %EQ, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  ret %String { i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 10, i32 0 }

endif:                                            ; preds = %if15
  ret %String { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i32 4, i32 0 }
}

define %Hero* @"new<Hero>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Hero* getelementptr (%Hero, %Hero* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Hero*
  ret %Hero* %AS1
}

define void @"copy<Hero>"(%Hero* %0, %Hero* %1, i32 %2) {
entry:
  %dst = alloca %Hero*, align 8
  %src = alloca %Hero*, align 8
  %count = alloca i32, align 4
  store %Hero* %0, %Hero** %dst, align 8
  store %Hero* %1, %Hero** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Hero*, %Hero** %dst, align 8
  %AS = bitcast %Hero* %"$dst" to i8*
  %"$src" = load %Hero*, %Hero** %src, align 8
  %AS1 = bitcast %Hero* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Hero* getelementptr (%Hero, %Hero* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Hero>"(%Hero* %0) {
entry:
  %data = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %data, align 8
  %"$data" = load %Hero*, %Hero** %data, align 8
  %AS = bitcast %Hero* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Hero>.reserve"(%"Array<Hero>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Hero>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Hero*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self3" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  br label %if17

if17:                                             ; preds = %endif
  %"*self6" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  ret void

endif1:                                           ; preds = %if17
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Hero>" = call %Hero* @"new<Hero>"(i32 %"$cap8")
  store %Hero* %"call.new<Hero>", %Hero** %bigger, align 8
  %"*self9" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Hero*, %Hero** %bigger, align 8
  %"$*self.data" = load %Hero*, %Hero** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Hero>"(%Hero* %"$bigger", %Hero* %"$*self.data", i32 %"$*self.count12")
  br label %if18

if18:                                             ; preds = %endif1
  %"*self13" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if18.then, label %endif2

if18.then:                                        ; preds = %if18
  %"*self16" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Hero*, %Hero** %"*self.data17", align 8
  call void @"free<Hero>"(%Hero* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if18.then, %if18
  %"*self19" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Hero*, %Hero** %bigger, align 8
  store %Hero* %"$bigger21", %Hero** %"*self.data20", align 8
  %"*self22" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Hero>.own"(%"Array<Hero>"* %0) {
entry:
  %self = alloca %"Array<Hero>"*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %self, align 8
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self2" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self3" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Hero>.reserve"(%"Array<Hero>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  ret void
}

define void @"Array<Hero>.clear"(%"Array<Hero>"* %0) {
entry:
  %self = alloca %"Array<Hero>"*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %self, align 8
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self1" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  br label %while21

while21:                                          ; preds = %while21.then, %endif
  %"*self2" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self4" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Hero*, %Hero** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Hero, %Hero* %"$*self.data", i32 %"$*self.count10"
  %"*self.data[].name" = getelementptr inbounds %Hero, %Hero* %"*self.data[]", i32 0, i32 0
  %1 = bitcast %String* %"*self.data[].name" to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  store %Hero zeroinitializer, %Hero* %"*self.data[]", align 8
  br label %while21

endloop:                                          ; preds = %while21
  ret void
}

define void @"Array<Hero>.append"(%"Array<Hero>"* %0, %Hero %1) {
entry:
  %self = alloca %"Array<Hero>"*, align 8
  %value = alloca %Hero, align 8
  %copy = alloca %Hero, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %self, align 8
  store %Hero %1, %Hero* %value, align 8
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self3" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Hero>.reserve"(%"Array<Hero>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  %"*self6" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Hero*, %Hero** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Hero, %Hero* %"$*self.data", i32 %"$*self.count9"
  store %Hero zeroinitializer, %Hero* %copy, align 8
  call void @"Hero.="(%Hero* %copy, %Hero* %value)
  %"$copy" = load %Hero, %Hero* %copy, align 8
  %"*self.data[].name" = getelementptr inbounds %Hero, %Hero* %"*self.data[]", i32 0, i32 0
  %2 = bitcast %String* %"*self.data[].name" to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %2)
  store %Hero %"$copy", %Hero* %"*self.data[]", align 8
  %"*self10" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  %value.name = getelementptr inbounds %Hero, %Hero* %value, i32 0, i32 0
  %3 = bitcast %String* %value.name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  ret void
}

define i32 @"Array<Hero>.len"(%"Array<Hero>"* %0) {
entry:
  %self = alloca %"Array<Hero>"*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %self, align 8
  %"*self" = load %"Array<Hero>"*, %"Array<Hero>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Hero>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Hero>"* %0, %"Array<Hero>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Hero>"*, %"Array<Hero>"** %left, align 8
  call void @"Array<Hero>.own"(%"Array<Hero>"* %"*left")
  %"*left1" = load %"Array<Hero>"*, %"Array<Hero>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Hero*, %Hero** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Hero, %Hero* %"$*left.data", i32 %"$index"
  ret %Hero* %"*left.data[]"
}

define i1 @"Array<Hero>.in"(%"Array<Hero>"* %0, i32* %1, %Hero* %2) {
entry:
  %left = alloca %"Array<Hero>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca %Hero*, align 8
  %copy = alloca %Hero, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %left, align 8
  store i32* %1, i32** %i, align 8
  store %Hero* %2, %Hero** %x, align 8
  br label %if23

if23:                                             ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*left" = load %"Array<Hero>"*, %"Array<Hero>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*left", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*left.count" = load i32, i32* %"*left.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*left.count"
  br i1 %GE, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  ret i1 false

endif:                                            ; preds = %if23
  %"*x" = load %Hero*, %Hero** %x, align 8
  store %Hero zeroinitializer, %Hero* %copy, align 8
  %"*left1" = load %"Array<Hero>"*, %"Array<Hero>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*left1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*left.data" = load %Hero*, %Hero** %"*left.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*left.data[]" = getelementptr inbounds %Hero, %Hero* %"$*left.data", i32 %"$*i3"
  call void @"Hero.="(%Hero* %copy, %Hero* %"*left.data[]")
  %"$copy" = load %Hero, %Hero* %copy, align 8
  %"*x.name" = getelementptr inbounds %Hero, %Hero* %"*x", i32 0, i32 0
  %3 = bitcast %String* %"*x.name" to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  store %Hero %"$copy", %Hero* %"*x", align 8
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<Hero>.drop"(%"Array<Hero>"* %0) {
entry:
  %value = alloca %"Array<Hero>"*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %value, align 8
  %"*value" = load %"Array<Hero>"*, %"Array<Hero>"** %value, align 8
  call void @"Array<Hero>.clear"(%"Array<Hero>"* %"*value")
  br label %if24

if24:                                             ; preds = %entry
  %"*value1" = load %"Array<Hero>"*, %"Array<Hero>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if24.then, label %endif

if24.then:                                        ; preds = %if24
  %"*value2" = load %"Array<Hero>"*, %"Array<Hero>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Hero>", %"Array<Hero>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Hero*, %Hero** %"*value.data", align 8
  call void @"free<Hero>"(%Hero* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if24.then, %if24
  ret void
}

define i32 @strongest(%"Array<Hero>"* %0) {
entry:
  %party = alloca %"Array<Hero>"*, align 8
  %best = alloca i32, align 4
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i1 = alloca i32, align 4
  %ref = alloca %Hero*, align 8
  %ref5 = alloca %Hero*, align 8
  store %"Array<Hero>"* %0, %"Array<Hero>"** %party, align 8
  store i32 0, i32* %best, align 4
  %"*party" = load %"Array<Hero>"*, %"Array<Hero>"** %party, align 8
  %"call.Array<Hero>.len" = call i32 @"Array<Hero>.len"(%"Array<Hero>"* %"*party")
  %call.Range.create = call %Range @Range.create(i32 1, i32 %"call.Array<Hero>.len")
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i1, align 4
  br label %while25

while25:                                          ; preds = %endif, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i1)
  br i1 %call.Range.in, label %while25.then, label %endloop

while25.then:                                     ; preds = %while25
  br label %if26

if26:                                             ; preds = %while25.then
  %"*party2" = load %"Array<Hero>"*, %"Array<Hero>"** %party, align 8
  %"$i" = load i32, i32* %i1, align 4
  %"call.Array<Hero>.[]" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %"*party2", i32 %"$i")
  store %Hero* %"call.Array<Hero>.[]", %Hero** %ref, align 8
  %"*ref" = load %Hero*, %Hero** %ref, align 8
  %"*ref.stats" = getelementptr inbounds %Hero, %Hero* %"*ref", i32 0, i32 1
  %"*ref.stats.atk" = getelementptr inbounds %Stats, %Stats* %"*ref.stats", i32 0, i32 1
  %"*party3" = load %"Array<Hero>"*, %"Array<Hero>"** %party, align 8
  %"$best" = load i32, i32* %best, align 4
  %"call.Array<Hero>.[]4" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %"*party3", i32 %"$best")
  store %Hero* %"call.Array<Hero>.[]4", %Hero** %ref5, align 8
  %"*ref6" = load %Hero*, %Hero** %ref5, align 8
  %"*ref.stats7" = getelementptr inbounds %Hero, %Hero* %"*ref6", i32 0, i32 1
  %"*ref.stats.atk8" = getelementptr inbounds %Stats, %Stats* %"*ref.stats7", i32 0, i32 1
  %"$*ref.stats.atk" = load i32, i32* %"*ref.stats.atk", align 4
  %"$*ref.stats.atk9" = load i32, i32* %"*ref.stats.atk8", align 4
  %GT = icmp sgt i32 %"$*ref.stats.atk", %"$*ref.stats.atk9"
  br i1 %GT, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"$i10" = load i32, i32* %i1, align 4
  store i32 %"$i10", i32* %best, align 4
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  br label %while25

endloop:                                          ; preds = %while25
  %"$best11" = load i32, i32* %best, align 4
  ret i32 %"$best11"
}

define i32* @"new<EnemyType>"(i32 %0) {
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

define void @"copy<EnemyType>"(i32* %0, i32* %1, i32 %2) {
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

define void @"free<EnemyType>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<EnemyType>.reserve"(%"Array<EnemyType>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<EnemyType>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<EnemyType>"* %0, %"Array<EnemyType>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if27

if27:                                             ; preds = %entry
  %"*self" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  %"*self3" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if27.then, %if27
  br label %if28

if28:                                             ; preds = %endif
  %"*self6" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if28.then, label %endif1

if28.then:                                        ; preds = %if28
  ret void

endif1:                                           ; preds = %if28
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<EnemyType>" = call i32* @"new<EnemyType>"(i32 %"$cap8")
  store i32* %"call.new<EnemyType>", i32** %bigger, align 8
  %"*self9" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<EnemyType>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if29

if29:                                             ; preds = %endif1
  %"*self13" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if29.then, label %endif2

if29.then:                                        ; preds = %if29
  %"*self16" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<EnemyType>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if29.then, %if29
  %"*self19" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<EnemyType>.own"(%"Array<EnemyType>"* %0) {
entry:
  %self = alloca %"Array<EnemyType>"*, align 8
  store %"Array<EnemyType>"* %0, %"Array<EnemyType>"** %self, align 8
  br label %if30

if30:                                             ; preds = %entry
  %"*self" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if30.then, label %endif

if30.then:                                        ; preds = %if30
  %"*self2" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self3" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<EnemyType>.reserve"(%"Array<EnemyType>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if30.then, %if30
  ret void
}

define void @"Array<EnemyType>.clear"(%"Array<EnemyType>"* %0) {
entry:
  %self = alloca %"Array<EnemyType>"*, align 8
  store %"Array<EnemyType>"* %0, %"Array<EnemyType>"** %self, align 8
  br label %if31

if31:                                             ; preds = %entry
  %"*self" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if31.then, label %endif

if31.then:                                        ; preds = %if31
  %"*self1" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if31.then, %if31
  br label %while32

while32:                                          ; preds = %while32.then, %endif
  %"*self2" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while32.then, label %endloop

while32.then:                                     ; preds = %while32
  %"*self4" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while32

endloop:                                          ; preds = %while32
  ret void
}

define i1 @"Array<EnemyType>.in"(%"Array<EnemyType>"* %0, i32* %1, i32* %2) {
entry:
  %left = alloca %"Array<EnemyType>"*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  store %"Array<EnemyType>"* %0, %"Array<EnemyType>"** %left, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  br label %if33

if33:                                             ; preds = %entry
  %"*i" = load i32*, i32** %i, align 8
  %"*left" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*left", i32 0, i32 1
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*left.count" = load i32, i32* %"*left.count", align 4
  %GE = icmp sge i32 %"$*i", %"$*left.count"
  br i1 %GE, label %if33.then, label %endif

if33.then:                                        ; preds = %if33
  ret i1 false

endif:                                            ; preds = %if33
  %"*x" = load i32*, i32** %x, align 8
  %"*left1" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*left1", i32 0, i32 0
  %"*i2" = load i32*, i32** %i, align 8
  %"$*left.data" = load i32*, i32** %"*left.data", align 8
  %"$*i3" = load i32, i32* %"*i2", align 4
  %"*left.data[]" = getelementptr inbounds i32, i32* %"$*left.data", i32 %"$*i3"
  %"$*left.data[]" = load i32, i32* %"*left.data[]", align 4
  store i32 %"$*left.data[]", i32* %"*x", align 4
  %"*i4" = load i32*, i32** %i, align 8
  %"$*i5" = load i32, i32* %"*i4", align 4
  %ADD = add i32 %"$*i5", 1
  store i32 %ADD, i32* %"*i4", align 4
  ret i1 true
}

define void @"Array<EnemyType>.drop"(%"Array<EnemyType>"* %0) {
entry:
  %value = alloca %"Array<EnemyType>"*, align 8
  store %"Array<EnemyType>"* %0, %"Array<EnemyType>"** %value, align 8
  %"*value" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %value, align 8
  call void @"Array<EnemyType>.clear"(%"Array<EnemyType>"* %"*value")
  br label %if34

if34:                                             ; preds = %entry
  %"*value1" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if34.then, label %endif

if34.then:                                        ; preds = %if34
  %"*value2" = load %"Array<EnemyType>"*, %"Array<EnemyType>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<EnemyType>", %"Array<EnemyType>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i32*, i32** %"*value.data", align 8
  call void @"free<EnemyType>"(i32* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if34.then, %if34
  ret void
}

define i32 @main() {
entry:
  %party = alloca %"Array<Hero>", align 8
  %i = alloca i32, align 4
  %h = alloca %Hero, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %champ = alloca %Hero, align 8
  %copy = alloca %Hero, align 8
  %ref = alloca %Hero*, align 8
  %tmp11 = alloca %String, align 8
  %format.size13 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text18 = alloca i8*, align 8
  %foes = alloca %"Array<EnemyType>", align 8
  %arr = alloca [3 x i32], align 4
  %array = alloca %"Array<EnemyType>", align 8
  %i27 = alloca i32, align 4
  %kind = alloca i32, align 4
  %tmp29 = alloca %String, align 8
  %format.size31 = alloca i32, align 4
  %format.text36 = alloca i8*, align 8
  %tmp44 = alloca %Range, align 8
  %i45 = alloca i32, align 4
  %i46 = alloca i32, align 4
  %ref48 = alloca %Hero*, align 8
  %ref51 = alloca %Hero*, align 8
  %tmp54 = alloca %String, align 8
  %ref57 = alloca %Hero*, align 8
  %format.size61 = alloca i32, align 4
  %format.text67 = alloca i8*, align 8
  %format.size75 = alloca i32, align 4
  %format.text80 = alloca i8*, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.2, i32 0, i32 0), i32 14, i32 0 })
  store %"Array<Hero>" zeroinitializer, %"Array<Hero>"* %party, align 8
  %call.Hero.create = call %Hero @Hero.create(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i32 6, i32 0 }, i32 100, i32 18)
  call void @"Array<Hero>.append"(%"Array<Hero>"* %party, %Hero %call.Hero.create)
  %call.Hero.create3 = call %Hero @Hero.create(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i32 5, i32 0 }, i32 90, i32 25)
  call void @"Array<Hero>.append"(%"Array<Hero>"* %party, %Hero %call.Hero.create3)
  %call.Hero.create4 = call %Hero @Hero.create(%String { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i32 4, i32 0 }, i32 80, i32 22)
  call void @"Array<Hero>.append"(%"Array<Hero>"* %party, %Hero %call.Hero.create4)
  call void @"io::out"(%String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 7, i32 0 })
  store i32 0, i32* %i, align 4
  store %Hero zeroinitializer, %Hero* %h, align 8
  br label %while35

while35:                                          ; preds = %while35.then, %entry
  %"call.Array<Hero>.in" = call i1 @"Array<Hero>.in"(%"Array<Hero>"* %party, i32* %i, %Hero* %h)
  br i1 %"call.Array<Hero>.in", label %while35.then, label %endloop

while35.then:                                     ; preds = %while35
  %"call.Hero.$" = call %String @"Hero.$"(%Hero* %h)
  store %String %"call.Hero.$", %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %h.stats = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  %h.stats.atk = getelementptr inbounds %Stats, %Stats* %h.stats, i32 0, i32 1
  %"$h.stats.atk" = load i32, i32* %h.stats.atk, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars, i32 %"$h.stats.atk")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars, i32 %"$h.stats.atk")
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  %3 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  br label %while35

endloop:                                          ; preds = %while35
  store %Hero zeroinitializer, %Hero* %copy, align 8
  %call.strongest = call i32 @strongest(%"Array<Hero>"* %party)
  %"call.Array<Hero>.[]" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %party, i32 %call.strongest)
  store %Hero* %"call.Array<Hero>.[]", %Hero** %ref, align 8
  %"*ref" = load %Hero*, %Hero** %ref, align 8
  call void @"Hero.="(%Hero* %copy, %Hero* %"*ref")
  %"$copy" = load %Hero, %Hero* %copy, align 8
  store %Hero %"$copy", %Hero* %champ, align 8
  %"call.Hero.$10" = call %String @"Hero.$"(%Hero* %champ)
  store %String %"call.Hero.$10", %String* %tmp11, align 8
  %call.String.chars12 = call i8* @String.chars(%String* %tmp11)
  %call.Hero.alive = call i1 @Hero.alive(%Hero* %champ)
  store i32 0, i32* %format.size13, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if36

if36:                                             ; preds = %endloop
  br i1 %call.Hero.alive, label %if36.then, label %else37

if36.then:                                        ; preds = %if36
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else37:                                           ; preds = %if36
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else37, %if36.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars12, i8* %"$bool.text")
  store i32 %call.snprintf14, i32* %format.size13, align 4
  %"$format.size15" = load i32, i32* %format.size13, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %AS17 = sext i32 %ADD16 to i64
  %call.calloc19 = call i8* @calloc(i64 %AS17, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc19, i8** %format.text18, align 8
  %"$format.text20" = load i8*, i8** %format.text18, align 8
  %"$bool.text21" = load i8*, i8** %bool.text, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text20", i64 %AS17, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars12, i8* %"$bool.text21")
  %"$format.size23" = load i32, i32* %format.size13, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text18, align 8
  %"$format.size26" = load i32, i32* %format.size13, align 4
  %4 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %5 = insertvalue %String %4, i32 %"$format.size26", 1
  %6 = insertvalue %String %5, i32 %ADD24, 2
  call void @"io::out"(%String %6)
  %7 = bitcast %String* %tmp11 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  store [3 x i32] [i32 0, i32 1, i32 2], [3 x i32]* %arr, align 4
  %8 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  %9 = insertvalue %"Array<EnemyType>" zeroinitializer, i32* %8, 0
  %10 = insertvalue %"Array<EnemyType>" %9, i32 3, 1
  %11 = insertvalue %"Array<EnemyType>" %10, i32 0, 2
  store %"Array<EnemyType>" %11, %"Array<EnemyType>"* %array, align 8
  call void @"Array<EnemyType>.own"(%"Array<EnemyType>"* %array)
  %"$array" = load %"Array<EnemyType>", %"Array<EnemyType>"* %array, align 8
  store %"Array<EnemyType>" %"$array", %"Array<EnemyType>"* %foes, align 8
  store i32 0, i32* %i27, align 4
  store i32 0, i32* %kind, align 4
  br label %while38

while38:                                          ; preds = %while38.then, %endif
  %"call.Array<EnemyType>.in" = call i1 @"Array<EnemyType>.in"(%"Array<EnemyType>"* %foes, i32* %i27, i32* %kind)
  br i1 %"call.Array<EnemyType>.in", label %while38.then, label %endloop1

while38.then:                                     ; preds = %while38
  %"$kind" = load i32, i32* %kind, align 4
  %call.enemy_hp = call i32 @enemy_hp(i32 %"$kind")
  %"$kind28" = load i32, i32* %kind, align 4
  %call.loot_for = call %String @loot_for(i32 %"$kind28")
  store %String %call.loot_for, %String* %tmp29, align 8
  %call.String.chars30 = call i8* @String.chars(%String* %tmp29)
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.11, i32 0, i32 0), i32 %call.enemy_hp, i8* %call.String.chars30)
  store i32 %call.snprintf32, i32* %format.size31, align 4
  %"$format.size33" = load i32, i32* %format.size31, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %AS35 = sext i32 %ADD34 to i64
  %call.calloc37 = call i8* @calloc(i64 %AS35, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc37, i8** %format.text36, align 8
  %"$format.text38" = load i8*, i8** %format.text36, align 8
  %call.snprintf39 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text38", i64 %AS35, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.11, i32 0, i32 0), i32 %call.enemy_hp, i8* %call.String.chars30)
  %"$format.size40" = load i32, i32* %format.size31, align 4
  %ADD41 = add i32 %"$format.size40", 1
  %"$format.text42" = load i8*, i8** %format.text36, align 8
  %"$format.size43" = load i32, i32* %format.size31, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text42", 0
  %13 = insertvalue %String %12, i32 %"$format.size43", 1
  %14 = insertvalue %String %13, i32 %ADD41, 2
  call void @"io::out"(%String %14)
  %15 = bitcast %String* %tmp29 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %15)
  br label %while38

endloop1:                                         ; preds = %while38
  %"call.Array<Hero>.len" = call i32 @"Array<Hero>.len"(%"Array<Hero>"* %party)
  %call.Range.create = call %Range @Range.create(i32 0, i32 %"call.Array<Hero>.len")
  store %Range %call.Range.create, %Range* %tmp44, align 4
  store i32 0, i32* %i45, align 4
  store i32 0, i32* %i46, align 4
  br label %while39

while39:                                          ; preds = %while39.then, %endloop1
  %call.Range.in = call i1 @Range.in(%Range* %tmp44, i32* %i45, i32* %i46)
  br i1 %call.Range.in, label %while39.then, label %endloop2

while39.then:                                     ; preds = %while39
  %"$i" = load i32, i32* %i46, align 4
  %"call.Array<Hero>.[]47" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %party, i32 %"$i")
  store %Hero* %"call.Array<Hero>.[]47", %Hero** %ref48, align 8
  %"*ref49" = load %Hero*, %Hero** %ref48, align 8
  %"*ref.stats" = getelementptr inbounds %Hero, %Hero* %"*ref49", i32 0, i32 1
  %"*ref.stats.hp" = getelementptr inbounds %Stats, %Stats* %"*ref.stats", i32 0, i32 0
  %"$*ref.stats.hp" = load i32, i32* %"*ref.stats.hp", align 4
  %SUB = sub i32 %"$*ref.stats.hp", 10
  store i32 %SUB, i32* %"*ref.stats.hp", align 4
  br label %while39

endloop2:                                         ; preds = %while39
  %"call.Array<Hero>.[]50" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %party, i32 0)
  store %Hero* %"call.Array<Hero>.[]50", %Hero** %ref51, align 8
  %"*ref52" = load %Hero*, %Hero** %ref51, align 8
  %"call.Hero.$53" = call %String @"Hero.$"(%Hero* %"*ref52")
  store %String %"call.Hero.$53", %String* %tmp54, align 8
  %call.String.chars55 = call i8* @String.chars(%String* %tmp54)
  %"call.Array<Hero>.[]56" = call %Hero* @"Array<Hero>.[]"(%"Array<Hero>"* %party, i32 0)
  store %Hero* %"call.Array<Hero>.[]56", %Hero** %ref57, align 8
  %"*ref58" = load %Hero*, %Hero** %ref57, align 8
  %"*ref.stats59" = getelementptr inbounds %Hero, %Hero* %"*ref58", i32 0, i32 1
  %"*ref.stats.hp60" = getelementptr inbounds %Stats, %Stats* %"*ref.stats59", i32 0, i32 0
  %"$*ref.stats.hp62" = load i32, i32* %"*ref.stats.hp60", align 4
  %call.snprintf63 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.12, i32 0, i32 0), i8* %call.String.chars55, i32 %"$*ref.stats.hp62")
  store i32 %call.snprintf63, i32* %format.size61, align 4
  %"$format.size64" = load i32, i32* %format.size61, align 4
  %ADD65 = add i32 %"$format.size64", 1
  %AS66 = sext i32 %ADD65 to i64
  %call.calloc68 = call i8* @calloc(i64 %AS66, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc68, i8** %format.text67, align 8
  %"$format.text69" = load i8*, i8** %format.text67, align 8
  %call.snprintf70 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text69", i64 %AS66, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.12, i32 0, i32 0), i8* %call.String.chars55, i32 %"$*ref.stats.hp62")
  %"$format.size71" = load i32, i32* %format.size61, align 4
  %ADD72 = add i32 %"$format.size71", 1
  %"$format.text73" = load i8*, i8** %format.text67, align 8
  %"$format.size74" = load i32, i32* %format.size61, align 4
  %16 = insertvalue %String zeroinitializer, i8* %"$format.text73", 0
  %17 = insertvalue %String %16, i32 %"$format.size74", 1
  %18 = insertvalue %String %17, i32 %ADD72, 2
  call void @"io::out"(%String %18)
  %19 = bitcast %String* %tmp54 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %19)
  %call.snprintf76 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.13, i32 0, i32 0), i32 ptrtoint (%Hero* getelementptr (%Hero, %Hero* null, i32 1) to i32), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf76, i32* %format.size75, align 4
  %"$format.size77" = load i32, i32* %format.size75, align 4
  %ADD78 = add i32 %"$format.size77", 1
  %AS79 = sext i32 %ADD78 to i64
  %call.calloc81 = call i8* @calloc(i64 %AS79, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc81, i8** %format.text80, align 8
  %"$format.text82" = load i8*, i8** %format.text80, align 8
  %call.snprintf83 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text82", i64 %AS79, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.13, i32 0, i32 0), i32 ptrtoint (%Hero* getelementptr (%Hero, %Hero* null, i32 1) to i32), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size84" = load i32, i32* %format.size75, align 4
  %ADD85 = add i32 %"$format.size84", 1
  %"$format.text86" = load i8*, i8** %format.text80, align 8
  %"$format.size87" = load i32, i32* %format.size75, align 4
  %20 = insertvalue %String zeroinitializer, i8* %"$format.text86", 0
  %21 = insertvalue %String %20, i32 %"$format.size87", 1
  %22 = insertvalue %String %21, i32 %ADD85, 2
  call void @"io::out"(%String %22)
  call void @"Array<EnemyType>.drop"(%"Array<EnemyType>"* %foes)
  %champ.name = getelementptr inbounds %Hero, %Hero* %champ, i32 0, i32 0
  %23 = bitcast %String* %champ.name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %23)
  %h.name = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 0
  %24 = bitcast %String* %h.name to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %24)
  call void @"Array<Hero>.drop"(%"Array<Hero>"* %party)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 034 — libft: isalnum

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

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

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 122
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 49
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i1 @isalnum(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.isalpha = call i1 @isalpha(i8 %"$c")
  %"$c1" = load i8, i8* %c, align 1
  %call.isdigit = call i1 @isdigit(i8 %"$c1")
  %OR = or i1 %call.isalpha, %call.isdigit
  ret i1 %OR
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isalnum = call i1 @isalnum(i8 49)
  br i1 %call.isalnum, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 035 — libft: isalpha

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"is alpha\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alpha\0A\00", align 1

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

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 122
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isalpha = call i1 @isalpha(i8 49)
  br i1 %call.isalpha, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 036 — libft: isdigit

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

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

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 49
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isdigit = call i1 @isdigit(i8 49)
  br i1 %call.isdigit, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

## 041 — link: an SDL2 window drawn from ura

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%Event = type { i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"ura + SDL2\00", align 1
@str.1 = private unnamed_addr constant [31 x i8] c"window closed after %d frames\0A\00", align 1

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

declare i32 @SDL_Init(i32)

declare i8* @SDL_CreateWindow(i8*, i32, i32, i32, i32, i32)

declare i8* @SDL_CreateRenderer(i8*, i32, i32)

declare i32 @SDL_SetRenderDrawColor(i8*, i8, i8, i8, i8)

declare i32 @SDL_RenderDrawPoint(i8*, i32, i32)

declare void @SDL_RenderPresent(i8*)

declare i32 @SDL_PollEvent(%Event*)

declare void @SDL_Delay(i32)

declare void @SDL_Quit()

define i32 @main() {
entry:
  %width = alloca i32, align 4
  %height = alloca i32, align 4
  %window = alloca i8*, align 8
  %renderer = alloca i8*, align 8
  %y = alloca i32, align 4
  %x = alloca i32, align 4
  %red = alloca i8, align 1
  %green = alloca i8, align 1
  %event = alloca %Event, align 8
  %running = alloca i1, align 1
  %frames = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 400, i32* %width, align 4
  store i32 300, i32* %height, align 4
  %call.SDL_Init = call i32 @SDL_Init(i32 32)
  %"$width" = load i32, i32* %width, align 4
  %"$height" = load i32, i32* %height, align 4
  %call.SDL_CreateWindow = call i8* @SDL_CreateWindow(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 805240832, i32 805240832, i32 %"$width", i32 %"$height", i32 0)
  store i8* %call.SDL_CreateWindow, i8** %window, align 8
  %"$window" = load i8*, i8** %window, align 8
  %call.SDL_CreateRenderer = call i8* @SDL_CreateRenderer(i8* %"$window", i32 -1, i32 0)
  store i8* %call.SDL_CreateRenderer, i8** %renderer, align 8
  store i32 0, i32* %y, align 4
  br label %while6

while6:                                           ; preds = %endloop, %entry
  %"$y" = load i32, i32* %y, align 4
  %"$height5" = load i32, i32* %height, align 4
  %LT = icmp slt i32 %"$y", %"$height5"
  br i1 %LT, label %while6.then, label %endloop1

while6.then:                                      ; preds = %while6
  store i32 0, i32* %x, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %while6.then
  %"$x" = load i32, i32* %x, align 4
  %"$width6" = load i32, i32* %width, align 4
  %LT7 = icmp slt i32 %"$x", %"$width6"
  br i1 %LT7, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$x8" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x8", 255
  %"$width9" = load i32, i32* %width, align 4
  %DIV = sdiv i32 %MUL, %"$width9"
  %AS = trunc i32 %DIV to i8
  store i8 %AS, i8* %red, align 1
  %"$y10" = load i32, i32* %y, align 4
  %MUL11 = mul i32 %"$y10", 255
  %"$height12" = load i32, i32* %height, align 4
  %DIV13 = sdiv i32 %MUL11, %"$height12"
  %AS14 = trunc i32 %DIV13 to i8
  store i8 %AS14, i8* %green, align 1
  %"$renderer" = load i8*, i8** %renderer, align 8
  %"$red" = load i8, i8* %red, align 1
  %"$green" = load i8, i8* %green, align 1
  %call.SDL_SetRenderDrawColor = call i32 @SDL_SetRenderDrawColor(i8* %"$renderer", i8 %"$red", i8 %"$green", i8 -128, i8 -1)
  %"$renderer15" = load i8*, i8** %renderer, align 8
  %"$x16" = load i32, i32* %x, align 4
  %"$y17" = load i32, i32* %y, align 4
  %call.SDL_RenderDrawPoint = call i32 @SDL_RenderDrawPoint(i8* %"$renderer15", i32 %"$x16", i32 %"$y17")
  %"$x18" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x18", 1
  store i32 %ADD, i32* %x, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"$y19" = load i32, i32* %y, align 4
  %ADD20 = add i32 %"$y19", 1
  store i32 %ADD20, i32* %y, align 4
  br label %while6

endloop1:                                         ; preds = %while6
  %"$renderer21" = load i8*, i8** %renderer, align 8
  call void @SDL_RenderPresent(i8* %"$renderer21")
  store %Event zeroinitializer, %Event* %event, align 4
  store i1 true, i1* %running, align 1
  store i32 0, i32* %frames, align 4
  br label %while8

while8:                                           ; preds = %endif3, %endloop1
  %"$running" = load i1, i1* %running, align 1
  br i1 %"$running", label %while8.then, label %endloop4

while8.then:                                      ; preds = %while8
  br label %while9

while9:                                           ; preds = %endif, %while8.then
  %call.SDL_PollEvent = call i32 @SDL_PollEvent(%Event* %event)
  %NQ = icmp ne i32 %call.SDL_PollEvent, 0
  br i1 %NQ, label %while9.then, label %endloop2

while9.then:                                      ; preds = %while9
  br label %if10

if10:                                             ; preds = %while9.then
  %event.type = getelementptr inbounds %Event, %Event* %event, i32 0, i32 0
  %"$event.type" = load i32, i32* %event.type, align 4
  %EQ = icmp eq i32 %"$event.type", 256
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  store i1 false, i1* %running, align 1
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while9

endloop2:                                         ; preds = %while9
  call void @SDL_Delay(i32 16)
  %"$frames" = load i32, i32* %frames, align 4
  %ADD22 = add i32 %"$frames", 1
  store i32 %ADD22, i32* %frames, align 4
  br label %if11

if11:                                             ; preds = %endloop2
  %"$frames23" = load i32, i32* %frames, align 4
  %GT = icmp sgt i32 %"$frames23", 180
  br i1 %GT, label %if11.then, label %endif3

if11.then:                                        ; preds = %if11
  store i1 false, i1* %running, align 1
  br label %endif3

endif3:                                           ; preds = %if11.then, %if11
  br label %while8

endloop4:                                         ; preds = %while8
  call void @SDL_Quit()
  %"$frames24" = load i32, i32* %frames, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str.1, i32 0, i32 0), i32 %"$frames24")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD25 = add i32 %"$format.size", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS26, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str.1, i32 0, i32 0), i32 %"$frames24")
  %"$format.size28" = load i32, i32* %format.size, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text, align 8
  %"$format.size31" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %1 = insertvalue %String %0, i32 %"$format.size31", 1
  %2 = insertvalue %String %1, i32 %ADD29, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

## 042 — link: a library that is not installed

```stderr
error: test.ura:3:1 library 'NoSuchLib' not found
  |
3 | link "NoSuchLib"
  | ^^^^
help: install it, or set URA_LIBC to the folder that holds it
error: aborting due to 1 error
1
```

## 043 — macros: @if / @elif / @else on os and installed()

```llvm
%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@name = global i8* null
@str = private unnamed_addr constant [13 x i8] c"%s sdl only\0A\00", align 1

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
  %arr = alloca [5 x i8], align 1
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [5 x i8] c"unix\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @name, align 8
  %"$name" = load i8*, i8** @name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$name")
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

## 044 — macros: @error stops the build with a message and a help

```stderr
error: test.ura:3:5 nothing_like_this is not installed
  |
3 |     @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"
  |     ^^^^^^
help: linux: sudo apt install libnothing-dev
error: aborting due to 1 error
1
```

## 045 — macros: @if on something it can't check

```stderr
error: test.ura:2:13 @if checks os, arch and installed("lib")
  |
2 | @if version == 2:
  |             ^^
help: use @if os == "linux" and installed("SDL2"):
error: aborting due to 1 error
1
```
