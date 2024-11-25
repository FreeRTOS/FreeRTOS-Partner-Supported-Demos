###############################################################################
# REGISTRY
#
PREPARE_CONFIGURATION_INTERFACE += MODULE_DBASE_REQ

CHECK_VARS_WHICH_ARE_REQUIRED += MODULE_DBASE_REQ


###############################################################################
# SPECIFIC
#
ifneq ($(MODULE_DBASE_REQ),yes)
  ifneq ($(MODULE_DBASE_REQ),no)
    $(error The value of the variable MODULE_DBASE_REQ is not valid. \
      Please specify whether database is required or not )
  endif
endif