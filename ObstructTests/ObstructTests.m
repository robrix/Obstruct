//  Copyright (c) 2014 Rob Rix. All rights reserved.

#import <XCTest/XCTest.h>
#import <Obstruct/Obstruct.h>
#import <Obstruct/apply.h>

@interface ObstructTests : XCTestCase
@end

@implementation ObstructTests

-(void)testBlockArity {
	XCTAssertEqual(obstr_block_get_arity((obstr_block_t)^(id _0, id<NSCopying> _1, NSString<NSCopying> *_2){ return @""; }), 3, @"");
}

-(void)testApply {
	XCTAssertEqualObjects((obstr_block_apply_array(^(id x){ return x; }, @[ self ])), self, @"");
	XCTAssertEqualObjects((obstr_block_apply_array(^(NSNumber *x, NSNumber *y){ return @(x.integerValue + y.integerValue); }, (@[ @12, @-5 ]))), @7, @"");
}

@end
