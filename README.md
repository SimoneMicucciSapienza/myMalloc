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

### memory helper
keep the pointer to head of linked list made of pages as void\* array and the current size.  
the page is mapped and work as an array of 1024 element, free element are setted at a value of -1, the last element point to next page.  
the location of pointer is at responsability of caller.

the init set the head with a map, the size to 0, require the address of struct location.
the alloc wrap a mmap and do bookkeeping with the struct.
the free wrap a munmap and match bookkeeping from the struct.
the destroy free all the pending address and the list progressively.
