################################################################################
#
# OSA: Operation System Adaptor Layer
#
################################################################################
ifeq ($(BR2_PACKAGE_OSA_VERSION),)
  OSA_VERSION = 1.0.0
else
  OSA_VERSION = $(BR2_PACKAGE_OSA_VERSION)
endif

OSA_SITE = $(TOPDIR)/package/osa/src
OSA_SITE_METHOD = local

# 安装规则，包含模块符号表和对外提供的头文件
define OSA_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/osa
    find $(OSA_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/osa/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/osa
    find $(OSA_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/osa/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/share/Module.symvers/osa
    $(INSTALL) -D -m 0755 $(@D)/Module.symvers $(STAGING_DIR)/usr/share/Module.symvers/osa/
endef

OSA_DEPENDENCIES = linux

$(eval $(kernel-module))
$(eval $(generic-package))
