#pragma once
#ifndef BOUNDED_STRING_HPP
#define BOUNDED_STRING_HPP

#include <algorithm>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

/// A string based on std::basic_string but with an upper bound.
/**
 * Meets the same requirements as std::basic_string.
 *
 * \param CharT Type of character
 * \param UpperBound The upper bound for the number of characters
 * \param Traits The traits type for the string's characters, defaults to std::char_traits<CharT>
 * \param Allocator Allocator type, defaults to std::allocator<CharT>
 */
template<
  class CharT,
  std::size_t UpperBound,
  class Traits = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>
>
class bounded_basic_string
  : protected std::basic_string<CharT, Traits, Allocator>
{
  using Base = std::basic_string<CharT, Traits, Allocator>;

public:
  // Forwarding member types from std::basic_string
  using typename Base::allocator_type;
  using typename Base::const_iterator;
  using typename Base::const_pointer;
  using typename Base::const_reference;
  using typename Base::const_reverse_iterator;
  using typename Base::difference_type;
  using typename Base::iterator;
  using typename Base::pointer;
  using typename Base::reference;
  using typename Base::reverse_iterator;
  using typename Base::size_type;
  using typename Base::traits_type;
  using typename Base::value_type;

  bounded_basic_string() {
    std::cout << "bounded_basic_string: " << UpperBound << '\n';
  }

  /// Create an empty %bounded_basic_string object.
  bounded_basic_string()
  noexcept (noexcept(Allocator()))
  : Base()
  {}

  /// Create an empty %bounded_basic_string object.
  /**
   * \param alloc An allocator object
   */
  explicit
  bounded_basic_string(
    const Allocator & alloc)
  noexcept
  : Base(alloc)
  {}

  /// Create a %bounded_basic_string object with default characters.
  /**
   * This constructor fills the %bounded_basic_string object with @a count
   * instances of the character @a ch.
   *
   * \param count The number of characters initially contained in the string
   * \param ch The character to populate the string with
   * \param alloc An allocator ojbect
   */
  bounded_basic_string(
    typename Base::size_type count,
    CharT ch,
    const Allocator & alloc = allocator_type())
  : Base(count, ch, alloc)
  {
    if (count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// Create a %bounded_basic_string as a substring of a provided string.
  /**
   * This constructor creates the %bounded_basic_string object as a substring
   * of @a other over the index range [@a pos, @a other.size()).
   *
   * \param other The string to generate a substring from
   * \param pos The starting position of the substring
   * \param alloc An allocator ojbect
   */
  bounded_basic_string(
    const bounded_basic_string& other,
    typename Base::size_type pos,
    const Allocator & alloc = allocator_type())
  : Base(other, pos, alloc)
  {
    if (other.length() - pos > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// Create a %bounded_basic_string as a substring of a provided string.
  /**
   * This constructor creates the %bounded_basic_string object as a substring
   * of @a other over the index range [@a pos, @a pos + @a count).
   * If @a count == std::basic_string::npos or if the substring range extends
   * past the end of @a other, then the effective index range is
   * [@a pos, @a other.size()).
   *
   * \param other The string to generate a substring from
   * \param pos The starting position of the substring
   * \param count The number of characters to include in the substring
   * \param alloc An allocator ojbect
   */
  bounded_basic_string(
    const bounded_basic_string& other,
    typename Base::size_type pos,
    typename Base::size_type count,
    const Allocator & alloc = allocator_type())
  : Base(other, pos, alloc)
  {
    typename Base::size_type end;
    if (count == Base::npos || pos + count > other.size()) {
      end = other.size();
    } else {
      end = pos + count;
    }
    if (end - pos > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string(
    const CharT* s,
    typename Base::size_type count,
    const Allocator & alloc = allocator_type())
  : Base(s, count, alloc)
  {
    if (count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string(
    const CharT* s,
    const Allocator & alloc = allocator_type())
  : Base(s, alloc)
  {
    if (Traits::length(s) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// Create a %bounded_basic_string from a range.
  /**
   * Constructs a %bounded_basic_string object with the characters
   * from the range [first, last).
   *
   * \param first An input iterator denoting the start of a range
   * \param last An input iterator denoting the end of a range
   * \param alloc An allocator ojbect
   */
  template<
    typename InputIterator
  >
  bounded_basic_string(
    InputIterator first,
    InputIterator last,
    const Allocator & alloc = allocator_type())
  : Base(first, last, alloc)
  {
    if (length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// %bounded_basic_string copy constructor.
  /**
   * Constructs a %bounded_basic_string with a copy of the
   * contents of @a other.
   *
   * \param other The %bounded_basic_string to copy
   */
  bounded_basic_string(
    const bounded_basic_string & other)
  : Base(other)
  {}

  /// %bounded_basic_string copy constructor with an alternative allocator.
  /**
   * Constructs a %bounded_basic_string with a copy of the
   * contents of @a other with the @a alloc allocator.
   *
   * \param other The %bounded_basic_string to copy
   * \param alloc An allocator ojbect
   */
  bounded_basic_string(
    const bounded_basic_string & other,
    const Allocator & alloc = allocator_type())
  : Base(other, alloc)
  {}

  /// %bounded_basic_string move constructor.
  /**
   * TODO
   */
  bounded_basic_string(
    bounded_basic_string && other)
  noexcept
  : Base(std::move(other))
  {}

  /// %bounded_basic_string move constructor with an alternative allocator.
  /**
   * TODO
   */
  bounded_basic_string(
    bounded_basic_string && other,
    const Allocator & alloc = allocator_type())
  : Base(std::move(other), alloc)
  {}


  /// Create a %bounded_basic_string from an initializer list.
  /**
   * TODO
   */
  bounded_basic_string(
    std::initializer_list<CharT> ilist,
    const Allocator & alloc = allocator_type())
  : Base(ilist, alloc)
  {
    if (ilist.size() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// TODO
  /**
   * TODO
   */
  template<
    typename T
  >
  explicit
  bounded_basic_string(
    const T & t,
    const Allocator & alloc = allocator_type())
  : Base(t, alloc)
  {
    if (length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// TODO
  /**
   * TODO
   */
  template<
    typename T
  >
  explicit
  bounded_basic_string(
    const T & t,
    typename Base::size_type pos,
    typename Base::size_type n,
    const Allocator & alloc = allocator_type())
  : Base(t, pos, n, alloc)
  {
    if (length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// Dtor does not perform any memory management.
  ~bounded_basic_string() noexcept
  {}

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(const bounded_basic_string & str)
  {
    (void)Base::operator=(str);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(const bounded_basic_string && str)
  noexcept
  {
    (void)Base::operator=(std::move(str));
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(const CharT * s)
  {
    (void)Base::operator=(s);
    if (Traits::length(s) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(CharT ch)
  {
    (void)Base::operator=(ch);
    if (length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(std::initializer_list<CharT> ilist)
  {
    (void)Base::operator=(ilist);
    if (ilist.size() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  template<
    typename T
  >
  bounded_basic_string &
  operator=(const T & t)
  {
    (void)Base::operator=(t);
    if (length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }
};

#endif /* BOUNDED_STRING_HPP */
