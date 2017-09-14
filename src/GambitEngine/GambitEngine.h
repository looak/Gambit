#pragma once

#include <stdint.h>

#ifdef GAMBIT_ENGINE_EXPORTS  
#define GAMBIT_API __declspec(dllexport)   
#else  
#define GAMBIT_API __declspec(dllimport)   
#endif  


namespace GambitEngine
{
	// Universal Chess Interface
	// http://wbec-ridderkerk.nl/html/UCIProtocol.html
	class UCI
	{
	public:
		static GAMBIT_API bool HelloWorld();
	};

	class FEN
	{
	public:
		static GAMBIT_API bool InputFen(char* fen, uint8_t length);
	};
}