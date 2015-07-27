# lipoplastic setup for armv6 + arm64 compilation
export TARGET := iphone:clang
export THEOS_PLATFORM_SDK_ROOT_armv6 = /Applications/Xcode-4.4.1.app/Contents/Developer
export SDKVERSION_armv6 = 5.1
export TARGET_IPHONEOS_DEPLOYMENT_VERSION = 3.0
export TARGET_IPHONEOS_DEPLOYMENT_VERSION_arm64 = 7.0
export ARCHS = armv6 arm64

THEOS_PACKAGE_DIR_NAME=debs

SUBPROJECTS = libhidsupport hidspringboard hidlowtide

include $(THEOS)/makefiles/common.mk
include $(FW_MAKEDIR)/aggregate.mk
