#include "../Header/UtilData.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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
