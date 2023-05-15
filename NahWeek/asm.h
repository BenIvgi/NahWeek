#pragma once
#ifndef NAH_ASM_H
#define NAH_ASM_H
#include "AST.h"

//-----------------------------------------------------------------------------
// Direct Asm Source Code Generating Processes.
//-----------------------------------------------------------------------------
//
// General : call fitting asm function according to AST child type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmSwitch(AST_N* ast);

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Compound.
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast compound type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmCompound(AST_N* ast);

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Function Call.
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast function call type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmCall(AST_N* ast);

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Function Declaration.
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast function declaration type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmFunction(AST_N* ast);

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Variable Assignment.
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast assignment type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmAssignment(AST_N* ast);

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Variable declaration.
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast variable type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmVar(AST_N* ast);

//-----------------------------------------------------------------------------
// Shell Function For The Assembly Generating Level.
//-----------------------------------------------------------------------------
//
// General : call the assembly switch.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmGenerate(AST_N* ast);
//-----------------------------------------------------------------------------
// Generate Assembly Source Code For Data Segment
//-----------------------------------------------------------------------------
//
// General : generate assembly code from any variable assignment that doesn't
// get allocated on the stack.
//
// Parameters :
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmDataSegment();

//-----------------------------------------------------------------------------
// Generate Assembly Source Code From Expression Solving
//-----------------------------------------------------------------------------
//
// General : generate assembly code from the ast expr type.
//
// Parameters :
// ast - an Abstract syntax tree struct.
//
// Return Value : assembly source code (string).
//
//-----------------------------------------------------------------------------
char* AsmExpr(AST_N* ast);
#endif
