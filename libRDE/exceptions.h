#ifndef exceptions_h__
#define exceptions_h__

#pragma once
#include <stdexcept>

class OutOfRange : public std::runtime_error {
public:
};

#endif // exceptions_h__
