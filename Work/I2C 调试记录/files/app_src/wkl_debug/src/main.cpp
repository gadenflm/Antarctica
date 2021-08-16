#include <iostream>
#include "wkl_debug.h"
#include "hdf_log.h"

#define HDF_LOG_TAG WKL_Debug_Main
using namespace std;

int main(int argc, char** argv) {

	/*if (argc == 0) {
		cout << "Wrong Arguments" << endl;
		return  WKL_DEBUG_WRONG;
	}

	if ( argv[0][0] == '-' ) {
		int temp_test = (argv[0][1] - 48) * 16 ^ 3 + (argv[0][2] - 48) * 16 ^ 2 + (argv[0][3] - 48) * 16 + argv[0][4];
		printf ("RegAddr = 0x%x\n", temp_test);
		//i2c_test();
		return  WKL_DEBUG_RIGHT;
	}*/

	unsigned int regAddr;
	scanf ("%x", &regAddr);
	i2c_test(regAddr);
	return  WKL_DEBUG_RIGHT;
}

