#!/bin/bash

for i in {01..03}; do
    ./bin/splc ./test/test_4_r$i.spl
    ./bin/tcg ./test/test_4_r$i.ir
done