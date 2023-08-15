#! /bin/bash

# 1 number of file
# 2 name of first file
# 3 = new name

if [ -v ${3} ];
then
	echo "Usage :"
	echo "1 = number of file"
	echo "2 = name of first file (with the number 01 and extension)"
	echo "3 = new name (without number or extension)"
else
	MAX=$1
	NEW="${3}"
	TMP1=".first.output"
	TMP2=".second.output"
	TMP3=".ext.output"

	c++ parse.cpp -o parse && ./parse $2
	if [ $? != 0 ]
	then
		echo Parsing return error
	else
		FIRST=$(cat $TMP1)
		SECOND=$(cat $TMP2)
		EXT=$(cat $TMP3)
		for (( i=1; i<=$MAX; i++ ))
		do
			if [ ${i} -lt 10 ]
			then
				mv ${FIRST}0${i}${SECOND} ${NEW}0${i}${EXT}
			else
				mv ${FIRST}${i}${SECOND} ${NEW}${i}${EXT}
			fi
		done
		rm $TMP1 $TMP2 $TMP3 parse
	fi
fi
