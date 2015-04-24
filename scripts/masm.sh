#!/bin/sh

export SDL_VIDEODRIVER=dummy
MASM_PATH=~/MASM611/BIN/
DOSBOX=dosbox
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
mv "$MASM_PATH""RESULT.OUT" "$MASM_PATH$filename.out"
mv "$MASM_PATH$filename.out" $dirname
rm dosbox.conf
cat "$MASM_PATH/TEMP.OUT"
cat $dirname/$filename.out
