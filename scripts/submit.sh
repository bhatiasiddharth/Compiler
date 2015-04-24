#!/bin/sh

GRP=Group_15.T2
rm -rf $GRP
mkdir -p $GRP/tree-printer $GRP/samples $GRP/docs $GRP/codegen $GRP/scripts
for i in *.c *.h
do 
   echo "/*\n * Group 15, Team 2\n * Siddharth Bhatia - 2011B4A7680P\n * Akshay Aurora    - 2011B4A7658P\n */\n" >> "Group_15.T2/$i"
   cat $i >> "$GRP/$i"
done
cp SUBMIT.md $GRP/README
cp -r docs/* $GRP/docs
cp -r codegen/* $GRP/codegen
cp -r scripts/* $GRP/scripts
cp Makefile $GRP/
cp -r tree-printer/* $GRP/tree-printer
rm -rf $GRP/tree-printer/node_modules
cp -rf samples/* $GRP/samples
tar -cvf "$GRP.tar" $GRP/
rm -rf $GRP