#include"mp3recovery.h"

VOID main(INT4 argc, CHAR** argv){
  //Ensures the user has given a device to read
  if(argc < 3){
    printf("Please include desired device as command line arguments\n");
    printf("Ex: ./make <partition> <name>\n");
    return;
  }

  //If a device has been provided it is passed through the initalize function
  CHAR *pDev = argv[1];
  CHAR *pName = argv[2];

  INT4 fd = open(pDev, O_RDWR);
  gpPartition = fopen(pDev, "r");

  if(gpPartition == NULL){
    printf("Failed to open device\n");
    return;
  }

  if(fd < 0){
    printf("Failed to open device\n");
    return;
  }

  InodeInit(fd);
  Mp3RecoveryInit(fd, pName);
  //Assuming the device has been properly opened for reading the blocks are then read in
  Mp3RecoveryUtilReadBlock(fd);
  InodeInitExit();
}

//TODO: How do we identify that the mp3 file we found is the one needing to be recovered?
