# Implementation

## Quality

The program takes a stream of input, and outputs the current windowset size and
the count of memory accesses that has already been fed to the program for every input beyond
the skipsize. The count of memory accesses start with 1. The output looks like this:

<pre>
memory-access-count, window-set-size
</pre>

## Details

The program uses three key data structures: (1) a queue, (2) a singly-linked list, and 
(3) a hash table.

When the program receives a line of input, it processes it and checks if it matches
the format of a memory access. A line that's a memory access looks like this:

<pre>
{I|S|L|M} address,size
</pre>

The address and size is simplified before being put into the data structures. Addresses
remove the least significant digits based on the given page size. The size is converted
into page counts that depend on where the address begin and ends. The formula below is 
relevant:

<pre>
page_size = 2^(page_exponent)
processed_address = address >> page_exponent
lower_address = processed_address
upper_address = (addresss + page_size - 1) >> page_exponent

// adjusted with + 1 since an access should at least occupy one page
page_count = (upper_address - lower_address) + 1
</pre>

Using the processed data, we enqueue them into a queue. This will help us knowing what we need
to evict once the windowset is full. To handle overlapping pages, we break apart the processed
data into individual pages, and we put them into our hashtable.

When the windowset exceeds its capacity, we dequeue the queue. We use the details about
the evicted memory once broken apart into individual pages to know what to 
delete in the hashtable.

When getting the size of the windowset, we get the size of the hashtable.

## Queue

The queue is a standard doubly-linked list. It's worst space complexity is O(n) where n is
the window size + 1. For enqueueing and dequeueing, the worst time complexity is O(1)
since we keep a reference of what's the tail and head.

## Singly-Linked List

The singly-linked list is used in the Hashtable. Due to how it's used in the program, all 
uses of singly-linked lists at most have 15 elements, which is based on the macro-defined 
constant SPLIT_SIZE. This would also mean that accesses in singly-linked list at most
have to traverse through 15 elements.

## Hashtable

The hashtable is composed of 16 buckets in an array, count of data in them, their level,
and the number of shifts.

These buckets contain the data. They are either used as another hashtable or a singly-linked 
list depending on the amount of data in them. We could have made the data structure smaller
by making the the two pointers into a union, but I felt like it's not worth the performance
cost of more creation and destruction of data structures.

The count of data in them are updated based on the detected changes in their buckets and based
on the action taken at the time. So, if a removal was done and a bucket size has changed,
then we reduce the count manually.

The level dictates what the shift is. It's kept for easy calculation when a bucket
is required to convert itself as a hashtable, and it would need to the parent's level
plus one as its level.

The shift dictates how data is indexed. It shifts the key, and grabs the 4 least significant
bits as the index.

(Assumption: this is in 64 bits) Worst case scenario for spacing (aside from using the 
entire address space) would occur if the difference between the keys are between 
the most significant bits. The max level of 16 would occur given the minimum 
page size of 2^4. The formula below better showcases the logic how the max level is obtained.

<pre>
max level = 15 = ((64 bits - 4 bits for the min page size) 
    / 4 bits from macro-defined TABLE_SIZE) + 1 due to the split size of 15 
    when 16 should be in the bucket
</pre>

The worst time complexity case for search follows the previous case, but it is in the case
where there are 15 levels, and 15 items in the singly-linked list. This would be
something like ~30 iterations of going from one data structure to another.

Insertion and deletion follows the worst time complexity case for search, with the
added fact of buckets that may shrink or split. So, the worst time for each is 15 levels 
multiplied by SPLIT_SIZE or MERGE_SIZE respectively. For insertion, that's 225 accesses
to distinct data structures. For deletion, that's 90 accesses to distinct data structures.

## Implementation performance

Space used by the implementation is the combination of that of the queue
and the hashatble's. Worst case for time spent are in the hashtable where there
may be expanding or shrinking buckets during insertion / deletion.