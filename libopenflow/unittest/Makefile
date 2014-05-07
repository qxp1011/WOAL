all: ofConnector openflow

.PHONY: ofConnector
ofConnector:
	$(MAKE) -C ./ofConnector -f Makefile

.PHONY: openflow
openflow:
	$(MAKE) -C ./openflow -f Makefile

clean:
	$(MAKE) -C ./ofConnector realclean
	$(MAKE) -C ./openflow realclean
