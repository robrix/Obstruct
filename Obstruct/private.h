//  Copyright (c) 2014 Rob Rix. All rights reserved.

#ifndef OBSTRUCT_PRIVATE
#define OBSTRUCT_PRIVATE

#include "Obstruct.h"

enum {
	OBSTR_BLOCK_HAS_COPY_DISPOSE =  (1 << 25),
	OBSTR_BLOCK_HAS_CTOR =          (1 << 26), // helpers have C++ code
	OBSTR_BLOCK_IS_GLOBAL =         (1 << 28),
	OBSTR_BLOCK_HAS_STRET =         (1 << 29), // IFF OBSTR_BLOCK_HAS_SIGNATURE
	OBSTR_BLOCK_HAS_SIGNATURE =     (1 << 30),
};
typedef int obstr_block_flags_t;

typedef struct obstr_block_s {
	void *isa;
	obstr_block_flags_t flags;
	int reserved;
	void (*invoke)(void *, ...);
	struct obstr_block_descriptor_s {
		unsigned long int reserved;
		unsigned long int size;
		union {
			struct {
				void (*copy)(void *destination, void *source);
				void (*dispose)(void *source);
				const char *signature_for_copy_dispose;
			};
			const char *signature;
		};
	} *descriptor;
} *obstr_block_t;

#endif // OBSTRUCT_PRIVATE
