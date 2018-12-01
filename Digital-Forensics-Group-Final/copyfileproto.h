/********************************************************************
 *
 * File         : copyfileproto.h
 * Author       : Team 7
 * Date created : October 1, 2018
 * Description  : This file contains all function prototypes
 *
 *******************************************************************/

/*****************************************************************************/
/*                          copyfileutil.c                                  */
/*****************************************************************************/

VOID CopyFileUtilCreateDirectoryEntry(struct ext3_dir_entry_2* pDirEntry, UINT4 u4InodeNo, CHAR* Name);
INT4 CopyFileUtilClaimFreeInode(UINT4* u4ClaimedInodeNo);
VOID CopyFileUtilCopyFile(INT4 fd, struct ext3_inode OldInode, CHAR* Name);
