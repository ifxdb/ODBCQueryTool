/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: ConvertWideString.h
//
// From the project: Marlin Server: Internet server/client
// See: https://github.com/edwig/Marlin
// 
// Copyright (c) 2016-2017 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#pragma once
#include "SQLComponents.h"

namespace SQLComponents
{
  using uchar = unsigned char;

  // Init the code page system
  void    InitCodePageNames();
  // Convert strings from/to Unicode
  bool    TryConvertWideString(const uchar* p_buffer
                            ,int          p_length
                            ,CString      p_charset
                            ,CString&     p_string
                            ,bool&        p_foundBOM);
  bool    TryCreateWideString(const CString& p_string
                            ,const CString  p_charset
                            ,const bool     p_doBom
                              ,uchar**  p_buffer
                              ,int&     p_length);
  // Getting the codepage number from the charset
  int     CharsetToCodepage(CString p_charset);
  // Getting the name of the codepage
  CString CodepageToCharset(int p_codepage);
  // Getting the description of the codepage
  CString CharsetToCodePageInfo(CString p_charset);
  // Find the charset in the content-type header
  CString FindCharsetInContentType(CString p_contentType);
  // Find the mimetype in the content-type header
  CString FindMimeTypeInContentType(CString p_contentType);
  // Construct an UTF-8 Byte-Order-Mark
  CString ConstructBOM();
}
