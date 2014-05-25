//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_APPLY
#define OBSTRUCT_APPLY

#include <Obstruct/Obstruct.h>
#include <assert.h>

#define obstr_block_apply(block, count, array) \
	_obstr_block_apply_map(block, count, __typeof__(*array), (array), _obstr_subscript_map)

#define _obstr_subscript_map(a, i) \
	a[i]

#define _obstr_block_apply_map(block, count, t, a, m) \
	({ \
		int32_t arity = obstr_block_get_arity(block); \
		assert(count >= arity && "count must be greater than or equal to arity of block"); \
		\
		t value = {0}; \
		switch (arity) { \
			case 0: value = ((t(^)())block)(); break; \
			case 1: value = ((t(^)(t))block)(m(a, 0)); break; \
			case 2: value = ((t(^)(t,t))block)(m(a, 0), m(a, 1)); break; \
			case 3: value = ((t(^)(t,t,t))block)(m(a, 0), m(a, 1), m(a, 2)); break; \
			case 4: value = ((t(^)(t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3)); break; \
			case 5: value = ((t(^)(t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4)); break; \
			case 6: value = ((t(^)(t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5)); break; \
			case 7: value = ((t(^)(t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6)); break; \
			case 8: value = ((t(^)(t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7)); break; \
			case 9: value = ((t(^)(t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8)); break; \
			case 10: value = ((t(^)(t,t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8), m(a, 9)); break; \
			case 11: value = ((t(^)(t,t,t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8), m(a, 9), m(a, 10)); break; \
			\
			default: assert(0 == "unsupported block arity"); \
		} \
		value; \
	})

#ifdef __OBJC__

#define obstr_block_apply_array(block, array) \
	_obstr_block_apply_map((obstr_block_t)block, (array).count, id, (array), _obstr_subscript_map)

#define obstr_block_apply_array_map(block, array, map) \
	_obstr_block_apply_map((obstr_block_t)block, (array).count, id, (array), map)

#endif

#endif
