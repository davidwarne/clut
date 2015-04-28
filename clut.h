/* CLUT: An OpenCL Utility Toolkit.
 * Copyright (C) 2015  David J. Warne
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file clut.h
 * @brief A free OpenCL Utility Toolkit.
 * @details Provides an API to simply construction of OpenCL applications.
 *
 * @author David J. Warne
 * @author High Performance Computing and Research Support
 * @author Queensland University of Technology
 *
 * @copyright GNU Public License version 3
 */
#ifndef __FREECLUT_H
#define __FREECLUT_H

#ifdef __APPLE__
// why does apple just have to be different?
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif // #ifdef __APPLE__

#ifdef __cplusplus
extern "C" {
#endif // #ifdef _cplusplus

/*Currently not worried about supporting windows, but in the future I may need to */
#ifdef _WIN32
    #error "Windows is not supported, please install a proper operating system."
#else

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FCAPI
#define FCAPIENTRY

typedef cl_uint CLenum;

// these are for clutGet

#define     CLUT_LAST_ERROR                     0x6
#define     CLUT_SOURCE                         0x7
#define     CLUT_BINARY                         0x8

// defines for malloc and memcpy flags
#define     CLUT_HOST_PTR                       0x1
#define     CLUT_DEVICE_PTR                     0x0
#define     CLUT_MALLOC_HOST_PTR                CLUT_HOST_PTR
#define     CLUT_MALLOC_DEVICE_PTR              CLUT_DEVICE_PTR
#define     CLUT_READ                           0x4
#define     CLUT_WRITE                          0x8
#define     CLUT_READ_WRITE                     0xC
#define     CLUT_BANK_SELECT_0                  0x10
#define     CLUT_BANK_SELECT_1                  0x20
// memcpy directions
#define     CLUT_MEMCPY_DEVICE2DEVICE           ((CLUT_DEVICE_PTR << 1) | CLUT_DEVICE_PTR)
#define     CLUT_MEMCPY_DEVICE2HOST             ((CLUT_DEVICE_PTR << 1) | CLUT_HOST_PTR)
#define     CLUT_MEMCPY_HOST2DEVICE             ((CLUT_HOST_PTR << 1) | CLUT_DEVICE_PTR)
#define     CLUT_MEMCPY_HOST2HOST               ((CLUT_HOST_PTR << 1) | CLUT_HOST_PTR)

// access to current OpenCL objects
#define     CLUT_CURRENT_CONTEXT                clut_global_contexts[clut_global_current_context]
#define     CLUT_CURRENT_QUEUE                  clut_global_command_queues[clut_global_current_context][clut_global_current_command_queue]
#define     CLUT_CURRENT_PROGRAM                clut_global_programs[clut_global_current_context][clut_global_current_program]
#define     CLUT_CURRENT_DEVICE                 clut_global_devices[clut_global_current_device]
#define     CLUT_CURRENT_KERNEL                 clut_global_kernels[clut_global_current_device][clut_global_current_kernel]
#define     CLUT_CURRENT_KERNEL_EVENT           clut_global_kernel_events[clut_global_current_device][clut_global_current_kernel]
#define     CLUT_CURRENT_KERNEL_ARG_COUNT       clut_global_kernel_arg_count[CLUT_CURRENT_KERNEL_GLOBAL_ID]
#define     CLUT_CURRENT_KERNEL_ARG(i)          clut_global_kernel_args[CLUT_CURRENT_KERNEL_GLOBAL_ID][(i)]
#define     CLUT_CURRENT_KERNEL_ARG_SIZE(i)     clut_global_kernel_arg_sizes[CLUT_CURRENT_KERNEL_GLOBAL_ID][(i)]

// access to current OpenCl object identifiers
#define     CLUT_CURRENT_CONTEXT_ID             clut_global_current_context
#define     CLUT_CURRENT_QUEUE_ID               clut_global_current_command_queue
#define     CLUT_CURRENT_PROGRAM_ID             clut_global_current_program
#define     CLUT_CURRENT_DEVICE_ID              clut_global_current_device
#define     CLUT_CURRENT_KERNEL_ID              clut_global_current_kernel
#define     CLUT_CURRENT_KERNEL_GLOBAL_ID       (clut_global_current_device*CLUT_MAX_NUM_KERNELS_PER_DEVICE + clut_global_current_kernel)

// current count of OpenCL objects
#define     CLUT_NUM_CONTEXTS                   clut_global_context_count
#define     CLUT_NUM_QUEUES                     clut_global_command_queue_count[clut_global_current_context]
#define     CLUT_NUM_PROGRAMS                   clut_global_program_count[clut_global_current_context]
#define     CLUT_NUM_DEVICES                    clut_global_device_count
#define     CLUT_NUM_KERNELS                    clut_global_kernel_count[clut_global_current_device]


// Altera SDK custom settings
#ifdef __ALTERA__

// size limits
#define     CLUT_MAX_NUM_CONTEXTS               4
#define     CLUT_MAX_NUM_QUEUES_PER_CONTEXT     28
#define     CLUT_MAX_NUM_PROGRAMS_PER_CONTEXT   20
#define     CLUT_MAX_NUM_ENQUEUED_KERNELS       1000
#define     CLUT_MAX_NUM_DEVICES                1
#define     CLUT_MAX_NUM_KERNELS_PER_DEVICE     32
#define     CLUT_MAX_NUM_ARGS_PER_KERNEL        128
#define     CLUT_MAX_KERNEL_ARGS_SIZE           256
#define     CLUT_MAX_NUM_KERNELS                (CLUT_MAX_NUM_DEVICES * CLUT_MAX_NUM_KERNELS_PER_DEVICE) 

// Altera FPGA device is classified as an ACCELERATOR
#ifdef CL_DEVICE_TYPE_DEFAULT
#undef CL_DEVICE_TYPE_DEFAULT
#define CL_DEVICE_TYPE_DEFAULT CL_DEVICE_TYPE_ACCELERATOR
#else
#define CL_DEVICE_TYPE_DEFAULT CL_DEVICE_TYPE_ACCELERATOR
#endif // # CL_DEVICE_TYPE_DEFAULT

#ifndef CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS
#define     CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS "-cl-fast-relaxed-math"
#endif // #ifdef CLUT_DEFAULT_PROGRAM_BUILD_OPTIONS
#endif // #ifdef __ALTERA__

// NVidia SDK custom settings
#ifdef __NVIDIA__
// I can't find info on this for NVidia... maybe unbounded, but I've 
// just set it to be the same as Altera till I get more information
#define     CLUT_MAX_NUM_CONTEXTS               4
#define     CLUT_MAX_NUM_QUEUES_PER_CONTEXT     28
#define     CLUT_MAX_NUM_PROGRAMS_PER_CONTEXT   20
#define     CLUT_MAX_NUM_ENQUEUED_KERNELS       1000
#define     CLUT_MAX_NUM_DEVICES                1
#define     CLUT_MAX_NUM_KERNELS_PER_DEVICE     32
#define     CLUT_MAX_NUM_ARGS_PER_KERNEL        128
#define     CLUT_MAX_KERNEL_ARGS_SIZE           256
#define     CLUT_MAX_NUM_KERNELS                (CLUT_MAX_NUM_DEVICES * CLUT_MAX_NUM_KERNELS_PER_DEVICE) 

// NVidia accelorators are GPUs
#ifdef CL_DEVICE_TYPE_DEFAULT
#undef CL_DEVICE_TYPE_DEFAULT
#define CL_DEVICE_TYPE_DEFAULT CL_DEVICE_TYPE_GPU
#else
#define CL_DEVICE_TYPE_DEFAULT CL_DEVICE_TYPE_GPU
#endif // #ifdef CL_DEVICE_TYPE_DEFAULT
#ifndef CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS
#define     CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS "-cl-fast-relaxed-math"
#endif // #ifdef CLUT_DEFAULT_PROGRAM_BUILD_OPTIONS
#endif // #ifdef __NVIDIA__

// AMD settings, currently not supported
#ifdef __AMD__
#error "AMD is not currently supported"
#endif // #ifdef __AMD__

// Intel settings, currently not supported
#ifdef __INTEL__
#error "Intel is not currently supported"
#endif // #ifdef __INTEL__

#endif // #ifdef _WIN32

/*Device set up functions*/
FCAPI void FCAPIENTRY clutInit(int, char **);

/* Get functions*/
FCAPI int FCAPIENTRY clutGet(CLenum);
FCAPI void* FCAPIENTRY clutContextGet(CLenum);
FCAPI void* FCAPIENTRY clutPlatformGet(CLenum);
FCAPI void* FCAPIENTRY clutDeviceGet(CLenum);
FCAPI void* FCAPIENTRY clutCommandQueueGet(CLenum);
FCAPI void* FCAPIENTRY clutProgramGet(CLenum);
FCAPI void* FCAPIENTRY clutKernelGet(CLenum);

/*Set functions*/
FCAPI void FCAPIENTRY clutSetContext(int);
FCAPI void FCAPIENTRY clutSetDevice(int);
FCAPI void FCAPIENTRY clutSetProgram(int);
FCAPI void FCAPIENTRY clutSetKernel(int);
FCAPI void FCAPIENTRY clutSetCommandQueue(int);

FCAPI void FCAPIENTRY clutProgramDevice(const char*,int);
/*Device close functions*/
FCAPI void FCAPIENTRY clutExit(void);

/*allocation*/
FCAPI void* FCAPIENTRY clutMalloc(size_t, unsigned int);
FCAPI void FCAPIENTRY clutMemcpy(void*, void*, size_t, unsigned int);
FCAPI void FCAPIENTRY clutMemset(void*, int, size_t, unsigned int);
/*kernel execution*/
FCAPI int FCAPIENTRY clutExec(const char *, void **, size_t *, int, int, size_t*, size_t*, unsigned int);
FCAPI void FCAPIENTRY clutWait(void);
FCAPI void FCAPIENTRY clutWaitAll(void);

/*Misc Utility functions*/
FCAPI double FCAPIENTRY clutGetLastExecTime(void);
FCAPI int FCAPIENTRY clutPrintLastError(void);
FCAPI int FCAPIENTRY clutErrorOccurred(void);
#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __FREECLUT_H

