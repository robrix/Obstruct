//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_DEFINES
#define OBSTRUCT_DEFINES

#include <stdint.h>

#ifdef __OBJC__

typedef id (^obstr_block_t)();

#else

typedef struct obstr_block_s *obstr_block_t;

#endif

#endif
