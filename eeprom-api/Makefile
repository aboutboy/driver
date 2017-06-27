#
# Copyright (C) 2013-2014 lintel<lintel.huang@gmail.com>
#
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=eeprom-api-ralink
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/eeprom-api-ralink
  CATEGORY:=Ralink
  TITLE:=Ralink EEPROM API driver
  DEPENDS:= @TARGET_ramips
  FILES:=$(PKG_BUILD_DIR)/ralink_eeprom_api.ko
  AUTOLOAD:=$(call AutoLoad,30,ralink_eeprom_api,1)
  KCONFIG:=
endef

define KernelPackage/eeprom-api-ralink/description
	The Ralink WiFi driver need eeprom api dirver support if target is ramips.
endef

define KernelPackage/eeprom-api-ralink/config
	source "$(SOURCE)/Config-mt-wifi.in"
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,eeprom-api-ralink))
