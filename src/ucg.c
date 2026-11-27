/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        ucg.c                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/27 03:34:29                              */
/*                                                             */
/* *********************************************************** */


#include "./includes/libc.h"
#include "./includes/ucg.h"
#include "./includes/llvm.h"
#include "./includes/macros.h"

#define UCG_MAX_INT_BITS 256

struct {
    Context ctx;
    Module module;
    Builder builder;
    TargetMachine machine;

    char *dir;
    char *ll_path;
    char *asm_path;
    char *obj_path;
    char *exe_path;
    expand(char *, libs);

    Ucg *ints[UCG_MAX_INT_BITS + 1];
    Ucg *uints[UCG_MAX_INT_BITS + 1];
    Ucg *floats[UCG_MAX_INT_BITS + 1];

    char *error_msg;

    expand(Ucg *, insts);
    expand(Ucg *, runtime);

    expand(Ucg *, blocks);
    int clean_start;
    Ucg *clean_var;

    bool is_cleaning;
    bool throws;
    expand(Ucg *, tries);

    Ucg *thrown;
    Ucg *error;
    Ucg *error_type;

    expand(Ucg *, error_types);
    expand(Ucg *, error_ids);

    expand(UcgError, errors);
} gb_ucg;


// UCG ABI
Ucg *ucg_new(Type type) {
    assert(type != 0);
    Ucg *new = arena_alloc(1, sizeof(Ucg));
    new->type = type;

    return new;
}

Ucg *ucg_int_type(int bits) {
    assert(bits > 0 && bits < UCG_MAX_INT_BITS + 1);
    Ucg *new = gb_ucg.ints[bits];
    if (new)
        return new;

    new = ucg_new(INT);
    new->bits = bits;
    return (gb_ucg.ints[bits] = new);
}

Ucg *ucg_uint_type(int bits) {
    assert(bits > 0 && bits < UCG_MAX_INT_BITS + 1);
    Ucg *new = gb_ucg.uints[bits];
    if (new)
        return new;

    new = ucg_new(INT);
    new->bits = bits;
    new->is_uint = true;
    return (gb_ucg.uints[bits] = new);
}

Ucg *ucg_float_type(int bits) {
    assert(bits > 0 && bits < 128 + 1);
    Ucg *new = gb_ucg.floats[bits];
    if (new)
        return new;

    new = ucg_new(FLOAT);
    new->bits = bits;
    new->is_val = true;
    return (gb_ucg.floats[bits] = new);
}

Ucg *ucg_ref_type(Ucg *to_ref) {
    Ucg *new = ucg_new(REF);
    new->to_ref = to_ref;
    return new;
}

Ucg *ucg_array_type(Ucg *elem, int count) {
    Ucg *new = ucg_new(ARRAY);
    new->arr.elem = elem;
    new->arr.items_count = count;
    return new;
}

