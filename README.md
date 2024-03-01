# Constelli
Interpreted programming language. A much better version of Cosmolti, with smarter parsing and execution. 

# Getting Started

# Dependencies

* None, just C++ :)

# Installing

* Download files
* Put them into a C++ program then write an interpreter

# Writing an interpreter

* Outside of your line-by-line interpretation, define a heap variable:
```
Stack::Stack* heap = new Stack::Stack;
```

* For each line, you need to parse and generate an instruction set 
```
Instruction_Set* in_St = Parser::new_inst_set(line.c_str());
```

* Lastly, you can send those instructions alongside your heap reference to the VM to interpret it
```
VM::execute(in_St, heap);
```
# How the interpreter works (Understanding the C++)

## Parser (con_parse.cpp)
*   Each line is parsed through the parser, the very first step. The parser tokenizes the line
 based on predetermined tokens which are later parsed again to determine the instruction
 associated with the parsed value.
*   Each tokenized value is pushed back into a vector which is given to the instruction set creator.

```
print("Hello");

^^ resulting vector: {"print", "(", "\"", "Hello", "\"", ")", ";"};
```

## Instruction Set Creator (con_parse.cpp)
*   Each vector value is compared to predefined calls (OP_CALL being the ONLY resulting OP_CODE from a CFunction call),
   then keywords (OP_IF, OP_JMP, OP_JMPLOC, etc), then single tokens (OP_OPENPARA, OP_ADD, OP_MOD).
*  An instruction set has a vector of a pair <INST(ruction), std::string>. For example:

```
Converting: print
OP_CODE (INST): OP_CALL
data: "print"

Instruction_Set.push_back(std::make_pair(OP_CALL, "print"));
```

## VM Interpreter (con_vm.cpp)
* The VM reads each instruction in reverse interpretation, aka the vector is flipped around. For example:
```
print("Hello World!");

The final OP_CODE in this line would be OP_ENDLINE (from ";").
However, the VM reads OP_ENDLINE as the first instruction in the set.
```
*   Why? Well, if we get an OP_ADD call, we can pop the stack for the most recent value. However, that's only
 half of the values we need. We have a value in the future that we simply do not have access to.
*   The solution to this dilemma is to index our instruction set back one each loop, defining
   `INST old_instr = Instructions[i-1]`. Then, inside of our OP_INT call (the future value),
   we can check if `if(old_instr == OP_ADD)`. If it does? Then we have our current value,
   say `x`, and we can now pop the stack for the other value, say `y`. Full code:

```c
Interpreting: 5 + 9;

Step 1:
instr=OP_INT, data=9
old_instr = OP_ENDLINE (from our ';')

if(instr == OP_INT) // check if our instruction is OP_INT
{
stack->push(data); //push 9 to the stack
}

Step 2:
OP_ADD has little importantance in the VM, simply verifying if the stack
has a value to begin with. We can disregard any code as it is not important
to understanding why this works.

Step 3:
instr=OP_INT, data=5
old_instr = OP_ADD

//Now, full
if(instr == OP_INT)
{
if(old_instr == OP_ADD)
{
  stack->push( Arith::arith(data, stack->pop(), old_instr);
}
}

```
*   Ok, lets break that down even more. In step 1, after interpreting the endline opcode (step 0), we are
 now working with the OP_INT opcode. The data attached to this opcode is 9. We push this data to the stack
 and continue. When we get to step 3, we once again have another OP_INT opcode. This time, when we check
 if the old_instruction is OP_ADD, it is.
* **So, we know that the previous data was a +, and we now need to add**
 **the current data to the top of the stack.** Arith, aka arithmetic, is called, passing our data and the popped stack
 data, alongside the old_instr to tell the arithmetic function what operation to perform.
  
*   **Variables in the VM** are stored in the heap in a map where they can be easily indexed when needed. They are stored
  without types, which was a bad decision, and are instead decided based on whether they are an integer or string.
  
*   **Functions** when first defined are not interpreted by the VM. When OP_JMPLOC is called, it enables the "function writer",
 which causes instructions sent to instead be pushed back into a vector where they are stored until the function is called.
