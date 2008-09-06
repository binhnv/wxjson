/////////////////////////////////////////////////////////////////////////////
// Name:        jsonreader.h
// Purpose:     the parser of JSON text
// Author:      Luciano Cattani
// Created:     2007/09/15
// RCS-ID:      $Id: jsonreader.h,v 1.3 2008/03/03 19:05:45 luccat Exp $
// Copyright:   (c) 2007 Luciano Cattani
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( _WX_JSONREADER_H )
#define _WX_JSONREADER_H

#ifdef __GNUG__
    #pragma interface "jsonreader.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/stream.h>
    #include <wx/string.h>
    #include <wx/arrstr.h>
#endif


#include "json_defs.h"
#include "jsonval.h"

// The flags of the parser
enum {
  wxJSONREADER_STRICT          = 0,
  wxJSONREADER_ALLOW_COMMENTS  = 1,
  wxJSONREADER_STORE_COMMENTS  = 2,
  wxJSONREADER_CASE            = 4,
  wxJSONREADER_MISSING         = 8,
  wxJSONREADER_MULTISTRING     = 16,
  wxJSONREADER_COMMENTS_AFTER  = 32,
  wxJSONREADER_TOLERANT        = wxJSONREADER_ALLOW_COMMENTS | wxJSONREADER_CASE |
                                 wxJSONREADER_MISSING | wxJSONREADER_MULTISTRING,
  wxJSONREADER_COMMENTS_BEFORE = wxJSONREADER_ALLOW_COMMENTS | wxJSONREADER_STORE_COMMENTS
};


class WXDLLIMPEXP_JSON  wxJSONReader
{
public:
  wxJSONReader( int flags = wxJSONREADER_TOLERANT, int maxErrors = 30 );
  virtual ~wxJSONReader();

  int Parse( const wxString& doc, wxJSONValue* val );
  int Parse( wxInputStream& doc, wxJSONValue* val );

  int   GetErrorCount() const;
  int   GetWarningCount() const;

  static int  UTF8NumBytes( char ch );

#if defined( wxJSON_64BIT_INT )
  static bool Strtoll( const wxString& str, wxInt64* i64 );
  static bool Strtoull( const wxString& str, wxUint64* ui64 );
#endif

  const wxArrayString& GetErrors() const;
  const wxArrayString& GetWarnings() const;

protected:

  int  Parse( wxJSONValue* val );
  int  DoRead( wxJSONValue& val );
  void AddError( const wxString& descr );
  void AddError( const wxString& fmt, const wxString& str );
  void AddError( const wxString& fmt, wxChar ch );
  void AddWarning( int type, const wxString& descr );
  int  GetStart();
  int  ReadChar();
  int  GetChar();
  int  PeekChar();
  void StoreValue( int ch, const wxString& key, wxJSONValue& value, wxJSONValue& parent );
  int  SkipWhiteSpace();
  int  SkipComment();
  void StoreComment( const wxJSONValue* parent );
  int  ReadString( wxJSONValue& val );
  int  ReadToken( int ch, wxString& s );
  int  ReadValue( int ch, wxJSONValue& val );
  int  ReadUnicode( long int& hex );
  int  AppendUnicodeSequence( wxString& s, int hex );
  int  NumBytes( char ch );

  static bool DoStrto_ll( const wxString& str, wxUint64* ui64, wxChar* sign );

  // constructor's parameters

  //! Flag that control the parser behaviour,
  int  m_flags;

  //! aximum number of errors stored in the error's array
  int  m_maxErrors;

  // data for the Parse() function

  //! The current line number (start at 1).
  int  m_lineNo;

  //! The current column number (start at 1).
  int  m_colNo;

  //! The current level of object/array annidation (start at ZERO).
  int  m_level;

  //! The pointer to the value object that is being read.
  wxJSONValue* m_current;

  //! The pointer to the value object that was last stored.
  wxJSONValue* m_lastStored;

  //! The pointer to the value object that will be read.
  wxJSONValue* m_next;

  //! The comment string read by SkipComment().
  wxString     m_comment;

  //! The starting line of the comment string.
  int          m_commentLine;

  //! The array of error messages.
  wxArrayString m_errors;

  //! The array of warning messages.
  wxArrayString m_warnings;

  //! The current character position for string input
  int           m_charPos;

  //! The input type (0=string, 1=stream)
  int           m_inType;

  //! The pointer to the input object (a string or a stream)
  void*         m_inObject;

  //! The character read by the PeekChar() function (-1 none)
  int           m_peekChar;

  //! The conversion object used in stream input
  /*!
   This data member is set to NULL in the ctor and it is initialized in the
   Parse() function when the input is from a stream object.
   The pointer points to a wxMBConvUTF8 class used by the GetChar() function
   in order to convert a variable number of bytes in a wide character.
   Input from strings does not need this conversion: the GetChar() function
   can access a single character in the string input by using the subscript
   operator or the wxString::At() function which returns a single character.

   Please note that in order to support input formats other than UTF-8 in stream
   input it is not sufficient to change the object pointed to by this data
   member because the GetChar() function also has to know the number of bytes
   that have to be read from the stream in order to get a character.
   Se NumBytes() for more info.
  */
  wxMBConv*     m_conv;
};


#endif			// not defined _WX_JSONREADER_H


