#pragma once
#ifndef ASSERTS_H
#define ASSERTS_H

#ifdef GASSERTIONS_ENABLED

	namespace engine {
		GAPI void reportAssertionFailure(const str& expression, const str& message, const char* codeFile, i32 codeLine);
	}

	#ifdef  _MSC_VER
		#include <intrin.h>
		#define debugBreak() __debugbreak()
	#else
		#define debugBreak() __builtin_trap()
	#endif //  _MSC_VER

	#define GASSERT(expr)															\
	{																				\
		if(expr) {																	\
		}																			\
		else {																		\
			engine::reportAssertionFailure(#expr, "", __FILE__, __LINE__);			\
				debugBreak();														\
		}																			\
	}

	#define GASSERT_MSG(expr, message)												\
	{																				\
		if(expr) {																	\
		}																			\
		else {																		\
			engine::reportAssertionFailure(#expr, message, __FILE__, __LINE__);		\
				debugBreak();														\
		}																			\
	}

	#ifdef GDEBUG
		#define GASSERT_DEBUG(expr, message)										\
		{																			\
			if(expr) {																\
			}																		\
			else {																	\
				engine::reportAssertionFailure(#expr, message, __FILE__, __LINE__);	\
					debugBreak();													\
			}																		\
		}
	#else
		#define GASSERT_DEBUG(expr, message)	// Nothing when not in debug mode
	#endif // GDEBUG

#else

	#define GASSERT(expr)
	#define GASSERT_MSG(expr, message)
	#define GASSERT_DEBUG(expr, message)

#endif
#endif
