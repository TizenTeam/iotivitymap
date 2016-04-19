#ifndef logger_h_
#define logger_h_ 1

#include <iostream>
#include <dlog.h>


class Log : public std::ostream
{
public:
  Log(std::ostream* out=0) {
    mStream = out;
  }
  Log& operator<<(const std::string& str) {
	  if (mStream)
    *mStream << str;
    dlog_print(DLOG_INFO, LOG_TAG, "%s", str.c_str() );

    return *this;
  }
  Log& operator<<(const char * str) {
	  if (mStream) *mStream << str;
    dlog_print(DLOG_INFO, LOG_TAG, "%s", str );
    return *this;
  }
  Log& operator<<(const char str) {
	  if (mStream) *mStream  << str;
    return *this;
  }

  Log& operator<<(const int str) {
	  if (mStream) *mStream << str;
    dlog_print(DLOG_INFO, LOG_TAG, "%d", str );
    return *this;
  }

  // http://stackoverflow.com/questions/1134388/stdendl-is-of-unknown-type-when-overloading-operator
  Log &operator<<(std::ostream& (*os)(std::ostream&)){
	  if (mStream) *mStream  << *os;
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Log& stream);
 private:
  std::ostream* mStream;
};

//std::ostream& operator<<(std::ostream& os, const Log& stream) {   return os; }

namespace std {
namespace local {
extern Log cout;
extern Log cerr;
}
}
#endif
