#include"mp3recovery.h"
#include<errno.h>

VOID Mp3RecoveryInit(INT4 fd, CHAR *name){
  //Appropriate values are set and the array containing block groups is preallocated
  gBlocksize = BLOCK_SIZE_;
  gpBlockArray = (UINT1*)calloc(BLOCK_SIZE_ * BLOCK_GROUP, sizeof(UINT1));

  //Initiatlizes file size and the global frame header struct
  gFileSize = 0;

  memset(&gNewInode, 0, sizeof(gNewInode));

  gpFileName = name;
}
