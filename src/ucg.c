/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        ucg.c                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/26 11:21:49                              */
/*                                                             */
/* *********************************************************** */

/*
TODO:
    + copy utils
    + copy ucg functions
    + copy ucg tests
    + print ucgs
    + copy llvm functions
*/

#include "./includes/libc.h"
#include "./includes/structs.h"
#include "./includes/llvm.h"
#include "./includes/macros.h"

#define UCG_MAX_INT_BITS 256

struct GUcg {
    Context context;
    Module module;
    Builder builder;
    TargetMachine machine;
    UcgOutputMode mode;

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
    static int index = 0;
    Ucg *new = arena_alloc(1, sizeof(Ucg));
    new->type = type;
    new->inst_index = index;
    index++;
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
    Ucg *new = ucg_new(FLOAT);
    new->bits = bits;
    new->is_val = true;
    return new;
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
    new->to_create->is_val = true;
    append(gb_ucg.insts, new);
    return new;
}

// LLVM ABI
void ucg_start(char *name, UcgOutputMode mode) {
    gb_ucg.context = LLVMContextCreate();
    zero_init(gb_ucg.runtime);
    zero_init(gb_ucg.libs);
    gb_ucg.module = LLVMModuleCreateWithNameInContext(name, gb_ucg.context);
    gb_ucg.builder = LLVMCreateBuilderInContext(gb_ucg.context);

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

    switch (gb_ucg.mode) {
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

void ucg_finish(void) {
    LLVMDisposeBuilder(gb_ucg.builder);
    LLVMDisposeModule(gb_ucg.module);
    LLVMDisposeTargetMachine(gb_ucg.machine);
    LLVMContextDispose(gb_ucg.context);
    gb_ucg.throws = false;
    gb_ucg.thrown = NULL;
    gb_ucg.error = NULL;
    gb_ucg.error_type = NULL;
    gb_ucg.error_types_count = 0;
    gb_ucg.error_ids_count = 0;
    gb_ucg.tries_count = 0;
    gb_ucg.blocks_count = 0;
}

TypeRef llvm_int_type(int bits) {
    return LLVMIntTypeInContext(gb_ucg.context, bits);
}

TypeRef llvm_float_type(int bits) {
    switch (bits) { // clang-format off
    case 16:	return LLVMHalfTypeInContext(gb_ucg.context);
    case 32:	return LLVMFloatTypeInContext(gb_ucg.context);
    case 64:	return LLVMDoubleTypeInContext(gb_ucg.context);
    case 80:	return LLVMX86FP80TypeInContext(gb_ucg.context);
    case 128:	return LLVMFP128TypeInContext(gb_ucg.context);
    default: { // clang-format on
        eprint("%s:%d invalid float type %d\n", bits);
        exit(1);
        break;
    }
    }
}

void llvm_create_value(Ucg *ucg) {
    Ucg *to_create = ucg->to_create;
    char *value = to_create->value;
}

void ucg_compile(Ucg *ucg) {
    assert(ucg && ucg->type);
    switch (ucg->type) {
    case INT: {
        ucg->rtype = llvm_int_type(ucg->bits);
        break;
    }
    case FLOAT: {
        ucg->rtype = llvm_float_type(ucg->bits);
        break;
    }
    case VAL_CREATE: {
        ucg_compile(ucg->data_type);
        Ucg *to_create = ucg->to_create;
        Type type = ucg->data_type->type;
        switch (type) {
        case INT: {
            break;
        }
        case FLOAT: {
            break;
        }
        default: {
            eprint("%s:%d handle this case %t\n", FILE, LINE, type);
            break;
        }
        }
        break;
    }
    default: {
        eprint("%s:%d %t\n", FILE, LINE, ucg->type);
        break;
    }
    }
}

void print_ucg(Ucg *ucg, char *prefix, char end) {
    if (ucg == NULL)
        return;
    print("%s%U", prefix, ucg);
    switch (ucg->type) {
    case VAL_CREATE: {
		print_ucg(ucg->left, " ", 0);
        print_ucg(ucg->right, " ", 0);
        break;
    }
    default: {
        print_ucg(ucg->left, " ", 0);
        print_ucg(ucg->right, " ", 0);
        break;
    }
    }
    print("%c", end);
}

void print_ucgs() {
    for (int i = 0; i < gb_ucg.insts_count; i++)
        print_ucg(gb_ucg.insts[i], "", '\n');
}

// TESTING
bool compile_test(int p) {
    // print(CYAN("Test %d: ================================\n"), p);
    // ucg_start(format("test%d", p));
    // bool r = run_test(p);
    // print_ucgs();
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
