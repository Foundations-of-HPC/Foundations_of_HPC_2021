# a short tour on ORFEO HPC facility 

- how to login into ORFEO
- how to use modules 
- how to submit jobs 
 - interactive usage 
 - batch usage
 - selecting resources ( cores and nodes/ walltime)
- how to compile in serial
- how to compile an MPI application 

## logging in and exploring home directory

Login:
```
$ ssh username@195.14.102.205 
```

Once logged you will land in your home directory:

```
Last login: Sat Oct  9 20:27:21 2021 from host-79-17-109-168.retail.telecomitalia.it

        __     _____    ______    __
       /  \   |  __ \  |______|  /  \
      / /\ \  | |  ) )  ____    / /\ \
     / __)\ \ | | / /  |____|  / __)\ \
    / /    \ \| | \ \_________/ /    \ \
   /_/      \___|  `.__________/      \_\
   __  __   __     __ __    __  __  __
  (__ /   |/_ |\ |/  /_    [__)[__][__)|_/
   __)\__ |\__| \|\__\__   |   |  ||  \| \

   Welcome to ORFEO
   [cozzini@login ~]$
```

Let us check our home directory


```
[cozzini@login ~]$ pwd
/u/area/cozzini
[cozzini@login ~]$

```



- show environment variables 
- show how to setup a enviroment variable

## storage systems 

a quick look at filesystems:

```
cozzini@pbs-centos7 ~]$ ls -lrt
total 2
lrwxrwxrwx 1 cozzini area 18 Apr  7 15:23 fast -> /fast/area/cozzini
lrwxrwxrwx 1 cozzini area 21 Apr  7 15:23 storage -> /storage/area/cozzini
lrwxrwxrwx 1 cozzini area 21 Apr 16 09:23 scratch -> /scratch/area/cozzini
```
check size:

```
[cozzini@pbs-centos7 ~]$ df -h /scratch
Filesystem                                                                 Size  Used Avail Use% Mounted on
10.128.6.211:6789,10.128.6.213:6789,10.128.6.212:6789,10.128.6.214:6789:/  407T  4.3T  402T   2% /large
[cozzini@pbs-centos7 ~]$ df -h /fast
Filesystem                                                                 Size  Used Avail Use% Mounted on
0.128.6.211:6789,10.128.6.212:6789,10.128.6.213:6789,10.128.6.214:6789:/   96T  5.5T   90T   6% /fast
[cozzini@pbs-centos7 ~]$ df -h /storage
Filesystem             Size  Used Avail Use% Mounted on
10.128.2.231:/storage   37T  2.9T   34T   8% /storage
```



## use module 

Software is encapsulated in modules, check avaliability and and load modules with:

```
$ module avail
$ module load
```

 - check enviroment 
 - load a module and check environment
 - unload a module and check enviroment again 


### Using PBSPRO local resource manager system


 - check status of resources 

```
[cozzini@login ~]$ pbsnodes -aS
vnode           state           OS       hardware host            queue        mem     ncpus   nmics   ngpus  comment
--------------- --------------- -------- -------- --------------- ---------- -------- ------- ------- ------- ---------
ct1pf-fnode001  free            --       --       ct1pf-fnode001  fat             1tb      36       0       0 --
ct1pf-fnode002  free            --       --       ct1pf-fnode002  fat             1tb      36       0       0 --
ct1pt-tnode001  job-busy        --       --       ct1pt-tnode001  --            754gb      24       0       0 --
ct1pt-tnode002  job-busy        --       --       ct1pt-tnode002  --            754gb      24       0       0 --
ct1pt-tnode004  free            --       --       ct1pt-tnode004  --            754gb      24       0       0 --
ct1pt-tnode005  free            --       --       ct1pt-tnode005  --            754gb      24       0       0 --
ct1pt-tnode006  free            --       --       ct1pt-tnode006  --            754gb      24       0       0 --
ct1pt-tnode007  free            --       --       ct1pt-tnode007  --            754gb      24       0       0 --
ct1pt-tnode008  free            --       --       ct1pt-tnode008  --            754gb      24       0       0 --
ct1pt-tnode009  free            --       --       ct1pt-tnode009  --            754gb      24       0       0 --
ct1pt-tnode010  free            --       --       ct1pt-tnode010  --            754gb      24       0       0 --
ct1pt-tnode003  state-unknown   --       --       ct1pt-tnode003  --            754gb      24       0       0 node down: communication closed
ct1pg-gnode001  job-busy        --       --       ct1pg-gnode001  --            252gb      48       0       0 --
ct1pg-gnode002  free            --       --       ct1pg-gnode002  --            252gb      48       0       0 --
ct1pg-gnode003  free            --       --       ct1pg-gnode003  --            252gb      48       0       0 --
ct1pg-gnode004  free            --       --       ct1pg-gnode004  --            252gb      48       0       0 --

