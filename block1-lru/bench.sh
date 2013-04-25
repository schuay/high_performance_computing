function bench() {
    fname=trace_m${1}_n${2}.bin;

    echo ${1} ${2} >> bench_results

    echo Generating trace for m: ${1} n: ${2}

    build/src/gentrace -m ${1} -n ${2} > $fname

    for i in {1..10}; do
        echo Running iteration $i
        timeout 1m build/src/lru-misses -b $fname >> bench_results
    done

    rm $fname
}

for exp in 10 12 14 16 18 20 22 24 26; do
    for domexp in 10 16 22; do
        m=$((2 ** domexp))
        n=$((2 ** exp))

        bench $m $n

        n=$(($n - 1))

        bench $m $n
    done;
done
