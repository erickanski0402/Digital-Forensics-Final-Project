/********************************************************************
 *
 * File         : cmndef.h
 * Author       : Naveen 
 * Date created : October 01, 2014
 * Description  : This file contains the constant common to  
 *                all the modules 
 *
 *******************************************************************/
#define ZERO 0

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE -1

#define SYS_CALL_FAILURE -1
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*if use a ide use this command, change the path as what you want it*/
#if 0
#define DELETED_FILES_CSV_PATH "/home/xinyili/deleted_files.csv"
#endif

#if 1
#define DELETED_FILES_CSV_PATH "../deleted_files.csv"
#endif


