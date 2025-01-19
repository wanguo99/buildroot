################################################################################
#
# PDM (Perpherial Driver Module) Driver
#
################################################################################

PDM_VERSION = $(call qstrip,$(BR2_PACKAGE_PDM_VERSION))
# Specify the license and where to find the license file(s)
PDM_LICENSE = GPL-2.0
PDM_LICENSE_FILES = LICENSE

# Compute PDM_SOURCE and PDM_SITE from the configuration
ifeq ($(BR2_PACKAGE_PDM_CUSTOM_LOCAL_DIRECTORY),y)
PDM_SITE = $(call qstrip,$(BR2_PACKAGE_PDM_CUSTOM_LOCAL_DIRECTORY_LOCATION))
PDM_SITE_METHOD = local
else ifeq ($(BR2_PACKAGE_PDM_CUSTOM_GIT),y)
PDM_SITE = $(call qstrip,$(BR2_PACKAGE_PDM_CUSTOM_REPO_URL))
PDM_SITE_METHOD = git
else # default using git
PDM_SITE = $(call qstrip,$(BR2_PACKAGE_PDM_CUSTOM_REPO_URL))
PDM_SITE_METHOD = git
endif

ifeq ($(PDM_SITE),)
$(error BR2_PACKAGE_PDM_VERSION: $(BR2_PACKAGE_PDM_VERSION))
$(error PDM_SITE: $(PDM_SITE))
$(error PDM_SITE_METHOD: $(PDM_SITE_METHOD))
$(error No custom kernel source set. Check your BR2_PACKAGE_PDM_VERSION setting)
endif

# PDM_MAKE_OPTS = 

$(eval $(kernel-module))

$(eval $(generic-package))
