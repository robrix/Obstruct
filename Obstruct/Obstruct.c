//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "Obstruct.h"
#include <assert.h>

enum {
	OBSTR_BLOCK_HAS_COPY_DISPOSE =  (1 << 25),
	OBSTR_BLOCK_HAS_CTOR =          (1 << 26), // helpers have C++ code
	OBSTR_BLOCK_IS_GLOBAL =         (1 << 28),
	OBSTR_BLOCK_HAS_STRET =         (1 << 29), // IFF OBSTR_BLOCK_HAS_SIGNATURE
	OBSTR_BLOCK_HAS_SIGNATURE =     (1 << 30),
};
typedef int obstr_block_flags_t;

typedef struct obstr_block_s {
	void *isa;
	obstr_block_flags_t flags;
	int reserved;
	void (*invoke)(void *, ...);
	struct obstr_block_descriptor_s {
		unsigned long int reserved;
		unsigned long int size;
		union {
			struct {
				void (*copy)(void *destination, void *source);
				void (*dispose)(void *source);
				const char *signature_for_copy_dispose;
			};
			const char *signature;
		};
	} *descriptor;
} *obstr_block_t;


const char *obstr_block_get_signature(obstr_block_t block) {
	assert(block != NULL);
	assert(block->flags & OBSTR_BLOCK_HAS_SIGNATURE);
	assert(block->descriptor != NULL);
	
	return (block->flags & OBSTR_BLOCK_HAS_COPY_DISPOSE)?
		block->descriptor->signature_for_copy_dispose
	:	block->descriptor->signature;
}


static const char *obstr_scan_character(const char *signature, char c) {
	if (signature == NULL) return NULL;
	
	return (*signature == c)?
		signature + 1
	:	NULL;
}

static const char *obstr_scan_character_in_range(const char *signature, char min, char max) {
	if (signature == NULL) return NULL;
	
	return ((min <= *signature) && (*signature <= max))?
		signature + 1
	:	NULL;
}

static const char *obstr_scan_until_character(const char *signature, char c) {
	if (signature == NULL) return NULL;
	
	return (!obstr_scan_character(signature, c))?
		obstr_scan_until_character(signature + 1, c)
	:	signature + 1;
}

static const char *obstr_scan_object_type(const char *signature) {
	if (signature == NULL) return NULL;
	
	return obstr_scan_until_character(obstr_scan_character(obstr_scan_character(signature, '@'), '"'), '"');
}

static int32_t obstr_signature_get_arity(const char *signature) {
	int32_t arity = 0;
	
	obstr_scan_character_in_range(obstr_scan_object_type(signature), '0', '9');
	
	// skip return type
	// skip ? for block type
	// count types
	
	return arity;
}

int32_t obstr_block_get_arity(obstr_block_t block) {
	const char *signature = obstr_block_get_signature(block);
	
	return obstr_signature_get_arity(signature);
}
