##Benchmark by means of  HPCG 

Download the package and compile it 

``
git clone https://github.com/hpcg-benchmark/hpcg.git

``

Compilation is straighforward: just copy the Make.orfeo in the setup directory

`` 
cp Make.orfe  hpcg/setup/.

``
load the openmpi module 
and then type:

`` 
cd hpcg
make arch=orfeo

``

Run the executable that will be placed  in hpcg/bin together with an input file named hpcg.dat


Things to try:


 - Run it for at least 300 second with a size enough to reach a decent performance (not less than 5 Gflops)
 - Compare with the performance you obtained on HPL 
 - Try the higlhy optimized Intel version of HPCG you should find in the 

``
 $MKLROOT/benchmarks/hpcg/bin
``
 
