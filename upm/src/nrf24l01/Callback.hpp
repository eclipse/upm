#pragma once

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


void generic_callback (Callback* callback)
{
    if (callback == NULL)
        return;
    callback->run();
}
