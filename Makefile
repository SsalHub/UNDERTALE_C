# Project: Undertale

CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
OBJ      = undertale/undertale.o undertale/PlayGame.o undertale/MainMenu.o utils/BaseData.o utils/ScreenRender.o utils/UtilData.o utils/ExceptionHandler.o
LINKOBJ  = undertale/undertale.o undertale/PlayGame.o undertale/MainMenu.o utils/BaseData.o utils/ScreenRender.o utils/UtilData.o utils/ExceptionHandler.o
LIBS     = -L"C:/MinGW/lib" -static-libgcc -lkernel32
INCS     = -I"C:/MinGW/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include" -I"./undertale" -I"./utils"
CXXINCS  = -I"C:/MinGW/include" -I"C:/MinGW/mingw32/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include" -I"C:/MinGW/lib/gcc/mingw32/6.3.0/include/c++"
BIN      = undertale.exe
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS)

.PHONY: all all-before all-after setup clean clean-custom

setup: all-before $(BIN) all-after

clean: clean-custom
	del /s *.o
	$(RM_OBJ)
	$(RM_BIN)

all: clean setup 

undertale/undertale.o: undertale/undertale.c
	$(CC) -c undertale/undertale.c -o undertale/undertale.o $(CFLAGS)

undertale/PlayGame.o: undertale/PlayGame.c
	$(CC) -c undertale/PlayGame.c -o undertale/PlayGame.o $(CFLAGS)

undertale/MainMenu.o: undertale/MainMenu.c
	$(CC) -c undertale/MainMenu.c -o undertale/MainMenu.o $(CFLAGS)

utils/BaseData.o: utils/BaseData.c
	$(CC) -c utils/BaseData.c -o utils/BaseData.o $(CFLAGS)

utils/ScreenRender.o: utils/ScreenRender.c
	$(CC) -c utils/ScreenRender.c -o utils/ScreenRender.o $(CFLAGS)

utils/UtilData.o: utils/UtilData.c
	$(CC) -c utils/UtilData.c -o utils/UtilData.o $(CFLAGS)

utils/ExceptionHandler.o: utils/ExceptionHandler.c
	$(CC) -c utils/ExceptionHandler.c -o utils/ExceptionHandler.o $(CFLAGS)
	
# Automatically Run
$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)