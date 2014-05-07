all: src unittest

.PHONY: src
src:
	$(MAKE) -C ./src -f Makefile

.PHONY: unittest
unittest:
	$(MAKE) -C ./unittest -f Makefile

clean:
	$(MAKE) -C ./src realclean
	$(MAKE) -C ./unittest realclean
