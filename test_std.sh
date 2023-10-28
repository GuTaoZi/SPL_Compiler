#!/bin/bash

for i in {01..12}; do
    ./bin/splc ./test-std/test_1_r$i.spl
    diff ./test-std/test_1_r$i.out ./test-std/test_out/test_1_r$i.out > /dev/null
    if [ $? -ne 0 ]; then
        echo "Test $i failed!"
        # break
    fi
done
