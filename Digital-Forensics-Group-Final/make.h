#Base Directory
BASE_DIR = ${PWD}

#Compiler configuration
CC = gcc
CPP = g++

# Compilation flags/switches
#DEBUG_FLAGS = -DTRACE_WANTED

CC_FLAGS = -c -g -O -gdwarf-3 -Wunused \
           $(DEBUG_FLAGS) \

CPP_FLAGS = -c -g -O2 -gdwarf-3 -Wunused \
           $(DEBUG_FLAGS) \

MODULE = copyfile

MODULE_DIR = ${PWD}

#Include files directory
MODULE_INC_DIR = ${MODULE_DIR}/inc

#Source files directory
MODULE_SRC_DIR = ${MODULE_DIR}/src

#Object files directory
MODULE_OBJ_DIR = ${MODULE_DIR}/obj

#Library files directory
MODULE_EXE_DIR = ${MODULE_DIR}/exe

#Library files directory
MODULE_LIB_DIR = 

#Other include directiories
COMMON_INC_DIR = ${BASE_DIR}/common/inc
HASH_INC_DIR = ${BASE_DIR}/uthash/include


MODULE_FINAL_INC_DIRS = -I${MODULE_INC_DIR} \
                        -I${COMMON_INC_DIR} \
                        -I${HASH_INC_DIR}

