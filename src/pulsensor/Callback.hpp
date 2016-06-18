#pragma once

#if defined(SWIGJAVA) || defined(JAVACALLBACK)
class Callback
{
  public:
    virtual ~Callback()
    {
    }
    virtual void run(clbk_data arg)
    { /* empty, overloaded in Java*/
    }
};

#endif
