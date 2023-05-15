#define _CRT_SECURE_NO_WARNINGS
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* NahReadFile(const char* arrcFileName)
{
	// Open file.
	FILE* pfFile = NULL;
	pfFile = fopen(arrcFileName, "r");
	// Check if file was opened successfully.
	if (pfFile == NULL)
	{
		printf("Error 2 - could not access file %s.\n", arrcFileName);
		exit(2);
	}

	// Allocate memory for line and buffer.
	char* pcLine = (char*)malloc(2 * sizeof(char));
	// Check memory allocation.
	if (pcLine == NULL)
	{
		printf("Error 1 - memory allocation failed.\n");
		exit(1);
	}
	char* pcBuffer = (char*)calloc(1, sizeof(char));
	// Check memory allocation.
	if (pcBuffer == NULL)
	{
		printf("Error 1 - memory allocation failed.\n");
		exit(1);
	}

	// Init line.
	pcLine[1] = '\0';
	// Read from file
	fgets(pcLine, 1, pfFile);
	strcpy(pcBuffer, pcLine);
	while (fgets(pcLine, 2, pfFile))
	{
		pcBuffer = realloc(pcBuffer, sizeof(char) * (strlen(pcBuffer) + 2));
		if (pcBuffer == NULL)
		{
			printf("Error 1 - memory allocation failed.\n");
			exit(1);
		}
		strcat(pcBuffer, pcLine);
	}
	// Close file and return buffer.
	fclose(pfFile);
	return pcBuffer;
}

void NahWriteFile(const char* arrcSrc, const char* arrcFileName)
{
	// Open file.
	FILE* pfFile = NULL;
	pfFile = fopen(arrcFileName, "w");
	// Check if file was opened successfully.
	if (pfFile == NULL)
	{
		printf("Error 2 - could not access file %s.\n", arrcFileName);
		exit(2);
	}

	// Allocate memory for line and buffer.
	fprintf(pfFile, arrcSrc);
	// Close file and return buffer.
	fclose(pfFile);
}