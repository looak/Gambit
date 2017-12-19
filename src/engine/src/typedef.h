#pragma once
#include <stdint.h>

#ifdef GAMBIT_ENGINE_EXPORTS  
#define GAMBIT_API __declspec(dllexport)   
#else  
#define GAMBIT_API //__declspec(dllimport)
#endif  

typedef unsigned char byte;
typedef unsigned long long u64;
typedef unsigned long u32;

const u64 universe = 0xffffffffffffffffULL;
