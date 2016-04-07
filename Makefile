# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012, 2016
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

.PHONY: all lib doc clean

OPENCV_DIR = com.ibm.streamsx.opencv
SAMPLE_DIR = samples

all:
	$(MAKE) -C $(OPENCV_DIR) all
	$(MAKE) -C $(SAMPLE_DIR) all

lib:
	$(MAKE) -C $(OPENCV_DIR) lib

doc:
	$(MAKE) -C $(OPENCV_DIR) doc

clean: $(SPL_CLEAN_TARGETS)
	$(MAKE) -C $(OPENCV_DIR) clean
	$(MAKE) -C $(SAMPLE_DIR) clean

