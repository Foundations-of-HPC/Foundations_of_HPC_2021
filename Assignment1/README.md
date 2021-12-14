# First Assigment  FHPC course 2021/2021

### Due date : 23.59.30.12.2021

***version 1.0***

#### Submission policy: creating a directory on /fast/dssc/username/2021Assignment01 on ORFEO and put all the files required there.

hints:

`` mkdir /fast/dssc/my_username/2021Assignement01`` 

### COMPUTATIONAL RESOURCES:

Please use the dssc queue and be careful to study performances and scalability using the same kind of nodes.

### Structure of directory: 

````
report.pdf 
section1/ring.[c][cc][py][f90]
section1/sum3Dmatrix.[c][cc][py][f90]
section1/README
section2/cvs-files
section3/cvs-files
````

where: 

- Report.pdf  will contain a detailed report of all the sections in this documents.

- section1/README should explain which software stack we should use to compile the codes and run all the programs you provided us. 
  Providing well-done Makefiles is appreciated. 
 
- all the *.csv file are commma separated values files all structured as explained below


	    
## Section  1: MPI programming 


1. implement in c or C++ an MPI program using P processors on a ring (i.e. a simple 1D topology where each processor  has a left and right neighbour). 
 The program should implement a stream of messages in both directions:
  -  as first step  P sends a message ( msgleft = rank ) to its left neighbour (P-1) and receives from its right neighbour (P+1) and send aother message ( msgright = -rank) to P+1 and receive from P-1.
  -  it then does enough iterations till all processors receive back the initial messages. At each iteration each processor add its rank to the received message if it comes from left, substracting if it comes from right. 
Both messages originating from a certain processor P should have a tag proportional to its rank (i.e. itag=P*10)
Provide the code that should print on a file  the following output:

`   I am process irank and i have received np messages. My final messages have tag itag and value msg-left,msg-right`

where irank,np,itag and msg and are parameters that should be printed by the program. 
Make sure that your code produces the correct answer, independent of the number of processes you use. 
Take the time of the program using MPI_walltime routines and produce  a plot of the runtime as a function of P.  If the total time if very small, you might need to repeat several times the iterations in order to get reasonable measurements.
Model network performance and discuss scalability in term of the number of processors. 


2. implement a simple 3d matrix-matrix addition in parallel using a 1D,2D and 3D distribution of data using virtual topology and study its scalability in term of communication within a single THIN node. Use here just collective operations to communicate among MPI processes.
Program should accept as input the sizes of the matrixes and should then allocate the matrix and initialize them using double precision random numbers.

Model the network performance and try to identify which the best distribution given the topology of the node you are using for the following sizes:  
 - 2400 x 100 x 100 ; 
 - 1200 x 200 x 100 ; 
 - 800 x 300 x 100; 

Discuss performance for the three domains in term of 1D/2D/3D distribution keeping the number of processor constant at 24.
Provide a table with all possible distribution on 1 D 2D and 3D partition and report the timing.
 
  

## Section 2: measure MPI point to point performance 

- Use the Intel MPI benchmark to estimate latency and bandwidth of all available combinations of topologies and networks on ORFEO computational nodes.  Use both IntelMPI and openmpi latest version libraries available on ORFEO.
Report numbers/graph and also fit data obtained against the simple communication model we discussed. Compare estimated latency and bandwidth parameters against the one provided by a least-square fitting model. 
Provide a csv file for each measure with the following format and provide a graph (pdf and/or jpeg or any image format you like)

```
#header_line 1: command line used 
#header_line 2: list of nodes involved 
#header_line 3: lamba, bandwith computed by fitting data 
#header: #bytes #repetitions      t[usec]   Mbytes/sec    t[usec] computed   Mbytes/sec (computed )
            0         1000         0.20         0.00
            1         1000         0.20         5.10
            2         1000         0.19        10.74
            4         1000         0.19        21.55
            8         1000         0.19        42.33
           16         1000         0.19        84.66
           32         1000         0.23       138.12
           64         1000         0.23       277.57
          128         1000         0.31       419.05
          256         1000         0.34       760.29
          512         1000         0.39      1329.67
         1024         1000         0.50      2048.23
         2048         1000         0.75      2716.02
         4096         1000         1.13      3628.17
         8192         1000         1.88      4360.87
        16384         1000         2.98      5499.05
        32768         1000         4.85      6760.47
        65536          640         8.36      7835.13
       131072          320        15.14      8657.80
       262144          160        12.64     20742.31
       524288           80        22.30     23508.26
      1048576           40        50.52     20754.20
      2097152           20       138.86     15102.29
      4194304           10       308.95     13575.88



````

Discuss in detail performance obtained. In particular discuss the difference in performance among different network (infiniband vs gigsbit) and compare different implementations. Check if THIN and GPU nodes are behaving the same way.

## Section 3 : Compare performance observed against performance model for Jacobi solver 

The application we are using here is a Jacobi application already discussed in class.

Steps to do:

- Compile and run the code on on single processor of a THIN and GPU node to estimate the serial time on one single core.
- Run it on 4/8/12 processes within the same node pinning the MPI processes within the same socket and across two sockets 
  	- Identify for the two cases the right latency and bandwith to use in the performance model. 
  	- Report and check check if scalability fits the expected behaviour of the model discussed in class.
- Run it on 12 24 48 processor using two thin nodes 
	- Identify for this case the right latency and bandwith to use in the performance model. 
  	- Report and check check if scalability fits the expected behaviour of the model discussed in class. 
- Repeat the previous experiment on a GPU node where hyperthreading is enabled. 
  	- Identify for this case the right latency and bandwith to use in the performance model.  
  	- Report and check check if scalability fits the expected behaviour of the model discussed in class.
  
