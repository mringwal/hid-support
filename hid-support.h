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

#pragma once

#if defined __cplusplus
extern "C" {
#endif
	
#include <stdint.h>

// Unicodes that identify function keys on the keyboard, see
// http://www.gnustep.org/resources/OpenStepSpec/ApplicationKit/TypesAndConstants/AppKitTypesSummary.html

enum {
    NSUpArrowFunctionKey        = 0xF700,
    NSDownArrowFunctionKey      = 0xF701,
    NSLeftArrowFunctionKey      = 0xF702,
    NSRightArrowFunctionKey     = 0xF703,
    NSF1FunctionKey             = 0xF704,
    NSF2FunctionKey             = 0xF705,
    NSF3FunctionKey             = 0xF706,
    NSF4FunctionKey             = 0xF707,
    NSF5FunctionKey             = 0xF708,
    NSF6FunctionKey             = 0xF709,
    NSF7FunctionKey             = 0xF70A,
    NSF8FunctionKey             = 0xF70B,
    NSF9FunctionKey             = 0xF70C,
    NSF10FunctionKey            = 0xF70D,
    NSF11FunctionKey            = 0xF70E,
    NSF12FunctionKey            = 0xF70F,
    NSF13FunctionKey            = 0xF710,
    NSF14FunctionKey            = 0xF711,
    NSF15FunctionKey            = 0xF712,
    NSF16FunctionKey            = 0xF713,
    NSF17FunctionKey            = 0xF714,
    NSF18FunctionKey            = 0xF715,
    NSF19FunctionKey            = 0xF716,
    NSF20FunctionKey            = 0xF717,
    NSF21FunctionKey            = 0xF718,
    NSF22FunctionKey            = 0xF719,
    NSF23FunctionKey            = 0xF71A,
    NSF24FunctionKey            = 0xF71B,
    NSF25FunctionKey            = 0xF71C,
    NSF26FunctionKey            = 0xF71D,
    NSF27FunctionKey            = 0xF71E,
    NSF28FunctionKey            = 0xF71F,
    NSF29FunctionKey            = 0xF720,
    NSF30FunctionKey            = 0xF721,
    NSF31FunctionKey            = 0xF722,
    NSF32FunctionKey            = 0xF723,
    NSF33FunctionKey            = 0xF724,
    NSF34FunctionKey            = 0xF725,
    NSF35FunctionKey            = 0xF726,
    NSInsertFunctionKey         = 0xF727,
    NSDeleteFunctionKey         = 0xF728,
    NSHomeFunctionKey           = 0xF729,
    NSBeginFunctionKey          = 0xF72A,
    NSEndFunctionKey            = 0xF72B,
    NSPageUpFunctionKey         = 0xF72C,
    NSPageDownFunctionKey       = 0xF72D,
    NSPrintScreenFunctionKey    = 0xF72E,
    NSScrollLockFunctionKey     = 0xF72F,
    NSPauseFunctionKey          = 0xF730,
    NSSysReqFunctionKey         = 0xF731,
    NSBreakFunctionKey          = 0xF732,
    NSResetFunctionKey          = 0xF733,
    NSStopFunctionKey           = 0xF734,
    NSMenuFunctionKey           = 0xF735,
    NSUserFunctionKey           = 0xF736,
    NSSystemFunctionKey         = 0xF737,
    NSPrintFunctionKey          = 0xF738,
    NSClearLineFunctionKey      = 0xF739,
    NSClearDisplayFunctionKey   = 0xF73A,
    NSInsertLineFunctionKey     = 0xF73B,
    NSDeleteLineFunctionKey     = 0xF73C,
    NSInsertCharFunctionKey     = 0xF73D,
    NSDeleteCharFunctionKey     = 0xF73E,
    NSPrevFunctionKey           = 0xF73F,
    NSNextFunctionKey           = 0xF740,
    NSSelectFunctionKey         = 0xF741,
    NSExecuteFunctionKey        = 0xF742,
    NSUndoFunctionKey           = 0xF743,
    NSRedoFunctionKey           = 0xF744,
    NSFindFunctionKey           = 0xF745,
    NSHelpFunctionKey           = 0xF746,
    NSModeSwitchFunctionKey     = 0xF747,
    
    // hid-support special
    NSBeginOfLineFunctionKey    = 0xf780,
    NSEndOfLineFunctionKey      = 0xf781
};
	
typedef enum BRRemoteAction {
	BRRemoteActionInvalid =  0,
	BRRemoteActionMenu    =  1,
	BRRemoteActionUp      =  3,
	BRRemoteActionDown    =  4,
	BRRemoteActionSelect  =  5,
	BRRemoteActionLeft    =  6,
	BRRemoteActionRight   =  7,
	BRRemoteActionPlay    = 10,
	BRRemoteActionKey     = 47	// used in hid_inject_text, cannot be used in hid_inject_remote_action_x
} BRRemoteAction_t;

typedef enum HWButton {
    HWButtonHome = 0,
    HWButtonLock,
    HWButtonVolumeUp,
    HWButtonVolumeDown,
    HWButtonVolumeMute,     // >= iOS 3.2
    HWButtonBrightnessUp,   // >= iOS 3.2
    HWButtonBrightnessDown, // >= iOS 3.2
    HWButtonTogglePlayPause,
    HWButtonPlay,
    HWButtonPause,
    HWButtonPreviousTrack,
    HWButtonNextTrack
} HWButton_t;
    
typedef enum hid_key_modifier {
	CMD   = 0x01,
	ALT   = 0x02,
	CTRL  = 0x04,
	SHIFT = 0x08
} hid_key_modifier_t;

int hid_inject_text(const char * utf8_text);

int hid_inject_key_down(uint32_t unicode, uint16_t key_modifier);
int hid_inject_key_up(uint32_t unicode);

int hid_inject_remote_down(uint16_t action);
int hid_inject_remote_up(uint16_t action);

int hid_inject_button_down(uint16_t action);
int hid_inject_button_up(uint16_t action);

int hid_inject_gseventrecord(uint8_t *event_record);

int hid_get_screen_dimension(int *width, int *height);

// not implemented yet
int hid_inject_mouse_keep_alive();
int hid_inject_mouse_rel_move(uint8_t buttons, float dx, float dy);
int hid_inject_mouse_abs_move(uint8_t buttons, float ax, float ay);

// not implemented yet
int hid_inject_accelerometer(float x, float y, float z);
							 
#if defined __cplusplus
}
#endif


