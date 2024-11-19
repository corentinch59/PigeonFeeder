#include <fmt/core.h>
#include <fmt/color.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <thread>
#include <iostream>
#include <array>
#include <mutex>

void func(unsigned int i)
{
	std::cout << "Hello from thread #" << i << "\n";
}

int main()
{
	std::array<std::thread, 10> threads;

	unsigned int counter = 0;
	for (std::thread& t : threads)
	{
		t = std::thread(func, counter++);
	}

	std::cout << "Hello from main" << "\n";

	for (std::thread& t : threads)
	{
		t.join();
	}

	return 0;
}