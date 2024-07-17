################################################################################
#
# PDL: Peripheral Driver Library
#
################################################################################

ifeq ($(BR2_PACKAGE_PDL_VERSION),)
  PDL_VERSION = 1.0.0
else
  PDL_VERSION = $(BR2_PACKAGE_PDL_VERSION)
endif

PDL_SITE = $(TOPDIR)/package/pdl/src
PDL_SITE_METHOD = local

# 安装规则，包含模块符号表和对外提供的头文件
define PDL_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/pdl
    find $(PDL_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/pdl/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/pdl
    find $(PDL_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/pdl/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/share/Module.symvers/pdl
    $(INSTALL) -D -m 0755 $(@D)/Module.symvers $(STAGING_DIR)/usr/share/Module.symvers/pdl/
endef

PDL_DEPENDENCIES = linux osa cfg

$(eval $(kernel-module))
$(eval $(generic-package))
