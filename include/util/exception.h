#ifndef __UTIL_EXCEPTION_INCLUDE
#define __UTIL_EXCEPTION_INCLUDE

#include "util/string.h"

/*!
 * \file util/exception.h
 * \brief Classe an template for exceptions
 * \author Grégoire Lejeune
 * \version 0.1
 * \date mars 2012
 *
 * \section Usage
 *
 * \subsection Cretae Create an exception class
 * \code
 * class MyException {};
 * \endcode
 *
 * \subsection Throw
 * \code
 * THROW(MyException, "Something went wrong, dude!");
 * // or
 * throw Exception<MyException>("Something went wrong, dude!", __FILE__, __LINE__);
 * \endcode
 *
 * \subsection Try Try ... Catch ...
 * \code
 * try {
 *   // ...
 * } catch(Exception<MyException> & e) {
 *   // ...
 * }
 * // or, If you want to catch all exceptions
 * try {
 *   // ...
 * } catch(RuntimeException & e) {
 *   // ...
 * }
 * \endcode
 */
#include <iostream>
#include <string>
#include <stdexcept>

/*!
 * \brief Runtime exception class
 */
class RuntimeException : public std::runtime_error {
  public:
    RuntimeException(const std::string &message, const std::string &file, size_t line) throw() : 
      std::runtime_error(message), 
      filename_(file), 
      line_(line) {}
    virtual ~RuntimeException() throw() {}

    /*!
     * \brief Get name of file that throw
     * \return Name of file that throw
     */
    virtual const std::string getFileName() const throw() {
      return filename_;
    }

    /*!
     * \brief Get throw exception line
     * \return Throw exception line
     */
    virtual size_t getLine() const throw() {
      return line_;
    }

    /*!
     * \brief Get description of this exception
     * \return Description of this exception
     */
    virtual const std::string getMessage() const throw() {
      return what();
    }

    /*!
     * \brief Print the exception to the give nstream (std:cerr by default)
     * \param stream : The stream to print to
     */
    virtual void print(std::ostream &stream = std::cerr) const throw() {
      stream << "RuntimeException" << std::endl;
      stream << "Error : " << what() << std::endl;
      stream << "File : " << getFileName() << std::endl;
      stream << "Line : " << getLine() << std::endl;
    }

  private:
    const std::string filename_;
    size_t line_;
};

/*!
 * \brief Template exception of radio
 */
template <typename T>
class Exception : public RuntimeException {
  public:
    Exception (const std::string &message, const std::string &file, size_t line) throw() : RuntimeException(message, file, line) {}
    virtual ~Exception () throw() {}
};

/*!
 * \brief Throw an exception
 * \param type : the exception class
 * \param message : the exception message, optionnaly containing format selectors (e.g: %s) 
 * \param args : a variable-length list of arguments used in formatted message
 */
#define THROW(type, message, args...) \
  std::string _msg_=lhs::util::format(message,## args); \
  std::cerr << _msg_ << std::endl; \
  throw Exception<type>((_msg_), __FILE__, __LINE__);

#define TRY try
#define CATCH(type, e) catch(Exception<type> & e)
//#define CATCH(e) catch(RuntimeException & e)

#endif // __UTIL_EXCEPTION_INCLUDE
