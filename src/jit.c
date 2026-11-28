
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
