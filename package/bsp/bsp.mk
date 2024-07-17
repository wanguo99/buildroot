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
    # 确保目标目录存在
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/bsp
    # 使用 find 和 xargs 处理通配符问题
    find $(TOPDIR)/package/bsp/include/bsp/ -name '*.h' | xargs -I {} \
		$(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/bsp/
endef

BSP_DEPENDENCIES = linux

# 使用 Buildroot 内置的 kernel-module 基础设施
$(eval $(kernel-module))
$(eval $(generic-package))
