#####################################
# Real Time Embedded Systems
# Project 1
# 
# Ⓒ 2021 K. Chatzis
# kachatzis <at> ece.auth.gr
#####################################

GCC=gcc
XGCC=arm-linux-gnueabihf-gcc

GCC_FLAGS=-O3
XGCC_FLAGS=-O3

INC_DIR=./include
SRC_DIR=./src

LIB=-lpthread 


################################
#   x86/GCC                    #
################################
L_OUTPUT=./x86.o
x86:
	$(GCC) $(GCC_FLAGS) -o $(L_OUTPUT) $(shell find $(SRC_DIR) -name '*.c') -I $(INC_DIR) $(LIB)
run_x86:
	$(L_OUTPUT)

################################
#   Mode A:                    #
#   Raspberry Pi Zero W/WH     #
#   Raspberry Pi 1 A/B/A+/B+   #
################################
RPI_A_FLAGS=-march=armv6 -mfloat-abi=hard -mfpu=vfp
RPI_A_OUTPUT=./rpi_a.o

rpi_a:
	$(XGCC) $(XGCC_FLAGS) -o $(RPI_A_OUTPUT) $(shell find $(SRC_DIR) -name '*.c') -I $(INC_DIR) $(RPI_A_FLAGS) $(LIB)

run_rpi_a:
	$(RPI_A_OUTPUT)

rpi_zero_w: rpi_a
rpi_zero_wh: rpi_a
rpi_1_a: rpi_a
rpi_1_b: rpi_a
rpi_1_ap: rpi_a
rpi_1_bp: rpi_a

run_rpi_zero_w: run_rpi_a
run_rpi_zero_wh: run_rpi_a
run_rpi_1_a: run_rpi_a
run_rpi_1_b: run_rpi_a
run_rpi_1_ap: run_rpi_a
run_rpi_1_bp: run_rpi_a	


###########################
#   Mode B:               #
#   Raspberry 2 A/B       #
#   Raspberry 3 A/B       #
###########################
RPI_B_FLAGS=-march=armv7-a -mfloat-abi=hard -mfpu=neon-vfpv4
RPI_B_OUTPUT=./rpi_b.o

# TODO: add executions

###########################
#   Mode C:               #
#   Raspberry 3 A+/B+     #
#   Raspberry 4 A+/B+     #
###########################
RPI_C_FLAGS=-march=armv8-a -mfloat-abi=hard -mfpu=neon-fp-armv8
RPI_C_OUTPUT=./rpi_b.o

# TODO: add executions



###########################
#   Utilities             #
###########################
help:
	@echo "For Raspberry Pi"
	@echo "  Build: make rpi_(version)"
	@echo "  Run:   make run_rpi_(version)"
	@echo ""
	@echo "  Versions: zero_w, zero_wh, 1_a, 1_ap, 1_b, 1_bp"
	@echo ""
	@echo "For x86/64"
	@echo "  Build: make x86"
	@echo "  Run:   make run_x86"

clean:
	rm -rf ./*.o ./*.out