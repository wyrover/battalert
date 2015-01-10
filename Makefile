#****************************************************************************
#
# BattAlert
# A minimalistic battery monitor for Win32.
#
# Copyright (C) 2011-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions :
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not
#    be misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#
#
# Author     : Jean-Charles Lefebvre
# Created On : 2013-01-09 15:30:56
#
# $Id$
#
#****************************************************************************


#----------------------------------------------------------------------------
PROJECT_NAME = BattAlert

DIR_ROOT        = .
DIR_BIN         = $(DIR_ROOT)/bin
DIR_OBJ         = $(DIR_ROOT)/obj
DIR_OBJ_DEBUG   = $(DIR_ROOT)/obj/debug
DIR_OBJ_RELEASE = $(DIR_ROOT)/obj/release
DIR_TOOLS       = $(DIR_ROOT)/tools
DIR_SRC         = $(DIR_ROOT)/src

# CAUTION: files in this list must be relative to DIR_SRC
HEADERS = \
  stable.h \
  resource.h \
  App.h \
  App.inl.h \
  BattIcon.h \
  BattIcon.inl.h \
  Config.h \
  Config.inl.h \
  Logger.h \
  Logger.inl.h \
  StringA.h \
  StringA.inl.h \
  version.h \
  Wnd.h \
  Wnd.inl.h \
  WndConfig.h \
  WndConfig.inl.h \
  WndMain.h \
  WndMain.inl.h

# CAUTION: files in this list must be relative to DIR_SRC
SOURCES = \
  main.cpp \
  App.cpp \
  BattIcon.cpp \
  Config.cpp \
  Logger.cpp \
  StringA.cpp \
  Wnd.cpp \
  WndConfig.cpp \
  WndMain.cpp

# CAUTION: files in this list must be relative to DIR_SRC
RESOURCES = BattAlert.rc


#----------------------------------------------------------------------------
CC      = gcc
WINDRES = windres
COPY    = copy /y
MKDIR   = mkdir
RMDIR   = rmdir /s /q
STRIP   = strip --strip-all
UPX     = upx --best
PVER    = $(DIR_TOOLS)/pver.cmd

EXE_SUFFIX=.exe

CFLAGS_COMMON  = -pipe
CFLAGS_COMMON += -Wall -Werror -Wno-deprecated -Wno-unused-value -Wno-char-subscripts
CFLAGS_DEBUG   = -g3 -D_DEBUG
CFLAGS_RELEASE = -O2 -s -DNDEBUG

CXXFLAGS_COMMON  = -fno-rtti
CXXFLAGS_DEBUG   =
CXXFLAGS_RELEASE =

LIBS_COMMON  = -lstdc++ -lcomctl32
LIBS_DEBUG   =
LIBS_RELEASE =

LDFLAGS_COMMON  = -mwindows -static
LDFLAGS_DEBUG   =
LDFLAGS_RELEASE =

PVERFLAGS_COMMON  = -C "$(DIR_ROOT)"
PVERFLAGS_COMMON += -D "PROJECT_NAME=%!$(PROJECT_NAME)"
PVERFLAGS_COMMON += -D "VERSION=head VERSION|append .|var REVISION"
PVERFLAGS_COMMON += -D "VERSION_WIN=var VERSION|winver"
PVERFLAGS_DEBUG   = -D _DEBUG
PVERFLAGS_RELEASE = -D NDEBUG

RCFLAGS_COMMON  =
RCFLAGS_DEBUG   = -D _DEBUG
RCFLAGS_RELEASE = -D NDEBUG


#----------------------------------------------------------------------------
TARGET_DIRS = $(DIR_BIN) $(DIR_OBJ) $(DIR_OBJ_DEBUG) $(DIR_OBJ_RELEASE)

TARGET_PRE_DEBUG     = $(DIR_OBJ_DEBUG)/$(PROJECT_NAME)D$(EXE_SUFFIX)
TARGET_PRE_RELEASE   = $(DIR_OBJ_RELEASE)/$(PROJECT_NAME)$(EXE_SUFFIX)

