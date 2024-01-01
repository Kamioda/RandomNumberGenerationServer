#ifndef RGS_RANDOM_HPP
#define RGS_RANDOM_HPP
#include <random>

class Random {
private:
	static thread_local std::mt19937 mt;
public:
	static std::mt19937& GetEngine() { return mt; }
};
#endif
