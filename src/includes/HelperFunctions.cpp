#include "Common.h"


int GetHzForStringNumber(int stringNumber){

	switch (stringNumber)
	{
		case 0: return 82; 	//E
		case 1: return 110; //A
		case 2: return 146;	//D
		case 3: return 197; //G
		case 4: return 248; //B
		case 5: return 330;	//e
		default: return 0;
	}
	
}