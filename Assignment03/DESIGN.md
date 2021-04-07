We want hash table cause we fancy.

What is our hash function???
hash function should consider page size!
strip off n bits >> where n is from pagesize = 2^(n-1)
maybe a modulo function is okay? cause if we assume that memory does it's
best to keep things compact, then maybe

Linear hashing???

Note:
Please be advised that the address can be 64 bits long. If you convert it into a number, be sure to use unsigned long long and strtoull() function instead of int and strtoll. This is a very subtle problem and may be hard to debug.