/*
 * Copyright 2017 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "FIRApp.h"
#import "FIRErrors.h"

/**
 * The internal interface to FIRApp. This is meant for first-party integrators, who need to receive
 * FIRApp notifications, log info about the success or failure of their configuration, and access
 * other internal functionality of FIRApp.
 *
 * TODO(b/28296561): Restructure this header.
 */
NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, FIRConfigType) {
  FIRConfigTypeCore = 1,
  FIRConfigTypeSDK = 2,
};

/**
 * Names of services provided by Firebase.
 */
extern NSString *const kFIRServiceAdMob;
extern NSString *const kFIRServiceAuth;
extern NSString *const kFIRServiceAuthUI;
extern NSString *const kFIRServiceCrash;
extern NSString *const kFIRServiceDatabase;
extern NSString *const kFIRServiceDynamicLinks;
extern NSString *const kFIRServiceInstanceID;
extern NSString *const kFIRServiceInvites;
extern NSString *const kFIRServiceMessaging;
extern NSString *const kFIRServiceMeasurement;
extern NSString *const kFIRServiceRemoteConfig;
extern NSString *const kFIRServiceStorage;

/**
 * Names of services provided by the Google pod, but logged by the Firebase pod.
 */
extern NSString *const kGGLServiceAnalytics;
extern NSString *const kGGLServiceSignIn;

extern NSString *const kFIRDefaultAppName;
extern NSString *const kFIRAppReadyToConfigureSDKNotification;
extern NSString *const kFIRAppDeleteNotification;
extern NSString *const kFIRAppIsDefaultAppKey;
extern NSString *const kFIRAppNameKey;
extern NSString *const kFIRGoogleAppIDKey;

/** @typedef FIRTokenCallback
    @brief The type of block which gets called when a token is ready.
 */
typedef void (^FIRTokenCallback)(NSString *_Nullable token, NSError *_Nullable error);

/** @typedef FIRAppGetTokenImplementation
    @brief The type of block which can provide an implementation for the @c getTokenWithCallback:
        method.
    @param forceRefresh Forces the token to be refreshed.
    @param callback The block which should be invoked when the async call completes.
 */
typedef void (^FIRAppGetTokenImplementation)(BOOL forceRefresh, FIRTokenCallback callback);

/** @typedef FIRAppGetUID
    @brief The type of block which can provide an implementation for the @c getUID method.
 */
typedef NSString *_Nullable (^FIRAppGetUIDImplementation)();

@interface FIRApp ()

/** @property getTokenImplementation
    @brief Gets or sets the block to use for the implementation of
        @c getTokenForcingRefresh:withCallback:
 */
@property(nonatomic, copy) FIRAppGetTokenImplementation getTokenImplementation;

/** @property getUIDImplementation
    @brief Gets or sets the block to use for the implementation of @c getUID.
 */
@property(nonatomic, copy) FIRAppGetUIDImplementation getUIDImplementation;

/**
 * Creates an error for failing to configure a subspec service. This method is called by each
 * FIRApp notification listener.
 */
+ (NSError *)errorForSubspecConfigurationFailureWithDomain:(NSString *)domain
                                                 errorCode:(FIRErrorCode)code
                                                   service:(NSString *)service
                                                    reason:(NSString *)reason;
/**
 * Checks if the default app is configured without trying to configure it.
 */
+ (BOOL)isDefaultAppConfigured;

/**
 * Used by each SDK to send logs about SDK configuration status to Clearcut.
 */
- (void)sendLogsWithServiceName:(NSString *)serviceName
                        version:(NSString *)version
                          error:(NSError *)error;

/**
 * Can be used by the unit tests in eack SDK to reset FIRApp. This method is thread unsafe.
 */
+ (void)resetApps;

/**
 * Can be used by the unit tests in each SDK to set customized options.
 */
- (instancetype)initInstanceWithName:(NSString *)name options:(FIROptions *)options;

/** @fn getTokenForcingRefresh:withCallback:
    @brief Retrieves the Firebase authentication token, possibly refreshing it.
    @param forceRefresh Forces a token refresh. Useful if the token becomes invalid for some reason
        other than an expiration.
    @param callback The block to invoke when the token is available.
 */
- (void)getTokenForcingRefresh:(BOOL)forceRefresh withCallback:(FIRTokenCallback)callback;

/**
 * Exposed for use by the Google pod. Configures the default app without sending notifications to
 * other SDKs. Otherwise, behaves exactly like +configure.
 */
+ (void)configureWithoutSendingNotification;

/**
 * Expose the UID of the current user for Firestore.
 */
- (nullable NSString *)getUID;

@end

NS_ASSUME_NONNULL_END
