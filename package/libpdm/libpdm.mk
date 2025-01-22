################################################################################
#
# PDM (Perpherial Driver Module) Library
#
################################################################################

LIBPDM_VERSION = $(call qstrip,$(BR2_PACKAGE_LIBPDM_VERSION))
# Specify the license and where to find the license file(s)
LIBPDM_LICENSE = GPL-2.0
LIBPDM_LICENSE_FILES = LICENSE

# Compute PDM_SOURCE and PDM_SITE from the configuration
ifeq ($(BR2_PACKAGE_LIBPDM_CUSTOM_LOCAL_DIRECTORY),y)
LIBPDM_SITE = $(call qstrip,$(BR2_PACKAGE_LIBPDM_CUSTOM_LOCAL_DIRECTORY_LOCATION))
LIBPDM_SITE_METHOD = local
else ifeq ($(BR2_PACKAGE_LIBPDM_CUSTOM_GIT),y)
LIBPDM_SITE = $(call qstrip,$(BR2_PACKAGE_LIBPDM_CUSTOM_REPO_URL))
LIBPDM_SITE_METHOD = git
else # default using git
LIBPDM_SITE = $(call qstrip,$(BR2_PACKAGE_LIBPDM_CUSTOM_REPO_URL))
LIBPDM_SITE_METHOD = git
endif

# dependences
LIBPDM_DEPENDENCIES = libwebsockets mosquitto

$(eval $(cmake-package))
