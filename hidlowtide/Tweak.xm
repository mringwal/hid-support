/**
 * Text injection and remote simulation
 * mach server: ch.ringwald.hidsupport
 */
 
#include <objc/runtime.h>
#include "../hid-support-internal.h"

#define KEYCODE_RETURN     '\n'
#define KEYCODE_ESCAPE      27

@interface BRWindow : NSObject
+ (BOOL)dispatchEvent:(id)event;    // 0x315d47b5
@end

@interface BREvent : NSObject
+ (id)eventWithAction:(int)action value:(int)value atTime:(double)time originator:(unsigned)originator eventDictionary:(id)dictionary allowRetrigger:(BOOL)retrigger;   // 0x315d54a5
@end
@interface BRApplicationStackManager:NSObject

+ (id)singleton;
- (id)stack;

@end;
@interface BRControllerStack:NSObject
- (id)peekController;
@end
@interface BRMenuController:NSObject
-(id)controls;
@end
@interface MoviesController:NSObject
@end
@interface BRViewController:NSObject
@end
@interface BRTextEntryControl:NSObject
-(void)deviceKeyboardClose;
- (void)_invokeInputActionWithDictionary:(id)dictionary;
@end
@interface BRMainMenuController:NSObject
@end
@interface BRTextEntryController:NSObject
@property(readonly, retain) BRTextEntryControl *editor;
@end
@interface BRSettingsFacade:NSObject
+ (id)singleton;
- (id)versionOS;    // 0x31648ce1
- (id)versionOSBuild;   // 0x31648ce5
- (id)versionSoftware;  // 0x3158f455
- (id)versionSoftwareBuild;
@end
static Class $BRSettingsFacade          = objc_getClass("BRSettingsFacade");
static Class $BRMainMenuController      = objc_getClass("BRMainMenuController");
static Class $BREvent                   = objc_getClass("BREvent");
static Class $BRWindow                  = objc_getClass("BRWindow");
static Class $BRApplicationStackManager = objc_getClass("BRApplicationStackManager");
static Class $BRControllerStack         = objc_getClass("BRControllerStack");
static Class $BRMenuController          = objc_getClass("BRMenuController");
static Class $MoviesController          = objc_getClass("MoviesController");
static Class $BRTextEntryController     = objc_getClass("BRTextEntryController");
static Class $BRViewController          = objc_getClass("BRViewController");
static Class $BRMediaMenuController     = objc_getClass("BRMediaMenuController");
static Class $BRTextEntryControl        = objc_getClass("BRTextEntryControl");
static void injectRemoteAction(int action, int down){
    // NSLog(@"Injecting action: %d down: %u", action, down);
    BREvent * event = [$BREvent eventWithAction:action value:down atTime:7400.0 originator:5 eventDictionary:nil allowRetrigger:1];
    [$BRWindow dispatchEvent:event];
}

