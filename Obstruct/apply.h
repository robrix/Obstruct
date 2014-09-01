//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_APPLY
#define OBSTRUCT_APPLY

#include <Obstruct/Obstruct.h>
#include <assert.h>

#define obstr_block_apply(block, count, array) \
	({ \
		__typeof__(block()) _value = {0}; \
		_obstr_block_apply_map(block, count, __typeof__(*array), __typeof__(*array), (array), _obstr_assign_value, _obstr_subscript_map); \
		_value; \
	})

#define _obstr_subscript_map(a, i) \
	a[i]

#define _obstr_assign_value(x) \
	_value = x
#define _obstr_ignore_value(x) \
	(void)x

#define _obstr_block_apply_map(block, count, t, u, a, v, m) \
	do { \
		int32_t arity = obstr_block_get_arity(block); \
		assert(count >= arity && "count must be greater than or equal to arity of block"); \
		\
		switch (arity) { \
			case 0: v(((u(^)())block)()); break; \
			case 1: v(((u(^)(t))block)(m(a, 0))); break; \
			case 2: v(((u(^)(t,t))block)(m(a, 0), m(a, 1))); break; \
			case 3: v(((u(^)(t,t,t))block)(m(a, 0), m(a, 1), m(a, 2))); break; \
			case 4: v(((u(^)(t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3))); break; \
			case 5: v(((u(^)(t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4))); break; \
			case 6: v(((u(^)(t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5))); break; \
			case 7: v(((u(^)(t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6))); break; \
			case 8: v(((u(^)(t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7))); break; \
			case 9: v(((u(^)(t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8))); break; \
			case 10: v(((u(^)(t,t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8), m(a, 9))); break; \
			case 11: v(((u(^)(t,t,t,t,t,t,t,t,t,t,t))block)(m(a, 0), m(a, 1), m(a, 2), m(a, 3), m(a, 4), m(a, 5), m(a, 6), m(a, 7), m(a, 8), m(a, 9), m(a, 10))); break; \
			\
			default: assert(0 == "unsupported block arity"); \
		} \
	} while (0)

#ifdef __OBJC__

#define obstr_block_apply_array(block, array) \
	_obstr_block_apply_map((obstr_block_t)block, (array).count, id, __typeof__(block()), (array), _obstr_subscript_map)

#define obstr_block_apply_array_map(block, array, map) \
	({ \
		__typeof__(block()) _value = {0}; \
		_obstr_block_apply_map((obstr_block_t)block, (array).count, id, __typeof__(block()), (array), _obstr_assign_value, map); \
		_value; \
	})

#endif

#endif
