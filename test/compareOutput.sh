#/bin/bash
###
 # @Description: compare output files
 # @Author: Ming Fang
 # @Date: 2021-04-06 15:08:52
 # @LastEditors: Ming Fang
 # @LastEditTime: 2021-04-06 15:14:31
### 

ToFFile="output/TOF_CFD"
ToFFileExpected="output_expected/TOF_CFD"
if [ -f $ToFFile ]; then
   diff $ToFFile $ToFFileExpected
   echo "Test passed."
else
   echo "Test failed."
fi
