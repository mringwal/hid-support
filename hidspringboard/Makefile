include $(THEOS)/makefiles/common.mk

TWEAK_NAME = hidspringboard
hidspringboard_FILES = Tweak.xm
hidspringboard_CFLAGS = -I../3rdParty
hidspringboard_PRIVATE_FRAMEWORKS = QuartzCore GraphicsServices IOKit
hidspringboard_LIBRARIES = substrate

# rocketbootstrap
hidspringboard_CFLAGS      += -I/Projects/RocketBootstrap/
# hidspringboard_LDFLAGS   += -L/Projects/RocketBootstrap/.theos/obj
# hidspringboard_LIBRARIES += rocketbootstrap

include $(THEOS_MAKE_PATH)/tweak.mk
