
#DEVICE = PMS150C
DEVICE = PFS154
#DEVICE = PFS173
TARGET_VDD_MV = 3000

# ---------------------------------------------------------------------

OUTPUT_NAME = ContinuityTester_$(DEVICE)

ifeq ($(DEVICE), PFS154)
	ARCH = pdk14
else ifeq ($(DEVICE), PFS173)
	ARCH = pdk15
else ifeq ($(DEVICE), PMS150C)
	ARCH = pdk13
else ifeq ($(DEVICE), PMS15A)
	ARCH = pdk13
else ifeq ($(DEVICE), PMS152)
	ARCH = pdk14
else ifeq ($(DEVICE), PMS154C)
	ARCH = pdk14
else ifeq ($(DEVICE), PMS171B)
	ARCH = pdk14
endif

BUILD_DIR = .build
OUTPUT_DIR = .output

OUTPUT = $(OUTPUT_DIR)/$(OUTPUT_NAME)

SOURCES = main.c
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.rel,$(SOURCES))

# http://sdcc.sourceforge.net/doc/sdccman.pdf
COMPILE = sdcc -m$(ARCH) -c --std-sdcc11 --opt-code-size -D$(DEVICE) -DTARGET_VDD_MV=$(TARGET_VDD_MV) -I.
LINK = sdcc -m$(ARCH)
EASYPDKPROG = easypdkprog

# symbolic targets:
all: size

print-%: ; @echo $* = $($*)

$(BUILD_DIR)/%.rel: %.c
	@mkdir -p $(dir $@)
	$(COMPILE) -o $@ $<

$(OUTPUT).ihx: $(OBJECTS)
	@mkdir -p $(dir $(OUTPUT))
	$(LINK) --out-fmt-ihx -o $(OUTPUT).ihx $(OBJECTS)

$(OUTPUT).bin: $(OUTPUT).ihx
	makebin -p $(OUTPUT).ihx $(OUTPUT).bin

build: $(OUTPUT).bin

size: build
	@echo '---------- Segments ----------'
	@egrep '(ABS,CON)|(REL,CON)' $(OUTPUT).map | gawk --non-decimal-data '{dec = sprintf("%d","0x" $$2); print dec " " $$0}' | /usr/bin/sort -n -k1 | cut -f2- -d' '
	@echo '------------------------------'
	@stat -L --printf "Size of $(OUTPUT_NAME).bin: %s bytes\n" $(OUTPUT).bin

program: size
	$(EASYPDKPROG) -n $(DEVICE) write $(OUTPUT).ihx
erase:
	$(EASYPDKPROG) -n $(DEVICE) erase
probe:
	$(EASYPDKPROG) probe

clean:
	rm -r -f $(BUILD_DIR) $(OUTPUT_DIR)
