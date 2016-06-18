#pragma once

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
class Callback
{
  public:
    virtual ~Callback()
    {
    }
    virtual void run()
    { /* empty, overloaded in Java*/
    }
};


static void generic_callback (Callback* callback)
{
    if (callback == NULL)
        return;
    callback->run();
}
#endif
