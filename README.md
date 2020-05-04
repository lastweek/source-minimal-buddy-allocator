# Buddy Allocator. A simplified Kernel Version.

During my recent work, I need to build an allocator
to manage a certain device's on-board DRAM (couple of GBs).
Since everything in our system is page-sized, we used a buddy allocator for that purpose.

Due to time limitation, I directly ported a simplified version from linux kernel (`mm/page_allo.c`).
Algorithm-wise, a buddy allocator is simple. But a practical implementation will add some complications,
as the case for the kernel one.

I did some modificatons, e.g., remove a lot zone and accounting code, but kept its core.
I decided to make it public, its linux's code anyway.


## Why?

1. This port is generic/small, can be *embedded* into other designs.
2. Good start if you want to learn how Linux manages physical memory.

## Tuning

Managed Physical Memory Range (without holes now):
```c
/*
 * The managed physical memory range.
 * We do not allow any holes at this point.
 */
unsigned long buddy_mem_start = PAGE_SIZE;
unsigned long buddy_mem_end = 1024 * 1024 * 1024;
```

Page Size:
```c
#define PAGE_SHIFT		(12)
```

The maximum orders `[0, MAX_ORDER)`:
```c
#define MAX_ORDER		(3)
```

Debugging:
```
(e.g., 16GB memory, MAX_ORDER=11)
Buddy Allocator
	-------- ---------- --------------------
	order    size (kb)  nr_free             
	-------- ---------- --------------------
	0        4          1                   
	1        8          1                   
	2        16         1                   
	3        32         1                   
	4        64         1                   
	5        128        1                   
	6        256        1                   
	7        512        1                   
	8        1024       1                   
	9        2048       1                   
	10       4096       4095                
	-------- ---------- --------------------
	Total Free: 16777212 kb, or around 16383 mb
```
