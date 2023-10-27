#!/bin/bash

for i in {01..05}; do
    ./bin/splc ./test_for_others/test_2_r$i.spl
done
