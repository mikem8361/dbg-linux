#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

int main ()
{
  std::wstring_convert<std::codecvt_utf8_utf16<WCHAR>,WCHAR> conversion;
  std::string mbs = conversion.to_bytes( u"\u4f60\u597d" );  // ni hao (你好)

  // print out hex value of each byte:
  std::cout << std::hex;
  for (int i=0; i<mbs.length(); ++i)
    std::cout << int(unsigned char(mbs[i])) << ' ';
  std::cout << '\n';

  return 0;
}
