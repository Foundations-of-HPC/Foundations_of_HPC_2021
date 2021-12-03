## High Performance Linpack (HPL) benchmark

**HPL** is a portable as well as freely available implementation of the High Performance Computing Linpack Benchmark used to measure computer performance in terms of 64-bit floating-opint operations per second or **FLOPS**. This benchmark is used to build the TOP500 list, ranking the world's most powerful supercomputers.

The HPL software requires an implementation of the Message Passing Interface **MPI** and an implementation of Linear Algebra Subprograms, **BLAS**, to run.

In this exercise we will compile and use HPL to benchmark Orfeo using different settings:

* different HPL parameters: N, NB, P, Q
* different MPI implementations: OpenMPI vs IntelMPI
* different BLAS implementation: OpenBlas vs IntelMKL

The objective is to achieve a performance as close to the theoretical peak performance R<sub>peak</sub> as possible.

--------------------------

### Resources

- [HPL tunning](https://www.netlib.org/benchmark/hpl/tuning.html): guideline where they explained the parameters used.
- [HPL parameters diagram](https://www.researchgate.net/profile/Jesus-Labarta/publication/228524393/figure/fig1/AS:301996025368584@1449012871620/An-example-of-P-by-Q-partitioning-of-a-HPL-matrix-in-6-processes-2x3-decomposition.png): An example of P by Q partitioning of a HPL matrix in 6 processes (2x3 decomposition)
- [ulhpc-tutorial](https://ulhpc-tutorials.readthedocs.io/en/latest/parallel/mpi/HPL/): used as main reference for this guide

#### Orfeo's theoretical peak performance on a thin node

R<sub>peak</sub> =  Cores x AVX512 all turbo cores frequency x DP FLOPS/cycle = 24 x 2.3 GHz x 32 FLOPS/cycle = 1.7 TFLOPS

Ref: [Intel. spec. sheet](https://www.intel.com/content/dam/www/public/us/en/documents/specification-updates/xeon-scalable-spec-update.pdf)

------------------------


### 1 Compile HPL-OpenMPI - [reference](https://sites.google.com/site/rangsiman1993/comp-env/test-and-benchmarks/build-hpl-with-blas-and-mpi)

**IMPORTANT:** Installation should be done in a compute node, NOT in the login node!

1. Download HPL v.2.3.

`wget --no-check-certificate https://www.netlib.org/benchmark/hpl/hpl-2.3.tar.gz`

2. Uncompress the gzip

`tar xvzf hpl-2.3.tar.gz`

3. Copy Make.Linux_PII_CBLAS

`cp ./setup/Make.Linux_PII_CBLAS Make.linux64`

4. Modify the following entries in Make.linux64:
```
ARCH         = linux64
TOPdir       = /path/to/hpl-2.3
MPdir        = /opt/area/shared/libraries/openmpi/4.1.2
MPlib        = $(MPdir)/lib/libmpi.so
LAdir        = /opt/area/shared/libraries/openBLAS/0.3.13
LAlib        = $(LAdir)/lib/libopenblas.a
CC           = mpicc
LINKER       = mpicc
```
5. Build HPL
```
module load openmpi #maybe gnu is good enough
make arch=linux64`
```

6. If installation succeded you should see **xhpl** and **HPL.dat** inside `ls hpl-2.2/bin/linux64/`. Try to run it:

`mpirun -np 4 xhpl > test`

### 2 Running HPL benchmark

**2.1.** Run HPL-OpenBlas with OpenMPI using 24 MPI processs on a thin node. The objective is to find out the right combination of parameters N, NB, P and Q,  that which maximizes the effective/real performance. Results should be expressed as a percentage of the theoretical peak performance. 

**Tips:**

- NB recommended setting for skylake architecture is 348 (you could try others but in that range)
- N should be a multiple of NB
- The problem is of size $N<sup>2</sup>. Double check that it fits in memory (764 GB for a thin node).
- Remember that time to solution scales with N
- P x Q  must be equal to the number of MPI processes



**2.2.** Using the sets of parameters found in the previous exercise rerun HPL using a hybrid approach: MPI + OpenMP.

Prepare HPL.dat file to run the following combinations:

<table style="width:30%">  <tr>
    <th>MPI Processes</th>
    <th>OMP threads</th>
  </tr>
  <tr>
    <td>12</td>
    <td>2</td>
  </tr>
  <tr>
    <td>6</td>
    <td>4</td>
  </tr>
  <tr>
    <td>2</td>
    <td>12</td>
  </tr>
</table>

**Tips:** 

* OMP threads can be set exporting the variable OMP_NUM_THREADS

* Remember that MPI processes = P x Q 

**2.3.** HPL - MKL vs OpenBLAS

Repeat the previous exercise but using Intel's version of the HPL benchmark:

`/opt/area/shared/programs/x86_64/intel/parallel_studio_xe_2020_update4_cluster_edition/mkl/benchmarks/mp_linpack/ xhpl_intel64_dynamic`

Compare results with the ones obtained in the previous exercise.

​	
