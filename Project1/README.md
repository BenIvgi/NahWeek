#Variable Declaration && Assignment.
Type: varName;
Type: varName = value;

#Examples:
//number
number: num0;
number: num1 = 3_000;
number: num2 = -15;
//string
string: name0;
string: name1 = "Ben";
string: name2 = boolToStr(True);
string: name3 = tostring(12452);
string: name4 = "Guy is " + num1 + " Days old.";
//bool
bool: cond0;
bool: cond1 = 1 < 2 && num2 > 0;
bool: cond2 = False || True;
bool: cond3 = "Hello" == name1;
bool: cond4 = "1234" == toString(1234);

#If Statment
if(condition)
{
	body of code...
}
#Example:
number: x = 3;
if(x > 2)
{
	typeNahl("The number: " + x + " is greater than two.");
}

#Numerical based loop #1
iter(upper limit)
{
	body of code...
}
-> meaning is that iter will go from zero to the limit.
-> in that case the language will create a variable "i".
-> "i" will be automatically incremented.
-> in nested loops, "i" will become "j", "k" and so on.
#Example:
number: x = 8;
iter(x)
{
	typeNahl("The number: " + i + " is lesser than " + x + ".");
}

#Numerical based loop #2
iter(lower limit, upper limit)
{
	body of code...
}
-> meaning is that iter will go from lower limit to upper limit.
-> in that case the language will create a variable "i".
-> "i" will be automatically incremented.
-> in nested loops, "i" will become "j", "k" and so on.
#Example:
number: x = 8;
number: y = 2;
iter(y, x)
{
	typeNah("The number: " + i + " is lesser than " + x + ",");
	typeNahl(" but greater or equal to " + y + ".");
}

#Numerical based loop #3
iter(lower limit, upper limit, custom iterator)
{
	body of code...
}
-> meaning is that iter will go from zero to the limit.
-> in that case the language will create an iterator.
-> the iterator's name will be as the user chose.
-> the iterator will be automatically incremented.
#Example:
number: x = 8;
number: y = 2;
iter(y, x, element)
{
	typeNah("The number: " + element + " is lesser than " + x + ",");
	typeNahl(" but greater or equal to " + y + ".");
}

#Conditional loop
while(condition)
{
	body of code...
}
-> meaning is that the loop will be executed as long as the condition is True.
#Example:
number: x = 8;
number: y = 11;
while(x < y)
{
	typeNahl("The number: " + x + " is lesser than " + y + ".");
	x = x - 1;
}

#Function Declaration
SendType: Name(Param1Type: param1, Param2Type: param2...);
SendType: Name(Param1Type: param1, Param2Type: param2...)
{
	body of code...
	send(value?);
}
#Examples:
void: typeSquare(number: length, number: width, string: comp)
{
	iter(length)
	{
		iter(width)
			{
				typeNah(comp);
			}
		typeNahl("");
	}
	send();
}
number: findRoot(number: num)
{
	number: root = 0;
	while(root*root <= num)
	{
		root = root + 1;
	}
	send(root - 1);
}

string: whatsMyName(number: id);
bool: isOdd(number: num);

#I/0 Handling
Type:? varName = Casting?(Input(msg));
typeNah/TypeNahl("" +?var);
Casting is needed when handling single non string values.
"" + is needed when an str value is being concated with a non str value in head.
#Examples:
string: name = Input("Whats My Name?");
number: num12 = toNumber(Input("Enter a Number"));
bool: cond0 = toNumber(Input("Enter 0 or 1"));
typeNah("HI!");
typeNahl("This is a line");
typeNah("" + num12 + "is a number? " + cond0);

#Casting
//toNumber
number/bool: variable = toNumber(msg);
//toString
string: msg = "" + value;
#Examples:
number: num3 = toNumber("1245");
string: num1 = "" + 125;
