func NahCompile:
	init lexer and parser.
	parse the source buffer into an AST.
	if chosen by user:
		print ast
		
	generate assembly code.
	
	if chosen by user:
		print assembly code.
		
	call NahRunFile.
	
func NahCompileFile:
	read file.
	call NahCompile.
	
NahRunFile:
	create path for assembly file.
	write assembly file.
	run system command to compile and run assembly file.

