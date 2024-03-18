ARCH = armv7-a
MCPU = cortex-a8

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./tr.ld
MAP_FILE = build/tr.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

C_SRCS = $(wildcard boot/*.c)
C_OBJS = $(patsubst boot/%.c, build/%.o, $(C_SRCS))

INC_DIRS  = -I include

tr = build/tr.axf
tr_bin = build/tr.bin

.PHONY: all clean run debug gdb

all: $(tr)

clean:
	@rm -fr build
	
run: $(tr)
	qemu-system-arm -M realview-pb-a8 -kernel $(tr)
	
debug: $(tr)
	qemu-system-arm -M realview-pb-a8 -kernel $(tr) -S -gdb tcp::1234,ipv4 -display none
	
gdb:
	gdb-multiarch tr.axf
	
$(tr): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(tr) $(ASM_OBJS) $(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(tr) $(tr_bin)
	
build/%.os: $(ASM_SRCS)
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) -c -g -o $@ $<
    
build/%.o: $(C_SRCS)
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) -c -g -o $@ $<