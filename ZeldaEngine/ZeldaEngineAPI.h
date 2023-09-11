#pragma once

#ifdef ZELDAENGINE_EXPORTS
#define ZELDAENGINE_API __declspec(dllexport)
#else
#define ZELDAENGINE_API __declspec(dllimport)
#endif

#pragma warning(disable:4251)