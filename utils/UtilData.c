#include <UtilData.h>

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
