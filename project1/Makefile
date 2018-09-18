# CSE121 Project 1 Makefile
#
# For use on ieng6-204.ucsd.edu ONLY
#
# Copyright (C) 2007-10 Alex C. Snoeren <snoeren@cs.ucsd.edu>

all:	cse121fs cse121mkfs

cse121fs:	cse121fs.c
	gcc $< -lfuse -D_FILE_OFFSET_BITS=64 -o $@

cse121mkfs:	cse121mkfs.c
	gcc $< -o $@

clean:	
	rm -f cse121fs cse121mkfs
