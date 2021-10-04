# define TA log level (error + warning + debung + flow)
CFG_TEE_TA_LOG_LEVEL ?= 4

# BINARY shall provide TA filename used to load the TA (needed by ta_dev_kit)
BINARY=5a742dca-2861-406d-b317-4f2b6e7ce7f4

# read the the ta_dev_kit.mk makefile before proceeding.
-include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

# wildcard checks if there exists a file matching the pattern, if not this clean target is run
ifeq ($(wildcard $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk), )
clean:
	@echo 'Note: $$(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk not found, cannot clean TA'
	@echo 'Note: TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)'
endif

