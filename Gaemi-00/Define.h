#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include <string>
#include <iostream>

typedef std::string str;

// Dll export
#ifdef GEXPORT
	// Exports
	#ifdef _MSC_VER
		#define GAPI __declspec(dllexport)
	#else
		#define GAPI __attribute__((visibility("default")))
	#endif
#else
// Imports
#ifdef _MSC_VER
		#define GAPI __declspec(dllimport)
	#else
		#define GAPI
	#endif
#endif

#endif

GAPI void display(const str& message);