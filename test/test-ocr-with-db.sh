#!/bin/bash
######################################################
#            PROJECT  : svpgm2txt                    #
#            VERSION  : 1.2                          #
#            DATE     : 08/2011                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
DATA_PATH="$1"
MODE="$2"

######################################################
safe_exec()
{
	echo "$*"
	$* || exit 1
}

######################################################
if [ -z "$DATA_PATH" ] || [ -z "$MODE" ]; then
	echo "Usage : $0 {DATA_PATH} {MODE=bin|ascii}" 1>&2
	exit 1
fi

######################################################
fname="test-data-${MODE}.pgm"
safe_exec cp "$DATA_PATH/$fname" .
yes | safe_exec ../src/svpgm2txt $fname -d "$DATA_PATH/data-db.txt"
safe_exec md5sum -c "$DATA_PATH/$fname.txt.md5sum"
safe_exec rm -f "$fname.txt"
