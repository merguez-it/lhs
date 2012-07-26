#ifndef __UTIL_ANY_H
#define __UTIL_ANY_H
// http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf

#include <string>
#include <algorithm>
#include <typeinfo>

namespace lhs {
  namespace util {
    class any {
      public:
        any() : content(0) {}

        any(const any &other) : content(other.content ? other.content->clone() : 0) {}

        any(const char *value) : content(new holder<std::string>(std::string(value))) {}

        template<typename value_type> any(const value_type &value) : content(new holder<value_type>(value)) {}

        ~any() {
          if(content) {
            delete content;
          }
        }

        any &swap(any &rhs) {
          std::swap(content, rhs.content);
          return *this;
        }

        template<typename value_type> any &operator=(const value_type &rhs) {
          any(rhs).swap(*this);
          return *this;
        }

        any &operator=(any &rhs) {
          rhs.swap(*this);
          return *this;
        }

        const std::type_info &type_info() const {
          return content ? content->type_info() : typeid(void);
        }

        operator const void *() const {
          return content;
        }

        template<typename value_type> bool copy_to(value_type &value) const {
          const value_type copyable = get<value_type>();
          if(copyable) {
            value = copyable;
          }
          return copyable;
        }

        template<typename value_type> const value_type get() const {
          placeholder * copy_content = content->clone();

          if(type_info() == typeid(value_type)) {
            value_type result = static_cast< holder<value_type> *>(copy_content)->held;
            delete copy_content;
            return result;
          } 
          throw std::bad_cast();
        }

      private:
        class placeholder {
          public:
            virtual ~placeholder() {}
            virtual const std::type_info & type_info() const = 0;
            virtual placeholder *clone() const = 0;
        };

        template<typename value_type> class holder : public placeholder {
          public:
            holder(const value_type &value) : held(value) {}

            virtual const std::type_info &type_info() const {
              return typeid(value_type);
            }

            virtual placeholder *clone() const {
              return new holder(held);
            }

            const value_type held;
        };

        placeholder *content;
    };

    template<typename value_type> value_type any_cast(const any &operand) {
      return operand.get<value_type>();
    }
  }
}

#endif // __UTIL_ANY_H
