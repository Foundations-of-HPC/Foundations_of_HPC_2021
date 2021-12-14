## 1D Stencil code 


### Credit:
This material has been slightly modified from the original Zip file available here:

http://developer.download.nvidia.com/compute/developertrainingmaterials/samples/cuda_c/1DStencil.zip 


### Description:

A simple 1D Stencil code useful for making aware students about the benefits
and usage of shared memory in a GPU. 



### Files:

  - Exercise:
     Exercise/exercise.cu - The serial C code lacking any OpenACC constructs
  - Answer:
     Answer/answer.cu - A simple OpenACC C accelerated version of the code.

### Compile:
Using the NVIDIA C Compiler:

```
nvcc -o stencil exercise.cu
``` 

### Stepts :
  - First compile and run the code without using any shared memory. Make
sure to time the code to see how long it's taking what the problem is.
  - Modify the code to make use of shared memory. Let us run the code several time.
Do you  get semi-random errors in the  output ? 
  - Try to add the \__syncthreads() function on the code. 
  - Once you are getting the correct answer, profile and time code again
to see the speed-up achieved.

### References:
For a description on what this 1D Stencil code does, please see slides in the presentation (taken again from the package on the NVdiasite) 

