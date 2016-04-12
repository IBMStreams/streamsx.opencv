# Copyright (C) 2012, 2016, International Business Machines Corporation
# All Rights Reserved

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

