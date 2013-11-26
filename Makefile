
# This is a GNU Makefile.

# It can be used to compile an OpenCL program with
# the Altera SDK for OpenCL.
# See README.txt for more information.


# You must configure ALTERAOCLSDKROOT to point the root directory of the Altera SDK for OpenCL
# software installation.
# See doc/getting_started.txt for more information on installing and
# configuring the Altera SDK for OpenCL.


# Creating a static and shared library
TARGET = libclut
STATIC = $(TARGET).a
SHARED = $(TARGET).so

CC = gcc
COPTS = -O2 -fPIC
AR = ar
AROPTS = -rcvs
# Libraries to use, objects to compile
SRC = clut.c clut_internal.c 
OBJ = $(SRC:.c=.o)

# Where is the Altera SDK for OpenCL software?
ifneq ($(wildcard $(ALTERAOCLSDKROOT)),)

ifeq ($(wildcard $(ALTERAOCLSDKROOT)/host/include/CL/opencl.h),)
$(error Set ALTERAOCLSDKROOT to the root directory of the Altera SDK for OpenCL software installation.)
endif

#$(error Set ALTERAOCLSDKROOT to the root directory of the Altera SDK for OpenCL software installation)
# OpenCL compile and link flags.
COPTS += -D__ALTERA__
OCL_COMPILE_CONFIG = $(shell aocl compile-config)
OCL_LINK_CONFIG = $(shell aocl link-config)
else
ifneq ($(wildcard $(NVIDIAOCLSDKROOT)),)

ifeq ($(wildcard $(NVIDIAOCLSDKROOT)/include/CL/opencl.h),)
$(error Set NVIDIAOCLSDKROOT to the root directory of the NVIDIA SDK for OpenCL software installation.)
endif
COPTS += -D__NVIDIA__ 
OCL_COMPILE_CONFIG = -I$(NVIDIAOCLSDKROOT)/include
OCL_LINK_CONFIG = 
endif
endif

INCLUDE = $(OCL_COMPILE_CONFIG) 
LIB = $(OCL_LINK_CONFIG) -lOpenCL

all:
	make $(STATIC)
	make $(SHARED)

# Make it all!
$(STATIC): $(OBJ)
	$(AR) $(AROPTS) $@ $?
	
$(SHARED): $(OBJ)
	$(CC) -shared -o $@ $? $(LIB)

clut.o: clut.c
	$(CC) $(COPTS) -o $@ -c $< $(INCLUDE) 

clut_internal.o: clut_internal.c
	$(CC) $(COPTS) -o $@ -c $< $(INCLUDE) 

# Standard make targets
clean :
	@rm -f *.o $(STATIC) $(SHARED)
