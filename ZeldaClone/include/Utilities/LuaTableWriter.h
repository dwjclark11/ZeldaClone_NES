#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

/*
	This class has functions that will help in creating formatted LUA tables
*/
class LuaTableWriter
{
  private:
	// Create member variables
	char valueSeparator;
	bool valueWritten;
	bool minimize;
	bool newLine;
	bool suppressNewLines;
	bool error;
	int indent;

	// Declare private template function
	template <typename TType>
	void Write( const TType value, std::fstream& fileName );

	// Declare Functions that can only be used inside the class
	void WriteIndent( std::fstream& file );
	void WriteNewLine( std::fstream& file );
	void PrepareNewValue( std::fstream& file );

  public:
	// Constructor/Destructor
	LuaTableWriter();
	~LuaTableWriter() = default;

	void WriteStartDocument();
	void WriteEndDocument( std::fstream& file );
	void WriteCommentSeparation( std::fstream& file );
	void WriteCommentLine( const std::string comment, std::fstream& file );
	void WriteStartReturnTable( std::fstream& file );
	void WriteDeclareTable( const std::string name, std::fstream& file );

	void WriteEndTable( std::fstream& file );
	void WriteEndTable( bool sameLine, std::fstream& file );
	void WriteEndTableWithSeparator( bool sameLine, std::fstream& file );

	// setters
	void PrepareNewLine( std::fstream& file );
	void SetSuppressNewLines( bool suppressNewLines );
	bool SuppressNewLines() const;
	void SetMinimize( bool minimize );

	bool Minimize() const;
	bool HasError() const;

	static std::string Quote( const std::string& str );

	// Template functions for enter values/keys of different types --> Generic
	template <typename TValue>
	void WriteValue( TValue value, std::fstream& file );
	template <typename TName>
	void WriteStartTable( TName name, bool quoted, std::fstream& file );
	template <typename TValue>
	void WriteUnquotedValue( TValue value, std::fstream& file );
	template <typename TValue>
	void WriteUnquotedValue( TValue value, bool newLine, std::fstream& file );
	template <typename TKey, typename TValue>
	void WriteQuotedKeyAndValue( TKey key, TValue value, std::fstream& file );
	template <typename TKey, typename TValue>
	void WriteKeyAndValue( TKey key, TValue value, std::fstream& file );
	template <typename TKey, typename TValue>
	void WriteKeyAndValue( TKey key, TValue value, bool finalValue, std::fstream& file );
	template <typename TKey, typename TValue>
	void WriteKeyAndUnquotedValue( TKey key, TValue value, std::fstream& file, bool sameLine = false,
								   bool lastValue = false );
	template <typename TKey, typename TValue>
	void WriteKeyAndQuotedValue( TKey key, TValue value, std::fstream& file );
};

// Template Initializations
template <typename TType>
void LuaTableWriter::Write( const TType value, std::fstream& fileName )
{
	fileName << value;
}

template <typename TValue>
void LuaTableWriter::WriteValue( TValue value, std::fstream& file )
{
	WriteUnquotedValue( value, file );
}

template <typename TName>
void LuaTableWriter::WriteStartTable( TName name, bool quoted, std::fstream& file )
{
	PrepareNewLine( file );
	Write( '[', file );
	if ( quoted )
		// Write(Quote(name), file);
		std::cout << "quoted";
	else
		Write( name, file );

	Write( minimize ? "]={" : "] = {", file );
	++indent;
	newLine = false;
	valueWritten = false;
}

template <typename TValue>
void LuaTableWriter::WriteUnquotedValue( TValue value, std::fstream& file )
{
	PrepareNewValue( file );
	Write( value, file );
	newLine = false;
	valueWritten = true;
}

template <typename TValue>
void LuaTableWriter::WriteUnquotedValue( TValue value, bool newLine, std::fstream& file )
{
	if ( newLine )
		PrepareNewValue( file );

	Write( value, file );
	newLine = false;
	valueWritten = true;
}

template <typename TKey, typename TValue>
void LuaTableWriter::WriteQuotedKeyAndValue( TKey key, TValue value, std::fstream& file )
{
	PrepareNewLine( file );
	Write( '[', file );
	Write( Quote( key ), file );
	Write( minimize ? "]=" : "] = ", file );
	Write( value, file );
}

template <typename TKey, typename TValue>
void LuaTableWriter::WriteKeyAndValue( TKey key, TValue value, std::fstream& file )
{
	PrepareNewLine( file );
	Write( ' ', file );
	Write( key, file );
	Write( minimize ? "=" : " = ", file );
	Write( '"', file );
	Write( value, file );
	// Write(valueSeparator, file);
	newLine = false;
	valueWritten = true;
}

template <typename TKey, typename TValue>
void LuaTableWriter::WriteKeyAndValue( TKey key, TValue value, bool finalValue, std::fstream& file )
{
	// PrepareNewLine(file);
	Write( ' ', file );
	Write( key, file );
	Write( minimize ? "=" : " = ", file );
	// Write('"', file);
	Write( value, file );
	if ( !finalValue )
		Write( valueSeparator, file );

	newLine = false;
	valueWritten = true;
}

template <typename TKey, typename TValue>
void LuaTableWriter::WriteKeyAndUnquotedValue( TKey key, TValue value, std::fstream& file, bool sameLine,
											   bool lastValue )
{
	if ( !sameLine )
		PrepareNewLine( file );

	Write( key, file );
	Write( minimize ? "=" : " = ", file );
	Write( value, file );

	// Separate the values
	if ( sameLine && !lastValue )
		Write( valueSeparator, file );

	newLine = false;
	valueWritten = true;
}

template <typename TKey, typename TValue>
void LuaTableWriter::WriteKeyAndQuotedValue( TKey key, TValue value, std::fstream& file )
{
	PrepareNewLine( file );
	Write( key, file );
	Write( minimize ? "=" : " = ", file );
	Write( Quote( value ), file );
	// Write('"', file);
	newLine = false;
	valueWritten = true;
}