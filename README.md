# Foundations of HPC, 2020/2021
This repository collects the materials from the course "Foundations of HPC" at Data Science and Scientific Computer, University of Trieste, year 2020/2021

Teachers:

**Stefano Cozzini**, Area di Ricerca, Trieste

**Luca Tornatore**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

Lab session are helped by:
**Dott. Antonio Ragagnin**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

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
The lectures (the precise calendar is shown in the table below) will be held in **ROOMs 3B-H3** and **F-A** on **Wednesday afternoon** from 14:00 to 16:00, and **Friday morning** from 11:00 to 13:00.
On **Friday afternoon**, from 14:00 to 16:00, there will be a lab session with exercises on the topics treated during the lectures.

The lectures will **start on Wednesday, 7th October 2020**.


|           | 11.00 - 11.55  | 12.00 - 12.55 | | 14.00 - 14.55 | 15.00 - 16.00 |
| --------- | ---------- | ----------- | - |----------- | ----------- |
| Wednesday |            |             | | first part  | second part |
| Friday    | first part | second part | | lab         | lab         |



###### Contact details:

Stefano Cozzini:  stefano.cozzini@areasciencepark.it

Luca Tornatore:  luca.tornatore@inaf.it 

Antonio Ragagnin:  antonio.ragagnin@inaf.it



#### EVALUATION

The final grade of the exam will be the result of a written and an oral examination.

During the course, the students have been asked to carry out 3 assignments. It will be required to submit the code developed and the results written in a given format (so to allow automatic checking) on a repo that we will indicate. Each assignment will have a due date of either 1 or 2 weeks. 

If the two assignments  have been submitted and will be successful, the student will be directly admitted to a final oral examination. The oral examination will focus on the topics covered in the lessons, and it will start from the examination of some of the assignments.

If the either the two assignments will not have been submitted, or they are not successful, a written exam will be required. This test will consist in a project that must be submitted at least 1 week before taking the oral examination.

For submitting 3 *successful* assignments in the due time before the end of the course, a +1 will be rewarded in the final mark.




#### CALENDAR

| DATE        | LECTURES                                                     | TUTORIALS                                            | ASSIGMENTS                                           |
| :---------- | :----------------------------------------------------------- | :--------------------------------------------------- | :--------------------------------------------------- |
| Wed, Oct 7  | [1] [Introduction to course/ introduction to HPC](day01/README.MD)              |                                                      |                                                      |
| Fri, Oct 9  | [2] [Introduction to HPC: part 2](day02/README.MD)                                 |                                                      |                                                      |
| Wed, Oct 14 | [3] [HPC Hardware and parallel computing](day03/README.MD)                      |                                                      |                                                      |
| Fri, Oct 16 | [4] [HPC Software stack and tools](day04/README.MD)                             | [1] Using an HPC infrastructure                      | [1] Assigment on introductory topics  *(1 week)*     |
| Wed, Oct 21 | [5] Single Core Optimization (SCO) / 1<br />*Modern Architecture + Opt. Intro + Stack & Heap + Memory allocation* |                                                      |                                                      |
| Fri, Oct 23 | [6] SCO / 2<br />*How to use the Compiler + Cache hierarchy* | [2] Exercises on SCO                                 |                                                      |
| Wed, Oct 28 | [7] SCO / 3<br />*loops*                                     |                                                      |                                                      |
| Fri, Oct 30 | [8] SCO / 4<br />*Pipelines +branch predictions*             | [3] Tools for profiling                              |                                                      |
| Wed, Nov 4  | [9] SCO / 5<br />*Prefetching + vectorization*               |                                                      |                                                      |
| Fri, Nov 6  | [10] SCO / 6<br />*Debugging*                                | [4] Exercises on SCO                                 |                                                      |
| Wed, Nov 11 | [11] [Message Passing and intro to MPI programming](day11/README.MD)                                                 |                                                      |                                                      |
| Fri, Nov 13 | [12] [Again on MPI programming](day12/README.MD)                                                  | [4] Exercises on MPI                                 |                   |
| Wed, Nov 18 | [13] OpenMP / 1                                              |                                                      |                                                      |
| Fri, Nov 20 | [14] OpenMP / 2                                              | [5] Exercises on OpenMP                              |                   |
| Mon, Nov 30 | [15] [domain decomposition with MPI](day15/README.MD)                                                 |                                                      |                                                      |
| Mon, Dec 7  | [16] [Network Hardware and Performance Model](day16/README.MD)                          |                                  |                                                      |
| Wed, Dec 2  | [17] OpenMP / 3                                              |                                                      |                                                      |
| Fri, Dec 4  | [18] OpenMP / 4                                              | [7] Exercises on OpenMP                              |                                                      |
| Wed, Dec 9  | [19] [Performance Model and  a short tutorial on measuring MPI performance on ORFEO nodes](day19/README.MD)                                                |                                                      |                                                      |
| Fri, Dec 11 | [20] OpenMP / 5                                              | [8] Exercises on MPI and OpenMP                      | [4] + [5] Assigments on MPI and OpenMP  *(3 weeks)*  |                                                      |
| Fri, Dec 18 | [21] [MPI libraries](day21/README.MD) [22] [Parallel HPC libraries](day22/README.MD)               | [9] Exercises on High Performance Libraries |                         |
| Fri, Jan 8  | [23] [Benchmarking HPC systems](day23/README.MD)             |  Exercises on benchmarking                                                    |                                                      |
| Wed, Jan 13 | [24] [Parallel Storage](day24/README.MD)                                  |                                                   |                                                      |
| Fri, Jan 15 | [25] [Intro to GPU programming](day25/README.MD)             | [10] Exercises on parallel storage                                       | |
| Mon, Jan 18 | [26]  roof model + parallel performance analysis                                |                                                      |                                                      |
| Fri, Jan 22 | [27] TBD - advanced materials                                |                                                      |                                                      |
| Wed, Jan 27 | [28] TBD - advanced materials                                |                                                      |                                                      |
| Fri, Jan 29 | [29] TBD - advanced materials                                |                                                      |                                                      |

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
