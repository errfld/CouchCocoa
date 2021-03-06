//
//  CouchTouchDBServer.h
//  CouchCocoa
//
//  Created by Jens Alfke on 12/20/11.
//  Copyright (c) 2011 Couchbase, Inc. All rights reserved.
//

#import "CouchServer.h"
@class TD_Server, TD_Database;
struct TD_DatabaseManagerOptions;


/** A convenience class that glues TouchDB into CouchCocoa.
    On creation, starts up an instance of TD_Server and sets up TDURLProtocol to serve it.
    The CouchServer URL is set to the root URL served by the protocol, so you can treat it just like a normal remote server instance. */
@interface CouchTouchDBServer : CouchServer
{
    @private
    TD_Server* _touchServer;
    NSError* _error;
    BOOL _observing;
}

/** A shared per-process instance. Remember that CouchCocoa is not thread-safe so you can't
 use this shared instance among multiple threads. */
+ (CouchTouchDBServer*) sharedInstance;

/** Preferred initializer. Starts up an in-process server. */
- (id)init;

/** Starts up a server that stores its data at the given path.
    @param serverPath  The filesystem path to the server directory. If it doesn't already exist it will be created. */
- (id) initWithServerPath: (NSString*)serverPath;

/** Starts up a server that stores its data at the given path.
    @param serverPath  The filesystem path to the server directory. If it doesn't already exist it will be created.
    @param options  Option settings; can be used to open in read-only mode or to disable the replicator. */
- (id) initWithServerPath: (NSString*)serverPath
                  options: (const struct TD_DatabaseManagerOptions*)options;

/** Inherited initializer, if you want to connect to a remote server for debugging purposes. */
- (id) initWithURL: (NSURL*)url;

/** Shuts down the TouchDB server. */
- (void) close;

/** If this is non-nil, the server failed to initialize. */
@property (readonly) NSError* error;

/** Invokes the given block on the TouchDB server thread, passing it a pointer to the TD_Server.
    You can use this to (carefully!) access the TD_Server API.
    Be aware that the block may not run immediately; it's queued and will be called immediately before the server handles the next REST call. */
- (void) tellTDServer: (void (^)(TD_Server*))block;

/** Invokes the given block on the TouchDB server thread, passing it a pointer to a TD_Database.
    You can use this to (carefully!) access the TD_Database API.
    Be aware that the block may not run immediately; it's queued and will be called immediately before the server handles the next REST call. */
- (void) tellTDDatabaseNamed: (NSString*)dbName to: (void (^)(TD_Database*))block;

@end
