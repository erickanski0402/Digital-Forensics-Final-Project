#include "copyfileinc.h"
#include "inodeinc.h"

VOID CopyFileUtilCopyFile(INT4 fd, struct ext3_inode OldInode, CHAR* Name) {
    UINT8 u8Offset;
    UINT4 u4ClaimedInodeNo;
    //struct ext3_inode OldInode;
    struct ext3_inode RootInode;
    struct ext3_dir_entry_2 NewDirectoryEntry;

    // Get old inode
    // memset(&OldInode, 0, sizeof(OldInode));
    // if (InodeUtilReadInode(u4OldInodeNo, &OldInode) == INODE_FAILURE)
    // {
    //     printf("ERROR: Failed to read Inode: %d %s:%d\n", u4OldInodeNo, __FILE__, __LINE__);
    //     return;
    // }

    // Claim new inode
    u4ClaimedInodeNo = 0;
    if (CopyFileUtilClaimFreeInode(&u4ClaimedInodeNo) == INODE_FAILURE)
    {
        printf("ERROR: Failed to claim new Inode: %s:%d\n", __FILE__, __LINE__);
        return;
    }

    // Get new inode offset
    u8Offset = 0;
    if (InodeUtilGetInodeOffset(u4ClaimedInodeNo, &u8Offset) == INODE_FAILURE)
    {
        printf("ERROR: Failed to get Inode offset: %d %s:%d\n", u4ClaimedInodeNo, __FILE__, __LINE__);
        return;
    }

    //Seek to new inode location and copy old inode info to it
    if (InodeUtilWriteDataOffset(u8Offset, &OldInode, sizeof(OldInode)) == INODE_FAILURE)
    {
	printf("ERROR: Failed to write Block %s:%d\n", __FILE__, __LINE__);
	return;
    }

    // Get root inode
    memset(&RootInode, 0, sizeof(RootInode));
    if (InodeUtilReadInode(ROOT_INODE, &RootInode) == INODE_FAILURE)
    {
        printf("ERROR: Failed to read Inode: %d %s:%d\n", ROOT_INODE, __FILE__, __LINE__);
        return;
    }

    // Create new directory entry to put in root inode
    CopyFileUtilCreateDirectoryEntry(&NewDirectoryEntry, u4ClaimedInodeNo, Name);

    // Add directory entry to root inode block 0
    if (InodeDirAddChildEntry(&NewDirectoryEntry, RootInode.i_block[0]) == INODE_FAILURE)
    {
	printf("ERROR: Failed to add child entry Inode: %d %s:%d\n", ROOT_INODE, __FILE__, __LINE__);
        return;
    }
}

// Claims an inode and returns the block number and inode number
INT4 CopyFileUtilClaimFreeInode(UINT4* u4ClaimedInodeNo) {
	UINT8 u8GbdOffset;
    	UINT4 u4GroupNo;
	UINT4 u4ByteIndex, u4BitIndex;
        UINT1 pBuffer[gu4BlockSize];
	INT1 i1IsBitUsed;
	struct ext3_group_desc GroupDes;

	u4GroupNo = 0;
	// Loop through block groups until a free inode is found
	while (1)
	{
		memset(&GroupDes, 0, sizeof(GroupDes));
    		u8GbdOffset = gu4BlockSize + u4GroupNo * sizeof(struct ext3_group_desc);
    		if (InodeUtilReadDataOffset(u8GbdOffset, &GroupDes, sizeof(struct ext3_group_desc)) == INODE_FAILURE)
		{
    		    printf("ERROR: Failed to read Block group descriptor table %s:%d\n", __FILE__, __LINE__);
    		    return INODE_FAILURE;
    		}
		if (InodeUtilReadDataBlock(GroupDes.bg_inode_bitmap, 0, pBuffer, gu4BlockSize) == INODE_FAILURE)
		{
			printf("ERROR: Failed to read Block %s:%d\n", __FILE__, __LINE__);
			return INODE_FAILURE;
		}
		for (u4ByteIndex = 0; u4ByteIndex < gu4BlockSize; u4ByteIndex++)
		{
			// Check each byte to see if it is all 1's (0xFF)
	    		if (pBuffer[u4ByteIndex] != FULL_BYTE)
			{
				// If byte is not all 1's, find the first free bit
				for (u4BitIndex = 0; u4BitIndex < BYTE; u4BitIndex++)
				{
					i1IsBitUsed = ((pBuffer[u4ByteIndex] >> u4BitIndex) & 1);

					if (i1IsBitUsed == 0)
					{
						// Write claimed bit into inode table
						pBuffer[u4ByteIndex] = (pBuffer[u4ByteIndex] | (1 << u4BitIndex));
						if (InodeUtilWriteDataBlock(GroupDes.bg_inode_bitmap, 0, pBuffer, gu4BlockSize) == INODE_FAILURE) {
							printf("ERROR: Failed to write Block %s:%d\n", __FILE__, __LINE__);
							return INODE_FAILURE;
						}

						*u4ClaimedInodeNo = (u4GroupNo * sb.s_inodes_per_group) + ((u4ByteIndex * BYTE) + u4BitIndex + 1);
						return INODE_SUCCESS;
					}
				}
			}
		}
		u4GroupNo++;
	}
	return 0;
}

VOID CopyFileUtilCreateDirectoryEntry(struct ext3_dir_entry_2* pDirEntry, UINT4 u4InodeNo, CHAR* Name)
{
	pDirEntry->inode = u4InodeNo;
	pDirEntry->file_type = EXT3_FT_REG_FILE;
	pDirEntry->name_len = strlen(Name);
	memset(pDirEntry->name, 0, EXT3_NAME_LEN);
	memcpy(pDirEntry->name, Name, strlen(Name));
	pDirEntry->rec_len = ceil((float)(pDirEntry->name_len + DIR_ENTRY_NAME_OFFSET) / 4) * 4;
}
