#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
/*


#include "base64.h"
#include <iostream>

int main() {
  const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung" ;

  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
  std::string decoded = base64_decode(encoded);

  std::cout << "encoded: " << encoded << std::endl;
  std::cout << "decoded: " << decoded << std::endl;

  return 0;
}
*/
