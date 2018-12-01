/********************************************************************
 *
 * File         : inodeglob.h
 * Author       : Naveen Raj Selvaraj
 * Date created : March 16, 2016
 * Description  : This file contains all the global variables used 
 *                by inode functions
 *
 *******************************************************************/
struct ext3_group_desc gd;
struct ext3_super_block sb;
struct htSortByCurDirInode *tempDeletedFiles; /*hash table store the "maybe" deleted files*/
struct htSortByCurDirInode *existingFiles; /*hash table store all existing files*/
struct htSortByCurDirInode *deletedFiles; /*hash table store all deleted files: tempDeletedFiles - existingFiles*/

UINT4 gu4BlockSize; /* Block Size */
UINT4 gu4FileDes;  /* File descriptor to the device */ 

UINT4 gu4CorruptedArray[MAX_CORR_INODES];
INT4 gi4ArrayIndex;

UINT4 gu4TotalInodes;
UINT1 *gpInodeArray;

UINT4 gu4RootDataBlock;
