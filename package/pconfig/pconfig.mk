################################################################################
#
# pconfig: product config module
#
################################################################################

PCONFIG_VERSION = 1.0
PCONFIG_SITE = $(TOPDIR)/package/pconfig/src
PCONFIG_SITE_METHOD = local

define PCONFIG_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/pconfig
    find $(PCONFIG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/pconfig/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/pconfig
    find $(PCONFIG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/pconfig/
endef

PCONFIG_DEPENDENCIES = linux

$(eval $(kernel-module))
$(eval $(generic-package))
