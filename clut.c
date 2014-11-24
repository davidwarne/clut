
#include "clut.h"
#include "clut_internal.h"



/* CLUT workspace */
cl_platform_id      clut_global_platform;
cl_context          clut_global_contexts[CLUT_MAX_NUM_CONTEXTS];
cl_command_queue    clut_global_command_queues[CLUT_MAX_NUM_CONTEXTS][CLUT_MAX_NUM_QUEUES_PER_CONTEXT];
cl_program          clut_global_programs[CLUT_MAX_NUM_CONTEXTS][CLUT_MAX_NUM_PROGRAMS_PER_CONTEXT];
cl_device_id        clut_global_devices[CLUT_MAX_NUM_DEVICES];
cl_kernel           clut_global_kernels[CLUT_MAX_NUM_DEVICES][CLUT_MAX_NUM_KERNELS_PER_DEVICE];
cl_event            clut_global_kernel_events[CLUT_MAX_NUM_DEVICES][CLUT_MAX_NUM_KERNELS_PER_DEVICE];
void *              clut_global_kernel_args[CLUT_MAX_NUM_KERNELS][CLUT_MAX_NUM_ARGS_PER_KERNEL];
size_t              clut_global_kernel_arg_sizes[CLUT_MAX_NUM_KERNELS][CLUT_MAX_NUM_ARGS_PER_KERNEL];

/* counters */
int                 clut_global_context_count;
int                 clut_global_command_queue_count[CLUT_MAX_NUM_CONTEXTS];
int                 clut_global_program_count[CLUT_MAX_NUM_CONTEXTS];
int                 clut_global_device_count;
int                 clut_global_kernel_count[CLUT_MAX_NUM_DEVICES];
int                 clut_global_kernel_arg_count[CLUT_MAX_NUM_KERNELS];

/* current state */
int                 clut_global_current_context;
int                 clut_global_current_command_queue;
int                 clut_global_current_program;
int                 clut_global_current_device;
int                 clut_global_current_kernel;

/*@todo modify so that multiple can execute concurrently*/
unsigned char       clut_global_error_flag;
cl_int              clut_global_error_id;

/*Device set up functions*/

/**
 * @brief Initialises the CL utility toolkit.
 * @details Initial context and device handles created for the default platform.
 * @param argc The number of commandline arguments.
 * @param argv The array of commandling arguments.
 */
FCAPI void FCAPIENTRY clutInit(int argc, char ** argv)
{
    cl_platform_id pf_id;
    cl_uint pf_num;
    cl_uint dev_num;
    cl_context context;
    cl_int rc;
    cl_int numQueues;
    cl_int i;
    /* @todo options might include, number of contexts, a program to start with
     * or a platform to get. For now, not user options are supported.
     */ 

    /* get platform ids*/
    rc = clGetPlatformIDs( 1,
                           &pf_id,
                           &pf_num
                         );
    if(clut_internal_handle_error(rc)) return;
    clut_global_platform = pf_id;
    /* get devices*/
    rc = clGetDeviceIDs( clut_global_platform,
                          CL_DEVICE_TYPE_DEFAULT,
                          0,
                          NULL,
                          &dev_num
                        );
    if(clut_internal_handle_error(rc)) return;
    /*Check the platform supports enough devices*/ 
    if (dev_num > CLUT_MAX_NUM_DEVICES)
    {
        clut_internal_handle_error(CL_OUT_OF_RESOURCES);
    }

    rc = clGetDeviceIDs( clut_global_platform,
                         CL_DEVICE_TYPE_DEFAULT,
                         dev_num,
                         clut_global_devices,
                         NULL
                       );
    if(clut_internal_handle_error(rc)) return;

    CLUT_NUM_DEVICES = dev_num;

    /*create an initial context*/
    context = clCreateContext( 0,
                               CLUT_NUM_DEVICES,
                               clut_global_devices,
                               NULL,
                               NULL,
                               &rc
                             );
    if(clut_internal_handle_error(rc)) return;

    CLUT_CURRENT_CONTEXT_ID = CLUT_NUM_CONTEXTS;
    CLUT_CURRENT_CONTEXT = context;
    
    CLUT_NUM_CONTEXTS++;

    /*create a command queue for every device or at least the max number */
    numQueues = (CLUT_MAX_NUM_QUEUES_PER_CONTEXT < CLUT_NUM_DEVICES) ? CLUT_MAX_NUM_QUEUES_PER_CONTEXT : CLUT_NUM_DEVICES;
    for (i=0;i<numQueues;i++)
    {
        CLUT_CURRENT_DEVICE_ID = i;
        cl_command_queue queue;
        queue = clCreateCommandQueue( CLUT_CURRENT_CONTEXT,
                                      CLUT_CURRENT_DEVICE,
                                      CL_QUEUE_PROFILING_ENABLE,
                                      &rc
                                    );
        if(clut_internal_handle_error(rc)) return;
        CLUT_CURRENT_QUEUE_ID = i;
        CLUT_CURRENT_QUEUE = queue;
        CLUT_NUM_QUEUES++;
    }

    /*all done*/
}

