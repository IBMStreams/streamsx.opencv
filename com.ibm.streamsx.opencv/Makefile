# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012, 2016
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

.PHONY: all lib toolkit doc clean

SRC_DIR = ./impl/src
HTML_DIR = ./doc/html
TOOLKIT_DIR = .

all: lib toolkit doc

lib:
	$(MAKE) -C $(SRC_DIR)

toolkit:
	spl-make-toolkit -i $(TOOLKIT_DIR)

doc:
	$(MAKE) -C $(HTML_DIR)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	$(MAKE) -C $(HTML_DIR) clean

