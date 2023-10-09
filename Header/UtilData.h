#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

typedef enum ConsoleColor
{
	_BLACK_ 		= 0,
	_BLUE_			= 1,
	_DARK_GREEN_	= 2,
	_SKYBLUE_		= 3,
	_RED_			= 4,
	_DARK_PURPLE_	= 5,
	_YELLOW_		= 6,
	_BRIGHT_GRAY_	= 7,
	_GRAY_			= 8,
	_OCEAN_BLUE_	= 9,
	_GREEN_			= 10,
	_SKY_			= 11,
	_HOTPINK_		= 12,
	_PURPLE_		= 13,
	_LIGHT_YELLOW_	= 14,
	_WHITE_			= 15,
} ConsoleColor;

typedef enum InputType
{
	_SPACE_ 			= 32,
	_ESCAPE_ 			= 27,
	_CARRIGE_RETURN_ 	= 13,
	
	_LEFT_ 				= 75,
	_RIGHT_ 			= 77,
	_UP_ 				= 72,
	_DOWN_ 				= 80,
	
	_UPPER_A_			= 65,
	_UPPER_B_				, 
	_UPPER_C_				, 
	_UPPER_D_				, 
	_UPPER_E_				, 
	_UPPER_F_				, 
	_UPPER_G_				, 
	_UPPER_H_				, 
	_UPPER_I_				, 
	_UPPER_J_				, 
	_UPPER_K_				, 
	_UPPER_L_				, 
	_UPPER_M_				, 
	_UPPER_N_				, 
	_UPPER_O_				, 
	_UPPER_P_				, 
	_UPPER_Q_				, 
	_UPPER_R_				, 
	_UPPER_S_				, 
	_UPPER_T_				, 
	_UPPER_U_				, 
	_UPPER_V_				, 
	_UPPER_W_				, 
	_UPPER_X_				, 
	_UPPER_Y_				, 
	_UPPER_Z_				, 
	
	_LOWER_A_			= 97,
	_LOWER_B_ 				,
	_LOWER_C_ 				,
	_LOWER_D_ 				,
	_LOWER_E_ 				,
	_LOWER_F_ 				,
	_LOWER_G_ 				,
	_LOWER_H_ 				,
	_LOWER_I_ 				,
	_LOWER_J_ 				,
	_LOWER_K_ 				,
	_LOWER_L_ 				,
	_LOWER_M_ 				,
	_LOWER_N_ 				,
	_LOWER_O_ 				,
	_LOWER_P_ 				,
	_LOWER_Q_ 				,
	_LOWER_R_ 				,
	_LOWER_S_ 				,
	_LOWER_T_ 				,
	_LOWER_U_ 				,
	_LOWER_V_ 				,
	_LOWER_W_ 				,
	_LOWER_X_ 				,
	_LOWER_Y_ 				,
	_LOWER_Z_ 				,
	
} InputType;

/* Equals like Sleep(int) or usleep(ms) */
void WaitForSeconds(float s);
