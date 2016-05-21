
CC = gcc
RANLIB = ranlib
AR = ar
LD = ld
STRIP = strip
BUILD_CC=gcc


INCS = -I$(TOPDIR)/include \
    -I$(TOPDIR)/capwap/include \
    -I/usr/include \
    -I/usr/local/include \

CONFIG_X86_64=y
CONFIG_EPOLL=y
DEBUG_CAPWAP_ON_PC=y
CFLAGS += -DCONFIG_X86_64 -DDEBUG_CAPWAP_ON_PC -DCONFIG_EPOLL

# check certificate common name
# CFLAGS += -DDTLS_CHECK_CERT_CN

ifdef SILENT
Q = @
define silentc
	@echo "compiling $@"
endef
define silentl
	@echo "linking $@"
endef
else
Q =
silentc=
silentl=
endif

#
# For a coverage image or if CONFIG_OPT=no, perform no optimization.
# Otherwise, for ARM and x86 use -Os, and for x86_64 use -O2.
#
# If you wish to build without optimization for the sake of debugging,
# use "make CONFIG_OPT=no ..."
#
CONFIG_OPT := yes
OPT_CFLAGS_arch_x86_64 = -O2
OPT_CFLAGS_arch_i386 = -Os
OPT_CFLAGS_opt_yes = $(OPT_CFLAGS_arch_$(ARCH))
OPT_CFLAGS_opt_no = -O0
OPT_CFLAGS_coverage_y = $(OPT_CFLAGS_opt_no)
OPT_CFLAGS_coverage_n = $(OPT_CFLAGS_opt_$(CONFIG_OPT))
OPT_CFLAGS_coverage_ = $(OPT_CFLAGS_coverage_n)
OPT_CFLAGS := $(OPT_CFLAGS_coverage_$(CONFIG_CODE_COVERAGE))

# CFLAGS modifications required when using clang
#
CFLAGS_clang_ := \
	-fno-unit-at-a-time \
	-Wno-unused-but-set-variable
CFLAGS_clang_y := -emit-llvm
# Uncomment for extra clang/linker debug output

COMMON_CFLAGS += \
	$(EXTRA_INCS) \
	$(INCS) \
	-Wall \
	-pipe \
	-g \
	-fno-strict-aliasing \
	-fms-extensions \
	-fsigned-char \
	-Werror \
	$(PROFILE) \
	$(COVERAGE_CFLAGS) \
	$(CFLAGS_clang_$(USE_CLANG)) \
	-Wno-address \
	$(OPT_CFLAGS)

ifdef CONFIG_X86_64
    COMMON_CFLAGS += -fno-omit-frame-pointer
endif

ifndef CONFIG_DEBUG
COMMON_CFLAGS += -DNDEBUG
endif


CFLAGS += $(COMMON_CFLAGS) $(EXTRA_CFLAGS) -Wno-pointer-sign -Wl,-z,now
CXXFLAGS += $(COMMON_CFLAGS) $(EXTRA_CXXFLAGS)

HOST_STD_LDFLAGS= \
    -L$(TOPDIR)/lib \
    -L$(TOPDIR)/ulib \
    -L$(COOKED_LIB) \
    -Wl,-R$(TOPDIR)/lib \
    -Wl,-R$(TOPDIR)/ulib \

BUILD_PROG_LDFLAGS= \

BUILD_PROG1_LDFLAGS= \

DEFAULT_LDFLAGS = \
    -lrt \
    -L/usr/lib \
    -L/usr/lib64 \
    -L/usr/local/lib \
    -L/usr/local/lib64 \


ifndef CONFIG_SIMULATOR
LDFLAGS = \
    -L$(TOPDIR)/lib \
    -L$(TOPDIR)/ulib \
    -L$(COOKED_LIB) \
    -L$(DEVDIR)/lib \
    $(DEFAULT_LDFLAGS) \
    $(EXTRA_LDFLAGS) \
    -Wl,-R$(TOPDIR)/lib \
    -Wl,-R$(TOPDIR)/ulib \
    -Wl,-R$(DEVDIR)/lib \
    -Wl,-R/lib