/* Get functions*/
FCAPI int FCAPIENTRY clutGet(CLenum state)
{
    // not really anything to Get yet
}

FCAPI void* FCAPIENTRY clutContextGet(CLenum info)
{
    
}

FCAPI void* FCAPIENTRY clutPlatformGet(CLenum info)
{
}

FCAPI void* FCAPIENTRY clutDeviceGet(CLenum info)
{
}

FCAPI void* FCAPIENTRY clutCommandQueueGet(CLenum info)
{
}

FCAPI void* FCAPIENTRY clutProgramGet(CLenum info)
{
}

FCAPI void* FCAPIENTRY clutKernelGet(CLenum info)
{
}

/*Set functions*/

/**
 * @brief Sets the current OpenCL context to the given context id.
 * @param context The context id of the desired context.
 */
FCAPI void FCAPIENTRY clutSetContext(int context)
{
    if (context < 0 || context > CLUT_NUM_CONTEXTS)
    {
        clut_internal_handle_error(CL_INVALID_CONTEXT); 
    }
    else
    {
        CLUT_CURRENT_CONTEXT_ID = context;
    }
}

/**
 * @brief Sets the current OpenCL device to the given device id.
 * @param device The device id of the desired device.
 */
FCAPI void FCAPIENTRY clutSetDevice(int device)
{
    if (device < 0 || device > CLUT_NUM_DEVICES)
    {
        clut_internal_handle_error(CL_INVALID_DEVICE); 
    }
    else
    {
        CLUT_CURRENT_DEVICE_ID = device;
    }
}

/**
 * @brief Sets the current OpenCL program to the given program id.
 * @param program The program id of the desired program.
 */
FCAPI void FCAPIENTRY clutSetProgram(int program)
{
    if (program < 0 || program > CLUT_NUM_PROGRAMS)
    {
        clut_internal_handle_error(CL_INVALID_PROGRAM); 
    }
    else
    {
        CLUT_CURRENT_PROGRAM_ID = program;
    }
}

/**
 * @brief Sets the current OpenCL kernel to the given kernel id.
 * @param kernel The kernel id of the desired kernel.
 */
FCAPI void FCAPIENTRY clutSetKernel(int kernel)
{
    if (kernel < 0 || kernel > CLUT_NUM_KERNELS)
    {
        clut_internal_handle_error(CL_INVALID_KERNEL); 
    }
    else
    {
        CLUT_CURRENT_KERNEL_ID = kernel;
    }
}

/**
 * @brief Sets the current OpenCL command queue to the given queue id.
 * @param queue The queue id of the desired queue.
 */
FCAPI void FCAPIENTRY clutSetCommandQueue(int queue)
{
    if (queue < 0 || queue > CLUT_NUM_QUEUES)
    {
        clut_internal_handle_error(CL_INVALID_COMMAND_QUEUE); 
    }
    else
    {
        CLUT_CURRENT_QUEUE_ID = queue;
    }
}

/**
 * @brief Programs the device with the given source of binary file.
 * @param file The filename of the source/binary device code file.
 * @param type Either CLUT_SOURCE or CLUT_BINARY
 */
