#pragma once

#if defined PC_WIDGETS_LIB
#define PC_WIDGETS_EXPORT __declspec(dllexport)
#else
#define PC_WIDGETS_EXPORT __declspec(dllimport)
#endif