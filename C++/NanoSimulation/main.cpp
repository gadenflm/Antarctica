#include "coeffi.h"
#include "simulation.h"
#include <iostream>

using namespace std;

int main()
{

	FSimulation *Simula = new FSimulation();

	delete Simula;
	// End:
	cout << "Type q and press Enter to finish!" << endl;
	char StopKey;
	while ((StopKey = getchar())!='q');

	return 0;
}
