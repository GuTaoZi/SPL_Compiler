#!/bin/bash

for i in {1..5}; do
    ./bin/splc ./test/test_12111624_$i.spl ./test/test_12111624_$i.out
done
