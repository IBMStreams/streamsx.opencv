# Copyright (C) 2012, 2016, International Business Machines Corporation
# All Rights Reserved

MD_DIR = ../md

HTML_FILES = index.html InstallingToolkit.html UsingToolkit.html OperatorsAndTypes.html SampleApplications.html

all: $(HTML_FILES)

%.html: $(MD_DIR)/%.md
	pandoc --from=markdown_github --to=html5 --output=$@ --standalone $<

clean:
	rm -vf *.html
