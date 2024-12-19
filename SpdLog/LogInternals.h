#pragma once
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include <spdlog\sinks\basic_file_sink.h>
class LogInternals
{
public:

	static LogInternals* Instance();

	std::shared_ptr<spdlog::logger> SpidexLogger = std::make_shared<spdlog::logger>("SpidexLogger");

	bool Initialize();

	void Trace(const char* Tmsg);
	void Debug(const char* Dmsg);
	void Info(const char* Imsg);
	void Warn(const char* Wmsg);
	void Error(const char* Emsg);
	void Critical(const char* Cmsg);


};

