#include "SystemInfo.hpp"

#include "Szczur/Config.hpp"

#include <string>
#include <sstream>

#include <glad/glad.h>

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

namespace rat::system_info
{


#ifdef OS_WINDOWS
using SystemInfoData = POSVERSIONINFOEX;
#endif

std::string getOSName(SystemInfoData info);
std::string getOSVersion(SystemInfoData info);


// reads string from Windows' register
std::string GetRegString(HKEY hKeyType, const char* subKey, const char* valueName)
{
	HKEY hKey;
	std::string keyValue;

	// open subhkey
	auto result = RegOpenKeyA(hKeyType, subKey, &hKey);

	if (result == ERROR_SUCCESS)
	{
		char value[128] = { 0 };
		DWORD nameSize = sizeof(value);
		DWORD lpType = REG_SZ;

		// read key value
		result = RegQueryValueExA(hKey, valueName, 0, &lpType, (LPBYTE)value, &nameSize);

		// if found
		if (result == ERROR_SUCCESS)
		{
			keyValue = value;
		}
	}

	// close
	RegCloseKey(hKey);

	return keyValue;
}

ProcessorInfo GetProcessorInfo()
{
	// @todo: figure out how to get info about processor without assembly
	return ProcessorInfo();
}

MemoryInfo GetMemoryInfo()
{
	MemoryInfo memoryInfo;

	#ifdef OS_WINDOWS
	MEMORYSTATUS state = { 0 };
	state.dwLength = sizeof(state);
	GlobalMemoryStatus(&state);

	memoryInfo.PhysicalTotal = state.dwTotalPhys;
	memoryInfo.PhysicalAvailable = state.dwAvailPhys;

	#endif

	return memoryInfo;
}

GPUInfo GetGPUInfo()
{
	GPUInfo gpuInfo;

	// check if OpenGL is initialized
	if (glGetString)
	{
		gpuInfo.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		gpuInfo.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}
	
	return gpuInfo;
}

OSInfo GetOSInfo()
{
	OSInfo osInfo;

	#ifdef OS_WINDOWS
	osInfo.Name = getOSName(nullptr);
	osInfo.Version = getOSVersion(nullptr);
	#endif

	return osInfo;
}


std::string getOSName(SystemInfoData info)
{
	std::string versionName = "Unknown";

	#ifdef OS_WINDOWS
	// read from Windows' explorer Windows' version name
	auto result = GetRegString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");

	if (!result.empty())
	{
		versionName = result;
	}
	#endif


	return versionName;
}

std::string getOSVersion(SystemInfoData info)
{
	std::string version;
	
	#ifdef OS_WINDOWS
	// read from Windows' explorer Windows' version
	auto result = GetRegString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentVersion");

	if (!result.empty())
	{
		version = result;
	}
	#endif

	return version;
}

}
