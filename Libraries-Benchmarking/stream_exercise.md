## STREAM benchmark

The STREAM benchmark is a simple synthetic benchmark program that measures sustainable memory bandwidth (in MB/s) and the corresponding computation rate for simple vector kernels of size N.

For the benchmark to work correctly you need to [adjust the problem size](http://www.cs.virginia.edu/stream/ref.html#size), N, so that the data is not cacheable. In this way you are truly measuring memory bandwidth.

### Resources

- https://www.cs.virginia.edu/stream/ref.html
- https://community.intel.com/t5/Intel-C-Compiler/STREAM-OMP-benchmark-compiled-with-ICC/td-p/1046864
- https://blogs.fau.de/hager/archives/8263

-----

### 1 Compile STREAM 

 1. Download STREAM

    `git clone https://github.com/jeffhammond/STREAM.git `

 2. Adjust the problem size to be at least 4 times the size of L3 caches. For this you need to edit the source code `stream.c`, specifically the following line:

    ```c
    #ifndef STREAM_ARRAY_SIZE
    #   define STREAM_ARRAY_SIZE    10000000
    #endif
    ```

    Playing with STREAM_ARRAY_SIZE in the order of 80 millions should be more than enough on our system.

    **Tip:**  you can check the L3 cache size using the command `lscpu`

 3. To compile run the following commands:

    ```
    module load gnu
    cd STREAM
    make all
    ```

    

### 2. Measure memory bandwidth

**2.1** STREAM is a really fast benchmark so to run it you can simply ask a job in interactive mode:

`qsub -q thin -l nodes=1:ppn=24,walltime=1:00:00 -I`

To measure the full node memory bandwidth you need ask for all 24 processors (on a thin node).

To run the test simply run the executable:

`./stream_c.exe`

1. Try it out using 12 and 24 OMP threads (reminder: `export OMP_NUM_THREADS=12`)

2. Compare the results against the theoretical memory bandwidth which for a thin node is: 128GB/sec on each socket for a total of 256 GB/sec.

**2.2 Understanding the cost of wrong allocation**

Using the previous results as a baseline play with different binding schemes to appreciate performance difference.

To do this you can use `numactl` (there are may other ways of doing this):

```_c
 numactl --cpunodebind 0 --membind 0 ./stream_c.exe
```

Benchmark the memory bandwidth as you increase the number of threads from 1 to 12 using the following configuration:

- threads and memory allocated in the same socket (cpunode= 0, m= 0)
- threads and memory allocated in different socket (cpunode= 0, m= 1)

Plot and comment the results.