Ucg *ucg_numeric_value(Ucg *data_type, char *value) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->data_type = data_type;
    new->to_create = ucg_new(data_type->type);
    new->to_create->value = value;
    new->to_create->bits = data_type->bits;
    new->to_create->is_val = true;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_proto(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_new(FN_DEC);
    new->name = name;
    new->fn.ret = ret;

    for (int i = 0; i < params_count; i++) {
        Ucg *param = params[i];
        if (param->type == VARIADIC) {
            new->fn.is_varg = true;
            break;
        } else
            append(new->fn.params, param);
    }
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_entry(Ucg *fn) {
    Ucg *new = ucg_new(FN_ENTRY);
    new->to_enter = fn;
    append(gb_ucg.insts, new);
    // append(gb_ucg.blocks, fn);
    return new;
}

Ucg *ucg_fn(char *name, Ucg *ret, Ucg **params, int params_count) {
    Ucg *new = ucg_proto(name, ret, params, params_count);
    ucg_entry(new);
    return new;
}

Ucg *ucg_ret(Ucg *ucg) {
    Ucg *new = ucg_new(RETURN);
    new->to_ret = ucg;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_end(Ucg *stmt) {
    // ucg_close_block();
    Ucg *new = ucg_new(END);
    new->to_end = stmt;
    while (stmt) {
        stmt->end = new;
        stmt = stmt->next;
    }
    append(gb_ucg.insts, new);
    return new;
}

// LLVM ABI
void ucg_asm_start(char *name, UcgOutputMode mode) {
    gb_ucg.ctx = LLVMContextCreate();
    zero_init(gb_ucg.runtime);
    zero_init(gb_ucg.libs);
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.ctx);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.ctx);

    if (LLVMInitializeNativeTarget()) {
        eprint("%s:%d Failed to initialize native target\n", FILE, LINE);
        exit(1);
    }

    if (LLVMInitializeNativeAsmPrinter()) {
        eprint("%s:%d Failed to initialize native asm printer\n", FILE, LINE);
        exit(1);
    }

    char *target_name = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(gb_ucg.module, target_name);

    Target target;
    char *error = NULL;

    if (LLVMGetTargetFromTriple(target_name, &target, &error)) {
        eprint("%s:%d GetTargetFromTriple error: %s\n", FILE, LINE, error);
        exit(1);
    }
    RelocMode reloc = LLVMRelocDefault;

    switch (mode) {
    case OUTPUT_EXE: {
        reloc = LLVMRelocDefault;
        break;
    }
    case OUTPUT_SHARED: {
        reloc = LLVMRelocPIC;
        break;
    }
    case OUTPUT_STATIC: {
        reloc = LLVMRelocDefault;
        break;
    }
    }
    gb_ucg.machine = LLVMCreateTargetMachine(
        target,
        target_name,
        "generic",
        "",
        LLVMCodeGenLevelDefault,
        reloc,
        LLVMCodeModelDefault
    );

    if (!gb_ucg.machine) {
        eprint("%s:%d Failed to create target machine\n", FILE, LINE);
        LLVMDisposeMessage(target_name);
        exit(1);
    }

    TargetData layout = LLVMCreateTargetDataLayout(gb_ucg.machine);
    LLVMSetModuleDataLayout(gb_ucg.module, layout);
    LLVMDisposeTargetData(layout);
    LLVMDisposeMessage(target_name);
}

void ucg_asm_print() {
    LLVMDumpModule(gb_ucg.module);
}

void ucg_asm_end(void) {
    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMDisposeTargetMachine(gb_ucg.machine);
    LLVMContextDispose(gb_ucg.ctx);
    gb_ucg.module = NULL;
    gb_ucg.throws = false;
    gb_ucg.thrown = NULL;
    gb_ucg.error = NULL;
    gb_ucg.error_type = NULL;
    gb_ucg.error_types_count = 0;
    gb_ucg.error_ids_count = 0;
    gb_ucg.tries_count = 0;
    gb_ucg.blocks_count = 0;
}

void ucg_write_ll() {
}

void ucg_bin() {
}

// void llvm_create_value(Ucg *ucg) {
//     Ucg *to_create = ucg->to_create;
//     char *value = to_create->value;
// }

bool is_bloc_terminated() {
    Bloc bloc = LLVMGetInsertBlock(gb_ucg.builder);
    return bloc != NULL && LLVMGetBasicBlockTerminator(bloc) != NULL;
}

