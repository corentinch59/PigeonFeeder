#include <fmt/core.h>
#include <fmt/color.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

int main()
{
	const char* devicelist = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	std::vector<std::string> devices;

	for (;;)
	{
		std::string device = devicelist;
		if (device.empty())
			break;

		devicelist += device.size() + 1;

		devices.push_back(std::move(device));
	}

	for (const std::string device : devices)
	{
		fmt::println("Device : {}", device);
	}

	ALCdevice* device = alcOpenDevice(nullptr);

	ALCcontext* context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	ALuint buffer;
	alGenBuffers(1, &buffer);


	return 0;
}