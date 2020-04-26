#!/usr/bin/bash

# Variables
PROTOCOL='https'
EECS_SERVER='eecs.wsu.edu'
CP_DIR='~cs460/samples/LASTnew'

# -r recursive, no-parent guarantees we won't ascend to .., A accept list
wget -r --no-parent -A '*' "$PROTOCOL://www.$EECS_SERVER/$CP_DIR/"
cp "www.$EECS_SERVER/$CP_DIR/"* .
rm -rf "www.$EECS_SERVER"
rm -rf *.html*
