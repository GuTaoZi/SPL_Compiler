#!/bin/bash

for i in {1..5}; do
    ./bin/splc ./test-tmp/$i.spl
done
