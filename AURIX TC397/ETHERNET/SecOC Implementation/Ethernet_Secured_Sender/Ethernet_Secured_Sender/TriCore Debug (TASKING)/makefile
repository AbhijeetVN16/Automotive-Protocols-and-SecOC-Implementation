################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Libraries/iLLD/TC39B/Tricore/_PinMap/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/_Lib/DataHandling/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/_Impl/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Stm/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Src/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Scu/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Port/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Pms/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Mtu/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Gtm/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Geth/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Geth/Eth/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Cpu/Trap/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Cpu/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Cpu/Irq/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Asclin/Std/subdir.mk
-include Libraries/iLLD/TC39B/Tricore/Asclin/Asc/subdir.mk
-include Libraries/UART/subdir.mk
-include Libraries/Service/CpuGeneric/StdIf/subdir.mk
-include Libraries/Infra/Ssw/TC39B/Tricore/subdir.mk
-include Libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include Libraries/Ethernet/lwip/src/netif/ppp/polarssl/subdir.mk
-include Libraries/Ethernet/lwip/src/netif/ppp/subdir.mk
-include Libraries/Ethernet/lwip/src/netif/subdir.mk
-include Libraries/Ethernet/lwip/src/core/ipv6/subdir.mk
-include Libraries/Ethernet/lwip/src/core/ipv4/subdir.mk
-include Libraries/Ethernet/lwip/src/core/subdir.mk
-include Libraries/Ethernet/lwip/src/api/subdir.mk
-include Libraries/Ethernet/lwip/port/src/subdir.mk
-include Libraries/Ethernet/Phy_Rtl8211f/subdir.mk
-include Configurations/Debug/subdir.mk
-include Configurations/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Ethernet_Secured_Sender
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
Ethernet_Secured_Sender.siz \


# All Target
all: main-build

# Main-build Target
main-build: Ethernet_Secured_Sender.elf secondary-outputs

# Tool invocations
Ethernet_Secured_Sender.elf: $(OBJS) makefile $(OPTIONAL_TOOL_DEPS) .Ethernet_Secured_Sender.elf.opt
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -f .Ethernet_Secured_Sender.elf.opt
	@echo 'Finished building target: $@'
	@echo ' '

.Ethernet_Secured_Sender.elf.opt:
	@argfile .Ethernet_Secured_Sender.elf.opt --lsl-file="../Lcf_Tasking_Tricore_Tc.lsl" -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="Ethernet_Secured_Sender.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --fp-model=3 -lrt --lsl-core=vtc --exceptions --strict --anachronisms --force-c++ -Ctc39xb -o"Ethernet_Secured_Sender.elf" -Wl-o"Ethernet_Secured_Sender.hex:IHEX" $(OBJS)

.PHONY: .Ethernet_Secured_Sender.elf.opt

Ethernet_Secured_Sender.siz: Ethernet_Secured_Sender.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	elfsize  "Ethernet_Secured_Sender.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Ethernet_Secured_Sender.siz Ethernet_Secured_Sender.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
