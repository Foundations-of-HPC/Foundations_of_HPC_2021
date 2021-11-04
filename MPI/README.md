## MPI section 

A collection of materials/references for the MPI lectures 


### lectures (all by S.Cozzini)
 
 - lecture 4: [introduction to parallel computing](lecture04-on-parallel-performance.pdf)
 - lecture 5: [Message Passing and MPI programming](lecture05-on-MPI-programming.pdf)
 - lecture 6: [Collective operation and virtual topology](lecture06-Collective-MPI-virtual-topology.pdf)
 - lecture 7: [domain decomposition with MPI](lecture07-Domain-Decomposition-with-MPI.pdf)


### Main references for MPI lectures: 
 - chapter 9 of reference 4 is a nice and detailed introduction to MPI. The final section of the chapter is lecture 7 
 - exercises and tutorials all over the web:
    - [Here a very good starting point]https://www.mcs.anl.gov/research/projects/mpi/tutorial/index.html)
    - [Another simple tutorial](https://mpitutorial.com/tutorials/)
    - 

###  notes on the codes provided:


#### Jacoby code
The  code here provided is taken from the [ website of reference 4 book](https://blogs.fau.de/hager/hpc-book#teaching)
It is a "simple" fortran implementation of the Jacobi solver in 3D

To compile, load the MPI module and then:

 ` mpif77 -ffixed-line-length-none Jacobi_MPI_vectormode.F -o jacoby3D.x `

To run the code

  ` mpirun --mca btl ^openib -np 24 ./jacoby3D.x  `

