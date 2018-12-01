#include"mp3recovery.h"

INT4 Mp3RecoveryUtilReadBlock(INT4 fd){
  //Total blocks in partition is read in from primary super block
  UINT4 numBlocks = Mp3RecoveryUtilGetTotalBlocks(), byteNum, checkByte, syncSafeSize;
  //Moves the file pointer to the beginning of the device
  fseek(gpPartition, 0, SEEK_SET);

  UINT2 currentGroup;
  //Iterates through every block group calculated to be on the device
  for(currentGroup = 0; currentGroup < (numBlocks / (BLOCK_GROUP)); currentGroup++){
    //Reads an entire block group into the array
    fread(gpBlockArray, BLOCK_SIZE_ * BLOCK_GROUP, 1, gpPartition);
    //Passes the block group being searched in to determine if there is an MP3 file located in it
    byteNum = Mp3RecoveryUtilCheckForTag(currentGroup);
    //printf("No other files found in block group %d\n", currentGroup);
    //Grabs the 'sync safe' size of the ID3 tag from the header, storing it as one number
    syncSafeSize = (gpBlockArray[byteNum + 6] << 24) |
                   (gpBlockArray[byteNum + 7] << 16) |
                   (gpBlockArray[byteNum + 8] << 8) |
                   (gpBlockArray[byteNum + 9] << 0);

    CHAR syncBinary[32];
    //Converts sync safe from decimal to binary string
    Mp3RecoveryUtilConvertToBinary(syncBinary, syncSafeSize);
    //Converts binary string containing sync safe size to true size
    UINT4 id3TagSize = Mp3RecoveryUtilRemoveSyncSafe(syncBinary);
    //Moves array to what should be the start of the first mp3 frame
    checkByte = byteNum + id3TagSize;

    //if the sync word is found, it is safe to assume an MP3 file has been found
    if(Mp3RecoveryUtilCheckForSyncWord(gpBlockArray[checkByte], gpBlockArray[checkByte + 1])){
      break;
    }
  }

  printf("Checking Direct blocks\n");
  UINT1 DirectBlocks = Mp3RecoveryUtilCheckDirects(byteNum, NUM_DIRECTS);
  UINT2 FirstIndirects = 0, SecondIndirects = 0, ThirdIndirects = 0;
  if(DirectBlocks == 12){
    printf("Checking First Indirects\n");
    UINT4 indirectBlockByte = Mp3RecoveryUtilFindIndirectBlock(currentGroup, (byteNum / 4096) + 11);
    FirstIndirects = Mp3RecoveryUtilCheckFirstIndirects(indirectBlockByte, gNewInode.i_block[11]);
    gNewInode.i_block[EXT3_IND_BLOCK] = FirstIndirects;

    if(FirstIndirects == 1024){
      //Look for Second Indirects
      if(SecondIndirects == 1024){
        //Look for Third Indirects
        gNewInode.i_blocks = EXT3_NDIR_BLOCKS + NUM_FIRST_INDIRECTS + NUM_SECOND_INDIRECTS + ThirdIndirects;
      }else{
        gNewInode.i_blocks = EXT3_NDIR_BLOCKS + NUM_FIRST_INDIRECTS + SecondIndirects;
      }
    }else{
      gNewInode.i_blocks = NUM_DIRECTS + FirstIndirects;
    }
  }else{
    gNewInode.i_blocks = DirectBlocks;
  }

  gNewInode.i_size = gNewInode.i_blocks * BLOCK_SIZE_;
  gNewInode.i_mode = 33188;

  CopyFileUtilCopyFile(fd, gNewInode, gpFileName);
}

