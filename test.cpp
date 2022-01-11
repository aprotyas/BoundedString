#include "BoundedString.hpp"

int main() {
  using BoundedString = bounded_basic_string<char, 10>;
  BoundedString b;
  return 0;
}
