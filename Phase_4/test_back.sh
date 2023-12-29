#!/bin/bash

for i in {01..03}; do
    ./bin/tcg ./test/stdIR/test_4_r$i.ir
done