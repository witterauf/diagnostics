#pragma once

#include <stdexcept>

#define STRINGIFY_HELPER(x) #x
#define TOSTRING(x) STRINGIFY_HELPER(x)

#define Expects(cond) if (!(cond)) \
    throw std::runtime_error{ "Contract violation: precondition failure at " __FILE__ ": " TOSTRING(__LINE__) };
#define Ensures(cond) if (!(cond)) \
    throw std::runtime_error{ "Contract violation: postcondition failure at " __FILE__ ": " TOSTRING(__LINE__) };

#define UNUSED(x) (void)(x)
