#include "../Header/UtilData.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <string.h>

/* Equals like Sleep(int) or usleep(ms) */
void WaitForSeconds(float s)
{
	int ms = s * 1000;
	#ifdef _WIN32
	Sleep(ms);
	#else
  	usleep(ms); 
	#endif
}

/* Similar with strcat(char*, char*). But it repeats n times and returns dst's begin pos, or NULL. */
char* strrptcat(char* dst, char* src, int repeat)
{
	if (repeat < 1) return NULL;
	
	char* pDstEnd;
	char* pSrcEnd;
	int i, srcLen;
	
	pSrcEnd = src;
	while (pSrcEnd != '\0') pSrcEnd++;
	srcLen = pSrcEnd - src + 1;
	
	pDstEnd = dst;
	while (pDstEnd != '\0') pDstEnd++;
	
	for (i = 0; i < repeat; i++)
		memmove(pDstEnd + (i * srcLen), src, srcLen);
		
	return dst;
}
