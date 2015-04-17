#pragma once

#ifdef ADBOOKBL_EXPORTS
#define ADBOOKBL_API __declspec(dllexport)
#else
#define ADBOOKBL_API __declspec(dllimport)
#endif
