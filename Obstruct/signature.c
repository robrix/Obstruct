//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "signature.h"
#include "private.h"
#include <assert.h>

const char *obstr_block_get_signature(obstr_block_t block) {
	assert(block != NULL);
	assert(block->flags & OBSTR_BLOCK_HAS_SIGNATURE);
	assert(block->descriptor != NULL);
	
	return (block->flags & OBSTR_BLOCK_HAS_COPY_DISPOSE)?
	block->descriptor->signature_for_copy_dispose
	:	block->descriptor->signature;
}
