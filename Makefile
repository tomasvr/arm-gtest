# Set the executable file name
EXEC = blackpill

# Switch between arm and x86 target
CROSS_COMPILE = arm-none-eabi-
CC = 		$(CROSS_COMPILE)gcc
CPP = 		$(CROSS_COMPILE)g++
OBJCOPY = 	$(CROSS_COMPILE)objcopy
OBJDUMP = 	$(CROSS_COMPILE)objdump
GDB = arm-none-eabi-gdb

# Set the C/C++ sources
CSRC = $(wildcard src/*.c)
CSRC += $(wildcard lib/libopencm3/lib/*.c)
CPPSRC =
TEST_CSRC = $(wildcard test/mocks/*.c)

# Set the debug, optimization, warning level
DEBUG = -ggdb -O0 -Wall -Wextra

INC_DIRS = inc
TEST_INC_DIRS = test/mocks

# Set STM32 target flags
# Insert microcontroller model and arm processor here
MCU = stm32f1
ARM = cortex-m3
MCU_UPPER := $(shell echo $(MCU) | tr a-z A-Z)

CPUFLAGS 	+= -mcpu=$(ARM) -mthumb -D$(MCU_UPPER)
LDFLAGS 	+= $(CPUFLAGS) -Llib/libopencm3/lib -lopencm3_$(MCU) -nostartfiles -Wl,-T,blackpill.ld
INC_DIRS 	+= lib/libopencm3/include
LDLIBS 		= -lc -lgcc -lnosys -lopencm3_$(MCU) -Wl,-Map=blackpill.map

INCLUDES += $(patsubst %,-I%, $(INC_DIRS))
TEST_INCLUDES += $(patsubst %,-I%, $(TEST_INC_DIRS))

BUILD_DIR = build
USER_TEST_DIR = test
# TESTS = leddriver_test
TESTS =

GTEST_DIR = lib/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Set the compiler flags
CFLAGS = $(DEBUG) $(OPT) $(WARNINGS) $(CPUFLAGS) # -std=c11 -ffunction-sections -fdata-sections
CPPFLAGS = $(DEBUG) $(OPT) $(WARNINGS) -std=c++14 -ffunction-sections -fdata-sections
# Gtest flags
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread

# Set the object files
COBJ = $(CSRC:%.c=$(BUILD_DIR)/%.o)
TEST_COJB = $(TEST_CSRC:%.c=$(BUILD_DIR)/%.o)
CPPOBJ = $(CPPSRC:.cc=.o)

# Set the target
all: $(EXEC).elf $(EXEC).bin $(EXEC).hex $(TESTS)

# LDLIBS (LIBRARY FLAGS) NEED TO BE INCLUDED AFTER $^ (DEPENDENCIES/OBJECT FILES)
$(EXEC).elf: $(COBJ) $(CPPOBJ)
	$(CC) $(LDFLAGS) $(INCLUDES) -o $@ $^ $(LDLIBS)
	@$(OBJDUMP) -S $@ > $(EXEC).lst

%.bin: %.elf
	@$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	@$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $@ -c $<

# %.o: %.cc
# 	@mkdir -p $(dir $@)
# 	$(CPP) $(CPPFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(COBJ) $(CPPOBJ) $(EXEC).elf $(EXEC).bin $(EXEC).hex $(EXEC).lst
	# rm -rf $(BUILD_DIR)
	rm -f $(TESTS) gtest.a gtest_main.a *.o

.PHONY: flash
flash: $(EXEC).bin
	st-flash write $< 0x8000000

.PHONY: gdb
gdb: $(EXEC).elf
	$(GDB) $<

.PHONY: opencm
opencm:
	if [ ! -f lib/libopencm3/Makefile ]; then \
		git submodule init; \
		git submodule update; \
	fi
	$(MAKE) -C lib/libopencm3 lib/stm32/f1

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CPP) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CPP) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

Builds a sample test.  A test should link with either gtest.a or
gtest_main.a, depending on whether it defines its own main()
function.

$(TESTS).o : $(USER_TEST_DIR)/$(TESTS).cc $(GTEST_HEADERS)
	$(CPP) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(TEST_INCLUDES) -c $(USER_TEST_DIR)/$(TESTS).cc

$(TESTS) : $(TESTS).o gtest_main.a
	$(CPP) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(TEST_INCLUDES) -lpthread $^ build/leddriver.o -o $@