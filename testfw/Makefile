CCPREFIX=arm-none-eabi
CC=$(CCPREFIX)-gcc
OBJCOPY=$(CCPREFIX)-objcopy

CPU_FLAGS=-march=armv7e-m -mcpu=cortex-m4

BIN=testfw
ELFTARGET=$(BIN).elf
HEXTARGET=$(BIN).hex

all: $(BIN) $(HEXTARGET)

$(BIN): $(ELFTARGET)
	$(OBJCOPY) -O binary $(ELFTARGET) $(BIN)

$(HEXTARGET): $(ELFTARGET)
	$(OBJCOPY) -O ihex $(ELFTARGET) $(HEXTARGET)

$(ELFTARGET): crt0.o main.o headers.o
	$(CC) $(CPU_FLAGS) -O0 -ggdb -nolibc -nodefaultlibs -nostdlib -T linker.lds -o $@ $^

%.o: %.c
	$(CC) $(CPU_FLAGS) -O0 -ggdb -nolibc -nodefaultlibs -nostdlib -c -o $@ $<

%.o: %.S 
	$(CC) -O0 -ggdb -nolibc -nodefaultlibs -nostdlib -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf $(ELFTARGET)
	rm -rf $(HEXTARGET)
	rm -rf $(BIN)
