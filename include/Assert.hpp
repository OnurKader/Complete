#pragma once

#ifndef NDEBUG
#ifndef massert

#include <cstdlib>		   // For std::abort();
#include <fmt/core.hpp>	   // For fmt::print();

#define TO_STR(x) #x
#define STR(x)	  TO_STR(x)
#define massert(cond, msg)                                     \
	if(!(cond))                                                \
	{                                                          \
		fmt::print(stderr,                                     \
				   "{}:{}: {}: Assertion `{}` failed. ({})\n", \
				   __FILE__,                                   \
				   __LINE__,                                   \
				   __PRETTY_FUNCTION__,                        \
				   STR(cond),                                  \
				   msg);                                       \
		std::abort();                                          \
	}
#endif
#else
#define massert(cond, msg) (void)0
#endif
