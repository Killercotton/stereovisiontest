#include <thread>
#include <iostream>

void printBla() {
	std::cout << "Bla" << std::endl;
}

int main() {
	std::thread first (printBla);
	first.join();
	return 0;
}
