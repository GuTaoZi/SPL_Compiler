#!/bin/bash

for i in {01..12}; do
    ./bin/splc ./test/test_1_r$i.spl
    diff ./test/test_1_r$i.out ./test_out/test_1_r$i.out > /dev/null
    if [ $? -ne 0 ]; then
        echo "Test $i failed!"
        # break
    fi
done
