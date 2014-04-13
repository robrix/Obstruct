//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_SIGNATURE
#define OBSTRUCT_SIGNATURE

#include <Obstruct/Obstruct.h>

const char *obstr_block_get_signature(obstr_block_t block);

typedef void (*obstr_scanner_callback_f)(const char *signature, intptr_t context);
const char *obstr_scan_signature(const char *signature, obstr_scanner_callback_f callback, intptr_t *context);

#endif