else
LDFLAGS = \
    -L$(TOPDIR)/lib \
    -L$(TOPDIR)/ulib \
    -L$(COOKED_LIB) \
    -L$(DEVDIR)/lib \
    $(DEFAULT_LDFLAGS) \
    $(EXTRA_LDFLAGS) \
    -Wl,-R$(DEVDIR)/lib \
    -Wl,-R$(TOPDIR)/lib \
    -Wl,-R$(TOPDIR)/ulib \
    -Wl,-R/lib
endif


SYSLIBS = $(COVERAGE_SYSLIBS) $(EXTRA_SYSLIBS) -ljemalloc -lcrypt -lc -ldl -lresolv -lmysqlclient -L/usr/lib64/mysql

TOPLIBDIR=$(TOPDIR)/lib
TOPOBJDIR=$(TOPDIR)/obj

#
# specify vpath so that libraries such as '-lcli' could be used as prerequisite
#
vpath %.so $(TOPDIR)/lib $(TOPDIR)/ulib $(DEVDIR)/lib
vpath %.a $(TOPDIR)/migbase $(TOPDIR)/cli $(TOPDIR)/cmf/miggui

#
# Common built targets:
#

#	TARGET_O : OBJECTS_O
#	TARGET_A : OBJECTS_A
#	TARGET_SO : OBJECTS_SO
#	TARGET_PROG : OBJECTS_PROG
#

# -r flag invalid when building with clang
#
LDFLAG_R_clang_ := -r
LDFLAG_R_clang_y :=
LDFLAG_R = $(LDFLAG_R_clang_$(USE_CLANG))

# TARGET_O: OBJECTS_O for compiling .o files into one single .o file
ifdef TARGET_O
$(TARGET_O): $(OBJECTS_O) $(call GCNO,$(OBJECTS_O))
	$(silentl)
	$(Q)$(LD) $(TARGET_O_LDFLAGS) $(LDFLAG_R) -o $@ $(sort $(OBJECTS_O))
endif

ifdef TARGET_O_CP
$(TARGET_O_CP): $(OBJECTS_O) $(TARGET_O_OBJECTS) $(call GCNO,$(OBJECTS_O))
	$(silentl)
	$(Q)$(LD) $(TARGET_O_LDFLAGS) $(LDFLAG_R) -o $@ $(sort $(OBJECTS_O) $(TARGET_O_OBJECTS))
	$(Q)mkdir -p $(TOPDIR)/obj
	$(Q)cp -f $@ $(TOPOBJDIR)
endif

ifdef TARGET_O_CP2
$(TARGET_O_CP2): $(OBJECTS_O2) $(TARGET_O_OBJECTS2) $(call GCNO,$(OBJECTS_O2))
    ifdef SILENT
	@echo "linking $@"
    endif
	$(Q)$(LD) $(TARGET_O_LDFLAGS) $(LDFLAG_R) -o  $@ $(sort $(OBJECTS_O2) $(TARGET_O_OBJECTS2))
	$(Q)mkdir -p $(TOPDIR)/obj
	$(Q)cp -f $@ $(TOPDIR)/obj	
endif

# TARGET_A: OBJECTS_A for compiling .o files into a static library (.a file)
ifdef TARGET_A
$(TARGET_A): $(OBJECTS_A) $(TARGET_A_OBJECTS) $(call GCNO,$(OBJECTS_A))
	$(silentl)
	$(Q)$(AR) r $@ $(OBJECTS_A) $(sort $(TARGET_A_OBJECTS))
	$(Q)$(RANLIB) $@
ifndef NOALIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_A1: OBJECTS_A1 for compiling .o files into a static library (.a file)
ifdef TARGET_A1
$(TARGET_A1): $(OBJECTS_A1) $(TARGET_A1_OBJECTS) $(call GCNO,$(OBJECTS_A1))
	$(silentl)
	$(Q)$(AR) r $@ $(sort $(OBJECTS_A1) $(TARGET_A1_OBJECTS))
	$(Q)$(RANLIB) $@
