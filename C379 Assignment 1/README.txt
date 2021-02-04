Allan Manuba
Note: Most of the driver actions are from the Assignment description.


==================
Driver 1 (mem_1.c)
------------------

Action: 
The driver allocates a massive array with malloc and initializes it.

Difference: 
4th memory region:
0x0804B000-0xF7DC5000 NO => 0x0804B000-0xE004C000 NO
- Size decreases by 400 003 072 bytes (0x17D79000 bytes)

5th memory region:
0xF7DC5000-0xF7DC6000 RW => 0xE004C000-0xF7DC6000 RW
- Size increases by 400 003 072 bytes (0x17D79000 bytes)

Explanation:
malloc allocates memory into the heap, which increases the 5th memory region. In the driver,
we allocate 100 000 000 integers, which is about 400 000 000 bytes. We are given a chunk of the
heap that fits the requested memory from malloc. Although, the 4th memory region shrinks
as a consequence.


==================
Driver 2 (mem_2.c)
------------------

Action: 
Driver mmaps a large file at a specific address.

Difference: 
- Before the action, there were 18 memory regions. After the action,
there were now 20 memory regions.

- The two new regions is in the middle of the original huge chunk no access memory region.
4th memory region (before action):
0x0804B000-0xF7DC5000 NO => 0x0806C000-0x70000000 NO
                         => 0x70000000-0x7000A000 RW
                         => 0x7000A000-0xF7DC5000 NO
- The read-write memory region create in-between is 40 960 bytes (0xA000 bytes).

Explanation:
I tried doing nothing, and I wanted to see a huge chunk where we don't have memory access.
I chose 0x70000000 as an address to the kernel where the mapping should start, because
that no-read memory region is in-between the stack starts to grow and where the heap
starts to grow (based on previous experiments, like driver 1). According to a rough guide
given to us in the labs, this would region is the memory mapping segment, if my guesses
were right. It seems like the kernel would likely follow up with my request to map the 
file on the given address if I chose that location. So, mmap maps a file into the process's 
virtual address space on the  spot I wanted to. I also specified the size of the mapped file 
as the size of 10 000 integers, which is 40 000 bytes. The read-write memory region produced 
by the action is 40 960 bytes, which fits the said memory.


==================
Driver 3 (mem_3.c)
------------------

Action:
Driver which performs a mathematical function using the standard math library, 
assuming always you compile dynamically loaded executables.

Difference: 
3rd memory region:
0x0804A000-0x0804B000 RW => 0x0804A000-0x0806C000 RW
- Size increases by 135 168 bytes (0x21000 bytes)

4th memory region:
0x0804B000-0xF7DC0000 NO => 0x0806C000-0xF7DC0000 NO
- Size decreases by 135 168 bytes (0x21000 bytes)

Explanation:
When cosine is first used, it is not yet loaded. So, the dynamic loader would load
the library containing cosine. In this case, the library is loaded at 0x0804A000.
The particular region where the library was loaded to is the memory mapping segment,
which is somewhere usually in between where the stack starts to grow, and where
the heap starts to grow. The 3rd memory region increases by 135 168 bytes, 
and the 4th memory region decreases by the same size as a consequence.