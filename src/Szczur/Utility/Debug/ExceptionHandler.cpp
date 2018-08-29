#include "ExceptionHandler.hpp"

#include <fstream>
#include <string>
#include <ctime>
#include <experimental/filesystem>
#include <sstream>
#include <iomanip>
#include <chrono>

#include <boost/stacktrace.hpp>

#include "Szczur/Config.hpp"
#include "Szczur/Utility/Logger.hpp"

#include "SystemInfo.hpp"


#ifdef OS_WINDOWS
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace rat::exc
{

#ifdef OS_WINDOWS

std::string getExceptionCodeName(DWORD exceptionCode);
void getInfoAboutAddress(void* address, uintptr_t& addressWithoutBase, std::string& moduleName);

std::string getHeader(_EXCEPTION_POINTERS* ExceptionInfo)
{
	std::stringstream result;

	// get current time 
	std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// get system info
	auto os = system_info::GetOSInfo();
	auto gpu = system_info::GetGPUInfo();
	auto mem = system_info::GetMemoryInfo();

	result << "Crash log create on: " << std::put_time(std::localtime(&now_c), "%F %T") << "\n";
	result << "OS: " << os.Name << " (version " << os.Version << ")\n";
	result << "CPU: One day there will be more info\n";
	result << "MEM: Total: " << mem.PhysicalTotal / 1024 / 1024 << "MB Available: " << mem.PhysicalAvailable / 1024 / 1024 << "MB\n";
	result << "GPU: " << gpu.Vendor << " " << gpu.Renderer << "\n";

	result << "\n" << std::hex;

	char buffer[512] = { 0 };

	sprintf(buffer,
			"Base address: %p\n"
			"Exception address: %p\n"
			"Exception code: %lX %s\n",
			static_cast<void*>(GetModuleHandle(nullptr)),
			ExceptionInfo->ExceptionRecord->ExceptionAddress,
			ExceptionInfo->ExceptionRecord->ExceptionCode,
			getExceptionCodeName(ExceptionInfo->ExceptionRecord->ExceptionCode).c_str());

	result << buffer;

	uintptr_t addressWithoutBase = 0;
	std::string moduleName;

	// get exe path and exc addr without base
	getInfoAboutAddress(ExceptionInfo->ExceptionRecord->ExceptionAddress, addressWithoutBase, moduleName);

	result << "Module file name: " << moduleName << "\n";
	result << "Address without base: 0x" << std::setfill('0') << std::setw(16) << std::hex << std::uppercase << addressWithoutBase << "\n";

	return result.str();
}

std::string getRegisters(_EXCEPTION_POINTERS* ExceptionInfo)
{
	std::stringstream result;

	result << "Registers:\n";

	char registers[512] = { 0 };

	// print every register
	sprintf(registers,
			"Rax: %016llX \nRcx: %016llX \nRdx: %016llX \nRbx: %016llX \nRsp: %016llX \nRbp: %016llX \nRsi: %016llX \nRdi: %016llX \nR8: %016llX \nR9: %016llX \nR10: %016llX \nR11: %016llX \nR12: %016llX \nR13: %016llX \nR14: %016llX \nR15: %016llX \n",
			ExceptionInfo->ContextRecord->Rax,
			ExceptionInfo->ContextRecord->Rcx,
			ExceptionInfo->ContextRecord->Rdx,
			ExceptionInfo->ContextRecord->Rbx,
			ExceptionInfo->ContextRecord->Rsp,
			ExceptionInfo->ContextRecord->Rbp,
			ExceptionInfo->ContextRecord->Rsi,
			ExceptionInfo->ContextRecord->Rdi,
			ExceptionInfo->ContextRecord->R8,
			ExceptionInfo->ContextRecord->R9,
			ExceptionInfo->ContextRecord->R10,
			ExceptionInfo->ContextRecord->R11,
			ExceptionInfo->ContextRecord->R12,
			ExceptionInfo->ContextRecord->R13,
			ExceptionInfo->ContextRecord->R14,
			ExceptionInfo->ContextRecord->R15);

	result << registers;

	return result.str();
}

std::string getCallStack(EXCEPTION_POINTERS* exceptionPointers)
{
	std::stringstream str;

	str << "Call stack:\n";
	str << boost::stacktrace::stacktrace(-1, 128);

	return str.str();
}

std::string getStack(EXCEPTION_POINTERS* exceptionPointers)
{
	std::stringstream result;

	result << "Stack:\n";

	unsigned char* currentByte = (unsigned char*)(exceptionPointers->ContextRecord->Rsp - 128);

	for (int i = 0; i < 20; ++i)
	{
		if (i == 4)
			result << ">";
		else
			result << " ";

		for (int j = 32; j > 0; --j)
		{
			if (IsBadReadPtr(currentByte, 1))
				result << " --";
			else
			{
				char byte[16] = { 0 };
				sprintf(byte, " %02X", *currentByte);
				result << byte;
			}

			++currentByte;
		}
		
		result << "\n";
	}

	return result.str();
}

std::string getModuleList()
{
	std::stringstream result;

	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return result.str();
	}

	MODULEENTRY32 moduleEntry;

	if (!Module32First(hSnapshot, &moduleEntry))
	{
		CloseHandle(hSnapshot);
		return result.str();
	}

	result << std::hex;
	result << "Modules:\n";

	do
	{
		char moduleInfo[MAX_PATH] = { 0 };

		sprintf(moduleInfo, "%p %08lX %s\n", 
				static_cast<void*>(moduleEntry.hModule),
				moduleEntry.modBaseSize,
				moduleEntry.szExePath);

		result << moduleInfo;
	} while (Module32Next(hSnapshot, &moduleEntry));

	return result.str();
}

