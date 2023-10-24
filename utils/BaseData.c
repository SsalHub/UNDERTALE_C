#include <BaseData.h>

/* Test Func */

void test()
{
	system("hi");
	printf("hello\n");
}

/* Game System Func */

void initGame()
{
	initScreen();
	MoveWindow(_CURRENT_SCREEN_, 0, 0, screenInfo.width, screenInfo.height, true);
}

void mainMenu()
{
	loadMainMenu();
}

void exitGame()
{
	exit(0);
}

/* Load DataFile Func */
/* Returns Image's Pointer - you MUST free() it */
char* loadImage(char* fname)
{
	FILE *fp;
	char rootPath[512];
	char* token, *buffer, *imagePath = rootPath;
	long bufferLen;
	int i, j, x, y;
	
	/* Search File Path */
	_getcwd(rootPath, 1000);
	sprintf(imagePath, "%s\\images\\%s", rootPath, fname);
	
	/* Load File */
	fp = fopen(imagePath, "r");
	if (fp == NULL)
		throwFatalException(_FILE_NOT_FOUND_);
		
    fseek(fp, 0, SEEK_END);
    bufferLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
	bufferLen *= 4; 	// Because unicode is over 10,000 numbebr. So it expressed by 4 bin, life { 0010 1000 1111 0110 }. 

	buffer = (char*)malloc(bufferLen + 1);
		
	buffer[0] = '\0';
	// while (feof(fp) == 0) {
    //     fgets(buffer, bufferLen, fp);
    // }
	fread(buffer, 1, bufferLen, fp); // Read the whole file at once
	buffer[bufferLen] = '\0'; // Null-terminate the string

    fclose(fp);
	//free(buffer);
	return buffer;
}
