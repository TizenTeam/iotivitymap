#include "logger.h"
#include <iostream>

namespace std {
namespace local {
  Log cout(&std::cout);
  Log cerr(&std::cerr);
}
}

#include "logger.inl"
