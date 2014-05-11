# Obstruct

Objective-C blocks are almost completely opaque, preventing you from calling them with an arbitrary array of arguments, or learning about their types, and so forth.

However, blocks have an ABI (Application Binary Interface), which is required to be stable (within certain bounds) for binary compatibility between releases of the compiler, the operating system, and application code. Obstruct takes advantage of this to introspect and interact with blocks:

    #import <Obstruct/Obstruct.h>
    …
    obstr_block_apply_array(^(id a, NSString *b, NSNumber *c) { … }, @[ self, @"string", @5 ]);

The blocks ABI incorporates a string encoding of the types used by the block. While Obstruct currently only scans this to count the argument types, it wouldn’t be too much effort to extract more specific types.

Obstruct is currently **incomplete**, lacking basic things like support for non-object types in the signature string, but the API is considered stable, and it functions correctly.
