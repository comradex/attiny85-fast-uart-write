AVRDUDE_MCU   = attiny85
GCC_MCU       = attiny85
CLOCK         = 16000000
BITSWAP_TABLE = 0x0500
PROGRAMMER    = atmelice_isp
PROGNAME      = attiny85-fast-uart-write
SOURCES       = main.cpp
OBJECTS = $(addsuffix .o,$(SOURCES))
ASMS = $(addsuffix .s,$(SOURCES))
HEXFILE = $(PROGNAME).hex
ELFFILE = $(PROGNAME).elf
LINKARGS = -Wl,-section-start=.bitswap_table=$(BITSWAP_TABLE)
# Fuse calculations at http://www.engbedded.com/fusecalc/
FUSES       = -U lfuse:w:0xb1:m -U hfuse:w:0x9f:m -U efuse:w:0xff:m 
AVRDUDE     = avrdude -c $(PROGRAMMER) -p $(AVRDUDE_MCU)
COMPILE     = avr-g++ -std=gnu++11 -Wall -O3 --param=min-pagesize=0 -DBITSWAP_TABLE=$(BITSWAP_TABLE) -DF_CPU=$(CLOCK) -mmcu=$(GCC_MCU) $(LINKARGS)

# "Portable" delete command.
ifdef ComSpec
	RM=del /F /Q
else
	RM=rm -f
endif

all: $(HEXFILE)

%.cpp.o: %.cpp
	$(COMPILE) -c $< -o $@
	
%.cpp.s: %.cpp
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(HEXFILE):i

fuse:
	$(AVRDUDE) $(FUSES)

clean:
	$(RM) $(HEXFILE) $(ELFFILE) $(OBJECTS) $(ASMS)
	
$(ELFFILE): $(OBJECTS)
	$(COMPILE) -o $(ELFFILE) $(OBJECTS)

$(HEXFILE): $(ELFFILE)
	avr-objcopy -j .text -j .data -j .bitswap_table -O ihex $(ELFFILE) $(HEXFILE)
	avr-objdump -Pmem-usage $(ELFFILE)
