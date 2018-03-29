#include <iostream>

#include "buzzer.hpp"
#include "buzzer_tones.h"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
  upm::Buzzer buzzer("p:33,vol:1.0");
  return 0;
}
