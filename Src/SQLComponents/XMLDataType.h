/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: XMLDataType.h
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

// XML DATA TYPES
#define XDT_String              0x00000001
#define XDT_Integer             0x00000002
#define XDT_Boolean             0x00000003
#define XDT_Double              0x00000004
#define XDT_Base64Binary        0x00000005  // Was XDT_Base64 !!
#define XDT_DateTime            0x00000006
// New in version 4
#define XDT_AnyURI              0x00000007
#define XDT_Date                0x00000008
#define XDT_DateTimeStamp       0x00000009
#define XDT_Decimal             0x00000010
#define XDT_Long                0x00000011
#define XDT_Int                 0x00000012
#define XDT_Short               0x00000013
#define XDT_Byte                0x00000014
#define XDT_NonNegativeInteger  0x00000015
#define XDT_PositiveInteger     0x00000016
#define XDT_UnsignedLong        0x00000017
#define XDT_UnsignedInt         0x00000018
#define XDT_UnsignedShort       0x00000019
#define XDT_UnsignedByte        0x00000020
#define XDT_NonPositiveInteger  0x00000021
#define XDT_NegativeInteger     0x00000022
#define XDT_Duration            0x00000023
#define XDT_DayTimeDuration     0x00000024
#define XDT_YearMonthDuration   0x00000025
#define XDT_Float               0x00000026
#define XDT_GregDay             0x00000027
#define XDT_GregMonth           0x00000028
#define XDT_GregMonthDay        0x00000029
#define XDT_GregYear            0x00000030
#define XDT_GregYearMonth       0x00000031
#define XDT_HexBinary           0x00000032
#define XDT_NOTATION            0x00000033
#define XDT_QName               0x00000034
#define XDT_NormalizedString    0x00000035
#define XDT_Token               0x00000036
#define XDT_Language            0x00000037
#define XDT_Name                0x00000038
#define XDT_NCName              0x00000039
#define XDT_ENTITY              0x00000040
#define XDT_ID                  0x00000041
#define XDT_IDREF               0x00000042
#define XDT_NMTOKEN             0x00000043
#define XDT_Time                0x00000044
#define XDT_ENTITIES            0x00000045
#define XDT_IDREFS              0x00000046
#define XDT_NMTOKENS            0x00000047
// End - new in version 4
#define XDT_CDATA               0x00004000
#define XDT_Complex             0x00008000

#define XDT_Mask                0x0000ffff

// WSDL Options (USE ONLY ONE (1) !!)
#define WSDL_Mandatory          0x00010000
#define WSDL_Optional           0x00020000
#define WSDL_ZeroOne            0x00040000
#define WSDL_OnceOnly           0x00080000
#define WSDL_ZeroMany           0x00100000
#define WSDL_OneMany            0x00200000
#define WSDL_Choice             0x00400000 // Choice
#define WSDL_Sequence           0x00800000 // Exact sequence

#define WSDL_Mask               0x00ff0000
#define WSDL_MaskOrder          0x00C00000
#define WSDL_MaskField          0x003F0000

// The XML Datatype holds datatypes AND wsdl options
typedef unsigned XmlDataType;
