#include "Utilities/LuaTableWriter.h"

void LuaTableWriter::WriteIndent( std::fstream& file )
{
	for ( int level = indent; level; --level )
	{
		Write( "    ", file );
	}
}

void LuaTableWriter::WriteNewLine( std::fstream& file )
{
	if ( !newLine )
	{
		if ( !minimize )
		{
			if ( suppressNewLines )
			{
				Write( ' ', file );
			}
			else
			{
				Write( '\n', file );
				WriteIndent( file );
			}
		}
		newLine = true;
	}
}

void LuaTableWriter::PrepareNewValue( std::fstream& file )
{
	if ( !valueWritten )
	{
		WriteNewLine( file );
	}
	else
	{
		Write( valueSeparator, file );
		if ( !minimize )
		{
			Write( ' ', file );
		}
	}
}

void LuaTableWriter::WriteCommentSeparation( std::fstream& file )
{
	// PrepareNewLine(file);
	Write( "----------------------------------------------------------", file );
	Write( '\n', file );
}

LuaTableWriter::LuaTableWriter()
	: valueSeparator( ',' )
	, valueWritten( false )
	, minimize( false )
	, newLine( false )
	, suppressNewLines( false )
	, error( false )
	, indent( 0 )
{
}

void LuaTableWriter::WriteStartDocument()
{
	assert( indent == 0 );
}

void LuaTableWriter::WriteEndDocument( std::fstream& file )
{
	assert( indent == 0 );
	Write( '\n', file );
}

void LuaTableWriter::WriteStartReturnTable( std::fstream& file )
{
	PrepareNewLine( file );
	Write( '{', file );
	++indent;
	newLine = false;
	valueWritten = false;
}
void LuaTableWriter::WriteDeclareTable( const std::string name, std::fstream& file )
{
	PrepareNewLine( file );
	Write( name, file );
	Write( minimize ? "={" : " = {", file );
	++indent;
	newLine = false;
	valueWritten = false;
}

void LuaTableWriter::WriteCommentLine( const std::string comment, std::fstream& file )
{
	// PrepareNewLine(file);
	Write( "-- " + comment, file );
	Write( '\n', file );
}

void LuaTableWriter::WriteEndTable( std::fstream& file )
{
	PrepareNewLine( file );
	--indent;
	if ( valueWritten )
	{
		WriteNewLine( file );
	}
	Write( '}', file );
	newLine = false;
	valueWritten = true;
}

void LuaTableWriter::WriteEndTable( bool sameLine, std::fstream& file )
{
	// PrepareNewLine(file);
	--indent;
	if ( valueWritten && !sameLine )
	{
		// PrepareNewLine(file);
		WriteNewLine( file );
		// WriteIndent(file);
	}
	Write( '}', file );
	newLine = false;
	valueWritten = true;
}

void LuaTableWriter::WriteEndTableWithSeparator( bool sameLine, std::fstream& file )
{
	// PrepareNewLine(file);
	--indent;
	if ( valueWritten && !sameLine )
	{
		WriteNewLine( file );
	}
	Write( '}', file );
	// Write(valueSeparator, file);
	newLine = false;
	valueWritten = true;
}

void LuaTableWriter::PrepareNewLine( std::fstream& file )
{
	if ( valueWritten )
	{
		Write( valueSeparator, file );
		valueWritten = false;
	}
	WriteNewLine( file );
}

void LuaTableWriter::SetSuppressNewLines( bool suppressNewLines )
{
	this->suppressNewLines = suppressNewLines;
}

bool LuaTableWriter::SuppressNewLines() const
{
	return suppressNewLines;
}

void LuaTableWriter::SetMinimize( bool minimize )
{
	this->minimize = minimize;
}

bool LuaTableWriter::Minimize() const
{
	return minimize;
}

bool LuaTableWriter::HasError() const
{
	return minimize;
}

std::string LuaTableWriter::Quote( const std::string& str )
{
	std::string quoted;
	quoted.reserve( str.length() + 2 );
	quoted += '"';

	for ( const char ch : str )
	{
		switch ( ch )
		{
		case '\\': quoted.append( "\\\\" ); break;
		case '"': quoted.append( "\\\"" ); break;
		case '\n': quoted.append( "\\n" ); break;
		default: quoted += ch;
		}
	}

	quoted += '"';
	return quoted;
	return std::string();
}
