#ifndef BOUNDED_STRING_HPP
#define BOUNDED_STRING_HPP

#include <algorithm>
#include <memory>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

/// A string based on std::basic_string but with an upper bound.
/**
 * Meets the same requirements as std::basic_string.
 * Only compiles if UpperBound is a positive integral value.
 *
 * \param CharT Type of character
 * \param UpperBound The upper bound for the number of characters
 * \param Traits The traits type for the string's characters, defaults to std::char_traits<CharT>
 * \param Allocator Allocator type, defaults to std::allocator<CharT>
 */
template<
  typename CharT,
  std::size_t UpperBound,
  typename Traits = std::char_traits<CharT>,
  typename Allocator = std::allocator<CharT>,
  typename = std::enable_if_t<(UpperBound > 0)>
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

  // Constructors
  /// Create an empty %bounded_basic_string object.
  bounded_basic_string()
  noexcept (noexcept(Allocator()))
  : Base()
  {
    std::cout << "bounded_basic_string: " << UpperBound << '\n';
  }

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
    if (this->length() > UpperBound) {
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
    if (this->length() > UpperBound) {
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
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
  }

  /// Dtor does not perform any memory management.
  ~bounded_basic_string() noexcept
  {}

  // Assignment operators
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
    if (Traits::length(s) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::operator=(s);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(CharT ch)
  {
    // No length check required since UpperBound > 0
    (void)Base::operator=(ch);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  operator=(std::initializer_list<CharT> ilist)
  {
    if (ilist.size() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::operator=(ilist);
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
    if (this -> length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(typename Base::size_type count, CharT ch)
  {
    if (count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(count, ch);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(const bounded_basic_string & str)
  {
    if (str.length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(str);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(const bounded_basic_string & str,
    typename Base::size_type pos,
    typename Base::size_type count = Base::npos)
  {
    (void)Base::assign(str, pos, count);
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(bounded_basic_string && str)
  noexcept
  {
    if (str.length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(std::move(str));
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(const CharT * s, typename Base::size_type count)
  {
    if (count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(s, count);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(const CharT * s)
  {
    if (Traits::length(s) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(s);
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  template<
    typename InputIterator
  >
  bounded_basic_string &
  assign(InputIterator first,
    InputIterator last)
  {
    (void)Base::assign(first, last);
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /**
   * TODO
   */
  bounded_basic_string &
  assign(std::initializer_list<CharT> ilist)
  {
    if (ilist.size() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::assign(ilist);
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
  assign(const T & t)
  {
    (void)Base::assign(t);
    if (this->length() > UpperBound) {
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
  assign(const T & t,
    typename Base::size_type pos,
    typename Base::size_type count = Base::npos)
  {
    (void)Base::assign(t, pos, count);
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  // TODO - capacitry
  using Base::capacity;
  using Base::empty;
  using Base::length;
  using Base::shrink_to_fit;
  using Base::size;

  /// Returns the size of the largest possible %bounded_basic_string.
  typename Base::size_type
  max_size() const noexcept
  {
    // Underlying string impl may have reserved more space than UpperBound
    return std::min(UpperBound, Base::max_size());
  }

  /// TODO
  /**
   * TODO
   */
  void
  reserve(typename Base::size_type new_cap = 0)
  {
    if (new_cap > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    Base::reserve(new_cap);
  }

  // TODO - iterators
  using Base::begin;
  using Base::end;
  using Base::cbegin;
  using Base::cend;
  using Base::crbegin;
  using Base::crend;
  using Base::rbegin;
  using Base::rend;

  // TODO - element access
  using Base::at;
  using Base::back;
  using Base::front;
  using Base::operator[];
  using Base::data;
  using Base::c_str;

  // TODO - operations
  /*
   * Not addressed yet
   * append
   * operator+=
   * replace
   * resize
   */
  using Base::clear;
  using Base::erase;
  using Base::pop_back;
  using Base::compare;
  using Base::substr;
  using Base::copy;

  /// TODO
  /*
   * TODO
   */
  void
  swap(bounded_basic_string & other)
  noexcept
  {
    if (other -> length() >= UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    Base::swap(other);
  }

  /// TODO
  /*
   * TODO
   */
  void
  push_back(CharT ch)
  {
    if (this -> length() >= UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    Base::push_back(ch);
  }

  /// TODO
  /*
   * TODO
   */
  bounded_basic_string &
  insert(typename Base::size_type index,
    typename Base::size_type count,
    CharT ch)
  {
    if (this->length() + count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::insert(index, count, ch);
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  bounded_basic_string &
  insert(typename Base::size_type index,
    const CharT * s)
  {
    if (this->length() + Traits::length(s) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::insert(index, s);
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  bounded_basic_string &
  insert(typename Base::size_type index,
    const CharT * s,
    typename Base::size_type count)
  {
    if (this->length() + count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::insert(index, s, count);
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  bounded_basic_string &
  insert(typename Base::size_type index,
    const bounded_basic_string & str)
  {
    if (this->length() + str.length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::insert(index, str);
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  bounded_basic_string &
  insert(typename Base::size_type index,
    const bounded_basic_string & str,
    typename Base::size_type index_str,
    typename Base::size_type count = Base::npos)
  {
    if (this->length() + str.substr(index_str, count).length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    (void)Base::insert(index, str, index_str, count);
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  typename Base::iterator
  insert(
    typename Base::const_iterator pos,
    CharT ch)
  {
    if (this->length() >= UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return Base::insert(pos, ch);
  }

  /// TODO
  /*
   * TODO
   */
  typename Base::iterator
  insert(
    typename Base::const_iterator pos,
    typename Base::size_type count,
    CharT ch)
  {
    if (this->length() + count > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return Base::insert(pos, count, ch);
  }

  /// TODO
  /*
   * TODO
   */
  template<
    typename InputIterator
  >
  typename Base::iterator
  insert(
    typename Base::const_iterator pos,
    InputIterator first,
    InputIterator last)
  {
    if (this->length() + std::distance(first, last) > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return Base::insert(pos, first, last);
  }

  /// TODO
  /*
   * TODO
   */
  typename Base::iterator
  insert(
    typename Base::const_iterator pos,
    std::initializer_list<CharT> ilist)
  {
    if (this->length() + ilist.size() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return Base::insert(pos, ilist);
  }

  /// TODO
  /*
   * TODO
   */
  template<
    typename T
  >
  bounded_basic_string &
  insert(
    typename Base::size_type pos,
    const T & t)
  {
    (void)Base::insert(pos, t);
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  /// TODO
  /*
   * TODO
   */
  template<
    typename T
  >
  bounded_basic_string &
  insert(
    typename Base::size_type index,
    const T & t,
    typename Base::size_type index_str,
    typename Base::size_type count = Base::npos)
  {
    (void)Base::insert(index, t, index_str, count);
    if (this->length() > UpperBound) {
      throw std::length_error("Exceeded upper bound");
    }
    return *this;
  }

  // TODO - search
  using Base::find;
  using Base::rfind;
  using Base::find_first_of;
  using Base::find_first_not_of;
  using Base::find_last_of;
  using Base::find_last_not_of;

  // TODO - nonmember functions

  // TODO - constants
  using Base::npos;

  // TODO - input/output

  // TODO - numeric conversions

  // TODO - literals

  // TODO - helper classes (hashing)
};

#endif /* BOUNDED_STRING_HPP */