FCAPI void FCAPIENTRY clutProgramDevice(const char* file, int type)
{
    cl_int rc,status;
    cl_program new_prog;
    switch (type)
    {
        // the file contains source code
        case CLUT_SOURCE:
        {
            FILE *fp;
            size_t nbytes;
            unsigned char *source;
            if (!(fp = fopen(file,"r")))
            {
                clut_internal_handle_error(CL_BUILD_PROGRAM_FAILURE);
                return;
            }
            // read the device executable file
            fseek(fp,0,SEEK_END);
            nbytes = ftell(fp);
            if(!(source = (char *)malloc(nbytes*sizeof(char))))
            {
                clut_internal_handle_error(CL_OUT_OF_HOST_MEMORY);
                return;
            }
            rewind(fp);

            if (!(fread((void *)source,nbytes,1,fp)))
            {
                clut_internal_handle_error(CL_BUILD_PROGRAM_FAILURE);
                return;
            }
            
            // check there are enough resources
            if (CLUT_NUM_PROGRAMS >= CLUT_MAX_NUM_PROGRAMS_PER_CONTEXT)
            {
                clut_internal_handle_error(CL_OUT_OF_RESOURCES);
                return;
            }

            new_prog = clCreateProgramWithSource( CLUT_CURRENT_CONTEXT,
                                                  1,
                                                  (const char**)&source,
                                                  (const size_t *) &nbytes,
                                                  &rc
                                                );
            
            if (clut_internal_handle_error(rc)) return;
            // build the program
            rc = clBuildProgram(new_prog,
                                1,
                                (const cl_device_id *)&(CLUT_CURRENT_DEVICE),
                                CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS,
                                NULL,
                                NULL
                               );
            // update the current program reference
            CLUT_CURRENT_PROGRAM_ID = CLUT_NUM_PROGRAMS;        
            CLUT_CURRENT_PROGRAM = new_prog;
            if (clut_internal_handle_error(rc)) return;


        }
            break;
        // the file contains pre-compiled executable code
        case CLUT_BINARY:
        {
            FILE *fp;
            size_t nbytes;
            unsigned char *binary;
            // open the binary file
            if (!(fp = fopen(file,"rb")))
            {
                clut_internal_handle_error(CL_INVALID_BINARY);
                return;
            }

            // read the device executable file
            fseek(fp,0,SEEK_END);
            nbytes = ftell(fp);
            if(!(binary = (unsigned char *)malloc(nbytes*sizeof(unsigned char))))
            {
                clut_internal_handle_error(CL_OUT_OF_HOST_MEMORY);
                return;
            }
            rewind(fp);

            if (!(fread((void *)binary,nbytes,1,fp)))
            {
                clut_internal_handle_error(CL_INVALID_BINARY);
                return;
            }
            
            // check there are enough resources
            if (CLUT_NUM_PROGRAMS >= CLUT_MAX_NUM_PROGRAMS_PER_CONTEXT)
            {
                clut_internal_handle_error(CL_OUT_OF_RESOURCES);
                return;
            }
            
            // create the program object
            new_prog = clCreateProgramWithBinary(CLUT_CURRENT_CONTEXT,
                                                 1,
                                                 (const cl_device_id*) &(CLUT_CURRENT_DEVICE),
                                                 &nbytes,
                                                 (const unsigned char **)&binary,
                                                 &status,
                                                 &rc
                                                );
            if (clut_internal_handle_error(rc)) return;
            // build the program
            rc = clBuildProgram(new_prog,
                                1,
                                (const cl_device_id *)&(CLUT_CURRENT_DEVICE),
                                CLUT_DEFAULT_DEVICE_PROGRAM_BUILD_OPTIONS,
                                NULL,
                                NULL
                               );
            // update the current program reference
            CLUT_CURRENT_PROGRAM_ID = CLUT_NUM_PROGRAMS;        
            CLUT_CURRENT_PROGRAM = new_prog;
            if (clut_internal_handle_error(rc)) return;

        }
            break;
    }
    return;
}

/**
 * @brief Releases OpenCL objects and exits.
 */
FCAPI void FCAPIENTRY clutExit(void)
{
    cl_int rc;
    int device, kernel, context, queue, program;
    /*clean up and exit gracefully*/
    
    // clean up all kernels on all devices
    for (device=0;device<CLUT_NUM_DEVICES;device++)
    {
        CLUT_CURRENT_DEVICE_ID = device;
        for (kernel=0;kernel<CLUT_NUM_KERNELS;kernel++)
        {
            CLUT_CURRENT_KERNEL_ID = kernel;
            rc = clReleaseKernel(CLUT_CURRENT_KERNEL);    
            if (clut_internal_handle_error(rc)) exit(1);
        }
    }
     // clean up every context
    for (context=0;context<CLUT_NUM_CONTEXTS;context++)
    {
        CLUT_CURRENT_CONTEXT_ID = context;
        // release all command queues
        for (queue=0;queue<CLUT_NUM_QUEUES;queue++)
        {
            CLUT_CURRENT_QUEUE_ID = queue;
            rc = clReleaseCommandQueue(CLUT_CURRENT_QUEUE);
            if (clut_internal_handle_error(rc)) exit(1);
        }
        // release all programs
        for (program=0;program<CLUT_NUM_PROGRAMS;program++)
        {
            CLUT_CURRENT_PROGRAM_ID = program;
            rc = clReleaseProgram(CLUT_CURRENT_PROGRAM);
            if (clut_internal_handle_error(rc)) exit(1);
        }
        // release the context
        rc = clReleaseContext(CLUT_CURRENT_CONTEXT);
        if (clut_internal_handle_error(rc)) exit(1);
    }
    exit(0);
}