static BRRemoteAction_t getRemoteActionForKey(uint16_t key){
    switch (key){
        case KEYCODE_ESCAPE:
            return BRRemoteActionMenu;
        case KEYCODE_RETURN:
            return BRRemoteActionSelect;
        case NSRightArrowFunctionKey:
            return BRRemoteActionRight;
        case NSLeftArrowFunctionKey:
            return BRRemoteActionLeft;
        case NSDownArrowFunctionKey:
            return BRRemoteActionDown;
        case NSUpArrowFunctionKey:
            return BRRemoteActionUp;
        default:
            return BRRemoteActionInvalid;
    }
}
static CFDataRef myCallBack(CFMessagePortRef local, SInt32 msgid, CFDataRef cfData, void *info) {
    //NSLog(@"hidsupport callback, msg %u", msgid);
    const char *data = (const char *) CFDataGetBytePtr(cfData);
    UInt16 dataLen = CFDataGetLength(cfData);
    char *buffer;
    NSString * text;
    BREvent *event = nil;
    BRRemoteAction_t action;
    NSDictionary * eventDictionary;
    // have pointers ready
    key_event_t     * key_event;
    remote_action_t * remote_action;
    unichar           theChar;
    // mouse_event_t   * mouse_event;
    // touch_event_t   * touch_event;
    // accelerometer_t * acceleometer;   

    switch ( (hid_event_type_t) msgid){

        case TEXT:
            // regular text
            if (dataLen == 0 || !data) break;
            // append \0 byte for NSString conversion
            buffer = (char*) malloc( dataLen + 1);
            if (!buffer) {
                break;
            }
            memcpy(buffer, data, dataLen);
            buffer[dataLen] = 0;
            text = [NSString stringWithUTF8String:buffer];
            // NSLog(@"Injecting text: %@", text);
            eventDictionary = [NSDictionary dictionaryWithObject:text forKey:@"kBRKeyEventCharactersKey"];
            event = [$BREvent eventWithAction:BRRemoteActionKey value:1 atTime:7400.0 originator:5 eventDictionary:eventDictionary allowRetrigger:1];
            [$BRWindow dispatchEvent:event];
            free(buffer);
            break;
            
        case KEY: {
            // individual key events
            key_event = (key_event_t*) data;
            key_event->down = key_event->down ? 1 : 0;
            // NSLog(@"Injecting single char: %C (%x), down: %u", key_event->unicode, key_event->unicode, key_event->down);
            // map special keys to remote actions
            if(key_event->unicode == KEYCODE_RETURN &&  // Check if it's select
               key_event->down){                        // Check if it's a down press
               
                id c = [[[$BRApplicationStackManager singleton] stack] peekController];
                if([c isKindOfClass:$BRTextEntryController])
                {
                    [[c editor] _invokeInputActionWithDictionary:[NSDictionary dictionaryWithObject:@"_inputActionEscape:" forKey:@"Action"]];
                    break;
                }
                
            }
            action = getRemoteActionForKey(key_event->unicode);

            if (action){
                if (action == BRRemoteActionMenu   &&  key_event->down) break; // ignore Menu down
                if (action == BRRemoteActionSelect && !key_event->down) break; // ignore Select up
                injectRemoteAction(action, key_event->down);
                break;
            }
            
            // regular chars don't support repeat and no up events are used
            if (!key_event->down) break;
            
            theChar = key_event->unicode;
            text = [NSString stringWithCharacters:&theChar length:1];
            if([text isEqualToString:@" "]) {
                id c = [[[$BRApplicationStackManager singleton] stack] peekController];
                if (([c isKindOfClass:$BRMenuController]||
                    [c isKindOfClass:$BRViewController]||
                    [c isKindOfClass:$BRMainMenuController]) && 
                    ![c isKindOfClass:$BRTextEntryController]) {

                    NSInteger v = [[c controls]indexOfObjectPassingTest:^(id obj, NSUInteger idx, BOOL *stop) {
                        if ([obj isKindOfClass:$BRTextEntryControl]) {
                                *stop = YES;
                                return YES;
                            }
                            return NO;
                            }];
                    if(v==NSNotFound) {
                        injectRemoteAction(BRRemoteActionSelect,1);
                        break;
                    }
                }
            }
            
            NSString *osBuild = [[$BRSettingsFacade singleton]versionOSBuild];
            int dicEventKeyCode = 48;
            if([osBuild isEqualToString:@"8M89"])
                dicEventKeyCode = 47;
            else if([osBuild isEqualToString:@"8C154"]||[osBuild isEqualToString:@"8C150"])
                dicEventKeyCode = 48;
                
//            id settings = [$BRSettingsFacade singleton];
//            NSLog(@"os %@ os build %@ software %@ software build %@",
//            [settings versionOS], 
//            [settings versionOSBuild], 
//            [settings versionSoftware], 
//            [settings versionSoftwareBuild]);
            eventDictionary = [NSDictionary dictionaryWithObject:text forKey:@"kBRKeyEventCharactersKey"];
            event = [$BREvent eventWithAction:dicEventKeyCode value:key_event->down atTime:7400.0 originator:5 eventDictionary:eventDictionary allowRetrigger:1];
            [$BRWindow dispatchEvent:event];
            break;
        }
            
        case REMOTE:
            // simple remote action
            remote_action = (remote_action_t*) data;
            remote_action->down = remote_action->down ? 1 : 0;
            injectRemoteAction(remote_action->action, remote_action->down);
            break;
            
        default:
            NSLog(@"HID_SUPPORT_PORT_NAME server, msgid %d not supported", (int) msgid);
    }
    return NULL;  // as stated in header, both data and returnData will be released for us after callback returns
}

// %hook BRWindow
// + (BOOL)dispatchEvent:(id)event { 
//     NSLog(@"dispatchEvent with event:%@", event);
//     return %orig;
// }
// %end

%hook LTAppDelegate
-(void)applicationDidFinishLaunching:(id)fp8 {
NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    CFMessagePortRef local = CFMessagePortCreateLocal(NULL, CFSTR(HID_SUPPORT_PORT_NAME), myCallBack, NULL, NULL);
    CFRunLoopSourceRef source = CFMessagePortCreateRunLoopSource(NULL, local, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
    [pool release]; 
    %log;
    %orig;
    // %orig does not return
}
%end
