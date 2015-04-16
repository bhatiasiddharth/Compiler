#!/bin/sh

MASM_PATH=~/MASM611/bin/
DOSBOX=/Users/akshay/Applications/DOSBox.app/Contents/MacOS/DOSBox
cp $1 $MASM_PATH
dirname=$(dirname "$1")
filename=$(basename "$1")
extension="${filename##*.}"
filename="${filename%.*}"
caps_filename=`echo $filename | tr '[:lower:]' '[:upper:]'`

CONF="[autoexec]\n
MOUNT C: $MASM_PATH\n
C:\n
ML $filename.asm\n
$filename.com > $filename.out\n
exit"
echo $CONF > dosbox.conf
$DOSBOX -conf dosbox.conf > /dev/null
mv "$MASM_PATH$caps_filename.out" "$MASM_PATH$filename.out"
mv "$MASM_PATH$filename.out" $dirname
rm dosbox.conf
cat $dirname/$filename.out
