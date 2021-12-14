### A simple exercise to sum two arrays on GPU


This material has been slighty modified from the one available at this web site:
 - http://developer.download.nvidia.com/compute/developertrainingmaterials/samples/cuda\_c/VectorAdd.zip

Credits goes to  Mark Harris - NVIDIA Corporation

#### Description:
A simple kernel which adds two vectors (1D arrays) with the GPU. This is a
good example showing off memory allocation and movement use the CUDA C 
runtime API, while using a very simple kernel function.


#### Files:
  Exercise:
    exercise.cu - one version of a hands-on lab exercise
  Answer:
    answer.cu - the solution to the presented exercise

### Compile:
Load the module and and then use the nvcc compiler

```
  nvcc -o vectoradd answer.cu
```

###  Propoposed exercise 

- After having reviewed  the concepts of cudaMemcpy, cudaMalloc and block
and threads, modify the code in exercise.cu to make it
compile and do the correct work on the GPU.
