#!/bin/bash

for i in {01..06}; do
    ./bin/splc ./test-std/test_3_r$i.spl
done
