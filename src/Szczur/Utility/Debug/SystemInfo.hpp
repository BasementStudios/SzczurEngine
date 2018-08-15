#pragma once

#include <string>

namespace rat::system_info
{

struct OSInfo
{
	std::string Name;
	std::string Version;
};

struct ProcessorInfo
{

};

struct MemoryInfo
{
	size_t PhysicalTotal;
	size_t PhysicalAvailable;
};

struct GPUInfo
{
	std::string Vendor;
	std::string Renderer;
};

OSInfo GetOSInfo();

ProcessorInfo GetProcessorInfo();

MemoryInfo GetMemoryInfo();

GPUInfo GetGPUInfo();

}
