# do not remove intermediate targets
.SECONDARY:

name := c64_bootloader

ld_config := rom.cfg

obj :=
obj += build/c64boot.o
obj += build/main.o
obj += build/screen.o
obj += build/rbcp.o

inc      := .

INCLUDE  := $(addprefix -I,$(inc))

.PHONY: all

all: build/$(name).bin

###############################################################################
headers := $(wildcard *.h)

build/%.s: %.c $(headers) | build
	cc65 -t c64 -T -O --static-locals -g $(INCLUDE) $(DEFINE) -o $@ $<

###############################################################################
build/%.o: build/%.s | build
	ca65 -t c64 -g $(INCLUDE) -o $@ $<

###############################################################################
build/%.o: %.s | build
	ca65 -t c64 -g $(INCLUDE) -o $@ $<

###############################################################################
build:
	@mkdir -p $@

build/$(name).bin: $(obj) $(ld_config)
	ld65 -o $@ -Ln $@.lbl -m $@.map -C $(ld_config) $(obj) -L /usr/local/lib/cc65/lib --lib c64.lib
	@cat $@.map | grep -e "^Name\|^RBCP\|^RAMCODE\|^ZEROPAGE\|^CODE\|^DATA\|^BSS\|^RODATA\|^ONCE\|^JMPTBL\|^VECTOR"
	@mv -f $@ .

.PHONY: clean
clean:
	@rm -fR build