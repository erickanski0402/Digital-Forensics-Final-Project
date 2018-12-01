/********************************************************************
 *
 * File         : inodetdfs.h
 * Author       : Naveen Raj Selvaraj
 * Date         : March 17, 2016
 * Description  : This file contains type definitions
 *                related to inode module
 *
 *******************************************************************/
typedef enum fileFilter_e
{
    FILE_TYPE_DIRS,
    FILE_TYPE_FILES,
    FILE_TYPE_ALL
}tFileFilter;

enum
{
    FILE_TYPE_UNKNOWN = 0,
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2
};

enum
{
	INODE_CORRUPT =1,
	INODE_FIX =2
};

struct DirEntryInfo
{
	struct ext3_dir_entry_2 DirEntry;
	char path[255];
	//todo: why 255
};

struct htFilesStoreInOneDir {
    UINT4 u4InodeNo;
    struct DirEntryInfo *entryInfo;
    UT_hash_handle hh;
};

struct htSortByCurDirInode {
	UINT4 u4CurrentDirInodeNo;
	struct htFilesStoreInOneDir *htFileStoreInThisDir;
	UT_hash_handle hh;
};
