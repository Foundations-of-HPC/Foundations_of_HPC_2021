# A guided tour on Orfeo storage system

A short tour to explore all the levels of ORFEO's storage and start measuring performance. 

## local disks on both login and computing nodes 

The login node is a small VM hosted on Orfeo's OpenStack infrastructure. 
Let us explore storage available:

``` 

[cozzini@login Foundations_of_HPC_2021]$ df -h
Filesystem                                                                 Size  Used Avail Use% Mounted on
devtmpfs                                                                   1.9G     0  1.9G   0% /dev
tmpfs                                                                      1.9G  514M  1.4G  28% /dev/shm
tmpfs                                                                      1.9G   82M  1.8G   5% /run
tmpfs                                                                      1.9G     0  1.9G   0% /sys/fs/cgroup
/dev/vda1                                                                   40G   11G   30G  28% /
10.128.6.211:6789,10.128.6.212:6789,10.128.6.213:6789,10.128.6.214:6789:/   88T  5.8T   82T   7% /fast
10.128.2.231:/storage                                                       37T   19T   19T  51% /storage
10.128.6.211:6789,10.128.6.213:6789,10.128.6.212:6789,10.128.6.214:6789:/  598T   93T  506T  16% /large
10.128.4.201:/opt/area                                                     381G  181G  201G  48% /opt/area
...
```

We noted that the local disk ```/dev/sda1``` is a virtual disks.

Let us now:

- Identify your FileSystem and its properties

```
[cozzini@login Foundations_of_HPC_2021]$ mount  | grep vda
/dev/vda1 on / type xfs (rw,relatime,attr2,inode64,noquota)
```

- Measure/Estimate the rough performance  of our by means of the simple  `dd ` command.

First: 1 single block from ``/dev/zero`` to ``/dev/null``
```
[cozzini@login Foundations_of_HPC_2021]$ dd if=/dev/zero  of=/dev/null count=1
1+0 records in
1+0 records out
512 bytes (512 B) copied, 8.3995e-05 s, 6.1 MB/s
```
Then 1 milliobn block:
```
[cozzini@login Foundations_of_HPC_2021]$ dd if=/dev/zero  of=/dev/null count=1M
1048576+0 records in
1048576+0 records out
536870912 bytes (537 MB) copied, 2.64147 s, 203 MB/s
[cozzini@login Foundations_of_HPC_2021]$
```
Performance is not excellent, due to the standard blocksize.
Let us try now on the local disk:

```
cozzini@login tmp]$ dd if=/dev/zero  of=/tmp/big count=1M
1048576+0 records in
1048576+0 records out
536870912 bytes (537 MB) copied, 4.05241 s, 132 MB/s
```
and on our home directory:

```
[cozzini@login tmp]$ dd if=/dev/zero  of=~/big count=1M
1048576+0 records in
1048576+0 records out
536870912 bytes (537 MB) copied, 5.09671 s, 105 MB/s

```

- Compare it with the ramfs on login node:

```
[cozzini@login tmp]$ dd if=/dev/zero  of=/dev/shm/big count=1M
1048576+0 records in
1048576+0 records out
536870912 bytes (537 MB) copied, 3.53512 s, 152 MB/s

```

We can now redo all the tests on a computing node.



## CEPH filesystem 

Let us now focus on CEPHfs:

 - identify the FS:

```
[cozzini@login Foundations_of_HPC_2020]$ mount  | grep fast
10.128.6.211:6789,10.128.6.212:6789,10.128.6.213:6789,10.128.6.214:6789:/ on /fast type ceph (rw,relatime,name=admin,secret=<hidden>,acl,mds_namespace=cephfs,wsize=16777216)
[cozzini@login Foundations_of_HPC_2020]$ mount  | grep large
10.128.6.211:6789,10.128.6.213:6789,10.128.6.212:6789,10.128.6.214:6789:/ on /large type ceph (rw,relatime,name=admin,secret=<hidden>,acl,mds_namespace=cephfs_spin,wsize=16777216)

```

Let us try to see basic performance we can obtain using IOR benchmarks on CEPH FS 


## Long term storage

