#pragma once

#include <algorithm>
#include <atomic>

#include <cstring>
#include <fmt/printf.h>

#include <functional>
#include <iostream>
#include <list>
#include <locale>
#include <map>

#include <new>
#include <signals-cpp/signals.hpp>
#include <string>
#include <thread>
#include <vector>

// Bring common functions from C++ into global namespace
using std::memcpy;
using std::isspace;
using std::isdigit;
using std::toupper;
using std::tolower;
using std::size_t;
using std::min;
using std::max;
using std::modf;
using std::atoi;
using std::string;
using std::vector;
using std::map;
using std::thread;
using std::atomic;
using std::atomic_flag;
using std::cout;
using std::endl;
using std::istream;
using std::ifstream;
using fmt::print;

// make_unique
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// make_resource
template <class T> using decay_t = typename std::decay<T>::type;

template <typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments &&... args) -> decltype(
    std::unique_ptr<decay_t<decltype(*c(std::forward<Arguments>(args)...))>,
                    decltype(d)>(c(std::forward<Arguments>(args)...), d)) {
  auto r = c(std::forward<Arguments>(args)...);
  if (!r) {
    throw std::system_error(errno, std::generic_category());
  }
  return std::unique_ptr<decay_t<decltype(*r)>, decltype(d)>(r, d);
}
