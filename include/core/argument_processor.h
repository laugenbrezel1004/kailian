#ifndef ARGUMENT_PROCESSOR_H
#define ARGUMENT_PROCESSOR_H

#include "error.h"

typedef struct ArgumentProcessor ArgumentProcessor;

ArgumentProcessor* argument_processor_create(void);
void argument_processor_destroy(ArgumentProcessor *processor);
ErrorCode argument_processor_handle(ArgumentProcessor *processor, 
                                 int argc, 
                                 char *argv[], 
                                 const char *file_buffer);

#endif 