# lipoplastic setup for armv6 + arm64 compilation
export TARGET := iphone:clang
export TARGET_IPHONEOS_DEPLOYMENT_VERSION = 3.0
export TARGET_IPHONEOS_DEPLOYMENT_VERSION_arm64 = 7.0
export ARCHS = armv7 arm64

THEOS_PACKAGE_DIR_NAME=debs

SUBPROJECTS = libhidsupport hidspringboard hidlowtide

include $(THEOS)/makefiles/common.mk
include $(FW_MAKEDIR)/aggregate.mk