* When the function is called, we define two new variables.
```
arg1 = stack.pop();
arg2 = stack.pop_index(1); //gets the second to top value in the stack
```
* Once these variables are defined, we loop through the stored instructions for the function and add these instructions
 to the current set of instructions we are parsing. Imagine this function:

```
vector<INST> function_foo_instructions = {OP_INT, OP_MUL, OP_INT};
vector<INST> current_instructions = {OP_NEWMEM, OP_JMP, OP_ENDLINE};

void call_function(vector<INST>& current_instructions)
{
  for(INST new_inst : function_foo_instructions)
  {
    current_instructions.push_back(new_inst);
  }
}

call_function(current_instructions);
```

* It's more complex in the VM, but thats essentially how it works. The instructions (and their data) are appended
 to our current instruction set. 

# Syntax / Writing Constelli

## Print
*To simply print out a value, use print
```
print("Hello World!");
```
## Ending Lines
* Each line must end in ";"

## Types & Variables
*   While you're scripting this language, you shouldn't really need to worry about types
 as all integers are automatically viewed as doubles (we don't need memory management).
*   The VM also interprets all values as strings until necessary operations are required
* (OP_ADD will compare if both strings are numbers or real strings).
  

* To define a variable you simply write:
```
var foo = 5;
```

* To call the variable, you must surround it in `[]` chars.
```
var foo = 5;
print([foo]); -> prints "5"
```

* To edit a variable, you just call it with an equal sign and its new value.
```
var foo = 5;
print('og var: ' + [foo]); -> prints "og var: 5"

[foo] = "Helo";
print('new var: ' + [foo]); -> prints "new var: Helo"
```

## Mathematical Operators & Operations Order
* As the interpreter runs backwards/"reverse interpretation" to support the stack not having
 to index future values and commit multiple instructions in advance, the order of operations
 is messed up and goes in reverse of how you type.
```
print(5 * 4 + 5 - 2);

^^^ In PEMDAS, this results in [(5-2) + 4] * 5.
```

## Defining Strings
* Strings can be defined with both " and '

# Defining Functions
* Functions (OP_JMP & FAMILY) are defined and stored as string names.
```
entry 'new_function';
print('Function called.');
leave; 
```
## Calling Functions
* The function call syntax is a bit confusing but becomes understandable.
```
goto 'new_function';
```

* Functions can also be passed arguments. A full function example is shown here:

```
entry 'print_arg';
print([arg1]);
leave;

goto 'print_arg' 'Hello World!';
```

## String Concatenation
* Two strings can be added by just using the operator +.
```
print('Hello' + ' ' + 'World!');
```
## Comparative Operators
* <,>,== work the same as all languages. They return 0 or 1, and can be parsed
 to "true" or "false" by using bool_parse(), a CFunction. This is discussed next.

```
var totally_false = 5 < 7;
print("number value: " + [totally_false]); -> prints: "number value: 1" as 5 is less than 7
print("parsed value: " + bool_parse([totally_false])) -> prints: "parsed value: true"
```

## CFunctions
* Built in functions coded in C can be accessed and have powerful properties.
* The main example of this is of course `print`, but others are usually added
 everytime I push an update. If you want to see a full list of all CFunctions,
 you can open `Parser/con_parse.cpp` and look for the vector `reserved_calls`
 or you can open `CFunctions/con_cfunction.cpp` and look around there :)

```c
Original CFunctions:
strlen("Hello"); //returns 5
pow(2,3); //returns 2^3, or 8
bool_parse(0); //returns "false", built in function to convert 0 and 1 to false and true.

```

## Authors

Contributors names and contact info

ex. AwesomeMc101 (Chris)
Discord: AwesomeMc101
Email: awesomemc101@yahoo.com

## Version History

* 0.2
    * Implemented functions, variables, and comparative operators
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the   GNU GENERAL PUBLIC LICENSE  - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* Also read Cosmolti and GLOSTA-Nebula if you are interested in the progression of my language-writing :).
