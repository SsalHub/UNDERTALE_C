# Project: Undertale

CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
OBJ      = main.o Source/BaseData.o Source/ScreenRender.o Source/PlayGame.o Source/UtilData.o Source/MainMenu.o Source/ExceptionHandler.o
LINKOBJ  = main.o Source/BaseData.o Source/ScreenRender.o Source/PlayGame.o Source/UtilData.o Source/MainMenu.o Source/ExceptionHandler.o
LIBS     = -L"C:/MinGW/lib" -static-libgcc -lkernel32
INCS     = -I"C:/MinGW/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include"
CXXINCS  = -I"C:/MinGW/include" -I"C:/MinGW/mingw32/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include/c++"
BIN      = Undertale.exe
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS)

# ifdef ComSpec	# True, PowerShell
# # @powershell -File "$(ROOT)/Config/ShellConfig.ps1"
# 	RM_OBJ := Remove-Item -Force -Recurse -Filter "*.o"
# 	ifneq ($(wildcard $(BIN)),)
# 		RM_BIN := Remove-Item -Force $(BIN)
# 	endif
# else			# False, CommandPrompt
# @call $(ROOT)/Config/PromptConfig.ps1
# RM_OBJ := for /r %i in (*.o) do del "%i"
# ifneq ($(wildcard $(BIN)),)
# 	RM_BIN := del  $(BIN)
# endif
# endif
	
# OBJ := $(addprefix $(ROOT)/,$(OBJ))
# LINKOBJ := $(addprefix $(ROOT)/,$(LINKOBJ))

.PHONY: all all-before all-after setup clean clean-custom

setup: all-before $(BIN) all-after

clean: clean-custom
	del /s *.o
	$(RM_OBJ)
	$(RM_BIN)

all: clean setup 

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

Source/BaseData.o: Source/BaseData.c
	$(CC) -c Source/BaseData.c -o Source/BaseData.o $(CFLAGS)

Source/ScreenRender.o: Source/ScreenRender.c
	$(CC) -c Source/ScreenRender.c -o Source/ScreenRender.o $(CFLAGS)

Source/PlayGame.o: Source/PlayGame.c
	$(CC) -c Source/PlayGame.c -o Source/PlayGame.o $(CFLAGS)

Source/UtilData.o: Source/UtilData.c
	$(CC) -c Source/UtilData.c -o Source/UtilData.o $(CFLAGS)

Source/MainMenu.o: Source/MainMenu.c
	$(CC) -c Source/MainMenu.c -o Source/MainMenu.o $(CFLAGS)

Source/ExceptionHandler.o: Source/ExceptionHandler.c
	$(CC) -c Source/ExceptionHandler.c -o Source/ExceptionHandler.o $(CFLAGS)
	
# Automatically Run
$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)