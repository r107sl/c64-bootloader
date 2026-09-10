# do not remove intermediate targets
.SECONDARY:

# TARGET=c64     8KB C64 kernal ROM
# TARGET=c64c    16KB C64C combined BASIC+kernal image, kernal in the upper half
TARGET ?= c64

ifeq ($(TARGET),c64c)
name      := c64c_bootloader
ld_config := rom_c64c.cfg
DEFINE    := -D C64C_COMBINED=1
else ifeq ($(TARGET),c64)
name      := c64_bootloader
ld_config := rom_c64.cfg
DEFINE    :=
else
$(error TARGET must be c64 or c64c)
endif

builddir := build/$(TARGET)

obj :=
obj += $(builddir)/c64boot.o
obj += $(builddir)/main.o
obj += $(builddir)/screen.o
obj += $(builddir)/rbcp.o

inc      := .

INCLUDE  := $(addprefix -I,$(inc))

.PHONY: all

all: $(builddir)/$(name).bin

###############################################################################
headers := $(wildcard *.h)

$(builddir)/%.s: %.c $(headers) | $(builddir)
	cc65 -t c64 -T -O --static-locals -g $(INCLUDE) $(DEFINE) -o $@ $<

###############################################################################
$(builddir)/%.o: $(builddir)/%.s | $(builddir)
	ca65 -t c64 -g $(INCLUDE) $(DEFINE) -o $@ $<

###############################################################################
$(builddir)/%.o: %.s | $(builddir)
	ca65 -t c64 -g $(INCLUDE) $(DEFINE) -o $@ $<

###############################################################################
$(builddir):
	@mkdir -p $@

$(builddir)/$(name).bin: $(obj) $(ld_config)
	ld65 -o $@ -Ln $@.lbl -m $@.map -C $(ld_config) $(obj) -L /usr/local/lib/cc65/lib --lib c64.lib
	@cat $@.map | grep -e "^Name\|^RBCP\|^RAMCODE\|^ZEROPAGE\|^CODE\|^DATA\|^BSS\|^RODATA\|^ONCE\|^JMPTBL\|^VECTOR"
	@mv -f $@ .

.PHONY: clean
clean:
	@rm -fR build