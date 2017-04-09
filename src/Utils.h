#ifndef UTILITY_H
#define UTILITY_H

#include <random>
#include <functional>
#include <QString>

//Functor object
struct randomNumberGenerator
{
    randomNumberGenerator(int min, int max) : distribution(min, max), generator(std::random_device()()) {}
    inline int operator() (){ return distribution(generator); }
private:
    //Random number generator engine
    std::default_random_engine generator;
    //Generator range
    const std::uniform_int_distribution<int> distribution;
};

QString intToQString(const int &value, const int &fieldWidth = 1, const int &base = 16);
#endif // UTILITY_H
