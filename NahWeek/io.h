#pragma once
#ifndef NAH_IO_H
#define NAH_IO_H
//-----------------------------------------------------------------------------
// Read Nah Code
//-----------------------------------------------------------------------------
//
// General : function reads code in file and puts it in buffer.
//
// Parameters :
// arrcFileName - a String (file address).
//
// Return Value : The text in file.
//
//-----------------------------------------------------------------------------
char* NahReadFile(const char* arrcFileName);

//-----------------------------------------------------------------------------
// Write Nah Code
//-----------------------------------------------------------------------------
//
// General : function writes code in file.
//
// Parameters :
// arrcFileName - a String (file address).\
// arrcSrc - the content to write in the file
//
// Return Value : The text in file.
//
//-----------------------------------------------------------------------------
void NahWriteFile(const char* arrcSrc, const char* arrcFileName);
#endif