std::string getExceptionCodeName(DWORD exceptionCode)
{
	switch (exceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			return "ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT:
			return "BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:
			return "FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:
			return "FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:
			return "FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:
			return "FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:
			return "FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:
			return "IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:
			return "INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:
			return "INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION:
			return "PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP:
			return "SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW:
			return "STACK_OVERFLOW";
		default:
			return "UNKNOWN";
	}
}

void getInfoAboutAddress(void* address, uintptr_t& addressWithoutBase, std::string& moduleName)
{
	MEMORY_BASIC_INFORMATION buffer;

	// get info about address
	int result = VirtualQuery(address, &buffer, sizeof(MEMORY_BASIC_INFORMATION));

	if (!result)
		return;

	char module[MAX_PATH] = { 0 };

	// gets program path where this addr is
	result = GetModuleFileNameA((HMODULE)buffer.AllocationBase, module, MAX_PATH);

	if (!result)
		return;

	moduleName = module;

	addressWithoutBase = reinterpret_cast<uintptr_t>(address) - reinterpret_cast<uintptr_t>(buffer.AllocationBase);
}

LONG unhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	namespace fs = std::experimental::filesystem;

	std::string crashlogDirectory = "Crashes";
	std::string crashlogPath = crashlogDirectory + "/crash_" + std::to_string(std::time(nullptr)) + ".txt";

	LOG_ERROR("Unhandled exception happens! Saving crashlog to ", crashlogPath);

	// check if directory exists
	if (!fs::exists(crashlogDirectory))
	{
		// if no then create it
		if (!fs::create_directory(crashlogDirectory))
		{
			// if cannot create directory then stop exception handling
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}

	std::ofstream crashlogFile(crashlogPath);

	// if can write to file
	if (crashlogFile.good())
	{
		crashlogFile << getHeader(ExceptionInfo) << "\n";
		crashlogFile << getRegisters(ExceptionInfo) << "\n";
		crashlogFile << getCallStack(ExceptionInfo) << "\n";
		crashlogFile << getStack(ExceptionInfo) << "\n";
		crashlogFile << getModuleList() << "\n";
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif

void init()
{
	#ifdef OS_WINDOWS
	SetUnhandledExceptionFilter(unhandledExceptionFilter);
	#endif
}

}