Ucg *llvm_compile(Ucg *ucg) {
    if (ucg->is_set)
        return ucg;
    Type type = ucg->type;
    Module mod = gb_ucg.module;
    Context ctx = gb_ucg.ctx;
    Builder lmb = gb_ucg.builder;

    switch (type) {
    case INT: {
        if (ucg->is_val) {
            print("%s:%d implement it\n", FILE, LINE);
            exit(1);
        } else {
            ucg->lltype = LLVMIntTypeInContext(ctx, ucg->bits);
        }
        break;
    }
    case FLOAT: {
        if (ucg->is_val) {
            print("%s:%d implement it\n", FILE, LINE);
            exit(1);
        } else {
            switch (ucg->bits) { // clang-format off
			case 16:  ucg->lltype = LLVMHalfTypeInContext(ctx);    break;
			case 32:  ucg->lltype = LLVMFloatTypeInContext(ctx);   break;
			case 64:  ucg->lltype = LLVMDoubleTypeInContext(ctx);  break;
			case 80:  ucg->lltype = LLVMX86FP80TypeInContext(ctx); break;
			case 128: ucg->lltype = LLVMFP128TypeInContext(ctx);   break;
			default: { // clang-format on
                eprint("%s:%d invalid float type %d\n", FILE, LINE, ucg->bits);
                exit(1);
                break;
            }
            }
        }
        break;
    }
    case VAL_CREATE: {
        Ucg *to_create = ucg->to_create;
        Ucg *data_type = ucg->data_type;
        Value elem = NULL;
        int base = 10; // TODO: handle base

        llvm_compile(ucg->data_type);
        switch (data_type->type) {
        case INT: {
            elem = LLVMConstIntOfString(data_type->lltype, to_create->value, base);
            break;
        }
        case FLOAT: {
            elem = LLVMConstRealOfString(data_type->lltype, to_create->value);
            break;
        }
        default: {
            eprint("%s:%d handle this case %t\n", FILE, LINE, type);
            break;
        }
        }
        ucg->elem = to_create->elem = elem;
        break;
    }
    case FN_DEC: {
        llvm_compile(ucg->fn.ret);
        char *name = ucg->name;
        TypeRef ret_type = ucg->fn.ret->lltype;
        bool is_varg = ucg->fn.is_varg;

        expand(TypeRef, types);
        zero_init(types);

        for (int i = 0; i < ucg->fn.params_count; i++) {
        }

        ucg->lltype = LLVMFunctionType(ret_type, types, types_count, is_varg);
        ucg->elem = LLVMGetNamedFunction(mod, name);
        if (ucg->elem == NULL || LLVMGlobalGetValueType(ucg->elem) != ucg->lltype)
            ucg->elem = LLVMAddFunction(mod, name, ucg->lltype);
        break;
    }
    case FN_ENTRY: {
        Value fn = ucg->to_enter->elem;
        ucg->bloc = LLVMAppendBasicBlockInContext(ctx, fn, "entry");
        LLVMPositionBuilderAtEnd(lmb, ucg->bloc);
        // in entry declare params as variables
        // for (int i = 0; i < ucg->fn.params_count; i++) {
        // // Ucg *param = ir->fn.params[i];
        // // param->elem = LLVMGetParam(ir->elem, i);
        // }
        break;
    }
    case RETURN: {
        // assert(0);
        llvm_compile(ucg->to_ret);
        if (is_bloc_terminated())
            break;
        LLVMBuildRet(gb_ucg.builder, ucg->to_ret->elem);
        break;
    }
    case END: {
        // if it's fn: return default
        Ucg *to_end = ucg->to_end;
        if (to_end->type == FN_DEC) {
            if (is_bloc_terminated())
                break;
            if (to_end->fn.ret->type == VOID)
                LLVMBuildRetVoid(gb_ucg.builder);
            else {
                assert(to_end->fn.ret->is_set);
                LLVMBuildRet(gb_ucg.builder, LLVMConstNull(to_end->fn.ret->lltype));
            }
            break;
        }
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, type);
        break;
    }
    }
    ucg->is_set = true;
    return ucg;
}

void ucg_asm_compile() {
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *ucg = gb_ucg.insts[i];
        if (!includes(ucg->type, FN_DEC, PROTO, 0))
            continue;
        llvm_compile(ucg);
    }
    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *ucg = gb_ucg.insts[i];
        if (includes(ucg->type, FN_DEC, PROTO, 0))
            continue;
        llvm_compile(ucg);
    }
}

void ucg_print() {
    int index = 0;
    int space = 0;

    for (int i = 0; i < gb_ucg.insts_count; i++) {
        Ucg *ucg = gb_ucg.insts[i];
        ucg->inst_index = index;

        print("[%.3d] ", index++);
        if (ucg->type == END)
            space -= TAB;
        print("%*s ", space, "");
        if (includes(ucg->type, FN_DEC, 0))
            space += TAB;

        switch (ucg->type) {
        case VAL_CREATE: {
            print("%U %U %U\n", ucg, ucg->to_create, ucg->data_type);
            break;
        }
        case FN_DEC: {
            print("%U RET %U\n", ucg, ucg->fn.ret);
            break;
        }
        case RETURN: {
            print("%U ", ucg);
            if (ucg->to_ret)
                print("[%.3d]", ucg->to_ret->inst_index);
            print("\n");
            break;
        }
        default: {
            print("%U ", ucg);
            if (ucg->left)
                print("%U ", ucg->left);
            if (ucg->right)
                print("%U ", ucg->right);
            print("\n");
            break;
        }
        }
    }
}

