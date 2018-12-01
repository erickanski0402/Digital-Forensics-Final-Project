/********************************************************************
 *
 * File         : copyfiletdfs.h
 * Author       : Team 1
 * Date         : September 16, 2018
 * Description  : This file contains type definitions        
 *                related to copyfile module
 *
 *******************************************************************/

#define BYTE_SIZE 8
#define FULL_BYTE 0xFF


struct free_inode {
	UINT4 block_num;
	UINT4 inode_num;
};
