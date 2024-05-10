#include <iostream>
#include <fstream>
using namespace std;
#include <boost/random.hpp>
boost::random::mt19937 boost_random_time_seed{ static_cast<std::uint32_t>
(std::time(0)) };
#include "build_in_progress/HL/dynamic/test_dynamic.h"
int main()
{
test_dynamic();
}
