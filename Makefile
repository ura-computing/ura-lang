.PHONY: install check dev

# make install LLVM=12, make dev LLVM=12: build with that LLVM
PIN := $(if $(LLVM),LLVM_VERSION=$(LLVM))

install:
	@$(PIN) sh config/install.sh

check:
	@sh config/install.sh check

dev:
	@$(PIN) bash --rcfile config/config.sh -i