/**
 * @brief Allocates host or device memory.
 * @param nbytes number of bytes to allocate.
 * @param flags CLUT memory flags 
 * @returns a pointer to the device or host memory
 */
FCAPI void* FCAPIENTRY clutMalloc(size_t nbytes,unsigned int flags)
{   
    cl_mem buffer;
    cl_int rc;
    void * ptr;
    cl_mem_flags mem_flags;

    // get read/write permission flags
    switch (flags & 0xC)
    {
        case CLUT_READ:
            mem_flags = CL_MEM_READ_ONLY;
            break;
        case CLUT_WRITE:
            mem_flags = CL_MEM_WRITE_ONLY;
            break;
        default:
        case CLUT_READ_WRITE:
            mem_flags = CL_MEM_READ_WRITE;
            break;
    }

#ifdef __ALTERA__
    // SDRAM bank select option if altera
    switch (flags & 0x30)
    {
        case CLUT_BANK_SELECT_0:
            mem_flags |= CL_MEM_BANK_1_ALTERA;
            break;
        case CLUT_BANK_SELECT_1:
            mem_flags |= CL_MEM_BANK_2_ALTERA;
            break;
    }
#endif

    // 
    switch (flags & 0x1)
    {
        case CLUT_MALLOC_HOST_PTR:
            mem_flags |= CL_MEM_ALLOC_HOST_PTR;
            buffer = clCreateBuffer(CLUT_CURRENT_CONTEXT,
                                    mem_flags,
                                    nbytes,
                                    NULL,
                                    &rc
                                   );
            if (clut_internal_handle_error(rc)) return NULL;

            ptr = clEnqueueMapBuffer(CLUT_CURRENT_QUEUE,
                                     buffer,
                                     CL_TRUE,
                                     CL_MAP_WRITE,
                                     0,
                                     nbytes,
                                     0,
                                     NULL,
                                     NULL,
                                     &rc
                                    );
            if (clut_internal_handle_error(rc)) return NULL;
            else return ptr;
            break;
        case CLUT_MALLOC_DEVICE_PTR:
            buffer = clCreateBuffer(CLUT_CURRENT_CONTEXT,
                                    mem_flags,
                                    nbytes,
                                    NULL,
                                    &rc
                                   );
            if (clut_internal_handle_error(rc)) return NULL;
            else return (void *) buffer;
            break;
    }
}

/**
 * @brief performs memory copy. 
 * @details This enqueues a read/write buffer command to the current command queue.
 * @param dst The destination address (host) or cl_mem (device) object
 * @param src The source address (host )or cl_mem (object) object
 * @param nbytes the number of bytes to copy.
 * @param flags the directional information of the copy
 * @todo Currently all memory operations are blocking, this will be extended later.
 */
FCAPI void FCAPIENTRY clutMemcpy(void* dst,void* src,size_t nbytes,unsigned int flags)
{
    cl_int rc;
    switch (flags & 0x3)
    {
        case CLUT_MEMCPY_HOST2DEVICE:
            rc = clEnqueueWriteBuffer( CLUT_CURRENT_QUEUE,
                                     (cl_mem) dst,
                                     CL_TRUE,
                                     0,
                                     nbytes,
                                     src,
                                     0,
                                     NULL,
                                     NULL
                                   );
            clut_internal_handle_error(rc);
            break;
        case CLUT_MEMCPY_HOST2HOST:
            /*just a standard mem copy*/
            memcpy(dst,src,nbytes);
            break;
        case CLUT_MEMCPY_DEVICE2HOST:
            rc = clEnqueueReadBuffer( CLUT_CURRENT_QUEUE,
                                    (cl_mem)src,
                                    CL_TRUE,
                                    0,
                                    nbytes,
                                    dst,
                                    0,
                                    NULL,
                                    NULL
                                  );
            clut_internal_handle_error(rc);
            break;
        case CLUT_MEMCPY_DEVICE2DEVICE:
            rc = clEnqueueCopyBuffer( CLUT_CURRENT_QUEUE,
                                    (cl_mem)src,
                                    (cl_mem)dst,
                                    0,
                                    0,
                                    nbytes,
                                    0,
                                    NULL,
                                    NULL
                                  );           
            clut_internal_handle_error(rc);
            break;
    }
    return;
}

