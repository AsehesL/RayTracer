#pragma once

#include <random>

namespace Random
{ 
	thread_local std::mt19937_64 randomDevice(std::random_device{}());
}