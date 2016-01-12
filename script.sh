#!/bin/sh

samples=100000
samples_opt=1000
samples_opt_p=10000
samples_copeland=100000
samples_copeland_opt=1000
samples_copeland_opt_p=1000
#samples=1000
#samples_opt=100
#samples_opt_p=100
#samples_copeland=100
#samples_copeland_opt=30
#samples_copeland_opt_p=30

for rule in borda harmonic geometric-0.5; do
    ./topk 2000 20 $rule $samples $samples_opt_p $samples_opt > results/$rule:2000:20 &
    ./topk 10000 50 $rule $samples > results/$rule:10000:50 &
    ./topk-mallows 2000 20 $rule $samples 0.7 0.951 0.01 > results/$rule:2000:20:mallows &
done
./topk-copeland 2000 20 $samples_copeland $samples_copeland_opt_p $samples_copeland_opt > results/copeland:2000:20 &
./topk-copeland 10000 50 $samples_copeland > results/copeland:10000:50 &
./topk-mallows 2000 20 copeland $samples_copeland 0.7 0.951 0.01 > results/copeland:2000:20:mallows &

wait