/**
 * @brief sets memory buffer a constant value.
 * @param dst the destination pointer or cl_mem object
 * @param value the value to set each element to
 * @param nbytes number of bytes to set
 * @param flags CLUT flags
 * @todo The device sid is really awkward, there has to be a better way.
 * @todo Need to work out the best way to release mapped cl_mem object, I would have thought free() or clReleaseMemObject should clean up both... but need to confirm this.
 */
FCAPI void FCAPIENTRY clutMemset(void* dst,int value,size_t nbytes,unsigned int flags)
{
    cl_int rc;
    switch(flags & 0x1)
    {
        case CLUT_HOST_PTR:
            /*easy just standard memset*/
            memset(dst,value,nbytes);
            break;
        case CLUT_DEVICE_PTR:
        {
            /*quite annoyingly there is not memset-like function in OpenCL*/
            cl_mem buf_h;
            void *buf_map;
            /*create a host buffer, memset and copy... really sucks*/
            buf_h = clCreateBuffer( CLUT_CURRENT_CONTEXT,
                                    CL_MEM_READ_WRITE,
                                    nbytes,
                                    NULL,
                                    &rc
                                  );
            if (clut_internal_handle_error(rc)) return;
            buf_map = clEnqueueMapBuffer(CLUT_CURRENT_QUEUE,
                                         buf_h,
                                         CL_TRUE,
                                         CL_MAP_WRITE,
                                         0,
                                         nbytes,
                                         0,
                                         NULL,
                                         NULL,
                                         &rc
                                        );
            if (clut_internal_handle_error(rc))
            {
                clReleaseMemObject(buf_h);
                return;
            }
            memset(buf_map,value,nbytes);
            rc = clEnqueueWriteBuffer(CLUT_CURRENT_QUEUE,
                                      (cl_mem)dst,
                                      CL_TRUE,
                                      0,
                                      nbytes,
                                      buf_map,
                                      0,
                                      NULL,
                                      NULL
                                     );
            clut_internal_handle_error(rc);
            clReleaseMemObject(buf_h);
            break;
        }   
    }
    return;
}

/**
 * @brief Frees host or device memory.
 * @param ptr The address or cl_mem object to release
 * @param flags CLUT flags 
 * @todo Needs to be cleaner for the host side, mapped cl_mem object could cause issues.
 */
FCAPI void FCAPIENTRY clutFree(void* ptr, unsigned int flags)
{
    cl_int rc;
    switch (flags & 0x1)
    {
        case CLUT_HOST_PTR:
            /*standard free*/
            free(ptr);
            break;
        case CLUT_DEVICE_PTR:
            rc = clReleaseMemObject((cl_mem)ptr);
            clut_internal_handle_error(rc);
            break;
    }
    return;
}


/**
 * @brief executes a kernel on the current device.
 * @param name function name of kernel to execute
 * @param args array of addresses to kernel arguments
 * @param sizes array of kernel argument sizes (in bytes as in the results from sizeof())
 * @param numargs The number of arguments being assigned
 * @param dim The dimension of the global work grid
 * @param globalWorkSize the number of work groups in each dimension
 * @param localWorkSize the number of work items in each work group in each dimension
 * @param flags CLUT flags
 * @retVal 0 if execution was successful
 * @retVal 1 if an error occurred
 * @todo currently blocks until complete, also not ideal this is just to test the functionality
 */
