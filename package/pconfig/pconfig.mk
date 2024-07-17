################################################################################
#
# pconfig: product config module
#
################################################################################
ifeq ($(BR2_PACKAGE_PCONFIG_VERSION),)
  PCONFIG_VERSION = 1.0.0
else
  PCONFIG_VERSION = $(BR2_PACKAGE_PCONFIG_VERSION)
endif

PCONFIG_SITE = $(TOPDIR)/package/pconfig/src
PCONFIG_SITE_METHOD = local

# 安装规则，包含模块符号表和对外提供的头文件
define PCONFIG_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/pconfig
    find $(PCONFIG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/pconfig/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/pconfig
    find $(PCONFIG_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/pconfig/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/share/Module.symvers/pconfig
    $(INSTALL) -D -m 0755 $(@D)/Module.symvers $(STAGING_DIR)/usr/share/Module.symvers/pconfig/
endef

PCONFIG_DEPENDENCIES = linux

$(eval $(kernel-module))
$(eval $(generic-package))
