#!/bin/bash

for i in {01..15}; do
    ./bin/splc ./test-std/test_2_r$i.spl
    diff ./test-std/test_2_r$i.out ./test-std/out_std/test_2_r$i.out > /dev/null
    if [ $? -ne 0 ]; then
        echo "Test $i failed!"
        # break
    fi
done
