/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        main.c                          */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 01:00:44    */
/*                                                             */
/*   updated: 2026/11/26 10:12:17                              */
/*                                                             */
/* *********************************************************** */

#include "./includes/libc.h"
#include "./includes/llvm.h"
#include "./includes/macros.h"
#include "./includes/structs.h"
#include "./includes/protos.h"

#include "./includes/arena.h"
#include "./includes/string.h"
#include "./includes/fmt.h"
#include "./includes/fs.h"
#include "./includes/ps.h"


#include "./ucg.c"


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

int main(void) {
    atexit(arena_free);

    ucg_numeric_value(ucg_int_type(32), "123");

	print_ucgs();
    return 0;
}
