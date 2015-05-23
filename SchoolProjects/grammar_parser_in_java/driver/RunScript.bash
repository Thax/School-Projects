#!/bin/bash

# input-file input-grammar
infilename=$(echo ${1} | sed -e "s_\..*__")
grfilename=$(echo ${2} | sed -e "s_\..*__")

echo "Filename: ${infilename}"

java TTokenizer ${1}

java GrammarParser ${2} ${grfilename}.blarg

java Driver ${grfilename}.blarg ${infilename}.tok

