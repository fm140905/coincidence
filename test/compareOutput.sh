#/bin/bash

ToFFile="output/TOF_0.400000_4_7.txt"
ToFFileExpected="output_expected/TOF_0.400000_4_7.txt"
if [ -f $ToFFile ]; then
   diff $ToFFile $ToFFileExpected
   echo "Test passed."
else
   echo "Test failed."
fi
