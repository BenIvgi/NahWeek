#define _CRT_SECURE_NO_WARNINGS
#include "nahweek.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Main Description
//-----------------------------------------------------------------------------
//
// General : The main function of the project.
//
// Input : None.
//
// Process : The function executes the functions of the project,
// and prints their outputs.
//
// Output : None.
//
//-----------------------------------------------------------------------------
// Programmer : Ben David Ivgi
// Project : NahWeek Compiler.
// Date : 08.11.2022.
//-----------------------------------------------------------------------------
void main(int argc, char** argv)
{
	// If no source file was given, error!
	if (argc < 2)
	{
		printf("NahWeek! Error 2 - [NahWeek : main] - input file not specified.\n");
		exit(2);
	}
	// Compile the file.
	NahCompileFile(argv[1]);
}
