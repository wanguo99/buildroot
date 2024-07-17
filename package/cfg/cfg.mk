################################################################################
#
# cfg: product config module
#
################################################################################
ifeq ($(BR2_PACKAGE_CFG_VERSION),)
  CFG_VERSION = 1.0.0
else
  CFG_VERSION = $(BR2_PACKAGE_CFG_VERSION)
endif

CFG_SITE = $(TOPDIR)/package/cfg/src
CFG_SITE_METHOD = local

define CFG_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/cfg
    find $(CFG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/cfg/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/cfg
    find $(CFG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/cfg/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/share/Module.symvers/cfg
    $(INSTALL) -D -m 0755 $(@D)/Module.symvers $(STAGING_DIR)/usr/share/Module.symvers/cfg/
endef

CFG_DEPENDENCIES = linux osa

$(eval $(kernel-module))
$(eval $(generic-package))