TARGET_FINAL_DEBUG   = $(DIR_BIN)/$(PROJECT_NAME)D$(EXE_SUFFIX)
TARGET_FINAL_RELEASE = $(DIR_BIN)/$(PROJECT_NAME)$(EXE_SUFFIX)

PVERFLAGS_DEBUG   += -D "EXE_NAME=%!$(notdir $(TARGET_FINAL_DEBUG))"
PVERFLAGS_RELEASE += -D "EXE_NAME=%!$(notdir $(TARGET_FINAL_RELEASE))"

FINAL_HEADERS = $(addprefix $(DIR_SRC)/,$(HEADERS))

OBJ_DEBUG   = $(addprefix $(DIR_OBJ_DEBUG)/,$(SOURCES:.cpp=.o))
OBJ_RELEASE = $(addprefix $(DIR_OBJ_RELEASE)/,$(SOURCES:.cpp=.o))

RESOBJ_DEBUG   = $(addprefix $(DIR_OBJ_DEBUG)/,$(RESOURCES:.rc=.coff))
RESOBJ_RELEASE = $(addprefix $(DIR_OBJ_RELEASE)/,$(RESOURCES:.rc=.coff))


#----------------------------------------------------------------------------
.PHONY: prepare debug d release r final upx all clean c distclean mrproper
.FORCE:
#.SECONDARY:

c: clean
d: debug
r: release

prepare: $(TARGET_DIRS)

debug: CFLAGS    = $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
debug: CXXFLAGS  = $(CXXFLAGS_COMMON) $(CXXFLAGS_DEBUG)
debug: LIBS      = $(LIBS_COMMON) $(LIBS_DEBUG)
debug: LDFLAGS   = $(LDFLAGS_COMMON) $(LDFLAGS_DEBUG)
debug: PVERFLAGS = $(PVERFLAGS_COMMON) $(PVERFLAGS_DEBUG)
debug: RCFLAGS   = $(RCFLAGS_COMMON) $(RCFLAGS_DEBUG)
debug: prepare
debug: $(TARGET_FINAL_DEBUG)

release: CFLAGS    = $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
release: CXXFLAGS  = $(CXXFLAGS_COMMON) $(CXXFLAGS_RELEASE)
release: LIBS      = $(LIBS_COMMON) $(LIBS_RELEASE)
release: LDFLAGS   = $(LDFLAGS_COMMON) $(LDFLAGS_RELEASE)
release: PVERFLAGS = $(PVERFLAGS_COMMON) $(PVERFLAGS_RELEASE)
release: RCFLAGS   = $(RCFLAGS_COMMON) $(RCFLAGS_RELEASE)
release: prepare
release: $(TARGET_FINAL_RELEASE)

upx:
	$(UPX) $(TARGET_FINAL_RELEASE)

final: clean release upx

all: debug release

clean:
	-$(RMDIR) $(subst /,\,$(DIR_BIN) $(DIR_OBJ)) 2>nul

distclean: clean
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


$(DIR_OBJ_DEBUG)/%.coff: $(DIR_SRC)/%.rc $(DIR_SRC)/version.h
	$(WINDRES) $(RCFLAGS) -o $@ -i $<

$(DIR_OBJ_RELEASE)/%.coff: $(DIR_SRC)/%.rc $(DIR_SRC)/version.h
	$(WINDRES) $(RCFLAGS) -o $@ -i $<


$(DIR_SRC)/%.h: $(DIR_SRC)/%.h.in
	$(subst /,\,$(PVER)) $(PVERFLAGS) "$<;$@"


$(DIR_OBJ_DEBUG)/%.o: $(DIR_SRC)/%.cpp $(FINAL_HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS) $(CXXFLAGS)

$(DIR_OBJ_RELEASE)/%.o: $(DIR_SRC)/%.cpp $(FINAL_HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS) $(CXXFLAGS)
