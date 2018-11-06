#pragma once

#include <string>
#include <vector>

// NOTE: A Logging mechanism for the whole library should be implemenented.
#include <iostream>

namespace upm {
  class UpmStringParser {
  public:
      static std::vector<std::string> parse(std::string initStr, std::string delim = ",") {
          if (initStr.empty()) {
            std::cout << "parse(): NULL or empty string given as argument." << std::endl;
            return {};
          }

          std::vector<std::string> strTokens;
          size_t start = 0;
          size_t end = initStr.find(delim);

          while (end != std::string::npos) {
              strTokens.push_back(initStr.substr(start, end - start));
              start = end + delim.length();
              end = initStr.find(delim, start);
          }
          strTokens.push_back(initStr.substr(start, end));

          return strTokens;
      }
  };
}
