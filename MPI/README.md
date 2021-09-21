## MPI section 

A collection of materials/references  for the MPI lectures 


### lectures
 
 - Stefano Cozzini : [domain decomposition with MPI](lecture07-Domain-Decomposition-with-MPI.pdf)


### references
 - chapter 9 of reference 4: final section: code here provided is taken from the [book's website](https://blogs.fau.de/hager/hpc-book#teaching)  
 
### tips/notes on the provided code 

It is a "simple" fortran implementation of the Jacobi solver in 3D 

To compile, load the MPI module and then: 

 ` mpif77 -ffixed-line-length-none Jacobi_MPI_vectormode.F -o jacoby3D.x `

To run the code

  ` mpirun --mca btl ^openib -np 24 ./jacoby3D.x  `

