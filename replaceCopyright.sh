#!/bin/bash
#set -o xtrace
#set -o pipefail


#find . -name "*.cgt" -exec perl -0777 -i.old.copyright -pe 's|/[*].+?copyright.+?[*]/|/*\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n*/|is' {} \;
#find . -name "*.spl" -exec perl -0777 -i.old.copyright -pe 's|/[*].+?copyright.+?[*]/|/*\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n*/|is' {} \;
#find . -name "*.cpp" -exec perl -0777 -i.old.copyright -pe 's|/[*].+?copyright.+?[*]/|/*\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n*/|is' {} \;
#find . -name "*.c"   -exec perl -0777 -i.old.copyright -pe 's|/[*].+?copyright.+?[*]/|/*\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n*/|is' {} \;
#find . -name "*.h"   -exec perl -0777 -i.old.copyright -pe 's|/[*].+?copyright.+?[*]/|/*\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n*/|is' {} \;

find . -name "*.xml" -exec perl -0777 -i.old.copyright -pe 's|<[?]xml version="1.0" [?]>.+?copyright.+-->|<?xml version="1.0" ?>\n<!--\nCopyright (C) 2012, 2016, International Business Machines Corporation\nAll Rights Reserved\n-->|is' {} \;







