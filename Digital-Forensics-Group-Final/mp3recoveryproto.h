VOID Mp3RecoveryInit(INT4, CHAR*);
INT4 Mp3RecoveryUtilReadBlock(INT4);
UINT4 Mp3RecoveryUtilCheckForTag(UINT4);
UINT4 Mp3RecoveryUtilGetTotalBlocks();
VOID Mp3RecoveryUtilConvertToBinary(CHAR[], UINT4);
UINT8 Mp3RecoveryUtilRemoveSyncSafe(CHAR*);
INT1 Mp3RecoveryUtilCheckForSyncWord(UINT1, UINT1);
//VOID Mp3RecoveryUtilStoreMp3Header(CHAR*, struct MP3_Header*);
UINT8 Mp3RecoveryUtilGetFrameSize(INT1, INT1);
VOID Mp3RecoveryUtilSubString(CHAR[], CHAR[], INT1, INT1);
UINT1 Mp3RecoveryUtilCheckDirects(UINT4, UINT4);
UINT2 Mp3RecoveryUtilCheckFirstIndirects(UINT4, UINT4);
UINT2 Mp3RecoveryUtilCheckSecondIndirects(UINT4);
UINT2 Mp3RecoveryUtilCheckThirdIndirects(UINT4);
UINT2 Mp3RecoveryUtilBytesOfBlockUsed(UINT4);
UINT4 Mp3RecoveryUtilFindIndirectBlock(UINT4, UINT4);