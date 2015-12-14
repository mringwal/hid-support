# lipoplastic setup for armv6 + arm64 compilation
export TARGET := iphone:clang
export THEOS_PLATFORM_SDK_ROOT_armv6 = /Applications/Xcode-4.4.1.app/Contents/Developer/
export THEOS_PLATFORM_SDK_ROOT_armv7 = /Applications/Xcode-6.4.app/Contents/Developer/
export SDKVERSION_armv6 = 5.1
export TARGET_IPHONEOS_DEPLOYMENT_VERSION = 3.0
export TARGET_IPHONEOS_DEPLOYMENT_VERSION_arm64 = 9.1
export ARCHS = armv6 armv7 armv7s arm64

THEOS_PACKAGE_DIR_NAME=debs

SUBPROJECTS = libhidsupport hidspringboard hidlowtide

include $(THEOS)/makefiles/common.mk
include $(FW_MAKEDIR)/aggregate.mk
