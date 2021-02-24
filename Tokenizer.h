////////////////////////////////////////
// Tokenizer.h
////////////////////////////////////////

#pragma once

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

// The Tokenizer class for reading simple ascii data files. The GetToken function
// just grabs tokens separated by whitespace, but the GetInt and GetFloat functions
// specifically parse integers and floating point numbers. SkipLine will skip to
// the next carraige return. FindToken searches for a specific token and returns
// true if it found it.

class Tokenizer 
{
public:
	Tokenizer();
	~Tokenizer();

	bool Open(const char *file);
	bool Close();

	bool Abort(char *error);	// Prints error & closes file, and always returns false

	// Tokenization
	char GetChar();
	char CheckChar();
	int GetInt();
	float GetFloat();
	bool GetToken(char *str);
	bool FindToken(const char *tok);
	bool SkipWhitespace();
	bool SkipLine();
	bool Reset();

	// Access functions
	char *GetFileName()			{return FileName;}
	int GetLineNum()			{return LineNum;}

private:
	void *File;
	char FileName[256];
	int LineNum;
};

////////////////////////////////////////////////////////////////////////////////
