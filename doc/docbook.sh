#!/bin/bash

arch=$(uname -m)
if [[ $arch == x86_64 ]]; then
    sed 's:@DOCBOOK_ROOT@:/usr/local/opt/docbook-xsl/docbook-xsl:g' man.xsl.in > man.xsl
    sed 's:@DOCBOOK_ROOT@:/usr/local/opt/docbook-xsl/docbook-xsl:g' html.xsl.in > html.xsl
elif [[ $arch == arm64 ]]; then
    sed 's:@DOCBOOK_ROOT@:/opt/homebrew/opt/docbook-xsl/docbook-xsl:g' man.xsl.in > man.xsl
    sed 's:@DOCBOOK_ROOT@:/opt/homebrew/opt/docbook-xsl/docbook-xsl:g' html.xsl.in > html.xsl
fi
