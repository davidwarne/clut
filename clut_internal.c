#include "clut_internal.h"

cl_uint error_state;
cl_int current_error;

cl_uint clut_internal_get_error_state(void)
{
    return error_state;
}
cl_uint clut_internal_safe_execute(void)
{
    return error_state == CLUT_STATUS_FATAL_ERROR;
}

cl_int clut_internal_get_current_error(void)
{
    return current_error;
}

unsigned char clut_internal_handle_error(cl_int return_code)
{
    if (return_code != CL_SUCCESS)
    {
        switch(return_code)
        {
            case CL_DEVICE_NOT_FOUND:
                error_state = CLUT_STATUS_DEVICE_ERROR;
                break;
            case CL_DEVICE_NOT_AVAILABLE:
                error_state = CLUT_STATUS_DEVICE_ERROR;
                break;
            case CL_COMPILER_NOT_AVAILABLE:
                error_state = CLUT_STATUS_PLATFORM_ERROR;
                break;
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_OUT_OF_RESOURCES:
                error_state = CLUT_STATUS_PLATFORM_ERROR;
                break;
            case CL_OUT_OF_HOST_MEMORY:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_PROFILING_INFO_NOT_AVAILABLE:
                error_state = CLUT_STATUS_QUEUE_ERROR;
                break;
            case CL_MEM_COPY_OVERLAP:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_IMAGE_FORMAT_MISMATCH:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_IMAGE_FORMAT_NOT_SUPPORTED:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_BUILD_PROGRAM_FAILURE:
                error_state = CLUT_STATUS_PROGRAM_ERROR;
                break;
            case CL_MAP_FAILURE:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_INVALID_VALUE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_DEVICE_TYPE:
                error_state = CLUT_STATUS_DEVICE_ERROR;
                break;
            case CL_INVALID_PLATFORM:
                error_state = CLUT_STATUS_PLATFORM_ERROR;
                break;
            case CL_INVALID_DEVICE:
                error_state = CLUT_STATUS_DEVICE_ERROR;
                break;
            case CL_INVALID_CONTEXT:
                error_state = CLUT_STATUS_CONTEXT_ERROR;
                break;
            case CL_INVALID_QUEUE_PROPERTIES:
                error_state = CLUT_STATUS_QUEUE_ERROR;
                break;
            case CL_INVALID_COMMAND_QUEUE:
                error_state = CLUT_STATUS_QUEUE_ERROR;
                break;
            case CL_INVALID_HOST_PTR:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_INVALID_MEM_OBJECT:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_IMAGE_SIZE:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_SAMPLER:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_BINARY:
                error_state = CLUT_STATUS_PROGRAM_ERROR;
                break;
            case CL_INVALID_BUILD_OPTIONS:
                error_state = CLUT_STATUS_PROGRAM_ERROR;
                break;
            case CL_INVALID_PROGRAM:
                error_state = CLUT_STATUS_PROGRAM_ERROR;
                break;
            case CL_INVALID_PROGRAM_EXECUTABLE:
                error_state = CLUT_STATUS_PROGRAM_ERROR;
                break;
            case CL_INVALID_KERNEL_NAME:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_KERNEL_DEFINITION:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_KERNEL:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_ARG_INDEX:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_ARG_VALUE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_ARG_SIZE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_KERNEL_ARGS:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_WORK_DIMENSION:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_WORK_GROUP_SIZE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_WORK_ITEM_SIZE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_GLOBAL_OFFSET:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                error_state = CLUT_STATUS_EVENT_ERROR;
                break;
            case CL_INVALID_EVENT:
                error_state = CLUT_STATUS_EVENT_ERROR;
                break;
            case CL_INVALID_OPERATION:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_GL_OBJECT:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_BUFFER_SIZE:
                error_state = CLUT_STATUS_MEMORY_ERROR;
                break;
            case CL_INVALID_MIP_LEVEL:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
            case CL_INVALID_GLOBAL_WORK_SIZE:
                error_state = CLUT_STATUS_KERNEL_ERROR;
                break;
            default:
                error_state = CLUT_STATUS_FATAL_ERROR;
                break;
        }       
    }
    else
    {
        error_state = CLUT_STATUS_NO_ERROR;
    }
    current_error = return_code;
    return (return_code != CL_SUCCESS);
}
