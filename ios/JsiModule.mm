#import "JsiModule.h"
#import "react-native-jsi-module.h"
#import <jsi/jsi.h>
#import <React/RCTBridge+Private.h>
@implementation JsiModule
RCT_EXPORT_MODULE()



- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
    return std::make_shared<facebook::react::NativeJsiModuleSpecJSI>(params);
}

RCT_EXPORT_SYNCHRONOUS_TYPED_METHOD(NSNumber *, install) {
    RCTBridge *bridge = [RCTBridge currentBridge];
        RCTCxxBridge* cxxBridge = (RCTCxxBridge *)bridge;
        
        if(cxxBridge == nil) return @NO;
        
        jsi::Runtime *jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
        
        if(jsiRuntime == nil) return @NO;
        
        jsimodule::install(*jsiRuntime);
    return @YES;
}

@end
