# Single-Core Optimization


This series of lectures is about code optimization on single-core.

1.  We start from scratch, revising some basic features of the [architecture of modern cpus](01--Modern_architecture.pdf) after a brief [historical review](00--No_more_free_lunch.pdf) of the reasons that led to them.
2.  We then introduce [some general concepts](02--Optimization-preliminary_and_basic_compiler_usage.pdf) about what “optimization” is, settling some common terms, and the usage of the compiler.
    _Code examples related to memory aliasing are [here](./examples_on_aliasing)_.
3.  The [running and memory model](03--Heap_stack_and_memory_model.pdf) in the **niX* environment is discussed in some detailed, with a focus on the stack and the heap memory.
    _Code examples related to this topic are in [this folder](./examples_on_stack_and_heap)_.
4.  Some more detailed facts about [memory allocation](04--Memory_allocation.pdf) are exposed.
    _Code examples related to the topic are in [this folder](./examples_on_memory_allocation)_.
5.  …



### Further readings _(optional, but useful)_

*  [What ever computer programmer should know about memory](materials/What_every_computer_programmer_should_know_about_memory.pdf)
*  [What ever computer scientist should know about floating point arithmetics](materials/What_every_computer_scientist_should_know_about_floating-point.pdf)

