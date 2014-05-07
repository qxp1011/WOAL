export ACE_ROOT=$(shell (pwd))/ACE_wrappers
export ACE_INCLUDE=$(ACE_ROOT)
export ACE_LIB_PATH=$(ACE_ROOT)/lib
LD_LIBRARY_PATH:=$(ACE_ROOT)/lib:$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH
all: ace
ace:
	@echo 'ACE_ROOT=' $(ACE_ROOT)
	@echo 'LD_LIBRARY_PATH=' $(LD_LIBRARY_PATH)
	@chmod a+x $(ACE_ROOT)/bin/add_rel_link.sh
	$(MAKE) -C ./ACE_wrappers -f GNUmakefile ACE static_libs=0
clean:
	$(MAKE) -C ./ACE_wrappers realclean
