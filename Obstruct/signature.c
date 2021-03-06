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


#pragma mark Scanning

typedef const char *(*obstr_scanner_f)(const char *, intptr_t);
typedef const char *(*obstr_nullary_scanner_f)(const char *);
typedef struct obstr_character_range_s {
	char min, max;
} *obstr_character_range_t;


#pragma mark Repeating

static const char *obstr_scan_while(const char *signature, obstr_scanner_f function, intptr_t *parameter) {
	if (signature == NULL) return NULL;
	
	return
		obstr_scan_while(function(signature, *parameter), function, parameter)
	?:	signature;
}

static const char *obstr_scan_until(const char *signature, obstr_scanner_f function, intptr_t *parameter) {
	if (signature == NULL) return NULL;
	
	return
		obstr_scan_until(function(signature, *parameter) == NULL? signature + 1 : NULL, function, parameter)
	?:	signature;
}

static const char *obstr_scan_optional(const char *signature, obstr_nullary_scanner_f function) {
	if (signature == NULL) return NULL;
	
	return function(signature) ?: signature;
}


#pragma mark Characters

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


#pragma mark Types

static const char *obstr_scan_offset(const char *signature);

static const char *obstr_scan_implicit_block_parameter(const char *signature) {
	return obstr_scan_optional(obstr_scan_character(obstr_scan_character(signature, '@'), '?'), obstr_scan_offset);
}


static const char *obstr_scan_type(const char *signature);
static const char *obstr_scan_type_list(const char *signature);

static const char *obstr_scan_block_type(const char *signature) {
	if (signature == NULL) return NULL;
	
	signature = obstr_scan_implicit_block_parameter(signature);
	signature = obstr_scan_character(signature, '<');
	signature = obstr_scan_type(signature);
	signature = obstr_scan_implicit_block_parameter(signature);
	signature = obstr_scan_type_list(signature);
	signature = obstr_scan_character(signature, '>');
	return signature;
}

static const char *obstr_scan_object_type(const char *signature) {
	signature = obstr_scan_character(signature, '@');
	intptr_t quote = '"';
	return
		obstr_scan_character(obstr_scan_until(obstr_scan_character(signature, quote), (obstr_scanner_f)obstr_scan_character, &quote), quote)
	?:	signature;
}

static const char *obstr_scan_void_type(const char *signature) {
	return obstr_scan_character(signature, 'v');
}

static const char *obstr_scan_unknown_type(const char *signature) {
	return obstr_scan_character(signature, '?');
}


static const char *obstr_scan_type(const char *signature) {
	if (signature == NULL) return NULL;
	
	signature =
		obstr_scan_block_type(signature)
	?:	obstr_scan_object_type(signature)
	?:	obstr_scan_void_type(signature)
	?:	obstr_scan_unknown_type(signature);
	return obstr_scan_optional(signature, obstr_scan_offset);
}

static const char *obstr_scan_type_list(const char *signature) {
	if (signature == NULL) return NULL;
	
	const char *next = NULL;
	while ((next = obstr_scan_type(signature))) {
		signature = next;
	}
	return signature;
}


static const struct obstr_character_range_s digits = { .min = '0', .max = '9' };

static const char *obstr_scan_offset(const char *signature) {
	return obstr_scan_while(
		signature,
		(obstr_scanner_f)obstr_scan_character_in_range,
		(intptr_t *)&(obstr_character_range_t){ (obstr_character_range_t)&digits });
}


#pragma mark Signature scanning

static const char *obstr_callout(const char *signature, obstr_scanner_callback_f callback, intptr_t *context) {
	if (signature != NULL && callback != NULL) callback(signature, *context);
	
	return signature;
}

const char *obstr_scan_signature(const char *signature, obstr_scanner_callback_f callback, intptr_t *context) {
	signature = obstr_callout(obstr_scan_type(signature), callback, context);
	signature = obstr_callout(obstr_scan_implicit_block_parameter(signature), callback, context);
	
	while ((signature = obstr_callout(obstr_scan_type(signature), callback, context))) {}
	
	return signature;
}


#pragma mark Arity

static void obstr_signature_arity_scanner_callback(const char *signature, int32_t *arity) {
	++(*arity);
}

static int32_t obstr_signature_get_arity(const char *signature) {
	int32_t arity = -2;
	
	obstr_scan_signature(signature, (obstr_scanner_callback_f)obstr_signature_arity_scanner_callback, (intptr_t *)&(int32_t *){ &arity });
	
	return arity;
}

int32_t obstr_block_get_arity(obstr_block_t block) {
	const char *signature = obstr_block_get_signature(block);
	
	return obstr_signature_get_arity(signature);
}
