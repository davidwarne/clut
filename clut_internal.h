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


#ifndef __CLUT_INTERNAL_H
#define __CLUT_INTERNAL_H

#ifdef __APPLE__
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif



#define CLUT_STATUS_NO_ERROR 0x0
#define CLUT_STATUS_FATAL_ERROR 0x1
#define CLUT_STATUS_MEMORY_ERROR 0x2
#define CLUT_STATUS_DEVICE_ERROR 0x4
#define CLUT_STATUS_PLATFORM_ERROR 0x8
#define CLUT_STATUS_QUEUE_ERROR 0x10
#define CLUT_STATUS_KERNEL_ERROR 0x20
#define CLUT_STATUS_PROGRAM_ERROR 0x40
#define CLUT_STATUS_CONTEXT_ERROR 0x80
#define CLUT_STATUS_EVENT_ERROR 0x100

unsigned char clut_internal_handle_error(cl_int );
cl_uint clut_internal_safe_execute(void);
cl_uint clut_internal_get_error_state(void);
cl_int clut_internal_get_current_error(void);

#ifdef __cplusplus
}
#endif
#endif
