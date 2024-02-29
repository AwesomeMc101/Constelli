# Constelli
Interpreted programming language. A much better version of Cosmolti, with smarter parsing and execution. 



ALL LICENSED CODE :)

- Wrote the initial commit in a few hours on a Wednesday night lol

**To write a compiler:**

define your heap variable:

  - Stack::Stack* heap = new Stack; //this is our more permanent stack

For each line of code you want to execute, you need to create an instruction set:
  -  Instruction_Set* in_St = Parser::new_inst_set(line);

With this instruction set and heap, you can compile it by running:
  - VM::execute(in_St, heap);


**Syntax & CFunctions**

Basic Syntax:
print("Hello World!");
print("Hello " + "World!");
print(1 * 5);

CFunctions:
print, 1 arg, prints top of stack
strlen, 1 arg, returns length of string
pow, 2 arg, arg1 ^ arg2

**Defining Vars**
- var foo = 5;

**Calling Vars**
print([foo]); 
prints 5^
- Must have [] around varname

**Defining Functions**
entry 'functionName';
print();
leave;

- print(); will print the value at the top of the stack

**Calling Functions**
goto 'functionName' 45; 
Now, 45 will be at the top of the stack and will be printed.


