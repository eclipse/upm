/* NOTE: WIP */

#pragma once

#include <string>
#include <vector>
#include "upm/upm_types.h"
#include <mraa/initio.hpp>

#include <iostream>

static const std::string DELIM = ":";

namespace upm
{
  class UpmObject
  {
  public:
    UpmObject();
    UpmObject(std::string initStr) : mraaIo(initStr)
    {
      parseUpmInitString(mraaIo.getLeftoverString());

      for (auto i : strTokens) {
        std::cout << i << " ";
      }
      std::cout << std::endl;
    }
    ~UpmObject() {};

  protected:
    std::vector<std::string> getParsedTokens()
    {
      return strTokens;
    }

    mraa::MraaIo mraaIo;

  private:
    upm_result_t parseUpmInitString(std::string initStr)
    {
      upm_result_t result = UPM_SUCCESS;

      if (initStr == "") {
        // No specific UPM init string provided.
        return result;
      }

      size_t start = 0;
      size_t end = initStr.find(DELIM);

      while (end != std::string::npos) {
        strTokens.push_back(initStr.substr(start, end - start));
        start = end + DELIM.length();
        end = initStr.find(DELIM, start);
      }
      strTokens.push_back(initStr.substr(start, end));

      return result;
    }

    std::vector<std::string> strTokens;
  };
}
