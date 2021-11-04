#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
	HANDLE test_hdl;

	try 
	{
		test_hdl = CreateFile( TEXT("COM6"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		CloseHandle(test_hdl);
	} 
	catch(...) 
	{
		cout << "error!" << endl;
	}


	char ch;
	while ((ch = getchar())!='q') ;
	return 0;
}