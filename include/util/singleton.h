#ifndef __UTIL_SINGLETON_H
#define __UTIL_SINGLETON_H
/*!
 * \file singleton.h
 * \brief Template class to create singleton
 * \author Grégoire Lejeune
 * \version 0.1
 * \date mars 2012
 *
 * \section Usage 
 * \code
 * class MySingleton : public Singleton<MySingleton> {
 *   friend class Singleton<MySingleton>;
 *
 *   private:
 *     MySingleton();
 *     ~MySingleton();
 *     // ...
 *
 *   public:
 *     void do_something();
 *     //...
 * };
 * \endcode
 *
 * Then :
 *
 * \code
 * MySingleton::getInstance()->do_something();
 * \endcode
 */
#include <stddef.h>

/*!
 * \class Singleton
 * \brief Create singleton
 */
template <typename T> class Singleton {
  protected:
    Singleton () { }
    ~Singleton () { }

  public:
    /*!
     * \brief Return the instance of the singleton class
     */
    static T *getInstance() {
      if (NULL == _singleton) {
        _singleton = new T;
      }

      return (static_cast<T*> (_singleton));
    }

    /*!
     * \brief Delete the instance of the singleton.
     *
     * If you delete the instance of the singleton, a new one will be created on the next call of getInstance.
     */
    static void kill() {
      if (NULL != _singleton) {
        delete _singleton;
        _singleton = NULL;
      }
    }

  private:
    static T *_singleton;
};

template <typename T> T *Singleton<T>::_singleton = NULL;

#endif // __UTIL_SINGLETON_H
