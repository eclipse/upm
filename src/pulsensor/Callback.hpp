#pragma once

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
