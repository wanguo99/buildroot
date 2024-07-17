################################################################################
#
# osa: OSA Layer
#
################################################################################

OSA_VERSION = 1.0
OSA_SITE = $(TOPDIR)/package/osa/src
OSA_SITE_METHOD = local

define OSA_INSTALL_TARGET_CMDS
    $(INSTALL) -d -m 0755 $(TARGET_DIR)/usr/include/osa
    find $(OSA_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(TARGET_DIR)/usr/include/osa/

    $(INSTALL) -d -m 0755 $(STAGING_DIR)/usr/include/osa
    find $(OSA_SITE)/include -name '*.h' | xargs -I {} \
        $(INSTALL) -m 0644 {} $(STAGING_DIR)/usr/include/osa/
endef

OSA_DEPENDENCIES = linux

$(eval $(kernel-module))
$(eval $(generic-package))
