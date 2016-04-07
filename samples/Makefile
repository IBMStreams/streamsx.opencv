# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

# To build a composite, specify its name:
#    make cam
# To clean a composite, specify the word 'clean' before its name:
#    make cleancam
# To build all composites, use the default 'all' target:
#    make
# To clean all composites, use the 'clean' target:
#    make clean
#
# The composite output directory is ./output/composite_name. It is
# deleted when the composite is cleaned. For example:
#    output/cam
# The composite data directory is ./data/composite_name. It is
# NOT deleted when the composite is cleaned. For example:
#    data/cam

CORE_COUNT = $(shell cat /proc/cpuinfo | grep processor | wc -l)

SPL_TARGETS = addtext blank blank2 box cam cam_proxy codebook contours_on_file contours_on_jpg copy crawler demo1 demo1b demo2 demo2b demo3 demo3b demo3b2 demo4 demo4b demo5 demo5b demo6 demo6b demo7 demo7b draw_contours_on_cam draw_contours_on_url faces_edges_cam faces_edges_file equalizehist facedetect file file_proxy fun imgcrawler log mix_cam_snow modec multiple_cameras onc1 onc2 onc3 onc4 onc5 onc_f proxy2 rank record rotate snow speed src_proxy threshold url video_stream xcombine

SPL_CLEAN_TARGETS := $(SPL_TARGETS:%=clean%)

.PHONY: all $(SPL_TARGETS) $(SPL_CLEAN_TARGETS) clean

SAMPLE_DIR = sample
OUTPUT_DIR = output
DATA_DIR = data

SPLC_FLAGS ?= -a -x -g3 -t ../com.ibm.streamsx.opencv
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?= 

SPL_BUILD_CMD = $(SPLC) $(SPLC_FLAGS) -T -j $(CORE_COUNT) -M $(SAMPLE_DIR)::$@ --output-directory=$(OUTPUT_DIR)/$@ --data-directory=$(DATA_DIR) $(SPL_CMD_ARGS)

all: $(SPL_TARGETS)

$(SPL_TARGETS): 
	$(SPL_BUILD_CMD)

clean:
	rm -rf $(OUTPUT_DIR)
	rm -f $(DATA_DIR)/*-out.avi