UINT4 Mp3RecoveryUtilCheckForTag(UINT4 currentGroup){
  UINT4 currentBlock, currentByte;
  //Reads the first four bytes in each block in the group
  for(currentBlock = 0; currentBlock < (BLOCK_GROUP); currentBlock++){
    currentByte = BLOCK_SIZE_ * currentBlock;
    //If the proper values are found in the right order it is more or less safe to assume an ID3 tag has been located
    if(gpBlockArray[currentByte] == ID_1 && gpBlockArray[(currentByte) + 1] == ID_2 && gpBlockArray[currentByte + 2] == ID_3){
      //Byte address is calculated and printed for the user
      printf("Mp3 ID3 tag found at block number: %d\n", (BLOCK_GROUP * BLOCK_SIZE_ * currentGroup + currentByte) / 4096);

      return currentByte;
    }
  }

  return BLOCK_FAILURE;
}

// VOID Mp3RecoveryUtilStoreID3Tag(UINT4 currentByte){
//   CHAR size[4];
//   INT4 temp;
//
//   gID3header.id[0] = gpBlockArray[currentByte];
//   gID3header.id[1] = gpBlockArray[currentByte + 1];
//   gID3header.id[2] = gpBlockArray[currentByte + 2];
//   gID3header.flags = gpBlockArray[currentByte + 3];
//   for(temp = currentByte; temp < currentByte + 4; temp++){
//     sprintf(size[temp - currentByte], "%d", currentByte);
//   }
//   printf("%s\n", size)
//   //tID3header.size = gpBlockArray[currentByte];
// }

UINT4 Mp3RecoveryUtilGetTotalBlocks(){
  UINT4 numBlocks = 0;
  //Seeks the position in the primary superblock where the value of the total blocks is stored
  fseek(gpPartition, 1028, SEEK_SET);
  //Reads the 4 byte value
  fread(&numBlocks, 1, 4, gpPartition);
  //returns it for use
  return numBlocks;
}

VOID Mp3RecoveryUtilConvertToBinary(CHAR binary[], UINT4 syncSafeSize){
  //binary = calloc(32, sizeof(CHAR));
  UINT8 c, k;

  for(c = 0; c < 32; c++){
    k = syncSafeSize >> c;

    if(k & 1)
      binary[31 - c] = '1';
    else
      binary[31 - c] = '0';
  }

  //printf("%s\n", binary);
}

UINT8 Mp3RecoveryUtilRemoveSyncSafe(CHAR* binary){
  UINT2 count = 0, iterate = 0;
  CHAR trueSize[28];

  while(count < 28){
    if(iterate % 8 != 0){
      trueSize[count] = binary[iterate];
      count++;
    }
    iterate++;
  }

  return (INT4)strtol(trueSize, NULL, 2) + SIZE_OF_ID3_HEADER;
}

INT1 Mp3RecoveryUtilCheckForSyncWord(UINT1 first, UINT1 second){
  if(first == MP3_1 && second == MP3_2)
    return 1;

  return -1;
}

// VOID Mp3RecoveryUtilStoreMp3Header(CHAR* header, struct MP3_Header *frameheader){
//   // UINT1 version, layer, crc, bitrate, freq, padding, private, channel, extension, copy, original, emphasis;
//
//   if(header[11] == '0'){
//     if(header[12] == '0'){
//       frameHeader.mpeg_version = "2.5";
//     }else if(header[12] == '1'){
//
//     }
//   }else if(header[11] == '1'){
//     if(header[12] == '0'){
//
//     }else if(header[12] == '1'){
//
//     }
//   }
// }

