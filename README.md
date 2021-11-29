# Foundations of HPC, 2021/2022
This repository collects the materials from the course "Foundations of HPC" at Data Science and Scientific Computer, University of Trieste, year 2021/2022

Teachers:

**Stefano Cozzini**, Area Science Park, Trieste

**Luca Tornatore**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

Lab session are helped by: Federico Barone, Area Science Park, Trieste


------

#### -- DSSC -- Data Science and Scientific Computing @ University of Trieste

#### -- Master in HPC @ SISSA/ICTP, Trieste

------



Welcome to the "Foundations of HPC" course at *(i)* DSSC Department of the University of Trieste and *(ii)* Master in HPC program at SISSA/ICTP in Trieste.

The scope of the course is to give you:

- an introduction to basic concepts of the High Performance Computing
- an introduction to the modern computer architecture and the techniques to exploit it to maximize performance
- an introduction to parallel/multi-threading programming with MPI and OpenMP
- an introduction to basic and advanced techniques for code optimisation for single- and multi-threaded codes



The lectures will be both in presence (this may change in the future according to the evolution of the COVID pandemic) **and** in streaming; each lecture will also be recorded and available to be viewed asynchronously.
The lectures (the precise calendar is shown in the table below) will be held in **Aula Morin, building H2bis**

The lectures will **start on Monday, 4th October 2020**.


| DAY       | 11.00 - 11.55  | 12.00 - 12.55 |  16.00 - 16.55 | 17.00 - 17.55 |
| --------- | -------------- | ------------- | -------------- | ------------- |
| Monday    |                |               |    first part  |  second part  |
| Tuesday   | first part     | second part   |                |               |
| Thursday  | first part     | second part   |                |               |
| Friday    | first part     | second part   |                |               |


##### HOw to get an account on ORFEO

Please fill in with your details this google table:

https://docs.google.com/spreadsheets/d/1UFZMffMRBDp-tXRxOwdH2rzRGyib6-ZpeFguXleTEEA/edit#gid=1853927763

To learn how to generate a private/public keys on windows here a link: https://www.maketecheasier.com/generate-ssh-public-private-keys-windows/


###### Units's Microsoft Teams
Link for today lecture 


https://teams.microsoft.com/l/meetup-join/19:6HsghqYmJTd3mXlRuhDl9EZV5vX9imECoBk8VBWbQXk1@thread.tacv2/1633960864011?context=%7B%22Tid%22:%22a54b3635-128c-460f-b967-6ded8df82e75%22,%22Oid%22:%22d7c26965-1813-464f-b1dd-239b179a37e8%22%7D

#### slack Channel link:

https://join.slack.com/t/fhpc2021/shared_invite/zt-wp3dpcsn-CtRi3ICKavfjWiisEdPXWg

###### Contact details:

Stefano Cozzini:  stefano.cozzini@areasciencepark.it

Luca Tornatore:  luca.tornatore@inaf.it 





#### EVALUATION

The final grade of the exam will be the result of a set of assignments during course, or alternatevely a project, and an oral examination based on the work done by students.
 
During the course, the students have been asked to carry out two assignments. It will be required to submit the code developed and the results written in a given format (so to allow automatic checking) on a repo that we will indicate. Each assignment will have a due date of either 1 or 2 weeks. 

If the two assignments  have been submitted and will be successful, the student will be directly admitted to a final oral examination. The oral examination will focus on the topics covered in the lessons, and it will start from the examination of some of the assignments.

If the either the two assignments will not have been submitted, or they are not successful, a written exam  will be required. This test will consist in a project that must be submitted at least 1 week before taking the oral examination.




#### CALENDAR

