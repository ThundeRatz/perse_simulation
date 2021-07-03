#!/bin/bash

HEADER_FILES=$(find . -name "*.h")
SOURCE_FILES=$(find . -name "*.cpp")

uncrustify -c uncrustify.cfg --replace --no-backup $HEADER_FILES

uncrustify -c uncrustify.cfg --replace --no-backup $SOURCE_FILES
