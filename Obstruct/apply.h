//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_APPLY
#define OBSTRUCT_APPLY

#include <Obstruct/Obstruct.h>
#include <assert.h>

#define obstr_block_apply(block, count, array) \
	_obstr_block_apply(block, count, __typeof__(*array), array)

#define _obstr_block_apply(block, count, type, a) (^{ \
		int32_t arity = obstr_block_get_arity(block); \
		assert(count >= arity && "count must be greater than or equal to arity of block"); \
		\
		typedef type t; \
		switch (arity) { \
			case 0: return ((t(^)())block)(); \
			case 1: return ((t(^)(t))block)(a[0]); \
			case 2: return ((t(^)(t,t))block)(a[0], a[1]); \
			case 3: return ((t(^)(t,t,t))block)(a[0], a[1], a[2]); \
			case 4: return ((t(^)(t,t,t,t))block)(a[0], a[1], a[2], a[3]); \
			case 5: return ((t(^)(t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4]); \
			case 6: return ((t(^)(t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5]); \
			case 7: return ((t(^)(t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]); \
			case 8: return ((t(^)(t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); \
			case 9: return ((t(^)(t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]); \
			case 10: return ((t(^)(t,t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]); \
			case 11: return ((t(^)(t,t,t,t,t,t,t,t,t,t,t))block)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10]); \
			\
			default: assert(0 == "unsupported block arity"); \
		} \
	}())

#ifdef __OBJC__

#define obstr_block_apply_array(block, array) \
	_obstr_block_apply((obstr_block_t)block, array.count, id, array)

#endif

#endif
