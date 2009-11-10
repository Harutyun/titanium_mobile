/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */
#ifdef MODULE_TI_GEOLOCATION

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <MapKit/MapKit.h>

#import "TitaniumModule.h"

@interface GeolocationModule : NSObject<TitaniumModule,CLLocationManagerDelegate> {
	NSMutableDictionary * proxyDictionary;
	CLLocationManager * locationManager;
	int	watchEventsFired;
	NSDate *lastEvent;
	NSLock *proxyLock;
	NSString *pageToken;
}
- (void)transmitGeoEvent:(CLLocation*)location fromLocation:(CLLocation*)fromLocation;
- (void) setPageToken: (NSString *)token;
@end

/**
 * @tiapi(method=True,name=Geolocation.getCurrentPosition,since=0.4) query the Geolocation services to obtain a location once
 * @tiarg(for=Geolocation.getCurrentPosition,name=success,type=function) success function callback
 * @tiarg(for=Geolocation.getCurrentPosition,name=failure,type=function) failure function callback
 * @tiarg(for=Geolocation.getCurrentPosition,name=options,type=object) geolocation options
 * @tiresult(for=Geolocation.getCurrentPosition,type=object) returns a token object
 *
 * @tiapi(method=True,name=Geolocation.watchPosition,since=0.4) continously query the Geolocation services for location updates
 * @tiarg(for=Geolocation.watchPosition,name=success,type=function) success function callback
 * @tiarg(for=Geolocation.watchPosition,name=failure,type=function) failure function callback
 * @tiarg(for=Geolocation.watchPosition,name=options,type=object) geolocation options
 * @tiresult(for=Geolocation.watchPosition,type=object) returns a token object
 *
 * @tiapi(method=True,name=Geolocation.clearWatch,since=0.4) stops querying the Geolocation services
 * @tiarg(for=Geolocation.clearWatch,name=token,type=object) the token created by Geolocation.getCurrentPosition or Geolocation.watchPosition
 *
 * @tiapi(method=True,name=Geolocation.forwardGeocoder,since=0.8) forward geocode an address into a latitude, longitude coordinate
 * @tiarg(for=Geolocation.forwardGeocoder,name=address,type=string) address to forward geocode
 * @tiarg(for=Geolocation.forwardGeocoder,name=callback,type=function) function to callback
 *
 * @tiapi(method=True,name=Geolocation.reverseGeocoder,since=0.8) reverse geocode a latitude, longitude coordinate into a set of locations
 * @tiarg(for=Geolocation.reverseGeocoder,name=latitude,type=string) latitude
 * @tiarg(for=Geolocation.reverseGeocoder,name=longitude,type=string) longitude
 * @tiarg(for=Geolocation.reverseGeocoder,name=callback,type=function) function to callback
 */ 

#endif
