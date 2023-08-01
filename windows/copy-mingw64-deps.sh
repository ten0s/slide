#!/bin/bash -e

#
# See how to determine what DLLs are needed
# https://ten0s.github.io/blog/2022/07/01/debugging-dll-loading-errors
# https://ten0s.github.io/blog/2022/07/25/find-dlls-and-typelibs-dependencies-for-nodejs-gtk-application-on-windows
#

if [[ $# -ne 1 ]]; then
    echo "Usage: $(basename $0) <DEST_DIR>"
    exit 1
fi

BASE_DIR=$(dirname $0)
DEST_DIR=$1

for file in $(cat $BASE_DIR/mingw64-dlls.txt); do
    cp -v /mingw64/bin/$file $DEST_DIR/bin/ || exit 1
done

exit 0
