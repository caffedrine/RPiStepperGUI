#ifndef LOCKHELPER_H
#define LOCKHELPER_H

#ifdef __ANDROID__

#include <QAndroidJniObject>

class KeepAwakeHelper
{
public:
    KeepAwakeHelper();
    virtual ~KeepAwakeHelper();

private:
    QAndroidJniObject m_wakeLock;
};

#endif
#endif // LOCKHELPER_H
