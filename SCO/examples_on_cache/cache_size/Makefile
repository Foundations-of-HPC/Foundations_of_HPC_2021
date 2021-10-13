
COMPILER=gcc
INCLUDE_PAPI=-I/scratch/Software/include
INCLUDE_PTIMING=-I${HOME}/include
LIB_PAPI=-L/scratch/Software/lib -lpapi
LIB_PTIMING=-L$(HOME)/lib -lptiming

ifeq ($(COMPILER),gcc)
CC = gcc
#CFLAGS = -Wall -O3 -march=native -ftree-vectorize -lm -D__i686__ 
CFLAGS = -Wall -lm -D__i686__
SUFFIX = .gcc
LIBM = -lm
endif

ifeq ($(COMPILER),icc)
CC = icc
#CFLAGS = -Wall -O3 -fast -axSSE4.2 -xHost -ipo
CFLAGS = -Wall
SUFFIX = .icc
LIBM =
endif

ifeq ($(COMPILER),pgcc)
CC = pgcc
#CFLAGS = -Wall -O4 -fast -Munroll -Mvect=simd,fuse,tile -Mipa -lm
CFLAGS = -Wall
SUFFIX = .pgcc
LIBM = -lm
endif

mountain: cache_size.c cache_size.pt.c
	$(CC) $(INCLUDE_PAPI) $(CFLAGS) -o cache_size$(SUFFIX) cache_size.c $(LIBM) $(LIB_PAPI)
	$(CC) $(INCLUDE_PTIMING) $(CFLAGS) -o cache_size.pt$(SUFFIX) cache_size.pt.c $(LIBM) $(LIB_PTIMING)

clean:
	rm -f cache_size$(SUFFIX) cache_size.pt$(SUFFIX) *.o *~
