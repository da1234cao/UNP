#pragma once

#include <exception>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

struct base_exception : virtual std::exception, virtual boost::exception {};

typedef boost::error_info<struct tag_err_str, std::string> err_str;