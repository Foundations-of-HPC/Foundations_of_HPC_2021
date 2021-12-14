### Using CUBLAS in place of BLAS: benefits and drawbacks

The goal of this tutorial is to show how to migrate from standard BLAS functions to CUBLAS, 
analysing benefits and limitations of the procedure. 

We will focus on the level 3 BLAS function called *gemm*. Such function comes in different flavours, for double precision (dgemm), single precision (sgemm) and 
also complex single and complex double (cgemm and zgemm).

The first code ``gemm.c`` is a standard *gemm* code, where 3 matrices A,B,C are allocated, A and B are filled and the BLAS routine calculates the matrix-matrix product C=A\*B

The relevant call is 

```

  GEMMCPU(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, m, B, k, beta, C, m);

```
where ``GEMMCPU`` is a macro to easily switch from single precision (``cblas_sgemm``) to double precision (``cblas_dgemm``).

The standard cblas interfaces are


```
  void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)

  void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA, const enum CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
```

The first argument ``Order`` specifies wheter we are using column major storage or row major storage. We will be using colmajor, since cublas 
uses such data layout. ``TransA`` and ``TransB`` tell that the matrices should be taken as they are, so not transposed. The rest are the standard GEMM arguments, to perform the operation

```
 
  C(M,N) = alpha*A(M,K)*B(K,N) + beta*C(K,N)
```

The parameters ``lda``, ``lbd`` and ``ldc`` are the leading dimensions of the matrices, which, since we are using colmajor order, should be the number of rows (lda=M, ldb=K, ldc=M)

To compile and run the code, first submit an interactive job to the queue system

On ORFEO: 
```

  qsub -q dscc_gpu -l walltime=2:00:00 -l nodes=1:ppn=24 -I

```

Load the needed module 

```

  module load intel

```  

And type 

```
  make cpu
```

To run the code simply issue

```

  ./gemm.x 

```

With no argument it will calculate a matrix multiplication with M=2000 K=200 and N=1000.

You can use positional argument to specify the size

```

  ./gemm.x 2000 1000 3000 

```

will use M=2000 K=1000 and N=3000, so we will get C(2000,3000) = A(2000,1000)\*B(1000,3000)

The present BLAS code is based on Intel MKL implementation, which is multithreaded. To control the number of threads you could use the environment variable ``OMP_NUM_THREADS``

```

  export OMP_NUM_THREADS=4
```

By default this variable has been set by the queue system to the number of cores requested at submission time (``ppn=24`` means ``OMP_NUM_THREADS=24``), but can be changed at runtime.

### GPU Version


The file ``gemm_gpu.c`` contains the GPU version using cuBLAS. It also contains the BLAS version for timing comparison. 

The cuBLAS code has been wrapped in a function called ``gpu_blas_dgemm`` in order to have the same interface of a standard cblas call. 

The main parts of such function are 

```

  void gpu_blas_dgemm(CBLAS_ORDER layout, CBLAS_TRANSPOSE h_A_transp, CBLAS_TRANSPOSE h_B_transp,
                    int m, int n, int k, const MYFLOAT alpha, const MYFLOAT* h_A, const int lda, const MYFLOAT* h_B, const int ldb,
                    const MYFLOAT beta, MYFLOAT* h_C, const int ldc)
  {
    ...

    //allocate device buffers

    if ( cudaSuccess != cudaMalloc((void**)&d_A, m * k * sizeof(MYFLOAT)))
        exit(1);

    if ( cudaSuccess != cudaMalloc((void**)&d_B, k * n * sizeof(MYFLOAT)))
        exit(1);

    if ( cudaSuccess != cudaMalloc((void**)&d_C, m * n * sizeof(MYFLOAT)))
        exit(1);

    //copy A and B from Host to Device
    cudaMemcpy(d_A,h_A, m * k * sizeof(MYFLOAT),cudaMemcpyHostToDevice);
    cudaMemcpy(d_B,h_B, k * n * sizeof(MYFLOAT),cudaMemcpyHostToDevice);

    // Create a handle for CUBLAS
    cublasHandle_t handle;
    cublasCreate(&handle);

    ...

    // Do the actual multiplication
    CUDAGEMM(handle, d_A_transp, d_A_transp, m, n, k, &alpha, d_A, lda, d_B, ldb, &beta, d_C, ldc);

    //Copy results back to Host    
    cudaMemcpy(h_C,d_C, m * n * sizeof(MYFLOAT),cudaMemcpyDeviceToHost);

    // Destroy the handle
    cublasDestroy(handle);

    // Free device arrays
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

  }

```

As you can see, to work with a GPU you need to allocate buffers on the device as well using ``cudaMalloc`` besides the buffer on the global memory, then you need to copy data to the device and the result
back to the host, using ``cudaMemcpy``.

This data movement costs quite some time, such that using cuBLAS becomes beneficial only for large matrices. However, matrices cannot be too big, since the GPU memory is typically way smaller that the system memory. 

To compile code load the CUDA environment

```
 module load cuda
```

and then issues:

```
  make gpu
```

To run, log into a node using the queue system

```

  qsub -q dssc_gpu -l walltime=2:00:00 -l nodes=1:ppn=24 -I
```

load the modules

```

  module load openblas/0.2.14/gnu/4.9.3
  module load cuda/7.5
```

and then issue
```

  ./gemm_gpu.x 
``` 

With no argument it will calculate a matrix multiplication with M=2000 K=200 and N=1000 
You can use positiional argument to specify the size
```

  ./gemm_gpu.x 2000 1000 3000 
```

will use M=2000 K=1000 and N=3000, so we will get C(2000,3000) = A(2000,1000)\*B(1000,3000)

The code runs first on GPU using cuBLAS, than on CPU using OpenBLAS calls
The code prints elapsed time in both cases, and for the GPU call reports also the time spent in allocation of device buffers and data movement.

You should see that for matrix of size around 10000, the GPU performs better than the CPU, even including the communication time.
However, given the size of our GPUs, you cannot fit matrices larger that 20000x20000.

You could also test the performance of double precision calculation. To this end, in ``Makefile`` at line 12 modify  ``-DUSE_FLOAT`` in ``-DUSE_DOUBLE``.

Then issue

```

  make clean
  make gpu
```


The performance in double precision should be roughly half of the performance in single precision. 
(This is NOT in general true for consumer-level GPUs (NVidia GTX e.g.) . This is due to the fact that such GPUs have a much lower count of double precision registers. 
The Tesla series instead have typically twice as much single precision register compared to double precision.) 

#### Proposed Exercise

- Increasing the matrices size up to 20000x20000 (single precision) or 14000x14000 (double precision) analyse the scaling of the GEMM calculation, for both CPU  and GPU and find the size for which the GPU is beneficial. Plot your results.

- Repeat the analysis for different values of OMP_NUM_THREADS. (Remember, this effect only the CPU BLAS, not the cuBLAS)
 
```
    export OMP_NUM_THREADS=4
```

#### Fortran Interface to cublas

see here:

 - https://docs.nvidia.com/cuda/cublas/index.html#appendix-b-cublas-fortran-bindings

#### More resources


For further information please visit the official cuBLAS page:

  - https://docs.nvidia.com/cuda/cublas/index.html
