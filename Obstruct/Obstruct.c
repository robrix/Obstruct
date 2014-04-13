//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "Obstruct.h"
#include "private.h"
#include "signature.h"
#include <assert.h>

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
