//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_APPLY
#define OBSTRUCT_APPLY

#include <Obstruct/Obstruct.h>
#include <assert.h>

#define obstr_block_apply(block, count, array) \
	_obstr_block_apply(block, count, __typeof__(*array), (array))

#define _obstr_block_apply(block, count, type, a) (^{ \
		int32_t arity = obstr_block_get_arity(block); \
		assert(count >= arity && "count must be greater than or equal to arity of block"); \
		\
		typedef type t; \
		t value = {0}; \
		switch (arity) { \
			case 0: value = ((t(^)())block)(); break; \
			case 1: value = ((t(^)(t))block)(a[0]); break; \
			case 2: value = ((t(^)(t,t))block)(a[0], a[1]); break; \
			case 3: value = ((t(^)(t,t,t))block)(a[0], a[1], a[2]); break; \
			case 4: value = ((t(^)(t,t,t,t))block)(a[0], a[1], a[2], a[3]); break; \
			case 5: value = ((t(^)(t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4]); break; \
			case 6: value = ((t(^)(t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5]); break; \
			case 7: value = ((t(^)(t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]); break; \
			case 8: value = ((t(^)(t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); break; \
			case 9: value = ((t(^)(t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]); break; \
			case 10: value = ((t(^)(t,t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]); break; \
			case 11: value = ((t(^)(t,t,t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10]); break; \
			\
			default: assert(0 == "unsupported block arity"); \
		} \
		return value; \
	}())

#ifdef __OBJC__

#define obstr_block_apply_array(block, array) \
	_obstr_block_apply((obstr_block_t)block, (array).count, id, (array))

#endif

#endif
