#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

template<class T, bool mustDelete = true>
class Singleton
{
public:
    static T& getInstance()
    {
        if (NULL == instance_m)
        {
            instance_m = new T;
            if (mustDelete) atexit(releaseInstance);
        }

        return *instance_m;
    }

    static T* getInstancePtr()
    {
        return &getInstance();
    }

    static void releaseInstance()
    {
        if (instance_m && mustDelete)
        {
            delete instance_m;
            instance_m = NULL;
        }
    }
protected:
    Singleton()
    {
    }

private:
    static T* instance_m;
};

template <class T, bool mustDelete> T* Singleton<T, mustDelete>::instance_m = NULL;

template <typename T>
class SingletonEx
{
public:
    SingletonEx()
    {
        assert(!instance_m);
        instance_m = static_cast<T*>(this);
    }

    ~SingletonEx()
    {
        assert(instance_m);
        instance_m = 0;
    }

    static T& getInstance()
    {
        assert(instance_m);
        return (*instance_m);
    }

    static T* getInstancePtr()
    {
        return instance_m;
    }

protected:
    static T* instance_m;
};

template <typename T> T* SingletonEx<T>::instance_m = NULL;

#endif // __SINGLETON_H__
