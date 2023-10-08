#include "../Header/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "../Header/BaseData.h"
#include "../Header/ScreenRender.h"
#include "../Header/ExceptionHandler.h"

void test()
{
	system("hi");
	printf("hello\n");
}

/* Game System Func */
void initGame()
{
	initScreen();
}

void exitGame()
{
	exit(0);
}

/* Load DataFile Func */
void loadImage(char* dst, char* fname)
{
	
	FILE *fp;
	char rootPath[256];
	char* token, *buffer, *imagePath = rootPath;
	int i, j, x, y;
	
	buffer = (char*)calloc(screenInfo.width, sizeof(char));
	
	_getcwd(rootPath, 1000);
	sprintf(imagePath, "%s\\Image\\%s", rootPath, fname);
	
	fp = fopen(imagePath, "r");
	if (fp == NULL)
		throwFatalException(_FILE_NOT_FOUND_);
		
	buffer[0] = '\0';
	while (feof(fp) == 0) {
        fgets(buffer, screenInfo.width, fp);
        strcat(dst, buffer);
    }
    fclose(fp);
	free(buffer);
}
