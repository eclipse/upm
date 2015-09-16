#pragma once

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
class IsrCallback
{
  public:
    virtual ~IsrCallback()
    {
    }
    virtual void run()
    { /* empty, overloaded in Java*/
    }

  private:
};


static void generic_callback_isr (void* data)
{
    IsrCallback* callback = (IsrCallback*) data;
    if (callback == NULL)
        return;
    callback->run();
}
#endif
