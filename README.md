# Buddy Allocator. A simplified Kernel Version.

During my recent work, I need to build an allocator
to manage a certain device's on-board DRAM (couple of GBs).
Since everything in our system is page-sized, we used a buddy allocator for that purpose.

Due to time limitation, I directly ported a simplified version from linux kernel (`mm/page_allo.c`).
Algorithm-wise, a buddy allocator is simple. But a practical implementation will add some complications,
as the case for the kernel one.

I did some modificatons, e.g., remove a lot zone and accounting code, but kept its core.
I decided to make it public, its linux's code anyway.
Hopefully this is generic enough and can be embedded into other designs.