# Stream Exercise on ORFEO (to be revised) 
Build `stream.c` using the provided `Makefile`.
```
qsub -l nodes=1:ppn=24 -l walltime=2400 -I
```
With the following line we loop over ten threads. We use `numactl` to bind the process to socket `0` and local memory `0`.
```
(for i in `seq 1 12`; do OMP_NUM_THREADS=$i numactl --cpunodebind 0 --membind 0 ./stream; done) | grep "Triad:" | awk '{print $2}'
```
No we force the process to use distant memory, simply correcting `--membind 1`.

Results are collected in the following table:

|NUMA region 0 [Mb/s] | NUMA region 1 [Mb/s] |
| ------------ | ------------- |
| 9464.1       |  4524.7       |
|17673.5       | 10604.0       |
|24375.8       | 14626.7       |
|29233.3       | 18565.6       |
|32372.3       | 21447.3       |
|34440.3       | 23509.6       |
|35167.7       | 24978.0       |
|35330.5       | 25520.6       |
|34894.2       | 25872.2       |
|34367.1       | 25861.2       |
