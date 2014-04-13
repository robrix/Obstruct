//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "Obstruct.h"
#include "private.h"
#include "signature.h"
#include <assert.h>

typedef const char *(*obstr_scanner_f)(const char *, intptr_t);
typedef struct obstr_character_range_s {
	char min, max;
} *obstr_character_range_t;


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

static const char *obstr_scan_object_type(const char *signature) {
	signature = obstr_scan_character(signature, '@');
	intptr_t quote = '"';
	return obstr_scan_character(obstr_scan_until(obstr_scan_character(signature, quote), (obstr_scanner_f)obstr_scan_character, &quote), quote) ?: signature;
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
	return obstr_scan_while(
		signature,
		(obstr_scanner_f)obstr_scan_character_in_range,
		(intptr_t *)&(obstr_character_range_t){ (obstr_character_range_t)&digits });
}


static const char *obstr_scan_return_type(const char *signature) {
	signature = obstr_scan_object_type(signature);
	signature = obstr_scan_offset(signature);
	return signature;
}

static const char *obstr_scan_receiver_type(const char *signature) {
	signature = obstr_scan_object_type(signature);
	signature = obstr_scan_unknown_type(signature);
	signature = obstr_scan_offset(signature);
	return signature;
}

static const char *obstr_scan_argument_type(const char *signature) {
	signature = obstr_scan_type(signature);
	signature = obstr_scan_offset(signature);
	return signature;
}


static int32_t obstr_signature_get_arity(const char *signature) {
	int32_t arity = 0;
	
	signature = obstr_scan_return_type(signature);
	signature = obstr_scan_receiver_type(signature);
	
	while ((signature = obstr_scan_argument_type(signature))) {
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
	assert(obstr_scan_while(numerals, (obstr_scanner_f)obstr_scan_character_in_range, (intptr_t *)&(obstr_character_range_t){(obstr_character_range_t)&digits}) == numerals + 10);
	
	assert(obstr_signature_get_arity(signature) == 3);
}
