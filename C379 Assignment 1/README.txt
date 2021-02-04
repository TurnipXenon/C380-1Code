/**
 * @author Allan Manuba
 * 
 * @brief Scans the entire memory area of the calling process
 * 
 * Some of the documentation texts are paraphrased from the Assignment instruction
 * and the lab slides.
 */

/*
todo:
* describes the (three different) actions performed in the three different drivers,
* describes, for each driver, what differences were found when comparing the memory 
layout beforeand after the action taken, and,
* explains the reason behind the differences found
*/

Allan Manuba
Note: Most of the driver actions are from the Assignment description.


==================
Driver 1 (mem_1.c)
------------------

Action: 
The driver allocates a massive array with malloc and initializes it.

Difference: 
3rd memory region:
0x0804A000-0x0804B000 RW => 0x0804A000-0x0806C000 RW
- Size increases by  135 168 bits (0x21000 bits)

4th memory region:
0x0804B000-0xF7DC5000 NO => 0x0806C000-0xF79F4000 NO
- Size decreases by 4 136 960 bits (0x3F2000 bits)

5th memory region:
0xF7DC5000-0xF7DC6000 RW => 0xF79F4000-0xF7DC6000 RW
- Size increases by 4 001 792 bits (0x3D1000 bits)

Explanation:
The 3rd memory region, which is a read-write memory region increases in size due to printf. 
A better explanation is on driver 3.

The 

==================
Driver 2 (mem_2.c)
------------------

Action: 
Driver mmaps a large file at a specific address.

Difference: 
- Before the action, there were 18 memory regions. After the action,
there were now 20 memory regions.

3rd memory region:
0x0804A000-0x0804B000 RW => 0x0804A000-0x0806C000 RW

- The two new regions is in the middle of the original huge chunk no access memory region.
4th memory region (before action):
0x0804B000-0xF7DC5000 NO => 0x0806C000-0x70000000 NO
                         => 0x70000000-0x7000A000 RW
                         => 0x7000A000-0xF7DC5000 NO

Explanation:



==================
Driver 3 (mem_3.c)
------------------

Action:
Driver which performs a mathematical function using the standard math library, 
assuming always you compile dynamically loaded executables.

Difference: 
3rd memory region:
0x0804A000-0x0804B000 RW => 0x0804A000-0x0806C000 RW

4th memory region:
0x0804B000-0xF7DC0000 NO => 0x0806C000-0xF7DC0000 NO

Explanation:
