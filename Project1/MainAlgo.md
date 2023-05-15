func ParserToStack:
	while next token is not Semicolon &&
	While a Comma is encountered its only in an inner call:
		push into the stack.	
return the stack in a fomatted form.

func FormatExpr:
	run twice: once for the (*, /, ==, !=, <, >, <=, >=) operations,
	and once for any other operation:
		if an operation is encountered:
			surround the two operands of the operation in parentheses.
			send any not-fomatted sub expression to this function.			
return the stack.

func ParseExpr:
	while the stack is not empty:
		create a node.
		
		if an opening parentheses is encountered:
			call this function for the right son.
			
		if an operation is encountered:
			call this function for the left son.
			enter the operation to this node.
			if the equation of left son and right son is solvable:
				call Evaluate.
			
		if a number or a word is encountered:
			enter the word to this node and return this node.
			
		if a closing parentheses is encountered:
			return this node.
return this node.

func Evaluate:
	define a and b as right and left son of node.
enter the solution to the equation a (operation stated in node) b to this node.

func ParseID:
	create a node.
	if the next token is '=':
		it's an assignment.
		variable value is ParseValue.
	
	if the next token is '(':
		it's a call for a function.
		list of arguments is ParseList.
	
	if the next token is ':':
		it's a declaration.
		the first token is the type, the next token will be the name.
		if the next token is '=':
			its a declaration and an assignment.
			variable value is ParseValue.
		
		if the next token is '(':
			it's a function declaration.
			list of parameters is ParseList.
return this node.	

func ParseList:
	create a node.
	while end of list is not met:
		parse parameter.
		eat comma.
		add parameter to node children.
return this node.

func ParseBlock:
	create a node.
	while end of block is not met:
		parse line.
		eat semicolon.
		add line to node children.
return this node.

func ParserSwitch:
	if token type is word return ParseID.
	if token type is '(' return parseList.
	if token type is '{' return parseBlock.
	
func ParseCompound:
	create a node
	while EOF is not met:
		call ParserSwitch.
		add to children.
return node.
		

