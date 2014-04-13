//  Copyright (c) 2014 Rob Rix. All rights reserved.

#include "Obstruct.h"

enum {
	OBSTR_BLOCK_HAS_COPY_DISPOSE =  (1 << 25),
	OBSTR_BLOCK_HAS_CTOR =          (1 << 26), // helpers have C++ code
	OBSTR_BLOCK_IS_GLOBAL =         (1 << 28),
	OBSTR_BLOCK_HAS_STRET =         (1 << 29), // IFF OBSTR_BLOCK_HAS_SIGNATURE
	OBSTR_BLOCK_HAS_SIGNATURE =     (1 << 30),
};
typedef int obstr_block_flags_t;
