################################################################################
# Makefile to compile and build the Sample application with the AUTOSAR GPT    #
# Driver Component (For Test purposes only)                                    #
# Compatible with GNU Make 3.81 for Win32.                                     #
################################################################################

################################################################################
# Definitions of global environment variables                                  #
################################################################################

# Get the name of the SRECORD file
CURRENT_APPL_SRECORD = $(CURRENT_APPL)

################################################################################
# Driver Component
include U:\ghs\make\renesas_defs.mak
include U:\ghs\make\renesas_rules.mak

################################################################################
# Command to generate standalone database                                      #
################################################################################
$(MODULE_DB).$(S_RECORD_SUFFIX):$(MODULE_DB).$(OBJ_FILE_SUFFIX) $(LNKFILE_DB)
	@echo *********************************************************************
	@echo Building the standalone database ...
	$(DBLINKER) $(LNKFILE_DB) $(LNKFILE_COMMON) \
	"$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(OBJ_FILE_SUFFIX)" \
	-map="$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(MAP_FILE_SUFFIX)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(EXE_FILE_SUFFIX)"
	@echo Generating Motorola S-Record file...
	$(CONVERTER) $(SFLAGS) "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(EXE_FILE_SUFFIX)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(S_RECORD_SUFFIX)"
	@echo Done ...

################################################################################
# End of the Base Make script                                                  #
################################################################################
