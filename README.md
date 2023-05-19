# Pseudo Malloc Project

*Micucci Simone 1934642*

## Implementation idea
This function relies on mmap for the physical allocation on the memory but choise between 3 option to optimize time and space:
 - large request (over 1/4 of page) => use a mmap
 - medium request (under 1/4 of page) => use buddy allocator
 - small request (under 64B) => use series of slab allocator

### large request
a simple wrapping for a mmap

### medium request
quick dynamic allocation of half huge page of 2MB

### small request
fixed and quick allocation of some slab allocator to avoid huge overhead on space request

## data structure
To implement this function we need some data structure to register the allocated space or pages to free them after

### bitmap

### buddy allocator

### slab allocator