| DATE        | LECTURES                                                     | TUTORIALS                                            | ASSIGNEMENTS                                           |
| :---------- | :----------------------------------------------------------- | :--------------------------------------------------- | :--------------------------------------------------- |
| Mon, Oct 4  | [1] [Introduction to course/ introduction to HPC](Intro/README.md)              |                                                      |                                                      |
| Tue, Oct 5  | [2] [Introduction to HPC: part 2](Intro/README.md)                                      |                                                      |                                                      |
| Thu, Oct 7 |  [3] [HPC Hardware and HPC software stack and tools](Intro/README.md)                      |                                                      |                                                      |
| Fri, Oct 8  |                              |                       | [Assignment 0: enable your account on ORFEO ](Intro/README.md)   | 
| Mon, Oct 11 |  |      [1] [Using an HPC infrastructure](Intro/README.md)                                                |                                                      |
| Tue, Oct 12 | [5] [Single Core Optimization (SCO) / 1<br />*Modern Architecture + Opt. Intro + Stack & Heap + Memory allocation*](SCO/README.md) |                                 |                                                      |
| Thu, Oct 14 | [6] [SCO / 2<br />*How to use the Compiler + Cache hierarchy*](SCO/README.md) |                                                      |                                                      |
| Fri, Oct 15 |  [7] [SCO / 3<br />*loops*](SCO/README.md)  |                             |                                                      |
| Mon, Oct  18  |             |         [2] [Tools for profiling](SCO/README.md)                                               |                                                      |
| Tue, Oct 19  | [8] [SCO / 4<br />*Pipelines +branch predictions*](SCO/README.md)                                |                                |                                                      |
| Thu, Oct 21 |[9] [SCO / 5<br />*Prefetching + vectorization*](SCO/README.md)                                               |                                                      |                                                      |
| Fri, Oct 22 |                                                   | [3] [Debugging](SCO/README.md)                                |                   |
| Mon, Oct  25 |[10] [Parallel programming concepts and performance](MPI/lecture04-on-parallel-performance.pdf)                                               |                                                      |                                                      |
| Tue, Oct 26 | [11] [Message Passing and MPI programminng](MPI/README.md)                                            |                             |                   |
| Thu, Oct 28 | [12] [Collective MPI operations and virtual topology](MPI/README.md)                                                 |                                                      |                                                      |
|Fri, Oct 29  | [13] [Domain Decomposition with MPI](MPI/README.md)                     |  [4] [Exercises on MPI](MPI/README.md)  |   |
| Thu, Nov 4  | [14] [Network basic for MPI application](MPI/README.md)                                             |                                                      |                                                      |
| Fri, Nov 5  | [15] [MPI libraries and performance model](MPI/README.md)                                              |                             |                                                      |
| Mon, Nov 8  |                                           |      [5] [short tutorial on measuring MPI performance on ORFEO nodes](MPI/README.md)                                                 |                                                      |
| Tue, Nov 9 | [16]  [Performance Evaluation for MPI](MPI/README.md)                                          |                     |   |                                                      |
| Thu, Nov 11 |             |  |    [Assignment 1](Assignment1/README.md)                     |
| Fri, Nov 12  |              |  [6] [short tutorial on using ORFEO at best for the assignment](MPI/README.md)                                                |                                                      |
| Mon, Nov 15 | [17] [OpenMP / 1](OpenMP/README.md)                                  |                                                   |                                                      |
| Tue, Nov 16 | [18] [OpenMP / 2](OpenMP/README.md)           |                                 | |
| Thu, Nov 18 | [19] [OpenMP / 3](OpenMP/README.md)                                |                                                      |                                                      |
| Fri, Nov 19 | [20] [OpenMP / 4](OpenMP/README.md)                 |            [7] [Exercises on openMP](OpenMP/README.md)   |   [Assigment 2](Assignment2/README.md) |
| Mon, Nov 22 | [21] [OpenMP / 5 ](OpenMP/README.md)     |
| Tue, Nov 23 | [22] [OpenMP / 6 ](OpenMP/README.md)     |   |   |
| Thu, Nov 25 | [23] [OpenMP / 7 ](OpenMP/README.md)     |   |   |
| Fri, Nov 26 | [23] [OpenMP / 8 ](OpenMP/README.md)     |   |   |
| Mon, Nov 29 | [24] [OpenMP / 9 ](OpenMP/README.md)     |   |   |
| Tue, Nov 30 | [25] [Parallel HPC libraries](Libraries/README.MD)  |    |   |
| Thu, Dec 2 |  [26] [Benchmarking HPC systems](Libraries/README.MD)     |    |      |
| Fri, Dec 3 |                                |       [9]  [Exercises on benchmarking](Libraries/README.md)      |    |
| Mon, Dec 6 | [27]  [Parallel I/O](ParallelStorage/README.MD)    |       |                                                      |
| Tue, Dec 7 | [28]  [exercise on parallel I/O](ParallelStorage/README.md)    |      |     |
| Thu, Dec 9 | [29]  [roof model + parallel performance analysis /1](PerformanceAnalysis/README.md)   |     |
| Fri, Dec 10 |c[30] [Parallel Performance Analysis /2](PerformanceAnalysis/README.md)                                |     (PerformanceAnalysis/README.md) |
| Mon, Dec 13 | [31]  [Parallel Performance Analysisscalasca](PerformanceAnalysis/README.md)                               |                                                      |
| Tue, Dec 14 |                                  |              [10] [exercise on parallel performance analysis]                                          |
| Thu, Dec 16 | [32]  [GPU programming/1](GPU/README.md)                                          |
| Fri, Dec 17 | [33]  [GPU programming/2](GPU/README.md)                                 |                                                      |

#### PRACTICAL ADVICES
As you will experiment, having a Linux is always the most adequate solution (Linux is running on the vast majority - possible the 100% - on HPC facilities and brings native tools and utilities for programming).

So, our advice is that on your laptop you run a Linux (of course, it is not needed that you run *only* Linux: you may a have a multi-boot machine with any other OS you want).

As a second option, you can install a Linux machine in a Virtual box under your preferred OS.

For Windows users, a third option is running Linux under the W10 WSL, that seems to be enough for our course needs.


#### REFERENCES & BOOKS

##### Ref 1: High Performance Computing Modern Systems and Practices
Available at ICTP library Authors: Thomas Sterling Matthew Anderson Maciej Brodowicz eBook ISBN: 9780124202153 Paperback ISBN: 9780124201583

Some of the materials of this book are presented during some lectures.

##### Ref 2: Introduction to High-Performance Scientific Computing,by Victor Eijkhout
The source and pdf of the book (as well as lecture slides) can be found at this link: https://pages.tacc.utexas.edu/~eijkhout/istc/istc.html, DOI: 10.5281/zenodo.49897

Some of the materials presented in classes are taken from this book.

##### Ref 3: Computer Organization and Design
by D. A. Patterson and J. L. Hennessy The Morgan Kaufmann Series in Computer Architecture and Design easily available as pdf on the net.

##### Ref 4: Introduction to High Performance Computing for Scientists and Engineers
by Georg Hager and Gerhard Wellein Paperback: 356 pages Publication date July 2, 2010 Editors (Chapman & Hall/CRC Computational Science)

##### Ref 5: Optimizing HPC Applications with Intel Cluster Tools
Paperback – October 15, 2014 by Alexander Supalov (Author), Andrey Semin (Author), Michael Klemm (Author), & 1 more ISBN-13: 978-1430264965 ISBN-10: 1430264969 Edition: 1st

##### Ref 6: High Performance Parallelism Pearls, 1st Edition
Multicore and Many-core Programming Approaches by Reinders & Jeffers
Morgan Kaufmann, Print Book ISBN :9780128021187
