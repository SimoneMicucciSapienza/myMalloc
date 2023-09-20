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
Keep the pointer of the effective map, the bytes of size and the bits stored.  
The set up is one time and map buffer and struct location have to be given (user responsability).  
Setting and getting bit is avaible by single bit, getting bit is avaible either by uint32\_t to enable user fast read.

Bit start counting by 1 and not by 0,  
the pointer access is a problem of user (use padding if you want to use bytes get),  
if required is implemented a naive print function,  
all the function make the best to signal the null pointer as parameters or the overflow of the bit on the map.

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