void foo() {
#if 0
	// Get target triple
	char *triple = LLVMGetDefaultTargetTriple();
	
	printf("Target: %s\n", triple);
	
	// Find target
	LLVMTargetRef target;
	char *error = NULL;
	
	if (LLVMGetTargetFromTriple(triple, &target, &error)) {
		fprintf(stderr, "Target error: %s\n", error);
		LLVMDisposeMessage(error);
		LLVMDisposeMessage(triple);
		return 1;
	}
	
	// Create target machine
	LLVMTargetMachineRef target_machine = LLVMCreateTargetMachine(
		target,
		triple,
		"generic",
		"",
		LLVMCodeGenLevelDefault,
		LLVMRelocDefault,
		LLVMCodeModelDefault
	);
	
	if (!target_machine) {
		fprintf(stderr, "Failed to create target machine\n");
		LLVMDisposeMessage(triple);
		return 1;
	}
	
	// Create context and module
	LLVMContextRef context = LLVMContextCreate();
	
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("ucg_module", context);
	
	LLVMSetTarget(module, triple);
	
	// Get target data layout (LLVM 12.0.1)
	LLVMTargetDataRef target_data = LLVMCreateTargetDataLayout(target_machine);
	
	LLVMSetModuleDataLayout(module, target_data);
	
	// Create builder
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	
	// Define main: i32 @main()
	LLVMTypeRef i32_type = LLVMInt32TypeInContext(context);
	
	LLVMTypeRef main_type = LLVMFunctionType(i32_type, NULL, 0, 0);
	
	LLVMValueRef main_func = LLVMAddFunction(module, "main", main_type);
	
	LLVMBasicBlockRef entry =
		LLVMAppendBasicBlockInContext(context, main_func, "entry");
	
	LLVMPositionBuilderAtEnd(builder, entry);
	
	// Create i256 type
	LLVMTypeRef i256_type = LLVMIntTypeInContext(context, 256);
	
	// Query target ABI size and alignment
	unsigned size = LLVMABISizeOfType(target_data, i256_type);
	
	unsigned alignment = LLVMABIAlignmentOfType(target_data, i256_type);
	
	printf("i256 size: %u bytes\n", size);
	printf("i256 ABI alignment: %u bytes\n", alignment);
	
	// Allocate i256
	LLVMValueRef value = LLVMBuildAlloca(builder, i256_type, "value");
	
	LLVMSetAlignment(value, alignment);
	
	LLVMTypeRef f128 = LLVMFP128TypeInContext(context);
	
	LLVMValueRef value =
		LLVMConstRealOfString(f128, "1.234567890123456789012345678901234");
	
	// Create huge decimal integer constant
	LLVMValueRef huge_value = LLVMConstIntOfString(
		i256_type, "12345678901234567890123456789012345678901234567890", 10
	);
	
	// Store using target ABI alignment
	LLVMValueRef store = LLVMBuildStore(builder, huge_value, value);
	
	LLVMSetAlignment(store, alignment);
	
	// Return 0
	LLVMBuildRet(builder, LLVMConstInt(i32_type, 0, 0));
	
	// Verify module
	if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
		fprintf(stderr, "Verification error: %s\n", error);
		LLVMDisposeMessage(error);
		return 1;
	}
	
	// Print LLVM IR
	char *ir = LLVMPrintModuleToString(module);
	
	printf("\nGenerated LLVM IR:\n\n%s\n", ir);
	
	// Cleanup
	LLVMDisposeMessage(ir);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMDisposeTargetData(target_data);
	LLVMDisposeTargetMachine(target_machine);
	LLVMContextDispose(context);
	LLVMDisposeMessage(triple);
#endif
}

