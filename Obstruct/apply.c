//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "apply.h"
#include <assert.h>

intptr_t obstruct_apply(obstr_block_t block, uintptr_t count, intptr_t a[count]) {
	int32_t arity = obstr_block_get_arity(block);
	assert(arity == count && "count must be equal to arity of block");
	
	typedef intptr_t t;
	switch (arity) {
		case 0: return ((t(^)(void))block)();
		case 1: return ((t(^)(t))block)(a[0]);
		case 2: return ((t(^)(t,t))block)(a[0], a[1]);
		case 3: return ((t(^)(t,t,t))block)(a[0], a[1], a[2]);
		case 4: return ((t(^)(t,t,t,t))block)(a[0], a[1], a[2], a[3]);
		case 5: return ((t(^)(t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4]);
		case 6: return ((t(^)(t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5]);
		case 7: return ((t(^)(t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
		case 8: return ((t(^)(t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
		case 9: return ((t(^)(t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]);
		case 10: return ((t(^)(t,t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
		
		default:
			assert(0 == "block arity must be in interval 0..10");
			break;
	}
	
	return 0;
}
