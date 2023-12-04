for i in {01..13}; do
    ./bin/splc ./test-others/test_2_s$i.spl
    diff ./test-others/test_2_s$i.out ./test-others/out-std/test_2_s$i.out > /dev/null
    if [ $? -ne 0 ]; then
        echo "Test $i failed!"
        # break
    fi
done
