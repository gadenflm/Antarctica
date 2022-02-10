echo "******************************* Compile Begin *******************************"
g++ -std=c++11 -Iinclude -Wall \
	main.cpp  simulation.cpp\
	-o bin/simulation.exe
echo "******************************* Compile End *******************************"
echo " "
echo "Press anykey to quit."
read