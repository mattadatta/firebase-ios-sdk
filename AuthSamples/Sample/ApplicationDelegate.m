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

#import "ApplicationDelegate.h"

#import "AuthProviders.h"
#import "FirebaseCommunity/FIRApp.h"
#import "FirebaseAuth.h"
#import "FirebaseCommunity/FIRLogger.h"
#import "GTMSessionFetcherLogging.h"
#import "MainViewController.h"

/** @var gOpenURLDelegate
    @brief The delegate to for application:openURL:... method.
 */
static __weak id<OpenURLDelegate> gOpenURLDelegate;

@implementation ApplicationDelegate {
  // The main view controller of the sample app.
  MainViewController *_sampleAppMainViewController;
}

+ (void)setOpenURLDelegate:(nullable id<OpenURLDelegate>)openURLDelegate {
  gOpenURLDelegate = openURLDelegate;
}

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  [GTMSessionFetcher  setLoggingEnabled:YES];
  FIRSetLoggerLevel(FIRLoggerLevelInfo);

  // Configure the default Firebase application:
  [FIRApp configure];

  // Load and present the UI:
  UIWindow *window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  _sampleAppMainViewController =
      [[MainViewController alloc] initWithNibName:NSStringFromClass([MainViewController class])
                                           bundle:nil];
  window.rootViewController = _sampleAppMainViewController;
  self.window = window;
  [self.window makeKeyAndVisible];

  return YES;
}

- (BOOL)application:(nonnull UIApplication *)application
            openURL:(nonnull NSURL *)url
            options:(nonnull NSDictionary<NSString *, id> *)options {
  return [self application:application
                   openURL:url
         sourceApplication:options[UIApplicationOpenURLOptionsSourceApplicationKey]
                annotation:options[UIApplicationOpenURLOptionsAnnotationKey]];
}

- (BOOL)application:(UIApplication *)application
              openURL:(NSURL *)url
    sourceApplication:(NSString *)sourceApplication
           annotation:(id)annotation {
  if ([gOpenURLDelegate handleOpenURL:url sourceApplication:sourceApplication]) {
    return YES;
  }
  if ([_sampleAppMainViewController handleIncomingLinkWithURL:url]) {
    return YES;
  }
  return NO;
}

- (BOOL)application:(UIApplication *)application
   continueUserActivity:(NSUserActivity *)userActivity
     restorationHandler:(void (^)(NSArray *))restorationHandler {
  if (userActivity.webpageURL) {
    return [_sampleAppMainViewController handleIncomingLinkWithURL:userActivity.webpageURL];
  }
  return NO;
}

@end
