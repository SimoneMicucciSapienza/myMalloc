# Pseudo Malloc Project

*Micucci Simone 1934642*

## Implementation idea
This function relies on mmap for the physical allocation on the memory but choise between 3 option to optimize time and space:
 - large request (over 1/4 of page) => use a mmap
 - medium request (under 1/4 of page) => use buddy allocator
 - small request (under 64B) => use series of slab allocator



