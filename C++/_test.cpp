/*test.h*/

int GlobalVariable = 1	;

/*test.c*/
#include <stdc++.h>

#include "test.h"

extern int GlobalVariable;

int main()
{
	std::cout << GlobalVariable << std::endl;
}

