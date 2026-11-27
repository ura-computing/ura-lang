/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        ucg.c                           */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/24 21:39:29    */
/*                                                             */
/*   updated: 2026/11/27 12:32:52                              */
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
    expand(ucgFile *, files);

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

Ucg *ucg_struct_type(char *name) {
    Ucg *new = ucg_new(STRUCT);
    new->name = name;
    return new;
}

Ucg *ucg_numeric_value(Ucg *data_type, char *value) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->data_type = data_type;
    new->value = value;
    new->value_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_array_value(Ucg *elem, int count) {
    Ucg *new = ucg_new(VAL_CREATE);
    new->data_type = ucg_array_type(elem, count);
    new->value_type = new->data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_string_value(char *text) {
    Ucg *type = ucg_int_type(8);
    int count = strlen(text) + 1;
    Ucg *new = ucg_array_value(type, count);
    for (int i = 0; i < count; i++)
        append(new->arr.items, ucg_numeric_value(type, format("%d", text[i])));
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

Ucg *ucg_ret(Ucg *ucg) {
    Ucg *new = ucg_new(RETURN);
    new->to_ret = ucg;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_var(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(VAR);
    new->name = name;
    new->data_type = data_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_assign(Ucg *var, Ucg *value) {
    Ucg *new = ucg_new(ASSIGN);
    new->left = var;
    new->right = value;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_load(Ucg *var) {
    Ucg *new = ucg_new(DREF);
    new->to_dref = var;
    new->value_type = var ? var->data_type : NULL;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_get_attr(Ucg *base, char *name) {
    Ucg *type = base->data_type;
    for (int i = 0; i < type->obj.attrs_count; i++) {
        Ucg *attr = type->obj.attrs[i];
        if (!strcmp2(attr->name, name))
            continue;
        Ucg *new = ucg_new(ATTR);
        new->name = format("%s.%s", base->name, name);
        new->data_type = attr->data_type;
        new->base = base;
        new->obj.index = i;
        append(gb_ucg.insts, new);
        return new;
    }
    return NULL;
}

Ucg *ucg_get_item(Ucg *base, Ucg *index) {
    Ucg *new = ucg_new(ACCESS);
    new->name = format("%s[]", base->name);
    new->data_type = base->data_type->arr.elem;
    new->base = base;
    new->index = index;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_ref(Ucg *to_ref) {
    Ucg *new = ucg_new(OWN);
    new->name = format("&%s", to_ref->name);
    new->to_ref = to_ref;
    new->value_type = ucg_ref_type(to_ref->data_type);
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_point_to(Ucg *ref) {
    Ucg *new = ucg_new(POINT_TO);
    new->name = ref->name;
    new->data_type = ref->data_type->to_ref;
    new->base = ref;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_stack_alloc(Ucg *elem, Ucg *count) {
    Ucg *new = ucg_new(STACK_ALLOC);
    new->data_type = elem;
    new->index = count;
    new->value_type = ucg_array_type(elem, 0);
    append(gb_ucg.insts, new);
    return new;
}

// TODO: to be verified
Ucg *ucg_stack_array(Ucg *elem, Ucg **sizes, int count) {
    Ucg *i32 = ucg_int_type(32);
    expand(Ucg *, totals);
    zero_init(totals);
    append(totals, sizes[0]);
    for (int i = 1; i < count; i++)
        append(totals, ucg_op(MUL, totals[i - 1], sizes[i]));

    Ucg *type = ucg_array_type(elem, 0);
    Ucg *below = ucg_var("", type);
    ucg_assign(below, ucg_stack_alloc(elem, totals[count - 1]));
    for (int level = count - 2; level >= 0; level--) {
        Ucg *table = ucg_var("", ucg_array_type(type, 0));
        ucg_assign(table, ucg_stack_alloc(type, totals[level]));
        Ucg *index = ucg_var("", i32);
        ucg_assign(index, ucg_numeric_value(i32, "0"));

        Ucg *loop = ucg_branch(LOOP, NULL);
        ucg_then(loop, ucg_op(LT, ucg_load(index), totals[level]));
        Ucg *offset = ucg_op(MUL, ucg_load(index), sizes[level + 1]);
        Ucg *row = ucg_ref(ucg_get_item(below, offset));
        ucg_assign(ucg_get_item(table, ucg_load(index)), row);
        Ucg *one = ucg_numeric_value(i32, "1");
        ucg_assign(index, ucg_op(ADD, ucg_load(index), one));
        ucg_end(loop);

        below = table;
        type = ucg_array_type(type, 0);
    }
    return ucg_load(below);
}

Ucg *ucg_branch(Type type, Ucg *prev) {
    Ucg *new = ucg_new(type);
    while (prev && prev->next)
        prev = prev->next;
    if (prev)
        prev->next = new;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_then(Ucg *stmt, Ucg *cond) {
    Ucg *new = ucg_new(THEN);
    stmt->cond = cond;
    new->stmt = stmt;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_cast(Ucg *value, Ucg *to_type) {
    Ucg *new = ucg_new(AS);
    new->left = value;
    new->to_type = to_type;
    new->value_type = to_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_widen(Ucg *value, Ucg *type) {
    Ucg *from = value ? value->value_type : NULL;
    bool is_int = from && type && from->type == INT && type->type == INT;
    if (!is_int || from == type)
        return value;
    bool is_wider = from->bits < type->bits;
    bool same_sign = from->is_uint == type->is_uint;
    bool is_bool = from->bits == 1;
    bool widens = is_wider && !is_bool && (same_sign || from->is_uint);
    return widens ? ucg_cast(value, type) : value;
}

Ucg *ucg_op(Type type, Ucg *left, Ucg *right) {
    Ucg *new = ucg_new(type);
    new->left = left;
    new->right = right;
    bool is_cmp = includes(type, GT, LT, GE, LE, EQ, NQ, NULL);
    bool is_logic = includes(type, AND, OR, AND_LIT, OR_LIT, NULL);
    is_cmp = is_cmp || is_logic;
    Ucg *left_type = left ? left->value_type : NULL;
    new->value_type = is_cmp ? ucg_uint_type(1) : left_type;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_param(char *name, Ucg *data_type) {
    Ucg *new = ucg_new(PARAM);
    new->name = name;
    new->data_type = data_type;
    return new;
}

Ucg *ucg_call(Ucg *fn, Ucg **params, int params_count) {
    Ucg *new = ucg_new(FN_CALL);
    new->to_call = fn;
    for (int i = 0; i < params_count; i++)
        append(new->fn.params, params[i]);
    new->value_type = fn->fn.ret;
    append(gb_ucg.insts, new);
    return new;
}

Ucg *ucg_jump(Type type, Ucg *loop) {
    Ucg *new = ucg_new(type);
    new->stmt = loop;
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


void ucg_bin(char *source) {
    Module mod = gb_ucg.module;
    TargetMachine machine = gb_ucg.machine;
    char *slash = strrchr(source, '/');
    char *name = strdup2(slash ? slash + 1 : source);
    char *dot = strrchr(name, '.');
    if (dot)
        *dot = '\0';
    char *bin = WINDOWS_MACHINE ? format("%s.exe", name) : name;
    char *obj = format("%s.o", bin);

    char *error = NULL;
    bool failed =
        LLVMTargetMachineEmitToFile(machine, mod, obj, LLVMObjectFile, &error);
    if (failed)
        ucg_error(NULL, 0, 0, format("can't write %s: %s", obj, error), NULL);
    ucg_check_errors();

    char *link[] = { "clang", obj, "-o", bin, NULL };
    if (run_command(link, NULL) != 0)
        ucg_error(NULL, 0, 0, format("can't link %s", bin), "is clang installed?");
    remove(obj);
    ucg_check_errors();
}

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
        // clang-format off
    case VOID: ucg->lltype = LLVMVoidTypeInContext(ctx);          break;
    case INT:  ucg->lltype = LLVMIntTypeInContext(ctx, ucg->bits); break;
    // clang-format on
    case FLOAT: {
        switch (ucg->bits) {
            // clang-format off
        case 16:  ucg->lltype = LLVMHalfTypeInContext(ctx);    break;
        case 32:  ucg->lltype = LLVMFloatTypeInContext(ctx);   break;
        case 64:  ucg->lltype = LLVMDoubleTypeInContext(ctx);  break;
        case 80:  ucg->lltype = LLVMX86FP80TypeInContext(ctx); break;
        case 128: ucg->lltype = LLVMFP128TypeInContext(ctx);   break;
        // clang-format on
        default:
            assert(0 && "float bits are checked by the front end");
        }
        break;
    }
    case ARRAY: {
        llvm_compile(ucg->arr.elem);
        TypeRef elem = ucg->arr.elem->lltype;
        int count = ucg->arr.items_count;
        ucg->lltype = count ? LLVMArrayType(elem, count) : LLVMPointerType(elem, 0);
        break;
    }
    case STRUCT: {
        ucg->lltype = LLVMStructCreateNamed(ctx, ucg->name);
        expand(TypeRef, types);
        zero_init(types);
        for (int i = 0; i < ucg->obj.attrs_count; i++)
            append(types, llvm_compile(ucg->obj.attrs[i]->data_type)->lltype);
        LLVMStructSetBody(ucg->lltype, types, types_count, false);
        break;
    }
    case ATTR: {
        Ucg *base = ucg->base;
        TypeRef lltype = base->data_type->lltype;
        int index = ucg->obj.index;
        ucg->elem = LLVMBuildStructGEP2(lmb, lltype, base->elem, index, ucg->name);
        break;
    }
    case ACCESS: {
        Ucg *array = ucg->base->data_type;
        TypeRef elem = array->arr.elem->lltype;
        Value index = ucg->index->elem;
        Value pointer = ucg->base->elem;
        if (array->arr.items_count) {
            Value zero = LLVMConstInt(LLVMInt32TypeInContext(ctx), 0, 0);
            Value indexes[] = { zero, index };
            ucg->elem = LLVMBuildGEP2(lmb, array->lltype, pointer, indexes, 2, "");
            break;
        }
        pointer = LLVMBuildLoad2(lmb, array->lltype, pointer, "");
        ucg->elem = LLVMBuildGEP2(lmb, elem, pointer, &index, 1, "");
        break;
    }
    case OWN: {
        ucg->elem = ucg->to_ref->elem;
        break;
    }
    case REF: {
        llvm_compile(ucg->to_ref);
        ucg->lltype = LLVMPointerType(ucg->to_ref->lltype, 0);
        break;
    }
    case POINT_TO: {
        Ucg *ref = ucg->base;
        ucg->elem = LLVMBuildLoad2(lmb, ref->data_type->lltype, ref->elem, ref->name);
        break;
    }
    case STACK_ALLOC: {
        llvm_compile(ucg->data_type);
        TypeRef elem = ucg->data_type->lltype;
        TypeRef i64 = LLVMInt64TypeInContext(ctx);
        Value count = ucg->index->elem;
        ucg->elem = LLVMBuildArrayAlloca(lmb, elem, count, "");
        Value count64 = LLVMBuildZExt(lmb, count, i64, "");
        Value bytes = LLVMBuildMul(lmb, LLVMSizeOf(elem), count64, "");
        Value zero = LLVMConstInt(LLVMInt8TypeInContext(ctx), 0, 0);
        LLVMBuildMemSet(lmb, ucg->elem, zero, bytes, 1);
        break;
    }
    case VAL_CREATE: {
        Ucg *data_type = ucg->data_type;
        int base = 10; // TODO: handle base
        llvm_compile(data_type);
        TypeRef lltype = data_type->lltype;

        if (data_type->type == ARRAY) {
            TypeRef item_type = data_type->arr.elem->lltype;
            expand(Value, values);
            zero_init(values);
            for (int i = 0; i < ucg->arr.items_count; i++)
                append(values, llvm_compile(ucg->arr.items[i])->elem);
            Value init = LLVMConstArray(item_type, values, values_count);
            Value global = LLVMAddGlobal(mod, lltype, "");
            LLVMSetInitializer(global, init);
            LLVMSetGlobalConstant(global, true);
            LLVMSetLinkage(global, LLVMPrivateLinkage);
            TypeRef pointer = LLVMPointerType(item_type, 0);
            ucg->elem = LLVMConstPointerCast(global, pointer);
        } else if (data_type->type == FLOAT)
            ucg->elem = LLVMConstRealOfString(lltype, ucg->value);
        else
            ucg->elem = LLVMConstIntOfString(lltype, ucg->value, base);
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
            Ucg *param = ucg->fn.params[i];
            llvm_compile(param->data_type);
            append(types, param->data_type->lltype);
        }

        ucg->lltype = LLVMFunctionType(ret_type, types, types_count, is_varg);
        ucg->elem = LLVMGetNamedFunction(mod, name);
        if (ucg->elem == NULL || LLVMGlobalGetValueType(ucg->elem) != ucg->lltype)
            ucg->elem = LLVMAddFunction(mod, name, ucg->lltype);
        break;
    }
    case FN_ENTRY: {
        Value fn = ucg->to_enter->elem;
        ucg->to_enter->caller_bloc = LLVMGetInsertBlock(lmb);
        ucg->bloc = LLVMAppendBasicBlockInContext(ctx, fn, "entry");
        LLVMPositionBuilderAtEnd(lmb, ucg->bloc);
        // in entry declare params as variables
        for (int i = 0; i < ucg->to_enter->fn.params_count; i++) {
            Ucg *param = ucg->to_enter->fn.params[i];
            TypeRef lltype = param->data_type->lltype;
            param->elem = LLVMBuildAlloca(lmb, lltype, param->name);
            LLVMBuildStore(lmb, LLVMGetParam(fn, i), param->elem);
        }
        break;
    }
    case FN_CALL: {
        Ucg *fn = ucg->to_call;
        expand(Value, params);
        zero_init(params);
        for (int i = 0; i < ucg->fn.params_count; i++)
            append(params, ucg->fn.params[i]->elem);
        int count = params_count;
        ucg->elem = LLVMBuildCall2(lmb, fn->lltype, fn->elem, params, count, "");
        break;
    }
    case VAR: {
        llvm_compile(ucg->data_type);
        ucg->elem = LLVMBuildAlloca(lmb, ucg->data_type->lltype, ucg->name);
        break;
    }
    case ASSIGN: {
        LLVMBuildStore(lmb, ucg->right->elem, ucg->left->elem);
        break;
    }
    case DREF: {
        Ucg *var = ucg->to_dref;
        ucg->elem = LLVMBuildLoad2(lmb, var->data_type->lltype, var->elem, var->name);
        break;
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD: {
        // clang-format on
        OpCode opcodes[LAST + 1] = {
            [ADD] = LLVMAdd,  [SUB] = LLVMSub,  [MUL] = LLVMMul,
            [DIV] = LLVMSDiv, [MOD] = LLVMSRem,
        };
        OpCode float_opcodes[LAST + 1] = {
            [ADD] = LLVMFAdd, [SUB] = LLVMFSub, [MUL] = LLVMFMul,
            [DIV] = LLVMFDiv, [MOD] = LLVMFRem,
        };
        Ucg *value_type = ucg->left->value_type;
        if (value_type->is_uint) {
            opcodes[DIV] = LLVMUDiv;
            opcodes[MOD] = LLVMURem;
        }
        bool is_float = value_type->type == FLOAT;
        OpCode op = is_float ? float_opcodes[type] : opcodes[type];
        Value left = ucg->left->elem;
        Value right = ucg->right->elem;
        ucg->elem = LLVMBuildBinOp(lmb, op, left, right, "");
        break;
    } // clang-format off
    case BRK: LLVMBuildBr(lmb, ucg->stmt->end->bloc); break;
    case CNT: LLVMBuildBr(lmb, ucg->stmt->bloc);      break;
    case AND: case AND_LIT: case OR: case OR_LIT: {
        // clang-format on
        Value left = ucg->left->elem;
        Value right = ucg->right->elem;
        if (includes(type, AND, AND_LIT, NULL))
            ucg->elem = LLVMBuildAnd(lmb, left, right, "");
        else
            ucg->elem = LLVMBuildOr(lmb, left, right, "");
        break;
    } // clang-format off
    case GT: case LT: case GE: case LE: case EQ: case NQ: {
        // clang-format on
        LLVMIntPredicate predicates[LAST + 1] = {
            [GT] = LLVMIntSGT, [LT] = LLVMIntSLT, [GE] = LLVMIntSGE,
            [LE] = LLVMIntSLE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
        };
        LLVMIntPredicate uint_predicates[LAST + 1] = {
            [GT] = LLVMIntUGT, [LT] = LLVMIntULT, [GE] = LLVMIntUGE,
            [LE] = LLVMIntULE, [EQ] = LLVMIntEQ,  [NQ] = LLVMIntNE,
        };
        LLVMRealPredicate float_predicates[LAST + 1] = {
            [GT] = LLVMRealOGT, [LT] = LLVMRealOLT, [GE] = LLVMRealOGE,
            [LE] = LLVMRealOLE, [EQ] = LLVMRealOEQ, [NQ] = LLVMRealUNE,
        };
        Ucg *value_type = ucg->left->value_type;
        Value left = ucg->left->elem;
        Value right = ucg->right->elem;
        if (value_type->type == FLOAT)
            ucg->elem = LLVMBuildFCmp(lmb, float_predicates[type], left, right, "");
        else if (value_type->is_uint)
            ucg->elem = LLVMBuildICmp(lmb, uint_predicates[type], left, right, "");
        else
            ucg->elem = LLVMBuildICmp(lmb, predicates[type], left, right, "");
        break;
    }
    case AS: {
        llvm_compile(ucg->to_type);
        Ucg *from = ucg->left->value_type;
        Ucg *to = ucg->to_type;
        Value value = ucg->left->elem;
        TypeRef lltype = to->lltype;
        bool from_float = from->type == FLOAT;
        bool to_float = to->type == FLOAT;
        bool is_signed = !(from_float ? to : from)->is_uint;
        OpCode to_fp = is_signed ? LLVMSIToFP : LLVMUIToFP;
        OpCode from_fp = is_signed ? LLVMFPToSI : LLVMFPToUI;
        if (from_float && to_float)
            ucg->elem = LLVMBuildFPCast(lmb, value, lltype, "");
        else if (!from_float && !to_float)
            ucg->elem = LLVMBuildIntCast2(lmb, value, lltype, is_signed, "");
        else
            ucg->elem =
                LLVMBuildCast(lmb, to_float ? to_fp : from_fp, value, lltype, "");
        break;
    }
    case IF: {
        Value fn = LLVMGetBasicBlockParent(LLVMGetInsertBlock(lmb));
        ucg->end->bloc = LLVMAppendBasicBlockInContext(ctx, fn, "end");
        break;
    }
    case LOOP: {
        Value fn = LLVMGetBasicBlockParent(LLVMGetInsertBlock(lmb));
        ucg->end->bloc = LLVMAppendBasicBlockInContext(ctx, fn, "end");
        ucg->bloc = LLVMAppendBasicBlockInContext(ctx, fn, "loop");
        LLVMBuildBr(lmb, ucg->bloc);
        LLVMPositionBuilderAtEnd(lmb, ucg->bloc);
        break;
    }
        // clang-format off
    case ELIF: case ELSE: {
        // clang-format on
        if (!is_bloc_terminated())
            LLVMBuildBr(lmb, ucg->end->bloc);
        LLVMPositionBuilderAtEnd(lmb, ucg->bloc);
        break;
    }
    case THEN: {
        Ucg *stmt = ucg->stmt;
        if (stmt->cond == NULL)
            break;
        Value fn = LLVMGetBasicBlockParent(LLVMGetInsertBlock(lmb));
        Bloc then = LLVMAppendBasicBlockInContext(ctx, fn, "then");
        if (stmt->next) {
            char *label = stmt->next->type == ELSE ? "else" : "elif";
            stmt->next->bloc = LLVMAppendBasicBlockInContext(ctx, fn, label);
        }
        Bloc other = stmt->next ? stmt->next->bloc : stmt->end->bloc;
        LLVMBuildCondBr(lmb, stmt->cond->elem, then, other);
        LLVMPositionBuilderAtEnd(lmb, then);
        break;
    }
    case RETURN: {
        // assert(0);
        if (is_bloc_terminated())
            break;
        if (ucg->to_ret == NULL) {
            LLVMBuildRetVoid(lmb);
            break;
        }
        llvm_compile(ucg->to_ret);
        LLVMBuildRet(lmb, ucg->to_ret->elem);
        break;
    }
    case END: {
        Ucg *to_end = ucg->to_end;
        bool is_branch = includes(to_end->type, IF, LOOP, NULL);
        if (is_branch) {
            Bloc back = to_end->type == LOOP ? to_end->bloc : ucg->bloc;
            if (!is_bloc_terminated())
                LLVMBuildBr(lmb, back);
            LLVMPositionBuilderAtEnd(lmb, ucg->bloc);
            break;
        }
        if (to_end->type == FN_DEC) {
            Ucg *ret = to_end->fn.ret;
            bool is_open = !is_bloc_terminated();
            if (is_open && ret->type == VOID)
                LLVMBuildRetVoid(lmb);
            else if (is_open)
                LLVMBuildRet(lmb, LLVMConstNull(ret->lltype));
            if (to_end->caller_bloc)
                LLVMPositionBuilderAtEnd(lmb, to_end->caller_bloc);
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
            print("%U %U\n", ucg, ucg->data_type);
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
    Type type = va_arg(ap, Type);
    while (type) {
        if (type == to_find)
            return true;
        type = va_arg(ap, Type);
    }
    return false;
}

// UCG FILES
ucgFile *ucg_find_file(char *path) {
    for (int i = 0; i < gb_ucg.files_count; i++) {
        if (strcmp2(gb_ucg.files[i]->path, path))
            return gb_ucg.files[i];
    }
    return NULL;
}

ucgFile *ucg_open_file(char *path) {
    ucgFile *file = arena_alloc(1, sizeof(ucgFile));
    file->path = realpath2(path);
    ioFile fp = fopen(file->path, "r");
    if (fp == NULL)
        return NULL;
    fseek(fp, 0, SEEK_END);
    file->len = ftell(fp);
    rewind(fp);
    file->buff = arena_alloc(file->len + 1, sizeof(char));
    fread(file->buff, file->len, sizeof(char), fp);
    fclose(fp);
    append(gb_ucg.files, file);
    return file;
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
