# Foundations of HPC, 2021/2022
This repository collects the materials from the course "Foundations of HPC" at Data Science and Scientific Computer, University of Trieste, year 2021/2022

Teachers:

**Stefano Cozzini**, Area di Ricerca, Trieste

**Luca Tornatore**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

Lab session are helped by:
TBD

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



###### Contact details:

Stefano Cozzini:  stefano.cozzini@areasciencepark.it

Luca Tornatore:  luca.tornatore@inaf.it 





#### EVALUATION

The final grade of the exam will be the result of a set of assignments during course, or alternatevely a project, and an oral examination based on the work done by students.
 
During the course, the students have been asked to carry out two assignments. It will be required to submit the code developed and the results written in a given format (so to allow automatic checking) on a repo that we will indicate. Each assignment will have a due date of either 1 or 2 weeks. 

If the two assignments  have been submitted and will be successful, the student will be directly admitted to a final oral examination. The oral examination will focus on the topics covered in the lessons, and it will start from the examination of some of the assignments.

If the either the two assignments will not have been submitted, or they are not successful, a written exam  will be required. This test will consist in a project that must be submitted at least 1 week before taking the oral examination.




#### CALENDAR

| DATE        | LECTURES                                                     | TUTORIALS                                            | ASSIGMENTS                                           |
| :---------- | :----------------------------------------------------------- | :--------------------------------------------------- | :--------------------------------------------------- |
| Mon, Oct 4  | [1] [Introduction to course/ introduction to HPC](day01/README.MD)              |                                                      |                                                      |
| Tue, Oct 5  | [2] [Introduction to HPC: part 2](day02/README.MD)                                 |                                                      |                                                      |
| Thu, Oct 7 | [3] [HPC Hardware and parallel computing](day03/README.MD)                      |                                                      |                                                      |
| Fri, Oct 8  | [4]   [HPC Software stack and tools](day04/README.MD)                             | [1] Using an HPC infrastructure                      | [1] Assigment on introductory topics  *(1 week)*     |
| Mon, Oct 11 | [5] Single Core Optimization (SCO) / 1<br />*Modern Architecture + Opt. Intro + Stack & Heap + Memory allocation* |                                                      |                                                      |
| Tue, Oct 12 | [6] SCO / 2<br />*How to use the Compiler + Cache hierarchy* | [2] Exercises on SCO                                 |                                                      |
| Thu, Oct 14 | [7] SCO / 3<br />*loops*                                     |                                                      |                                                      |
| Fri, Oct 15 | [8] SCO / 4<br />*Pipelines +branch predictions*             | [3] Tools for profiling                              |                                                      |
| Mon, Oct  18  | [9] SCO / 5<br />*Prefetching + vectorization*               |                                                      |                                                      |
| Tue, Oct 19  | [10] SCO / 6<br />*Debugging*                                | [4] Exercises on SCO                                 |                                                      |
| Thu, Oct 21 | [11] [Message Passing and intro to MPI programming](day11/README.MD)                                                 |                                                      |                                                      |
| Fri, Oct 22 | [12] [Again on MPI programming](day12/README.MD)                                                  | [4] Exercises on MPI                                 |                   |
| Mon, Oct  25 | [13] OpenMP / 1                                              |                                                      |                                                      |
| Tue, Oct 26 | [14] OpenMP / 2                                              | [5] Exercises on OpenMP                              |                   |
| Thu, Oct 28 | [15] [domain decomposition with MPI](day15/README.MD)                                                 |                                                      |                                                      |
|Fri, Oct 29  | [16] [Network Hardware and Performance Model](day16/README.MD)                          |                                  |                                                      |
| Thu, Nov 4  | [17] OpenMP / 3                                              |                                                      |                                                      |
| Fri, Nov 5  | [18] OpenMP / 4                                              | [7] Exercises on OpenMP                              |                                                      |
| Mon, Nov 8  | [19] [Performance Model and  a short tutorial on measuring MPI performance on ORFEO nodes](day19/README.MD)                                                |                                                      |                                                      |
| Tue, Nov 9 | [20] OpenMP / 5                                              | [8] Exercises on MPI and OpenMP                      | [4] + [5] Assigments on MPI and OpenMP  *(3 weeks)*  |                                                      |
| Thu, Nov 11 | [21] [MPI libraries](day21/README.MD) [22] [Parallel HPC libraries](day22/README.MD)               | [9] Exercises on High Performance Libraries |                         |
| Fri, Nov 12  | [23] [Benchmarking HPC systems](day23/README.MD)             |  Exercises on benchmarking                                                    |                                                      |
| Mon, Nov 15 | [24] [Parallel Storage](day24/README.MD)                                  |                                                   |                                                      |
| Tue, Nov 16 | [25] [Intro to GPU programming](day25/README.MD)             | [10] Exercises on parallel storage                                       | |
| Thu, Nov 18 | [26]  roof model + parallel performance analysis                                |                                                      |                                                      |
| Fri, Nov 19 | [27] TBD - advanced materials                                |                                                      |                                                      |
| Mon, Nov 22 | [28] TBD - advanced materials                                |                                                      |                                                      |
| Tue, Nov 23 | [29] TBD - advanced materials                                |                                                      |                                                      |
| Thu, Nov 25 | [26]  roof model + parallel performance analysis                                |                                                      |                                                      |
| Fri, Nov 26 | [27] TBD - advanced materials                                |                                                      |                                                      |
| Mon, Nov 29 | [28] TBD - advanced materials                                |                                                      |                                                      |
| Tue, Nov 30 | [29] TBD - advanced materials                                |                                                      |                                                      |
| Thu, Dec 2 | [26]  roof model + parallel performance analysis                                |                                                      |                                                      |
| Fri, Dec 3 | [27] TBD - advanced materials                                |                                                      |                                                      |
| Mon, Dec 6 | [28] TBD - advanced materials                                |                                                      |                                                      |
| Tue, Dec 7 | [29] TBD - advanced materials                                |                                                      |                                                      |
| Thu, Dec 9 | [26]  roof model + parallel performance analysis                                |                                                      |
| Fri, Dec 10 | [26]  roof model + parallel performance analysis                                |                                                      |
| Mon, Dec 13 | [26]  roof model + parallel performance analysis                                |                                                      |
| Tue, Dec 14 | [26]  roof model + parallel performance analysis                                |                                                      |
| Thu, Dec 16 | [26]  roof model + parallel performance analysis                                |                                                      |
| Fri, Dec 17 | [26]  roof model + parallel performance analysis                                |                                                      |

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
The source and pdf of the book (as well as lecture slides) can be found in this repository: https://bitbucket.org/VictorEijkhout/hpc-book-and-course 10.5281/zenodo.49897

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
