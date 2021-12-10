# How to install [ior](https://github.com/hpc/ior/releases) on ORFEO



First of all, request a node and then load the appropriate MPI module 

Get the latest version just clonig the repo
```console
$ git clone https://github.com/hpc/ior.git
$ cd ior
```
 
Configure and build it:
```console
$ 
$ ./configure  CC=$(which mpicc)  --with-mpiio  --with-hdf5 --with-posix  

$ make -j4
$ make install
```

Check if everything is fine performing some basic and advanced testing:

```console
./testing/basic-tests.sh
./testing/complex-tests.sh
```

To perform a benchmark with mdtest:
```console
$ cd $HOME/software/ior/bin/
$ $(which mpirun) -np 8 $PWD/mdtest -d <filesystem> -n 10 -i 200 -y -N 60
```
where <filesystem> can be `/fast`, `/large` (your home), storage (`/storage`) and a local directory (`/tmp`)

To get more options for mdtest, read the help: `mdtest -h`.

To perform a benchmark with ior:
```
$ $(which mpirun) -np 8 $HOME/software/ior/bin/ior -s 1 -d 5 -w -r -i 1 -e -t 1m -b 1g -vv -k -B -a API -o /path/to/testfile -F
```
where the most important details are:
* `-t 1m` the size that ior will use to save files to disk;
* `-b 1g` the total size of the file written by each process;
* `-o /path/to/testfile` is the path to a file that will be produced (and automatically deleted after tests); the path is important, because you can use different filesystems: the CEPH SSD  fs `~/fast/area/$USER`, the CEPH HDD fs `~/scratch/area/$USER`  a `NFS` FS (long term storage), e.g. `~/storage/area/$USER`, or a directory on a local disk (not a network FS, so you don't need to pass through the network and communicate to a metadata server), such as `/tmp` or `/local_scratch`;
* `-F` will make each process write its own dedicated file (e.g. if you passed `-o /fast/area/$user$/testfile` and you are running an 8 cores MPI process, it will produce 8 temporary files `/fast/area/$user$/testfile.0000000X`, with X varying from 0 to 7);
* API can be `POSIX|DUMMY|HDF5|MPIIO|NCMPI|MMAP` (`NCMPI` is the NetCDF version, built with [`parallel-netcdf`](https://parallel-netcdf.github.io/)).

For more info: `ior --help`.
