#ifndef __CONFIG_OBJECT_H__
#define __CONFIG_OBJECT_H__

template <class T>
class ConfigObject
{
public:
    virtual bool init() = 0;
    virtual bool loadConfig() = 0;
};

#endif