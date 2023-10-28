#!/bin/bash

for i in {1..8}; do
    ./bin/splc ./test-ex/test_$i.spl ./test-ex/test_$i.out
done
