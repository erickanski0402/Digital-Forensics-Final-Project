#Mp3Recovery
This program searches for the markers denoting an Mp3 file, determines metadata information (including block addresses), allocates a new inode, stores the information and creates a new link at root directory.

### Contributors
*Eric Kanski - Wrote modules: mp3recoverymain.c mp3recoveryinit.c mp3recoveryutil.c
*Ali Haider

### General Steps
1. Searches each block for ID3 tag markers
2. Upon finding markers looks ahead to confirm the first Mp3 frame is present
3. Determines if all direct blocks are in use
4. Assuming all directs are in use, checks all the first indirect blocks
5. Does the same with second and third indirects
6. Calculates filesize based on blocks used
7. Stores metadate information in a new inode 
8. Links a new file to root.

### Compiling Instructions

1. gcc mp3recoverymain.c mp3recoveryinit.c mp3recoveryutil.c copyfileutil.c inodeinit.c inodeutil.c inodedir.c -o make -lm

2. ./make <partition> <file name>

