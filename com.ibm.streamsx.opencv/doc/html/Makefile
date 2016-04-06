# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2016
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

MD_DIR = ../md

HTML_FILES = index.html InstallingToolkit.html UsingToolkit.html OperatorsAndTypes.html SampleApplications.html

all: $(HTML_FILES)

%.html: $(MD_DIR)/%.md
	pandoc --from=markdown_github --to=html5 --output=$@ --standalone $<

clean:
	rm -vf *.html
