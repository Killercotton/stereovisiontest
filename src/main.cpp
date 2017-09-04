#include <thread>

void foo() {
}

int main() {
	std::thread first (foo);
	first.join();
	return 0;
}