ifndef NOALIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_A2: OBJECTS_A2 for compiling .o files into a static library (.a file)
ifdef TARGET_A2
$(TARGET_A2): $(OBJECTS_A2) $(TARGET_A2_OBJECTS) $(call GCNO,$(OBJECTS_A2))
	$(silentl)
	$(Q)$(AR) r $@ $(sort $(OBJECTS_A2) $(TARGET_A2_OBJECTS))
	$(Q)$(RANLIB) $@
ifndef NOALIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_A3: OBJECTS_A3 for compiling .o files into a static library (.a file)
ifdef TARGET_A3
$(TARGET_A3): $(OBJECTS_A3) $(TARGET_A3_OBJECTS) $(call GCNO,$(OBJECTS_A3))
	$(silentl)
	$(Q)$(AR) r $@ $(sort $(OBJECTS_A3) $(TARGET_A3_OBJECTS))
	$(Q)$(RANLIB) $@
ifndef NOALIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_A4: OBJECTS_A4 for compiling .o files into a static library (.a file)
ifdef TARGET_A4
$(TARGET_A4): $(OBJECTS_A4) $(TARGET_A4_OBJECTS) $(call GCNO,$(OBJECTS_A4))
	$(silentl)
	$(Q)$(AR) r $@ $(sort $(OBJECTS_A4) $(TARGET_A4_OBJECTS))
	$(Q)$(RANLIB) $@
ifndef NOALIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_SO: OBJECTS_SO for compiling .o files into a shared library (.so file)
ifdef TARGET_SO
$(TARGET_SO): $(OBJECTS_SO) $(TARGET_SO_OBJECTS) $(call GCNO,$(OBJECTS_SO))
	$(silentl)
	$(Q)$(CC) -shared -Wl,-soname -Wl,$(TARGET_SO) -fPIC $(LDFLAGS) -o $@ $(sort $(OBJECTS_SO) $(TARGET_SO_OBJECTS)) $(TARGET_SO_LIBS) -lc
ifndef NOSOLIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_SO: OBJECTS_SO for compiling .o files into a shared library (.so file)
ifdef TARGET_SO1
$(TARGET_SO1): $(OBJECTS_SO1) $(TARGET_SO1_OBJECTS) $(call GCNO,$(OBJECTS_SO1))
	$(silentl)
	$(Q)$(CC) -shared -Wl,-soname -Wl,$(TARGET_SO1) -fPIC $(LDFLAGS) -o $@ $(sort $(OBJECTS_SO1) $(TARGET_SO1_OBJECTS)) $(TARGET_SO1_LIBS) -lc
ifndef NOSOLIBCOPY
	$(Q)cp -f $@ $(TOPLIBDIR)
endif
endif

