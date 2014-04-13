//  Copyright (c) 2014 Rob Rix. All rights reserved.

#import <XCTest/XCTest.h>
#import <Obstruct/Obstruct.h>

@interface ObstructTests : XCTestCase
@end

@implementation ObstructTests

-(void)testBlockArity {
	XCTAssertEqual(obstr_block_get_arity((__bridge obstr_block_t)^ NSString * (id _0, id<NSCopying> _1, NSString<NSCopying> *_2){ return @""; }), 3, @"");
}

@end
