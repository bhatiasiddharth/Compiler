#!/bin/sh

MASM_PATH=~/MASM611/bin/
DOSBOX=/Users/akshay/Applications/DOSBox.app/Contents/MacOS/DOSBox
cp $1 "$MASM_PATH/temp.asm"
dirname=$(dirname "$1")
filename=$(basename "$1")
extension="${filename##*.}"
filename="${filename%.*}"
caps_filename=`echo $filename | tr '[:lower:]' '[:upper:]'`

CONF="[autoexec]\n
MOUNT C: $MASM_PATH\n
C:\n
ML temp.asm > temp.out \n
temp.com > result.out\n
exit"
echo $CONF > dosbox.conf
$DOSBOX -conf dosbox.conf > /dev/null
mv "$MASM_PATH""RESULT.out" "$MASM_PATH$filename.out"
mv "$MASM_PATH$filename.out" $dirname
rm dosbox.conf
cat "$MASM_PATH/TEMP.out"
cat $dirname/$filename.out
