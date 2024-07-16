################################################################################
#
# osa: OSA Layer
#
################################################################################

OSA_VERSION = 1.0
OSA_SITE = ./package/osa/src
OSA_SITE_METHOD = local

$(eval $(kernel-module))
$(eval $(generic-package))
