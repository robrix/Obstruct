//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_APPLY
#define OBSTRUCT_APPLY

#include <Obstruct/Obstruct.h>

intptr_t obstruct_apply(obstr_block_t block, uintptr_t count, intptr_t arguments[count]);

#ifdef __OBJC__

static id obstruct_apply_array(id(^block)(), NSArray *array) {
	__unsafe_unretained id objects[array.count];
	[array getObjects:objects range:(NSRange){ .length = array.count }];
	return (__bridge id)(void *)obstruct_apply((__bridge obstr_block_t)block, array.count, (intptr_t *)(void *)objects);
}

#endif

#endif
