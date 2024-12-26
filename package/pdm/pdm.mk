################################################################################
#
# PDM (Perpherial Driver Module)
#
################################################################################

PDM_VERSION = $(call qstrip,$(BR2_PACKAGE_PDM_GIT_REPO_VERSION))
PDM_SITE = $(call qstrip,$(BR2_PACKAGE_PDM_GIT_REPO_URL))
PDM_SITE_METHOD = git

# Specify the license and where to find the license file(s)
PDM_LICENSE = GPL-2.0
PDM_LICENSE_FILES = LICENSE

# Assuming PDM is a kernel module based on the dependency in Config.in.
$(eval $(kernel-module))

# For any additional configuration or installation steps, add them here.
# For example, if there are specific make options or install commands:
# PDM_MAKE_OPTS = ...
# PDM_INSTALL_TARGET_OPTS = ...

# If PDM has dependencies on other packages, list them here:
# PDM_DEPENDENCIES = some-other-package

# Include this if you have patches to apply:
# define PDM_APPLY_PATCHES
#    cd $(@D); \
#    $(APPLY_PATCHES) $(PDM_PKGDIR) $(PDM_VERSION)
# endef
# PDM_POST_EXTRACT_HOOKS += PDM_APPLY_PATCHES

$(eval $(generic-package))
