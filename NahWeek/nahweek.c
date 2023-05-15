#define _CRT_SECURE_NO_WARNINGS
#include "nahweek.h"
#include "lexer.h"
#include "io.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "asm.h"

void NahCompile(char* pcSrc)
{
	//Init lexer, parser, AST and first token.
	lexer_N* lexer = InitLexer(pcSrc);
	parser_N* parser = InitParser(lexer);
	AST_N* root = ParserParse(parser);
	free(parser);
	free(lexer);
	
	// Print the AST.
	PrintAST(root, 0);
	
	// Generate and pring the assembly source code.
	char* nahSource = AsmGenerate(root);
	free(root);
	printf("[NahWeek : Assembly]\nDisplaying Assembly source code.\n\n%s\n(C)Ben David Ivgi, 2023\n\n", nahSource);

	// Run the assembly code.
	NahRunFile(__argv[1], nahSource);
}

void NahCompileFile(const char* arrcFileName)
{
	// Read code file.
	char* pcSrc = NahReadFile(arrcFileName);
	// Compile file content.
	NahCompile(pcSrc);
}

void NahRunFile(const char* arrcFilePath, const char* nahSource)
{
	// get folder name from file path.
	char* foldername = arrcFilePath;
	foldername[strlen(foldername) - strlen(strrchr(foldername, '\\')) + 1] = 0;

	// write assembly file.
	char asmloc[200] = { 0 };
	snprintf(asmloc, 100, "%snahweek.asm", foldername);
	NahWriteFile(nahSource, asmloc);

	// run file.
	char command[300] = { 0 };
	snprintf(command, 300, "start DOSBox -noconsole -c \" tasm /zi %snahweek >> %stasmmsg.txt\""
		"-c \"tlink /v nahweek.obj\" -c \"nahweek.exe\" -c \"exit\"", foldername, foldername);
	system(command);
	
}