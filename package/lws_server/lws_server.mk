################################################################################
#
# Libwebsockets Server 
#
################################################################################

LWS_SERVER_VERSION = $(call qstrip,$(BR2_PACKAGE_LWS_SERVER_VERSION))
LWS_SERVER_LICENSE = GPL-2.0
LWS_SERVER_LICENSE_FILES = LICENSE

LWS_SERVER_SITE = $(call qstrip,$(BR2_PACKAGE_LWS_SERVER_CUSTOM_LOCAL_DIRECTORY_LOCATION))
LWS_SERVER_SITE_METHOD = local

# dependences
LWS_SERVER_DEPENDENCIES = libwebsockets cjson

$(eval $(cmake-package))
