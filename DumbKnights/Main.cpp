#include <iostream>

#include <Gaemi.h>
#include <Log.h>


int main() {
	LOG(engine::LogLevel::Trace) << "Gaemi is ready";
	LOG(engine::LogLevel::Debug) << "Gaemi is ready";
	LOG(engine::LogLevel::Info) << "Gaemi is ready";
	LOG(engine::LogLevel::Warning) << "Gaemi is ready";
	LOG(engine::LogLevel::Error) << "Gaemi is ready";
	LOG(engine::LogLevel::Fatal) << "Gaemi is ready";

	LOG(engine::LogLevel::Trace) << "Hello World";
	return 0;
}
