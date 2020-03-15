/***********************************************
  The MIT License (MIT)

  Copyright (c) 2012 Athrun Arthur <athrunarthur@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 *************************************************/
#ifndef FF_PARA_EXECEPTION_H_
#define FF_PARA_EXECEPTION_H_
#include "ff/functionflow/common/common.h"
#include <exception>

namespace ff {
class used_para_exception : public std::exception {
 public:
  used_para_exception()
      : std::exception(),
        msg("Fatal error! Can't use a para or paragroup more than one time!") {}
  virtual const char* what() const throw() { return msg; }

 protected:
  const char* msg;
};  // end class used_para_exception

class empty_para_exception : public std::exception {
 public:
  empty_para_exception()
      : std::exception(),
        msg("Fatal error! Can't use an empty para or paragroup!") {}
  virtual const char* what() const throw() { return msg; }

 protected:
  const char* msg;
};  // end class used_para_exception
}  // end namespace ff;

#endif
