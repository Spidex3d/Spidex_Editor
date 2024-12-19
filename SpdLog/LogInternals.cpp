#include "LogInternals.h"
#include <stdio.h>
#include <stdarg.h>

LogInternals* LogInternals::Instance()
{
	static LogInternals* log = new LogInternals;

	return log;
}

bool LogInternals::Initialize()
{
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto errorLogFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("SpdLog/logFile/error.log");
	errorLogFileSink->set_level(spdlog::level::warn);
	auto debugLogFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("SpdLog/logFile/debug.log");
	debugLogFileSink->set_level(spdlog::level::debug);


	SpidexLogger->sinks().push_back(consoleSink);
	SpidexLogger->sinks().push_back(errorLogFileSink);
	SpidexLogger->sinks().push_back(debugLogFileSink);

	SpidexLogger->set_level(spdlog::level::trace);
	SpidexLogger->set_pattern("%d.%m.%Y %T: [%n] :%^ %v%$");
	/*
	SpidexLogger->log(spdlog::level::trace, "TRACE");
	SpidexLogger->log(spdlog::level::debug, "DEBUG");
	SpidexLogger->log(spdlog::level::info, "INFO");
	SpidexLogger->log(spdlog::level::warn, "WARN");
	SpidexLogger->log(spdlog::level::err, "ERROR");
	SpidexLogger->log(spdlog::level::critical, "CRITICAL");
	*/
	return true;
}

void LogInternals::Trace(const char* Tmsg)
{
	SpidexLogger->log(spdlog::level::trace, "Trace : {0}", Tmsg);
}

void LogInternals::Debug(const char* Dmsg)
{
	SpidexLogger->log(spdlog::level::debug, "Debug : {0}", Dmsg);
}

void LogInternals::Info(const char* Imsg)
{
	SpidexLogger->log(spdlog::level::info, "Info : {0}", Imsg);
}

void LogInternals::Warn(const char* Wmsg)
{
	SpidexLogger->log(spdlog::level::warn, "Warn : {0}", Wmsg);
}

void LogInternals::Error(const char* Emsg)
{
	SpidexLogger->log(spdlog::level::err, "Error : {0}", Emsg);
}

void LogInternals::Critical(const char* Cmsg)
{
	SpidexLogger->log(spdlog::level::critical, "Critical : {0}", Cmsg);
}