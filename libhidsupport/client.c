/*
 * Copyright (C) 2010 by Matthias Ringwald
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MATTHIAS RINGWALD AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

	
#include <sys/sysctl.h>
#import <CoreFoundation/CoreFoundation.h>

#include "../hid-support-internal.h"
#include "../3rdParty/GraphicsServices/GSEvent.h"
#include "RocketBootstrap.h"

#define INVALID_RESULT -5

static int use_backboardd = 0;
static int init = 0;

static int check_for_backboardd(void){
    
    // backboardd only on iOS 6 and higher
    if (kCFCoreFoundationVersionNumber < 793) return 0;

    // no backboardd on ATV
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    int isATV = strncmp("AppleTV", machine, 7) == 0;
    free(machine);
    
    if (isATV) return 0;

    return 1;
}

// Connect to Springboard or lowtide
static CFMessagePortRef hid_support_message_port = 0;

static void hid_support_message_port_refresh(){
	// still valid
	if (hid_support_message_port && !CFMessagePortIsValid(hid_support_message_port)){
		CFRelease(hid_support_message_port);
		hid_support_message_port = NULL;
	}
	// create new one
	if (!hid_support_message_port) {
		hid_support_message_port = rocketbootstrap_cfmessageportcreateremote(NULL, CFSTR(HID_SUPPORT_PORT_NAME));
	}
}

static int hid_support_send_message(hid_event_type_t cmd, uint16_t dataLen, uint8_t *data, CFDataRef *resultData){
	// check for port
	hid_support_message_port_refresh();
	
	if (!hid_support_message_port) {
		printf("hid_support_send_message cannot find server " HID_SUPPORT_PORT_NAME "\n");
		return kCFMessagePortIsInvalid;
	}
	
	// create and send message
	CFDataRef cfData = CFDataCreate(NULL, data, dataLen);
	CFStringRef replyMode = NULL;
	if (resultData) {
		replyMode = kCFRunLoopDefaultMode;
	}
	int result = CFMessagePortSendRequest(hid_support_message_port, cmd, cfData, 1, 1, replyMode, resultData);
	if (cfData) {
		CFRelease(cfData);
	}
	return result;
}


// Connect to backboardd if available
static CFMessagePortRef hid_support_bb_message_port = 0;

static void hid_support_bb_message_port_refresh(){

    // still valid
    if (hid_support_bb_message_port && !CFMessagePortIsValid(hid_support_bb_message_port)){
        CFRelease(hid_support_bb_message_port);
        hid_support_bb_message_port = NULL;
    }
    // create new one
    if (!hid_support_bb_message_port) {
		hid_support_bb_message_port = rocketbootstrap_cfmessageportcreateremote(NULL, CFSTR(HID_SUPPORT_PORT_NAME_BB));
    }
}

static int hid_support_bb_send_message(hid_event_type_t cmd, uint16_t dataLen, uint8_t *data, CFDataRef *resultData){

    if (!init) {
        use_backboardd = check_for_backboardd();
        init = 1;
    }

    if (!use_backboardd) {
        // fallback
        return hid_support_send_message(cmd, dataLen, data, resultData);
    }

    // check for port
    hid_support_bb_message_port_refresh();
    
    if (!hid_support_bb_message_port) {
        printf("hid_support_bb_send_message cannot find server " HID_SUPPORT_PORT_NAME_BB "\n");
        return kCFMessagePortIsInvalid;
    }
    
    // create and send message
    CFDataRef cfData = CFDataCreate(NULL, data, dataLen);
    CFStringRef replyMode = NULL;
    if (resultData) {
        replyMode = kCFRunLoopDefaultMode;
    }
    int result = CFMessagePortSendRequest(hid_support_bb_message_port, cmd, cfData, 1, 1, replyMode, resultData);
    if (cfData) {
        CFRelease(cfData);
    }
    return result;
}

int hid_inject_text(const char * utf8_text){
	return hid_support_bb_send_message(TEXT, strlen(utf8_text), (uint8_t *) utf8_text, 0);
}

int hid_inject_key_down(uint32_t unicode, uint16_t key_modifier) {
	key_event_t event;
	event.down = 1;
	event.modifier = key_modifier;
	event.unicode = unicode;
	return hid_support_bb_send_message(KEY, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_key_up(uint32_t unicode){
	key_event_t event;
	event.modifier = 0;
	event.down = 0;
	event.unicode = unicode;
	return hid_support_bb_send_message(KEY, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_remote_down(uint16_t action) {
	remote_action_t event;
	event.down = 1;
	event.action = action;
	return hid_support_send_message(KEY, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_remote_up(uint16_t action){
	remote_action_t event;
	event.down = 0;
	event.action = action;
	return hid_support_send_message(KEY, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_button_down(uint16_t action){
	button_event_t event;
	event.down   = 1;
	event.action = action;
	return hid_support_send_message(BUTTON, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_button_up(uint16_t action){
	button_event_t event;
	event.down   = 0;
	event.action = action;
	return hid_support_send_message(BUTTON, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_mouse_keep_alive(){
	mouse_event_t event;
	event.type = KEEP_ALIVE;
	return hid_support_send_message(MOUSE, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_mouse_rel_move(uint8_t buttons, float dx, float dy){
	mouse_event_t event;
	event.type = REL_MOVE;
	event.buttons = buttons;
	event.x = dx;
	event.y = dy;
	return hid_support_bb_send_message(MOUSE, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_mouse_abs_move(uint8_t buttons, float ax, float ay){
	mouse_event_t event;
	event.type = ABS_MOVE;
	event.buttons = buttons;
	event.x = ax;
	event.y = ay;
	return hid_support_bb_send_message(MOUSE, sizeof(event), (uint8_t*) &event, 0);
}

int hid_inject_accelerometer(float x, float y, float z){
	accelerometer_t event;
	event.x = x;
	event.y = y;
	event.z = z;
	return hid_support_send_message(ACCELEROMETER, sizeof(event), (uint8_t*) &event, 0);
}
	
int hid_inject_gseventrecord(uint8_t *event_record){
    // get size of GSEventRecord
    int size = sizeof(GSEventRecord) + ((GSEventRecord*)event_record)->infoSize;
    return hid_support_bb_send_message(GSEVENTRECORD, size, event_record, 0);
}

int hid_get_screen_dimension(int *width, int *height){
    CFDataRef resultData;
    int result = hid_support_send_message(GET_SCREEN_DIMENSION, 0, NULL, &resultData);
    if (result < 0) {
        return result;
    }
    const dimension_t *dimension = (const dimension_t *) CFDataGetBytePtr(resultData);
    if (!dimension) {
        return INVALID_RESULT;
    }
    if (CFDataGetLength(resultData) != sizeof(dimension_t)) {
        CFRelease(resultData);
        return INVALID_RESULT;
    }
    if (width)  *width  = dimension->width;
    if (height) *height = dimension->height;
    CFRelease(resultData);
    return 0; 
}


