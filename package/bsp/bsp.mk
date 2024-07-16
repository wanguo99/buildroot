################################################################################
#
# bsp: A simple kernel-module
#
################################################################################

BSP_VERSION = 1.0
BSP_SITE = ./package/bsp/src
BSP_SITE_METHOD = local

define BSP_BUILD_CMDS
	$(MAKE) -C '$(@D)' LINUX_DIR='$(LINUX_DIR)' PWD='$(@D)' CC='$(TARGET_CC)' LD='$(TARGET_LD)'
endef

# define BSP_INSTALL_TARGET_CMDS
# 	$(INSTALL) -D -m 0755 '$(@D)/bsp.ko' '$(TARGET_DIR)/lib/modules/$(LINUX_VERSION)/bsp.ko'
# endef

$(eval $(kernel-module))
$(eval $(generic-package))
