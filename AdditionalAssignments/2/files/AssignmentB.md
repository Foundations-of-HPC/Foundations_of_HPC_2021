# Foundations of HPC 

# @ Data Science and Scientific Computing 2021-2022

### <font color='red'>Assignment B, to be done by students that did not submit the partial assignment in due date.</font>

<font color='red'>Note: you can choose either this assignment A or the other assignment B that you find in ../1/</font>

**In this assignment you are requested either to **
A) **implement the solution to the proposed problem both a MPI and a OpenMP code.* (so, we do expect 2 codes).**
B) **Implement the solution with an hybrid MPI+OpenMP code.**</font>



The candidate is required to develop a parallel code that 

1) Computes the **local density $\rho$** of a given 3D distribution of $N_p$ particles.
   The local density is calculated at the node points of a regular 3D grid with grid number $N$, and it is defined as the number of particles that lie within a sphere of radius $R$ centred at every grid node point; the grid number $N$ and the radius $R$ are parameters acquired at the command line.

2) Evaluate a **“potential-like function”** at every point’s coordinates for all the $N_p$ particles.
   The $V_i$ function is defined as
   $$
   V_i = \sum_{j=0,j\neq i}^{N_p}\frac{1}{r_{ij}}
   $$
   where $r_{ij}$ is defined as the distance between particles $i$ and $j$, $r_{ij}= \lVert \mathbf{r}_i-\mathbf{r}_j\rVert$.

The input distribution is acquired from an external binary file whose format is specified below.
The code must be able to generate a random distribution of particles in case that no external input file is provided.
The code must have a the following command line: `executable N R [input file]`.

**_Format of the input file_**

The input file is a binary file whose first 4 bytes are an integer that specifies the number $N_p$ of particles contained in the file. Following, 3 single-precision floating-point coordinates are present for each particle. The coordinates are the _x_, _y_ and _z_ position of the point in space and the condition $0 \le x,y,x < 1$ holds, i.e. points are all generated in a cubic box of side 1 with one vertex in the origin.

**_Required output_**
Two output files are required, one for the density values and one for the potential values.
_Density file_
The first 4 bytes must be an integer that specifies the grid number $N$. The file will then contain $N^3$ single-precision floating-points. The grid points are supposed to be written in row-major order; i.e. starting from the grid point $(0,0,0)$ the fastest coordinate changing is the $z$: $(0,0,0),\,(0,0,1),\,\cdots, (0,0,N-1),\,(0,1,0),\,\cdots,\,(0,1,N-1),\,\cdots\, $.

_Potential file_
The first 4 bytes must be an integer that specifies the number $N_p$ of points. Then a quadruplet $\{x_i,y_i,z_i,V_i\}$ will follow for each point with single-precision floating-point numbers.

**_Additional requirements_**

1.  The candidate must assess the behaviour of the code when the _size_ of the problem changes while the number of MPI tasks / OpenMP threads is fixed. Specifically, he/she must assess the behaviour of the code for a random distribution:

    -  when the grid number $N$ is fixed and the number $N_p$ of input particles grows (hint: let $R$ vary as a function of the mean inter-point separation $1.0/N_p^{1/3}$);

    -  when the number $N_p$ of particles and the value of $R$ are fixed and the grid number $N$ grows.

2.  The candidate should provide either a Makefile or a shell script named `compile` to compile the code whose executable name must be `density`.

##### 