int foo_jit(void) {
#if 0
    LLVMOrcLLJITRef jit = NULL;
    LLVMOrcThreadSafeContextRef tsc = NULL;
    LLVMBuilderRef builder = NULL;
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMErrorRef err = LLVMOrcCreateLLJIT(&jit, LLVMOrcCreateLLJITBuilder());

    if (err) {
        char *msg = LLVMGetErrorMessage(err);
        fprintf(stderr, "JIT creation: %s\n", msg);
        LLVMDisposeErrorMessage(msg);
        return 1;
    }

    // Create LLVM module
    tsc = LLVMOrcCreateNewThreadSafeContext();
    if (!tsc)
        goto fail;

    LLVMContextRef ctx = LLVMOrcThreadSafeContextGetContext(tsc);
    LLVMModuleRef mod = LLVMModuleCreateWithNameInContext("ura", ctx);

    if (!mod)
        goto fail;

	builder = LLVMCreateBuilderInContext(ctx);
	
    // Declare: i32 test()
    LLVMTypeRef i32 = LLVMInt32TypeInContext(ctx);
    LLVMTypeRef fn_type = LLVMFunctionType(i32, NULL, 0, 0);
    LLVMValueRef fn = LLVMAddFunction(mod, "test", fn_type);

    // Function body: return 123
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx, fn, "entry");

    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMBuildRet(builder, LLVMConstInt(i32, 123, 0));

    LLVMDisposeBuilder(builder);
    builder = NULL;

    // Transfer module ownership to ThreadSafeModule
    LLVMOrcThreadSafeModuleRef tsm = LLVMOrcCreateNewThreadSafeModule(mod, tsc);

    if (!tsm)
        goto fail;

    LLVMOrcDisposeThreadSafeContext(tsc);
    tsc = NULL;

    // Add module to JIT
    err = LLVMOrcLLJITAddLLVMIRModule(jit, LLVMOrcLLJITGetMainJITDylib(jit), tsm);

    if (err) {
        char *msg = LLVMGetErrorMessage(err);
        fprintf(stderr, "JIT module: %s\n", msg);
        LLVMDisposeErrorMessage(msg);
        goto fail;
    }

    // Lookup function
    LLVMOrcJITTargetAddress addr = 0;

    err = LLVMOrcLLJITLookup(jit, &addr, "test");

    if (err) {
        char *msg = LLVMGetErrorMessage(err);
        fprintf(stderr, "JIT lookup: %s\n", msg);
        LLVMDisposeErrorMessage(msg);
        goto fail;
    }

    // Execute
    int (*test)(void) = (int (*)(void))(uintptr_t)addr;

    printf("%d\n", test()); // 123

    LLVMOrcDisposeLLJIT(jit);
    return 0;

fail:
    if (builder)
        LLVMDisposeBuilder(builder);

    if (tsc)
        LLVMOrcDisposeThreadSafeContext(tsc);

    LLVMOrcDisposeLLJIT(jit);
#endif
    return 1;
}

// TESTING
bool compile_test(int p) {
    // print(CYAN("Test %d: ================================\n"), p);
    // ucg_start(format("test%d", p));
    // bool r = run_test(p);
    // ucg_print();
    // // ucg_compile();
    // // mkdir2("build");
    // // ucg_path("build/out.ll");
    // // ucg_bin_path(r ? "build/exe.out" : NULL);
    // // ucg_gen();
    // // if (r)
    // //     ucg_run_bin();
    // ucg_finish();
    // return r;
    return true;
}

// UTILS
__attribute__((sentinel)) bool includes(Type to_find, ...) {
    va_list ap;
    va_start(ap, to_find);
    Type curr = va_arg(ap, Type);
    while (curr) {
        if (curr == to_find)
            return true;
        curr = va_arg(ap, Type);
    }
    return false;
}

// UCG ERRORS
void ucg_error(ucgFile *file, int s, int e, char *msg, char *help) {
    UcgError error = { .file = file, .s = s, .e = e, .msg = msg, .help = help };
    append(gb_ucg.errors, error);
}

void ucg_check_errors(void) {
    if (gb_ucg.errors_count == 0)
        return;
    for (int i = 0; i < gb_ucg.errors_count; i++) {
        UcgError *error = &gb_ucg.errors[i];
        if (error->file == NULL) {
            eprint("%s: %s\n", RED("error"), error->msg);
            if (error->help)
                eprint("%s: %s\n", CYAN("help"), error->help);
            continue;
        }
        char *buff = error->file->buff;
        char *path = relative_path(error->file->path);
        int line = 1;
        for (int j = 0; j < error->s; j++)
            line += buff[j] == '\n';
        int start = error->s;
        while (start > 0 && buff[start - 1] != '\n')
            start--;
        int end = error->s;
        while (buff[end] && buff[end] != '\n')
            end++;
        int col = error->s - start;
        int width = snprintf(NULL, 0, "%d", line);

        eprint("%s: %s\n", RED("error"), error->msg);
        eprint("at %s:%d:%d\n", path, line, col + 1);
        eprint("%*s |\n", width, "");
        eprint("%d | %.*s\n", line, end - start, buff + start);
        eprint("%*s | %*s", width, "", col, "");
        for (int j = error->s; j < error->e || j == error->s; j++)
            eprint(RED("^"));
        eprint("\n");
        if (error->help)
            eprint("%s: %s\n", CYAN("help"), error->help);
    }
    eprint(
        "%s: aborting due to %d error%s\n",
        RED("error"),
        (int)gb_ucg.errors_count,
        gb_ucg.errors_count > 1 ? "s" : ""
    );
    exit(1);
}
