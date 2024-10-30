#######################################################################
# REGISTRY
#

CC_FILES_TO_BUILD +=

MAKE_DEBUG_RULES += debug_ghs_makefile


#######################################################################
# Command to print debug information                                  #
#######################################################################
debug_ghs_makefile:
	@echo COMPILER_INSTALL_DIR = $(COMPILER_INSTALL_DIR)
	@echo CC = $(CC)
	@echo CFLAGS = $(CFLAGS)
	@echo MAKELIB = $(MAKELIB)

#######################################################################