```

check queues and limits: 

```
[cozzini@login ~]$ qstat -q

server: login.hpc

Queue            Memory CPU Time Walltime Node   Run   Que   Lm  State
---------------- ------ -------- -------- ---- ----- ----- ----  -----
gpu                --      --    300:00:0  --      1     0   --   E R
all                --      --    48:00:00  --      0     0   --   E R
fat                --      --    300:00:0  --      0     0   --   E R
thin               --      --    96:00:00  --     16    95   --   E R
dssc               --      --    300:00:0  --      0     0   --   E R
                                               ----- -----
                                                  17    95
[cozzini@login ~]$
```

other way:

```
[cozzini@login ~]$ qstat -Q
Queue              Max   Tot Ena Str   Que   Run   Hld   Wat   Trn   Ext Type
---------------- ----- ----- --- --- ----- ----- ----- ----- ----- ----- ----
gpu                  0     1 yes yes     0     1     0     0     0     0 Exec
all                  0     0 yes yes     0     0     0     0     0     0 Exec
fat                  0     0 yes yes     0     0     0     0     0     0 Exec
thin                 0   111 yes yes    95    16     0     0     0     0 Exec
dssc                 0     0 yes yes     0     0     0     0     0     0 Exec
```


Submit a job

- in  Interactive mode, on 4 cores

```
[cozzini@pbs-centos7 ~]$ qsub -q blade -l nodes=1:ppn=4 -I
qsub: waiting for job 1072.192.168.10.5 to start
qsub: job 1072.192.168.10.5 ready

[cozzini@ct1pf-fnode002 ~]$
```

 - in batch mode to submit a job described in a shell script, on 96 cores

```
qsub -q blade -l nodes=4:ppn=24 job_test.sh`
```

 - to submit a job described in a shell script, on 24 cores of node thin1 + 12 cores of thin2, with a walltime of 2 hours (default is 1 hour)

```
qsub -q blade -l nodes=thin1:ppn=24+:thin1:ppn=12,walltime=2:00:00 job_test.sh`
```

 - to trace job's life


`tracejob [PBS_JOBID]`

 - to delete a job


`qdel [PBS_JOBID]`


 - to list jobs on queues 

```
qstat -a
````

```
qstat -r 
````

````
qstat -nr
````

The number of processors allocated to the job can be retrieved with:

```
[cozzini@pbs-centos7 ~]$  qsub -q thin -l nodes=4:ppn=3 -l walltime=24:00:00 -I
qsub: waiting for job 1075.192.168.10.5 to start
qsub: job 1075.192.168.10.5 ready

[cozzini@ct1pt-tnode001 ~]$ cat $PBS_NODEFILE
ct1pt-tnode001
ct1pt-tnode001
ct1pt-tnode001
ct1pt-tnode002
ct1pt-tnode002
ct1pt-tnode002
ct1pt-tnode004
ct1pt-tnode004
ct1pt-tnode004
ct1pt-tnode005
ct1pt-tnode005
ct1pt-tnode005
[cozzini@ct1pt-tnode001 ~]$
```


## Run Serial Code

```
$ gcc pi.c -o serial_pi
$ ./serial_pi 1000000
```

## Run Parallel code

### Interactive mode

Load mpi modules
```
$ module load openmpi
```
Compile in parallel:
```
$ mpicc mpi_pi.c -o parallel_pi
```
