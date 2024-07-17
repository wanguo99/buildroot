################################################################################
#
# bsp: A simple kernel-module
#
################################################################################

BSP_VERSION = 1.0
BSP_SITE = ./package/bsp/src
BSP_SITE_METHOD = local

BSP_DEPENDENCIES = linux osa

$(eval $(kernel-module))
$(eval $(generic-package))
