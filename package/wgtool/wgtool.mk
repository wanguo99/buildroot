################################################################################
#
# WGTOOL: wgtool test app
#
################################################################################

WGTOOL_VERSION = 1.0.0

# Top-level directory of the source code
WGTOOL_SITE = $(TOPDIR)/package/wgtool/src

WGTOOL_SITE_METHOD = local
WGTOOL_TARGET_APP = wgtool

define WGTOOL_BUILD_CMDS
    $(MAKE) -C $(@D) CROSS_COMPILE=$(TARGET_CROSS)
endef

define WGTOOL_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/local/bin
    $(INSTALL) -D -m 0755 $(@D)/$(WGTOOL_TARGET_APP) $(TARGET_DIR)/usr/local/bin/
endef


$(eval $(generic-package))
