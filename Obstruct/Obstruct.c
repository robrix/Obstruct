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


typedef const char *(*obstr_scanner_f)(const char *, void *);
typedef struct obstr_character_range_s {
	char min, max;
} *obstr_character_range_t;


static const char *obstr_while(const char *signature, obstr_scanner_f function, void **parameter) {
	if (signature == NULL) return NULL;
	
	return
		obstr_while(function(signature, *parameter), function, parameter)
	?:	signature;
}

static const char *obstr_until(const char *signature, obstr_scanner_f function, void **parameter) {
	if (signature == NULL) return NULL;
	
	return
		obstr_until(function(signature, parameter) == NULL? signature + 1 : NULL, function, *parameter)
	?:	signature;
}


static const char *obstr_scan_character(const char *signature, intptr_t c) {
	if (signature == NULL) return NULL;
	
	return (*signature == c)?
		signature + 1
	:	NULL;
}

static const char *obstr_scan_character_in_range(const char *signature, obstr_character_range_t range) {
	assert(range != NULL);
	
	if (signature == NULL) return NULL;
	
	return ((range->min <= *signature) && (*signature <= range->max))?
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
	return obstr_scan_until_character(obstr_scan_character(obstr_scan_character(signature, '@'), '"'), '"');
}

static const char *obstr_scan_unknown_type(const char *signature) {
	return obstr_scan_character(signature, '?');
}

static const char *obstr_scan_type(const char *signature) {
	return
		obstr_scan_object_type(signature)
	?:	obstr_scan_unknown_type(signature);
}


static const struct obstr_character_range_s digits = { .min = '0', .max = '9' };

static const char *obstr_scan_offset(const char *signature) {
	return obstr_while(
		signature,
		(obstr_scanner_f)obstr_scan_character_in_range,
		(void **)&(obstr_character_range_t){ (obstr_character_range_t)&digits });
}


static const char *obstr_scan_return_type(const char *signature) {
	return obstr_scan_offset(obstr_scan_object_type(signature));
}

static const char *obstr_scan_receiver_type(const char *signature) {
	return obstr_scan_unknown_type(obstr_scan_object_type(signature));
}


static int32_t obstr_signature_get_arity(const char *signature) {
	int32_t arity = 0;
	
	signature = obstr_scan_return_type(signature);
	signature = obstr_scan_receiver_type(signature);
	
	while ((signature = obstr_scan_type(signature)) && (signature = obstr_scan_offset(signature) ?: signature)) {
		++arity;
	}
	
	return arity;
}

int32_t obstr_block_get_arity(obstr_block_t block) {
	const char *signature = obstr_block_get_signature(block);
	
	return obstr_signature_get_arity(signature);
}

__attribute__((constructor)) static void obstr_selftest(void) {
	const char *signature = "@\"NSString\"32@?0@8@\"<NSCopying>\"16@\"NSString<NSCopying>\"24";
	
	assert(obstr_scan_character(signature, '@') == signature + 1);
	assert(obstr_scan_object_type(signature) == signature + 11);
	
	const char *numerals = "1234567890a";
	assert(obstr_while(numerals, (obstr_scanner_f)obstr_scan_character_in_range, (void **)&(obstr_character_range_t){(obstr_character_range_t)&digits}) == numerals + 10);
	
	assert(obstr_signature_get_arity(signature) == 3);
}
