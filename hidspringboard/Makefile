include $(THEOS)/makefiles/common.mk

TWEAK_NAME = hidspringboard
hidspringboard_FILES = Tweak.xm
hidspringboard_CFLAGS = -I../3rdParty
hidspringboard_LDFLAGS = $(ISYSROOT)/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit
hidspringboard_PRIVATE_FRAMEWORKS = QuartzCore GraphicsServices

# rocketbootstrap
hidspringboard_CFLAGS    += -I/Projects/RocketBootstrap/
hidspringboard_LDFLAGS   += -L/Projects/RocketBootstrap/.theos/obj
hidspringboard_LIBRARIES += substrate rocketbootstrap

include $(THEOS_MAKE_PATH)/tweak.mk
