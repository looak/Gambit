#pragma once
#ifdef GAMBIT_ENGINE_EXPORTS  
#define GAMBIT_API __declspec(dllexport)   
#else  
#define GAMBIT_API __declspec(dllimport)
#endif  

typedef unsigned char byte;
typedef unsigned __int64 u64;
typedef unsigned __int32 u32;

const u64 universe = 0xffffffffffffffffULL;