FCAPI int FCAPIENTRY clutExec(const char* name,void ** args, size_t *sizes,int numargs, int dim, size_t *globalWorkSize,size_t * localWorkSize, unsigned int flags)
{
    cl_event kernel_event;
    cl_int rc;
    int i;
    char buffer[256];
    size_t count;
    if (name != NULL)
    {
        /*check if the kernel exists*/
        for (i=0;i<CLUT_NUM_KERNELS;i++)
        {
            CLUT_CURRENT_KERNEL_ID = i;
            rc = clGetKernelInfo(CLUT_CURRENT_KERNEL,CL_KERNEL_FUNCTION_NAME,256*sizeof(char),buffer,&count);
            if (clut_internal_handle_error(rc)) return 1;
            if (strcmp(buffer,name) == 0)
            {
                break;
            }
        }

        if (i >= CLUT_NUM_KERNELS)
        {
            /*create a new kernel*/
            if (CLUT_NUM_KERNELS < CLUT_MAX_NUM_KERNELS_PER_DEVICE)
            {
                cl_kernel new_kernel = clCreateKernel( CLUT_CURRENT_PROGRAM,
                                                       name,
                                                       &rc
                                                     ); 
                if (clut_internal_handle_error(rc)) return 1;
                /*update the current*/
                CLUT_CURRENT_KERNEL_ID = CLUT_NUM_KERNELS;
                CLUT_CURRENT_KERNEL = new_kernel;
                CLUT_NUM_KERNELS++;
                CLUT_CURRENT_KERNEL_ARG_COUNT = numargs;
                for (i=0;i<CLUT_CURRENT_KERNEL_ARG_COUNT;i++)
                {
                    CLUT_CURRENT_KERNEL_ARG(i) = args[i];
                    CLUT_CURRENT_KERNEL_ARG_SIZE(i) = sizes[i];
                }
            }
        }
    }
    else /*execute the current kernel*/
    {
        if (args != NULL)
        {
            /*set new args*/
            if (numargs == CLUT_CURRENT_KERNEL_ARG_COUNT)
            {
                for (i=0;i<CLUT_CURRENT_KERNEL_ARG_COUNT;i++)
                {
                    CLUT_CURRENT_KERNEL_ARG(i) = args[i];
                    CLUT_CURRENT_KERNEL_ARG_SIZE(i) = sizes[i];
                }
            }
            else
            {
                clut_internal_handle_error(CL_INVALID_KERNEL_ARGS);
                return 1;
            }
        }
    }
    
    /*update kernel inputs*/
    for (i=0;i<CLUT_CURRENT_KERNEL_ARG_COUNT;i++)
    {
        rc = clSetKernelArg( CLUT_CURRENT_KERNEL,
                             i,
                             CLUT_CURRENT_KERNEL_ARG_SIZE(i),
                             CLUT_CURRENT_KERNEL_ARG(i)
                           );
        if (clut_internal_handle_error(rc)) return 1;
    }
        
    /*enqueue the execution*/
    rc = clEnqueueNDRangeKernel( CLUT_CURRENT_QUEUE,
                                 CLUT_CURRENT_KERNEL,
                                 dim,
                                 NULL,
                                 globalWorkSize,
                                 localWorkSize,
                                 0,
                                 NULL,
                                 &kernel_event
                               );
    if (clut_internal_handle_error(rc)) return 1;
    CLUT_CURRENT_KERNEL_EVENT = kernel_event;
    return 0;
}

/**
 * @brief blocks host execution pipeline until the current kernel execution is complete.
 */
FCAPI void FCAPIENTRY clutWait(void)
{
    cl_int rc;
    rc = clWaitForEvents(1,&(CLUT_CURRENT_KERNEL_EVENT));
    clut_internal_handle_error(rc); 
}

/**
 * @brief blocks host until all commands in the current queue are executed and completed.
 */
FCAPI void FCAPIENTRY clutWaitAll(void)
{
    cl_int rc;
    rc = clFinish(CLUT_CURRENT_QUEUE);
    clut_internal_handle_error(rc);
}

/**
 * @brief Returns execution time of the last profiled event.
 * @returns The execution time in seconds.
 */
FCAPI double FCAPIENTRY clutGetLastExecTime(void)
{
    cl_ulong start_t, end_t;
    cl_int rc;
    rc = clGetEventProfilingInfo( CLUT_CURRENT_KERNEL_EVENT,
                                  CL_PROFILING_COMMAND_END,
                                  sizeof(cl_ulong),
                                  &end_t,
                                  NULL
                                );
    if (clut_internal_handle_error(rc)) return -1.0; 
    rc = clGetEventProfilingInfo( CLUT_CURRENT_KERNEL_EVENT,
                                  CL_PROFILING_COMMAND_START,
                                  sizeof(cl_ulong),
                                  &start_t,
                                  NULL
                                );
    if (clut_internal_handle_error(rc)) return -1.0; 
    return (double)1.0e-9 * (end_t - start_t);
}

/**
 * @brief Prints the last error to occur to stderr.
 */
FCAPI int FCAPIENTRY clutPrintLastError(void)
{
    fprintf(stderr,"Error [%d] occurred.\n",clut_internal_get_current_error());
    return 0;
}

/**
 * @brief Tests if CLUT is in an error state.
 * @retVal 0 if no error occurred.
 */
FCAPI int FCAPIENTRY clutErrorOccurred(void)
{
    return clut_internal_get_error_state();
}
