#/bin/bash

cd ..
make
cd test
rm -r bin/
cp -r ../bin/ .
pwd
bin/main input.txt
