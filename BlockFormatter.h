#pragma once

/**
 * Handly class for tracing in cosnole the start of block of code
 */
class BlockFormatter
{
public:
  //class c-tor
  BlockFormatter(int aLine = __LINE__)
  {
    std::cout << "Starting block at line: " << aLine << std::endl << std::endl;
  }

  //class d-tor
  ~BlockFormatter()
  {
    std::cout << std::endl << std::endl;
  }
};

#define TRACE_BLOCK BlockFormatter bf(__LINE__);
