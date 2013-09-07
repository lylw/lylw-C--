#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Singleton.h"

template <class T>
class Manager
    : public Singleton<T>
{
protected:
    virtual bool init() = 0;
    virtual void destroy() = 0;
};

#endif