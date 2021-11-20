# Single-Core Optimization


This series of lectures is about code optimization on single-core.

1.  We start from scratch, revising some basic features of the [architecture of modern cpus](01--Modern_architecture.pdf) after a brief [historical review](00--No_more_free_lunch.pdf) of the reasons that led to them.
2.  We then introduce [some general concepts](02--Optimization-preliminary_and_basic_compiler_usage.pdf) about what “optimization” is, settling some common terms, and the usage of the compiler.
    _Code examples related to memory aliasing are [here](./examples_on_aliasing)_.
3.  The [running and memory model](03--Heap_stack_and_memory_model.pdf) in the **niX* environment is discussed in some detailed, with a focus on the stack and the heap memory.
    _Code examples related to this topic are in [this folder](./examples_on_stack_and_heap)_.
4.  Some more detailed facts about [memory allocation](04--Memory_allocation.pdf) are exposed.
    _Code examples related to the topic are in [this folder](./examples_on_memory_allocation)_.
5.  Basic precautions and simple habits in writing codes so that to [avoid trivial performance killers](./05--optimization__avoid_the_avoidable.pdf) are discussed. Examples are [here](./avoid_the_avoidable)
6.  Optimization concepts are subdivided in 4 big chapters:
    1.  cache usage ([slides](./06--optimization__cache.pdf), [examples](./examples_on_cache))
    2.  conditional branching ([slides](./07--optimization__branches.pdf) ,[examples](./examples_on_branching))
    3.  how to exploit pipelines (i.e. ILP; [slides](./08--optimization__pipelines.pdf), [examples](./examples_on_pipelines) )
    4.  simple techniques for loops and prefetching of data ([slides](./09--optimization__loops_and_prefetching.pdf), [examples on loops](./examples_on_pipelines), [examples on prefetching](./examples_on_prefetching) )


Some materials about how to use the debugger and how to profile a code are also [provided](./profiling.pdf) with some examples on [debugging](./debugging_examples) and on [profiling](./exercises_for_profiling).

### What is required for the exam

_In general consider that you will not be required anything at assembler level. All the examples and discussions presented in the slides have been discussed only to let you undertand better what happens behind the scenes._

##### Architecture
As for the final oral exam, it will be required that the candidate has a firm understanding of the modern CPU architecture, so what being out-of-order is about, the pipeline concept, the cache hierarchy and how a typical cache works, the branch prediction and the concept of conditional branching and its impact.

##### Running model and memory allocation

The candidate must know what the running model is in its fundamental traits, and definitely to have a clear idea about what the stack and the heap are.
Also the main concepts about how the memory allocation works and what is the memory alignement must be digested. 

##### Optimization / cache

Basically all the materials presented in the slides should be mastered by the candidate.
The “cache resonance” and the exact details of cache mapping are required only to get the maximum grade, a simpler general understanding of the concept is sufficient even for high marks.

##### Optimization / branches

The candidate must know what a conditional branch is and what is its impact on the performance.
He/she/they must have a clear idea about the basci techniques to avoid conditional branches when possible and in any case how to ease the compiler’s job in issuing performant code.

##### Optimization / pipelines

The candidate must be familiar with the concept of pipeline and ILP and with how exploiting them “translates” in coding in a general sense.

##### Optimization / loops and prefetching

Some basic concepts and techniques on how to write loops are presented. We require that they are all clear to the candidate, as well as the concept of “prefetching” data in loops (direct prefetching is not a must-have).



### Further readings _(optional, but useful)_

*  [What ever computer programmer should know about memory](materials/What_every_computer_programmer_should_know_about_memory.pdf)
*  [What ever computer scientist should know about floating point arithmetics](materials/What_every_computer_scientist_should_know_about_floating-point.pdf)

