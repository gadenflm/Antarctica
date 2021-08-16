#include "ServerSDK_Func.h"
#include "ServerManager.h"

#include <stdio.h>

int main(int argc, const char** argv) {
	ServerInit();
	show_info();
	char ch;
	while ( (ch = getchar()) ) {
		switch (ch) {
		case '1':
			ServerTest();
			break;

		case 'h':
		case 'H':
			show_info();
			break;

		case 'o':
		case 'O':
			// Cfg_UI(); TODO
			show_info();
			break;

		case 'q':
		case 'Q':
			goto EXIT;
		default:
			break;
		}
		fflush(stdin);
	}

EXIT:
	printf ("Bye~~, Have a beautiful time!\n");
	return WKL_SUCC;
}