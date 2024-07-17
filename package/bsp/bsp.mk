################################################################################
#
# bsp: BSP Layer
#
################################################################################

BSP_VERSION = 1.0
BSP_SITE = $(TOPDIR)/package/bsp/src
BSP_SITE_METHOD = local

# 安装规则，包含模块和对外提供的头文件
define BSP_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/bsp
    find $(BSP_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/bsp/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/bsp
    find $(BSP_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/bsp/
endef

BSP_DEPENDENCIES = linux

$(eval $(kernel-module))
$(eval $(generic-package))