# TARGET_PROG: OBJECTS_PROG for compiling .o files into a executable
ifdef TARGET_PROG
TARGET_PROG_OBJECTS += $(OBJECTS_PROG)
$(TARGET_PROG): $(TARGET_PROG_OBJECTS) $(TARGET_PROG_LIBS) $(LIBS) $(call GCNO,$(TARGET_PROG_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(LDFLAGS) $(TARGET_PROG_EXTRA_LDFLAGS) $(sort $(TARGET_PROG_OBJECTS)) $(TARGET_PROG_LIBS) $(LIBS) $(TARGET_PROG_EXTRA_SYSLIBS) $(SYSLIBS) 
endif

# TARGET_PROG1: OBJECTS_PROG1 for compiling .o files into a executable
ifdef TARGET_PROG1
TARGET_PROG1_OBJECTS += $(OBJECTS_PROG1)
$(TARGET_PROG1): $(TARGET_PROG1_OBJECTS) $(TARGET_PROG1_LIBS) $(LIBS) $(call GCNO,$(TARGET_PROG1_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(LDFLAGS) $(TARGET_PROG1_EXTRA_LDFLAGS) $(sort $(TARGET_PROG1_OBJECTS)) $(TARGET_PROG1_LIBS) $(LIBS) $(TARGET_PROG1_EXTRA_SYSLIBS) $(SYSLIBS) 
endif

# TARGET_PROG2: OBJECTS_PROG2 for compiling .o files into a executable
ifdef TARGET_PROG2
TARGET_PROG2_OBJECTS += $(OBJECTS_PROG2)
$(TARGET_PROG2): $(TARGET_PROG2_OBJECTS) $(TARGET_PROG2_LIBS) $(LIBS) $(call GCNO,$(TARGET_PROG2_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(LDFLAGS) $(TARGET_PROG2_EXTRA_LDFLAGS) $(sort $(TARGET_PROG2_OBJECTS)) $(TARGET_PROG2_LIBS) $(LIBS) $(TARGET_PROG2_EXTRA_SYSLIBS) $(SYSLIBS) 
endif

# TARGET_PROG3: OBJECTS_PROG3 for compiling .o files into a executable
ifdef TARGET_PROG3
TARGET_PROG3_OBJECTS += $(OBJECTS_PROG3) 
$(TARGET_PROG3): $(TARGET_PROG3_OBJECTS) $(TARGET_PROG3_LIBS) $(LIBS) $(call GCNO,$(TARGET_PROG3_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(LDFLAGS) $(TARGET_PROG3_EXTRA_LDFLAGS) $(sort $(TARGET_PROG3_OBJECTS)) $(TARGET_PROG3_LIBS) $(LIBS) $(TARGET_PROG3_EXTRA_SYSLIBS) $(SYSLIBS) 
endif

# HOST_STD_PROG: HOST_STD_PROG_OBJECTS for compiling .o files into a executable
ifdef HOST_STD_PROG

$(HOST_STD_PROG): $(HOST_STD_PROG_OBJECTS) $(HOST_STD_PROG_LIBS) $(call GCNO,$(HOST_STD_PROG_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(HOST_STD_LDFLAGS) $(HOST_STD_PROG_EXTRA_LDFLAGS) $(filter-out %.gcno,$^) $(HOST_STD_PROG_EXTRA_SYSLIBS) $(COVERAGE_SYSLIBS)
endif

# HOST_STD_PROG1: HOST_STD_PROG1_OBJECTS for compiling .o files into a executable
ifdef HOST_STD_PROG1
$(HOST_STD_PROG1): $(HOST_STD_PROG1_OBJECTS) $(HOST_STD_PROG1_LIBS) $(call GCNO,$(HOST_STD_PROG1_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(HOST_STD_PROG1_LDFLAGS) $(filter-out %.gcno,$^) $(HOST_STD_PROG1_EXTRA_SYSLIBS) $(COVERAGE_SYSLIBS)
endif

# HOST_STD_PROG2: HOST_STD_PROG2_OBJECTS for compiling .o files into a executable
ifdef HOST_STD_PROG2
$(HOST_STD_PROG2): $(HOST_STD_PROG2_OBJECTS) $(HOST_STD_PROG2_LIBS) $(call GCNO,$(HOST_STD_PROG2_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(HOST_STD_LDFLAGS) $(HOST_STD_PROG2_EXTRA_LDFLAGS) $(filter-out %.gcno,$^) $(COVERAGE_SYSLIBS)
endif


# UPGRADE_TARGET_PROG: UPGRADE_OBJECTS_PROG for compiling .o files into a executable
ifdef UPGRADE_TARGET_PROG
ULDFLAGS += -Wl,-R,/data/lib

$(UPGRADE_TARGET_PROG): $(UPGRADE_OBJECTS_PROG) $(UPGRADE_TARGET_PROG_OBJECTS) $(LIBS) $(call GCNO,$(UPGRADE_OBJECTS_PROG) $(UPGRADE_TARGET_PROG_OBJECTS))
	$(silentc)
	$(Q)$(CC) -o $@ $(LDFLAGS) $(ULDFLAGS) $(UPGRADE_TARGET_PROG_EXTRA_LDFLAGS) $(filter-out %.gcno,$^) $(SYSLIBS)
endif

ifdef SUBDIRS
SUBDIRSTARGET=subdirs
.PHONY: $(SUBDIRSTARGET) $(SUBDIRS)

GOALSTR=
ifneq ("$(MAKECMDGOALS)","")
GOALSTR=($(MAKECMDGOALS))
endif

#$(SUBDIRSTARGET): $(sort $(SUBDIRS))
$(SUBDIRSTARGET): $(SUBDIRS)

$(SUBDIRS):
	@echo "===> $@ $(GOALSTR)"; \
	if [ -f "$@/Makefile" ]; then \
		$(MAKE) -C $@ $(MAKECMDGOALS) || exit $$?; \
	fi; \
	echo "<=== $@ $(GOALSTR)";
endif

# BUILD_PROG: for compile-time executables
ifdef BUILD_PROG

$(BUILD_PROG): $(BUILD_PROG_OBJECTS) $(BUILD_PROG_LIBS)
	$(silentc)
	$(Q)$(BUILD_CC) -o $@ $(BUILD_PROG_LDFLAGS) $(BUILD_PROG_EXTRA_LDFLAGS) $^ $(BUILD_PROG_SYSLIBS) $(COVERAGE_LIB)

$(BUILD_PROG_OBJECTS): %.o: %.c
	$(silentc)
	$(Q)$(BUILD_CC) $(CFLAGS) $(BUILD_PROG_EXTRA_CFLAGS) -c $< -o $@ -MD -MP -MF $(@:.o=.d)
endif

# BUILD_PROG1: for compile-time executables
ifdef BUILD_PROG1

$(BUILD_PROG1): $(BUILD_PROG1_OBJECTS) $(BUILD_PROG1_LIBS)
	$(silentc)
	$(Q)$(BUILD_CC) -o $@ $(BUILD_PROG1_LDFLAGS) $(BUILD_PROG1_EXTRA_LDFLAGS) $^ $(BUILD_PROG1_SYSLIBS) $(COVERAGE_LIB)

$(BUILD_PROG1_OBJECTS_DIR)/%.o:%.c
	$(silentc)
	$(Q)$(BUILD_CC) $(CFLAGS) $(BUILD_PROG1_EXTRA_CFLAGS) -c $< -o $@
endif

# BUILD_PROG2: for compile-time executables
ifdef BUILD_PROG2

$(BUILD_PROG2): $(BUILD_PROG2_OBJECTS) $(BUILD_PROG2_LIBS)
	$(silentc)
	$(Q)$(BUILD_CC) -o $@ $(HOST_STD_LDFLAGS) $(BUILD_PROG2_LDFLAGS) $(BUILD_PROG2_EXTRA_LDFLAGS) $^ $(BUILD_PROG2_SYSLIBS) $(COVERAGE_LIB)

$(BUILD_PROG2_OBJECTS): %.o: %.c
	$(silentc)
	$(Q)$(BUILD_CC) $(CFLAGS) $(BUILD_PROG2_EXTRA_CFLAGS) -c $< -o $@ -MD -MP -MF $(@:.o=.d)
endif


#
# Generating dependency files in .deps/ directory while compiling
#
DEPDIR = .deps
DEPFILE = $(DEPDIR)/$(subst /,_,$*.d)
%.o %.gcno: %.c
	-@[ -d $(DEPDIR) ] || mkdir -p $(DEPDIR)
	$(silentc)
	$(Q)$(CC) $(CFLAGS) $(ARCHFLAGS) -c $< -o $(@:.gcno=.o) -MD -MP -MF $(DEPFILE)

%.o %.gcno: %.cpp
	-@[ -d $(DEPDIR) ] || mkdir -p $(DEPDIR)
	$(silentc)
	$(Q)$(CC) $(CXXFLAGS) $(ARCHFLAGS) -c $< -o $(@:.gcno=.o) -MD -MP -MF $(DEPFILE)

%.gcno: %.o

-include $(DEPDIR)/*.d

realclean: clean
	@-rm -fr $(DEPDIR) *.d
