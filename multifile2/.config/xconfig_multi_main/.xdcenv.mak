#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_tivac_2_14_00_10/packages;C:/ti/tirtos_tivac_2_14_00_10/products/bios_6_42_01_20/packages;C:/ti/tirtos_tivac_2_14_00_10/products/ndk_2_24_03_35/packages;C:/ti/tirtos_tivac_2_14_00_10/products/uia_2_00_02_39/packages;C:/ti/ccsv6/ccs_base;C:/Users/justin/workspace_v6_1/multifile2/.config
override XDCROOT = C:/ti/xdctools_3_31_01_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_tivac_2_14_00_10/packages;C:/ti/tirtos_tivac_2_14_00_10/products/bios_6_42_01_20/packages;C:/ti/tirtos_tivac_2_14_00_10/products/ndk_2_24_03_35/packages;C:/ti/tirtos_tivac_2_14_00_10/products/uia_2_00_02_39/packages;C:/ti/ccsv6/ccs_base;C:/Users/justin/workspace_v6_1/multifile2/.config;C:/ti/xdctools_3_31_01_33_core/packages;..
HOSTOS = Windows
endif