UINT8 Mp3RecoveryUtilGetFrameSize(INT1 bitrateBinary, INT1 freqBinary){
  INT4 bitrate, freq;
  switch(bitrateBinary){
      case 0: bitrate = BITRATE_free;
    break;
      case 1: bitrate = BITRATE_32;
    break;
      case 2: bitrate = BITRATE_40;
    break;
      case 3: bitrate = BITRATE_48;
    break;
      case 4: bitrate = BITRATE_56;
    break;
      case 5: bitrate = BITRATE_64;
    break;
      case 6: bitrate = BITRATE_80;
    break;
      case 7: bitrate = BITRATE_96;
    break;
      case 8: bitrate = BITRATE_112;
    break;
      case 9: bitrate = BITRATE_128;
    break;
      case 10: bitrate = BITRATE_160;
    break;
      case 11: bitrate = BITRATE_192;
    break;
      case 12: bitrate = BITRATE_224;
    break;
      case 13: bitrate = BITRATE_256;
    break;
      case 14: bitrate = BITRATE_320;
    break;
      default: bitrate = BITRATE_bad;
    break;
  }

  switch(freqBinary){
      case 0: freq = SAMPLE_RATE_44100;
    break;
      case 1: freq = SAMPLE_RATE_48000;
    break;
      case 2: freq = SAMPLE_RATE_32000;
    break;
      default: freq = SAMPLE_RATE_reserved;
    break;
  }

  printf("%d %d\n", bitrate, freq);
  return (144 * bitrate / freq);
}

VOID Mp3RecoveryUtilSubString(CHAR str[], CHAR new[], INT1 start, INT1 length){
  UINT2 character = 0;
  while(character < length){
    new[character] = str[start + character];
    character++;
  }
  new[character] = '\0';
}

UINT4 Mp3RecoveryUtilFindIndirectBlock(UINT4 currentGroup, UINT4 lastDirect){
  UINT4 currentBlock, currentByte, address1, address2, address3;
  //Reads the first bytes in each block in the group
  for(currentBlock = 0; currentBlock < (BLOCK_GROUP); currentBlock++){
    currentByte = BLOCK_SIZE_ * currentBlock;

    address1 = (gpBlockArray[3 + currentByte] << 24) |
               (gpBlockArray[2 + currentByte] << 16) |
               (gpBlockArray[1 + currentByte] << 8)  |
               (gpBlockArray[0 + currentByte] << 0);

    address2 = (gpBlockArray[7 + currentByte] << 24) |
               (gpBlockArray[6 + currentByte] << 16) |
               (gpBlockArray[5 + currentByte] << 8)  |
               (gpBlockArray[4 + currentByte] << 0);

    address3 = (gpBlockArray[11 + currentByte] << 24) |
               (gpBlockArray[10 + currentByte] << 16) |
               (gpBlockArray[9 + currentByte] << 8)   |
               (gpBlockArray[8 + currentByte] << 0);

    if(address1 == lastDirect + 1 && address2 == address1 + 1 && address3 == address2 + 1){
      //Byte address is calculated and printed for the user
      printf("Indirect found at block number: %d\n", (BLOCK_GROUP * BLOCK_SIZE_ * currentGroup + currentByte) / 4096);

      return currentByte;
    }
  }
}

UINT1 Mp3RecoveryUtilCheckDirects(UINT4 currentByte, UINT4 numDirects){
  UINT1 blocksUsed = 0;
  UINT4 blocks, bytes;
  for(blocks = 0; blocks < numDirects; blocks++){
    bytes = 0;
    while(gpBlockArray[currentByte + (BLOCK_SIZE * blocks) - bytes - 1] != 0){
      bytes++;
    }

    if(bytes < BLOCK_SIZE_){
      blocksUsed++;
      gNewInode.i_block[blocks] = (currentByte + (blocks * BLOCK_SIZE_)) / BLOCK_SIZE_;
    }
  }

  return blocksUsed;
}

UINT2 Mp3RecoveryUtilCheckFirstIndirects(UINT4 currentByte, UINT4 lastDirect){
  UINT2 blocksUsed = 0;
  UINT4 addresses, bytes, lastFirstIndirect, currentAddress = lastDirect + 1;

  while(currentAddress != 0){
    currentAddress = (gpBlockArray[3 + currentByte] << 24) |
                     (gpBlockArray[2 + currentByte] << 16) |
                     (gpBlockArray[1 + currentByte] << 8)  |
                     (gpBlockArray[0 + currentByte] << 0);

    blocksUsed++;
    currentByte += 4;
  }

  return blocksUsed;
}
