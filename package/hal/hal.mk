################################################################################
#
# HAL: Hardware Abstraction Layer
#
################################################################################

ifeq ($(BR2_PACKAGE_HAL_VERSION),)
  HAL_VERSION = 1.0.0
else
  HAL_VERSION = $(BR2_PACKAGE_HAL_VERSION)
endif

HAL_SITE = $(TOPDIR)/package/hal/src
HAL_SITE_METHOD = local

# 安装规则，包含模块符号表和对外提供的头文件
define HAL_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/hal
    find $(HAL_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/hal/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/hal
    find $(HAL_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/hal/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/share/Module.symvers/hal
    $(INSTALL) -D -m 0755 $(@D)/Module.symvers $(STAGING_DIR)/usr/share/Module.symvers/hal/
endef

HAL_DEPENDENCIES = linux osa

$(eval $(kernel-module))
$(eval $(generic-package))
