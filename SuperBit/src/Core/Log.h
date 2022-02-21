#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace SuperBit {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

// Core log macros
#if 1
#define SB_INFO(...)     ::SuperBit::Log::GetLogger()->info(__VA_ARGS__)
#define SB_TRACE(...)    ::SuperBit::Log::GetLogger()->trace(__VA_ARGS__)
#define SB_WARN(...)     ::SuperBit::Log::GetLogger()->warn(__VA_ARGS__)
#define SB_ERROR(...)    ::SuperBit::Log::GetLogger()->error(__VA_ARGS__)
#define SB_FATAL(...)    ::SuperBit::Log::GetLogger()->fatal(__VA_ARGS__)

#else
#define SB_INFO(...)     
#define SB_TRACE(...)    
#define SB_WARN(...)     
#define SB_ERROR(...)    
#define SB_FATAL(...)    
#endif

