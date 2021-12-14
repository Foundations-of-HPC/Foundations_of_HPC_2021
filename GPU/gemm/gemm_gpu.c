/*
 * authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
 * date      : October 2018
 * copyright : GNU Public License
 */

#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include "mkl_cblas.h"
#include <time.h>

#ifdef USE_FLOAT
#define MYFLOAT float
#define CUDAGEMM cublasSgemm
#define DATATYPE printf(" Using float \n\n");
#define GEMMCPU cblas_sgemm
#endif
#ifdef USE_DOUBLE
#define MYFLOAT double
#define CUDAGEMM cublasDgemm
#define DATATYPE printf(" Using double \n\n");
#define GEMMCPU cblas_dgemm
#endif

struct timespec diff(struct timespec start, struct timespec end)
{
        struct timespec temp;
        if ((end.tv_nsec-start.tv_nsec)<0) {
                temp.tv_sec = end.tv_sec-start.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        } else {
                temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
}


#define GEMMGPU gpu_blas_dgemm
#include <cuda_runtime.h>
#include <cublas_v2.h>
void gpu_blas_dgemm(CBLAS_ORDER layout, CBLAS_TRANSPOSE h_A_transp, CBLAS_TRANSPOSE h_B_transp, 
                    int m, int n, int k, const MYFLOAT alpha, const MYFLOAT* h_A, const int lda, const MYFLOAT* h_B, const int ldb, 
                    const MYFLOAT beta, MYFLOAT* h_C, const int ldc)
{
    const MYFLOAT alf = alpha;
    const MYFLOAT bet = beta;
    const MYFLOAT *alpha_d = &alf;
    const MYFLOAT *beta_d = &bet;
    MYFLOAT *d_A, *d_B, *d_C;
    cublasOperation_t d_A_transp;
    cublasOperation_t d_B_transp;
    struct timespec begin, end;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    // Allocate device arrays 
    if ( cudaSuccess != cudaMalloc((void**)&d_A, m * k * sizeof(MYFLOAT)))
	{
	printf(" Cannot allocat matrix A on device. Quitting. \n");
	exit(1);
	}
    
    if ( cudaSuccess != cudaMalloc((void**)&d_B, k * n * sizeof(MYFLOAT)))
        {
        printf(" Cannot allocat matrix B on device. Quitting. \n");
        exit(1);
        }
   
    if ( cudaSuccess != cudaMalloc((void**)&d_C, m * n * sizeof(MYFLOAT)))
        {
        printf(" Cannot allocat matrix C on device. Quitting. \n");
        exit(1);
        }
    clock_gettime(CLOCK_MONOTONIC, &end);
    struct timespec diff1 = diff(begin, end);

    //copy A and B from Host to Device
    clock_gettime(CLOCK_MONOTONIC, &begin);
    cudaMemcpy(d_A,h_A, m * k * sizeof(MYFLOAT),cudaMemcpyHostToDevice);
    cudaMemcpy(d_B,h_B, k * n * sizeof(MYFLOAT),cudaMemcpyHostToDevice); 
    clock_gettime(CLOCK_MONOTONIC, &end);
    struct timespec diff2 = diff(begin, end);

    // Create a handle for CUBLAS
    cublasHandle_t handle;
    cublasCreate(&handle);

    if (layout != CblasColMajor)
        {
        printf("Only ColMajor Order in cuda! Quitting");
        exit(1);
        }

    if (h_A_transp == CblasNoTrans)
        d_A_transp = CUBLAS_OP_N;
    else
        d_A_transp = CUBLAS_OP_T;

    if (h_B_transp == CblasNoTrans)
        d_B_transp = CUBLAS_OP_N;
    else
        d_B_transp = CUBLAS_OP_T;

 
    // Do the actual multiplication
    CUDAGEMM(handle, d_A_transp, d_A_transp, m, n, k, &alpha, d_A, lda, d_B, ldb, &beta, d_C, ldc);

    //Copy results back to Host    
    clock_gettime(CLOCK_MONOTONIC, &begin);
    cudaMemcpy(h_C,d_C, m * n * sizeof(MYFLOAT),cudaMemcpyDeviceToHost);

    // Destroy the handle
    cublasDestroy(handle);

    // Free device arrays
    cudaFree(d_A);
    cudaFree(d_B);  
    cudaFree(d_C);

    clock_gettime(CLOCK_MONOTONIC, &end); 
    struct timespec diff3 = diff(begin, end);
    printf(" Communication time %d.%d s", diff1.tv_sec + diff2.tv_sec + diff3.tv_sec, diff1.tv_nsec + diff2.tv_nsec + diff3.tv_sec);
}

int main(int argc, char** argv)
{
    MYFLOAT *A, *B, *C;
    int m, n, k, i, j;
    MYFLOAT alpha, beta;
    struct timespec begin, end, begin1, end1;

    if (argc == 1)
    {
    m = 2000, k = 200, n = 1000;
    }
    else if (argc == 4)
    {
        m = atoi(argv[1]);
        k = atoi(argv[2]);
        n = atoi(argv[3]);
    }
    else
    {
    printf( "Usage: %s M K N, the corresponding matrices will be  A(M,K) B(K,N) \n", argv[0]); 
    return 0; 
    }


    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " CUBLAS and BLAS function gemm, where A, B, and  C are matrices and \n"
            " alpha and beta are scalars\n\n");


    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    DATATYPE
    alpha = 1.0; beta = 0.0;

    A = (MYFLOAT *)malloc( m*k*sizeof( MYFLOAT ));
    B = (MYFLOAT *)malloc( k*n*sizeof( MYFLOAT ));
    C = (MYFLOAT *)malloc( m*n*sizeof( MYFLOAT ));
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      free(A);
      free(B);
      free(C);
      return 1;
    }

    for (i = 0; i < (m*k); i++) {
        A[i] = (MYFLOAT)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (MYFLOAT)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf (" Computing matrix product using gemm function via CUBLAS interface \n");
    clock_gettime(CLOCK_MONOTONIC, &begin);
    GEMMGPU(CblasColMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, m, B, k, beta, C, m);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf ("\n Elapsed time %d.%d s\n\n\n", diff(begin,end).tv_sec, diff(begin,end).tv_nsec );

    fflush(stdout);    

    printf (" Computing matrix product using gemm function via CBLAS interface \n");
    clock_gettime(CLOCK_MONOTONIC, &begin1);
    GEMMCPU(CblasColMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A, m, B, k, beta, C, m);
    clock_gettime(CLOCK_MONOTONIC, &end1);
    printf ("\n Elapsed time %d.%d s\n\n\n", diff(begin1,end1).tv_sec, diff(begin1,end1).tv_nsec );

#ifdef PRINT
    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }
#endif

    free(A);
    free(B);
    free(C);

    printf (" Example completed. \n\n");
    return 0;
}
