#pragma once
#ifndef NAHWEEK_H
#define NAHWEEK_H
//-----------------------------------------------------------------------------
// Compile for NahWeek
//-----------------------------------------------------------------------------
//
// General : function compiles for NahWeek.
//
// Parameters :
// pcSrc - Source, a pointer to a char.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void NahCompile(char* pcSrc);

//-----------------------------------------------------------------------------
// Compile for NahWeek From file
//-----------------------------------------------------------------------------
//
// General : function reads from file and uses NahCompile
//
// Parameters :
// arrcFileName - the file address.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void NahCompileFile(const char* arrcFileName);

//-----------------------------------------------------------------------------
// Run NahWeek From file
//-----------------------------------------------------------------------------
//
// General : function writes to file and runs it.
//
// Parameters :
// arrcFilePath - the file address.
// nahSource - the code to run.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void NahRunFile(const char* arrcFilePath, const char* nahSource);

#endif
