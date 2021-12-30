###
 # @Description: Run test
 # @Author: Ming Fang
 # @Date: 2021-04-06 15:00:15
 # @LastEditors: Ming Fang
 # @LastEditTime: 2021-04-06 15:17:41
### 

cd ..
mkdir -p build
cd build
cmake ..
# make
cmake --build .
cd ../test
pwd
cp ../bin/main .
./main -i test.json

./compareOutput.sh