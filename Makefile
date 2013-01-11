#
# Author     : Jean-Charles Lefebvre
# Created On : 2013-01-09 15:30:56
#
# $Id$
#


#----------------------------------------------------------------------------
PROJECT_NAME = BattAlert

DIR_ROOT        = .
DIR_BIN         = $(DIR_ROOT)/bin
DIR_OBJ         = $(DIR_ROOT)/obj
DIR_OBJ_DEBUG   = $(DIR_ROOT)/obj/debug
DIR_OBJ_RELEASE = $(DIR_ROOT)/obj/release
DIR_SRC         = $(DIR_ROOT)/src

# CAUTION: files in this list must be relative to DIR_SRC
HEADERS = \
  stable.h \
  resource.h \
  App.h \
  App.inl.h \
  BattIcon.h \
  Logger.h \
  Logger.inl.h \
  StringA.h \
  StringA.inl.h \
  Wnd.h \
  Wnd.inl.h \
  WndMain.h \
  WndMain.inl.h

# CAUTION: files in this list must be relative to DIR_SRC
SOURCES = \
  main.cpp \
  App.cpp \
  BattIcon.cpp \
  Logger.cpp \
  StringA.cpp \
  Wnd.cpp \
  WndMain.cpp

# CAUTION: files in this list must be relative to DIR_SRC
RESOURCES = BattAlert.rc


#----------------------------------------------------------------------------
CC      = mingw32-gcc
WINDRES = windres
COPY    = copy /y
MKDIR   = mkdir
RMDIR   = rmdir /s /q
STRIP   = strip --strip-all
UPX     = upx --best

EXE_SUFFIX=.exe

CFLAGS_COMMON  = -pipe
CFLAGS_COMMON += -Wall -Werror -Wno-deprecated -Wno-unused-value -Wno-char-subscripts
CFLAGS_DEBUG   = -g3 -D_DEBUG
CFLAGS_RELEASE = -O2 -s -DNDEBUG

CXXFLAGS_COMMON  = -fno-rtti
CXXFLAGS_DEBUG   =
CXXFLAGS_RELEASE =

LIBS_COMMON  = -lstdc++
LIBS_DEBUG   =
LIBS_RELEASE =

LDFLAGS_COMMON  = -mwindows -static
LDFLAGS_DEBUG   =
LDFLAGS_RELEASE =


#----------------------------------------------------------------------------
TARGET_DIRS = $(DIR_BIN) $(DIR_OBJ) $(DIR_OBJ_DEBUG) $(DIR_OBJ_RELEASE)

TARGET_PRE_DEBUG     = $(DIR_OBJ_DEBUG)/$(PROJECT_NAME)D$(EXE_SUFFIX)
TARGET_PRE_RELEASE   = $(DIR_OBJ_RELEASE)/$(PROJECT_NAME)$(EXE_SUFFIX)

TARGET_FINAL_DEBUG   = $(DIR_BIN)/$(PROJECT_NAME)D$(EXE_SUFFIX)
TARGET_FINAL_RELEASE = $(DIR_BIN)/$(PROJECT_NAME)$(EXE_SUFFIX)

FINAL_HEADERS = $(addprefix $(DIR_SRC)/,$(HEADERS))

OBJ_DEBUG   = $(addprefix $(DIR_OBJ_DEBUG)/,$(SOURCES:.cpp=.o))
OBJ_RELEASE = $(addprefix $(DIR_OBJ_RELEASE)/,$(SOURCES:.cpp=.o))

RESOBJ_DEBUG   = $(addprefix $(DIR_OBJ_DEBUG)/,$(RESOURCES:.rc=.coff))
RESOBJ_RELEASE = $(addprefix $(DIR_OBJ_RELEASE)/,$(RESOURCES:.rc=.coff))


#----------------------------------------------------------------------------
.PHONY: prepare debug d release r final upx all clean c mrproper

c: clean
d: debug
r: release

prepare: $(TARGET_DIRS)

debug: CFLAGS   = $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
debug: CXXFLAGS = $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)
debug: LIBS     = $(LIBS_COMMON) $(LIBS_DEBUG)
debug: LDFLAGS  = $(LDFLAGS_COMMON) $(LDFLAGS_DEBUG)
debug: prepare
debug: $(TARGET_FINAL_DEBUG)

release: CFLAGS   = $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
release: CXXFLAGS = $(CXXFLAGS_COMMON) $(CXXFLAGS_RELEASE)
release: LIBS     = $(LIBS_COMMON) $(LIBS_RELEASE)
release: LDFLAGS  = $(LDFLAGS_COMMON) $(LDFLAGS_RELEASE)
release: prepare
release: $(TARGET_FINAL_RELEASE)

upx:
	$(UPX) $(TARGET_FINAL_RELEASE)

final: clean release upx

all: debug release

clean:
	-$(RMDIR) $(subst /,\,$(DIR_BIN) $(DIR_OBJ))

mrproper: clean


#----------------------------------------------------------------------------
$(TARGET_DIRS):
	$(MKDIR) $(subst /,\,$@)

$(TARGET_FINAL_DEBUG): $(TARGET_PRE_DEBUG)
	$(COPY) $(subst /,\,$(TARGET_PRE_DEBUG) $(TARGET_FINAL_DEBUG))

$(TARGET_FINAL_RELEASE): $(TARGET_PRE_RELEASE)
	$(COPY) $(subst /,\,$(TARGET_PRE_RELEASE) $(TARGET_FINAL_RELEASE))


$(TARGET_PRE_DEBUG): $(RESOBJ_DEBUG) $(OBJ_DEBUG)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET_PRE_RELEASE): $(RESOBJ_RELEASE) $(OBJ_RELEASE)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


$(DIR_OBJ_DEBUG)/%.coff: $(DIR_SRC)/%.rc
	$(WINDRES) -o $@ -i $<

$(DIR_OBJ_RELEASE)/%.coff: $(DIR_SRC)/%.rc
	$(WINDRES) -o $@ -i $<


$(DIR_OBJ_DEBUG)/%.o: $(DIR_SRC)/%.cpp $(FINAL_HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS) $(CXXFLAGS)

$(DIR_OBJ_RELEASE)/%.o: $(DIR_SRC)/%.cpp $(FINAL_HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS) $(CXXFLAGS)
