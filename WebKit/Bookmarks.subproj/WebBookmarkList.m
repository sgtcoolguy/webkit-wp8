//
//  WebBookmarkList.m
//  WebKit
//
//  Created by John Sullivan on Tue Apr 30 2002.
//  Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
//

#import <WebKit/WebBookmarkList.h>
#import <WebKit/WebBookmarkLeaf.h>
#import <WebKit/WebBookmarkSeparator.h>
#import <WebKit/WebBookmarkPrivate.h>
#import <WebKit/WebBookmarkGroupPrivate.h>
#import <WebKit/WebKitDebug.h>

#define TitleKey		@"Title"
#define ChildrenKey		@"Children"

@implementation WebBookmarkList

- (id)initWithTitle:(NSString *)title
              group:(WebBookmarkGroup *)group
{
    [super init];

    _title = [title copy];
    _list = [[NSMutableArray alloc] init];
    [self _setGroup:group];
    
    return self;
}

- (id)initFromDictionaryRepresentation:(NSDictionary *)dict withGroup:(WebBookmarkGroup *)group
{
    NSArray *storedChildren;
    WebBookmark *child;
    unsigned index, count;
    
    WEBKIT_ASSERT_VALID_ARG (dict, dict != nil);

    [super init];

    [self _setGroup:group];

    // FIXME: doesn't restore images
    _title = [[dict objectForKey:TitleKey] retain];
    _list = [[NSMutableArray alloc] init];

    storedChildren = [dict objectForKey:ChildrenKey];
    if (storedChildren != nil) {
        count = [storedChildren count];
        for (index = 0; index < count; ++index) {
            child = [WebBookmark bookmarkFromDictionaryRepresentation:[storedChildren objectAtIndex:index]
                                                           withGroup:group];	

            if (child != nil) {
                [self insertChild:child atIndex:index];
            }
        }
    }

    return self;
}

- (NSDictionary *)dictionaryRepresentation
{
    NSMutableDictionary *dict;
    NSMutableArray *childrenAsDictionaries;
    unsigned index, childCount;

    dict = [NSMutableDictionary dictionaryWithCapacity: 3];

    // FIXME: doesn't save images
    if (_title != nil) {
        [dict setObject:_title forKey:TitleKey];
    }

    [dict setObject:WebBookmarkTypeListValue forKey:WebBookmarkTypeKey];

    childCount = [self numberOfChildren];
    if (childCount > 0) {
        childrenAsDictionaries = [NSMutableArray arrayWithCapacity:childCount];

        for (index = 0; index < childCount; ++index) {
            WebBookmark *child;

            child = [_list objectAtIndex:index];
            [childrenAsDictionaries addObject:[child dictionaryRepresentation]];
        }

        [dict setObject:childrenAsDictionaries forKey:ChildrenKey];
    }
    
    return dict;
}

- (void)dealloc
{
    [_title release];
    [_list release];
    [super dealloc];
}

- (id)copyWithZone:(NSZone *)zone
{
    WebBookmarkList *copy;
    unsigned index, count;
    
    copy = [[WebBookmarkList alloc] initWithTitle:[self title]
                                            group:[self group]];

    count = [self numberOfChildren];
    for (index = 0; index < count; ++index) {
        WebBookmark *childCopy = [[_list objectAtIndex:index] copyWithZone:zone];
        [copy insertChild:childCopy atIndex:index];
        [childCopy release];
    }

    return copy;
}

- (NSString *)title
{
    return _title;
}

- (void)setTitle:(NSString *)title
{
    if ([title isEqualToString:_title]) {
        return;
    }

    [_title release];
    _title = [title copy];

    [[self group] _bookmarkDidChange:self]; 
}

- (NSImage *)icon
{
    return nil;
}

- (WebBookmarkType)bookmarkType
{
    return WebBookmarkTypeList;
}

- (NSArray *)children
{
    return [NSArray arrayWithArray:_list];
}

- (unsigned)numberOfChildren
{
    return [_list count];
}

- (unsigned)_numberOfDescendants
{
    unsigned result;
    unsigned index, count;
    WebBookmark *child;

    count = [self numberOfChildren];
    result = count;

    for (index = 0; index < count; ++index) {
        child = [_list objectAtIndex:index];
        result += [child _numberOfDescendants];
    }

    return result;
}

- (void)removeChild:(WebBookmark *)bookmark
{
    WEBKIT_ASSERT_VALID_ARG (bookmark, [bookmark parent] == self);
    WEBKIT_ASSERT_VALID_ARG (bookmark, [_list containsObject:bookmark]);
    
    [_list removeObject:bookmark];
    [bookmark _setParent:nil];

    [[self group] _bookmarkChildrenDidChange:self]; 
}


- (void)insertChild:(WebBookmark *)bookmark atIndex:(unsigned)index
{
    WEBKIT_ASSERT_VALID_ARG (bookmark, [bookmark parent] == nil);
    WEBKIT_ASSERT_VALID_ARG (bookmark, ![_list containsObject:bookmark]);

    [_list insertObject:bookmark atIndex:index];
    [bookmark _setParent:self];
    [bookmark _setGroup:[self group]];
    
    [[self group] _bookmarkChildrenDidChange:self];
}

- (void)_setGroup:(WebBookmarkGroup *)group
{
    if (group == [self group]) {
        return;
    }

    [super _setGroup:group];
    [_list makeObjectsPerformSelector:@selector(_setGroup:) withObject:group];
}

@end
