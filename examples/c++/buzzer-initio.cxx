#include <iostream>

#include "buzzer.hpp"
#include "buzzer_tones.h"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
  upm::Buzzer buzzer("p:32,vol:0.01,play:3800:500000");
  return 0;
}
