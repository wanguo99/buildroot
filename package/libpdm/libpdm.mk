################################################################################
#
# libpdm
#
################################################################################

LIBPDM_VERSION = $(call qstrip,$(BR2_PACKAGE_LIBPDM_GIT_REPO_VERSION))
LIBPDM_SITE = $(call qstrip,$(BR2_PACKAGE_LIBPDM_GIT_REPO_URL))
LIBPDM_SITE_METHOD = git
LIBPDM_LICENSE = GPL-2.0
LIBPDM_LICENSE_FILES = LICENSE


LIBPDM_CONF_OPTS += \
	-DBUILD_APPS=OFF \
	-DCMAKE_CXX_FLAGS="$(TARGET_CXXFLAGS) -std=c++11 -fPIC -DAP4_PLATFORM_BYTE_ORDER=$(LIBPDM_BYTE_ORDER)"

$(eval $(cmake-package))
