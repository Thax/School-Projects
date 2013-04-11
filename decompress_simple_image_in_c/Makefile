################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
################################################################################

# The name of your desired GBA image
PROGNAME = mytimedlab5

# The object files you want to compile into your program
OFILES   = main.o mylib.o rle.o

# The cflags you want to compile with
CFLAGS   = -Wall -Werror -std=c99 -pedantic -O2

################################################################################
# These are various settings used to make the GBA toolchain work
# Change these at your own risk
################################################################################

TOOLDIR  = /usr/local/cs2110-tools
CFLAGS  += -mthumb-interwork -mlong-calls -nostartfiles
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-thumb-eabi/4.4.1/thumb \
		  -L $(TOOLDIR)/arm-thumb-eabi/lib \
		  --script $(TOOLDIR)/arm-thumb-eabi/lib/arm-gba.ld
CC      = $(TOOLDIR)/bin/arm-thumb-eabi-gcc
AS      = $(TOOLDIR)/bin/arm-thumb-eabi-as
LD      = $(TOOLDIR)/bin/arm-thumb-eabi-ld
OBJCOPY = $(TOOLDIR)/bin/arm-thumb-eabi-objcopy
GDB     = $(TOOLDIR)/bin/arm-thumb-eabi-gdb

################################################################################
# These are the targets for the GBA build system
################################################################################

all: $(PROGNAME).gba

$(PROGNAME).gba: $(PROGNAME).elf
	$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf: $(OFILES) crt0.o
	$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc

crt0.o: $(TOOLDIR)/arm-thumb-eabi/lib/crt0.s
	$(AS) -mthumb-interwork $^ -o crt0.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.gba

vba: $(PROGNAME).gba
	$(TOOLDIR)/bin/vbam -f1 $(PROGNAME).gba
