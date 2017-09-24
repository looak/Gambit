#pragma once
#include "typedef.h"


namespace GambitEngine
{
namespace BitMath
{
	static void abs(int& val)
	{
		int temp = val >> 31;     // make a mask of the sign bit
		val ^= temp;                   // toggle the bits if value is negative
		val += temp & 1;               // add one if value was negative
	}

}
}

