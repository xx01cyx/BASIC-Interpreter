# BASIC Interpreter

Welcome to my BASIC Interpreter! Implemented in C++, the interpreter is designed for the minimal version of BASIC programming language. 

This is the first homework project of SJTU Advanced Data Structure course in 2021 Spring. The document will give a brief introduction to BASIC and make some simple guidelines of my interpreter.

## Quick Start

1. [Get QT installed](https://www.qt.io/) if you do not have QT yet. 
2. Open the project in QT and then run it. The user interface of the interpreter will heave in sight.
3. Click LOAD button to load a BASIC file. You will see the program codes shown in the code box.
4.  Then click RUN button. You will see the execution result and the abstract syntax trees displayed on the screen.


## Introduction to BASIC

### What is BASIC?

The programming language BASIC was developed in the mid-1960s at Dartmouth College by John Kemeny and Thomas Kurtz. It was one of the first languages designed to be easily learned and used, whose ideas live on in Microsoft’s Visual Basic system. 

### Primary Grammar of BASIC

The minimal version of BASIC **requires a line number at the beginning of each line of code.** The statements in a program are executed in an ascending numerical order, starting from the lowest line number. The line number must be a positive number that is no more than 1000000.

The variable name in the minimal version of BASIC can **only contain letters, digits and underlines** and must NOT start with a digit.

The rules for expressions and statements in BASIC are as follows:

1. Expressions

    The minimal version of BASIC supports merely **integer arithmetic**. The operators are as follows:

    | Operator | Interpretation |
    | :---: | :---: |
    | + | Plus |
    | - | Minus on binary while make negative on unary |
    | * | Multiply |
    | / | Divide by rounding down |
    | ** | Power |

2. Statements
   
    There are several statements in the minimal version of BASIC.

    | Statement | Grammar | Interpretation |
    | :---: | :---: | :---: |
    | REM | REM *comment* | Make comments for the program. |
    | LET | LET *var* = *expr* | Assign a value to a variable. |
    | PRINT | PRINT *expr* | Print the value of the expression on the result box with a tail newline character. |
    | INPUT | INPUT *var* | Read an integer value from user input and assign it to a variable. |
    | GOTO | GOTO *n* | Make the program continue executing from line *n*. |
    | IF | IF *cmp* THEN *n* | Make the program continue executing from line *n* on holding the condition. Otherwise, it continues from the next line. |
    | END | END | Halt the execution of the program. |

### Example of BASIC Program

Using the grammars stated above, a BASIC program for printing the Fibonacci sequence could be generated as follows:

    100 REM Program to print the Fibonacci sequence.
    110 LET max = 100000
    120 LET n1 = 0
    130 LET n2 = 1
    140 IF n1 > max THEN 190
    145 PRINT n1
    150 LET n3 = n1 + n2
    160 LET n1 = n2
    170 LET n2 = n3
    180 GOTO 140
    190 END


## Design Pattern of BASIC Interpreter

The BASIC Interpreter is designed for correctly interpreting the BASIC program and detecting the errors in the program. Besides, it generates an [abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) for each BASIC statement. 

My interpreter is composed of four parts: *filter*, *scanner*, *parser* and *interpreter*, which are respectively elaborated below.

* **Filter**

  The BASIC codes read in first go through *filter*, which sorts the codes according to their line numbers. After filtering, the BASIC codes are in the correct execution order.

  The filter might throw an error if the line number is not valid.

* **Scanner**

  The correctly ordered BASIC codes then go into *scanner*, which scans the codes character by character and transforms them into a series of tokens. 
  
  A token is a data structure that consists of the token type and the lexeme (which is optional). For example, a constant `12` owns a token type of *NUMBER* and a lexeme of QString `"12"`. A `LET` token has a token type of *LET* and does not require a lexeme.

  The scanner might throw an error if it meets with an unexpected character.

* **Parser**

  The tokens then go into *parser*, where they are parsed into an array of expressions and finally statements. My way of parsing expressions is shown below:

        expression  ->  term
        term        ->  factor (("+" | "-") factor)*
        factor      ->  unaryPower (("*" | "/") unaryPower)*
        unaryPower  ->  ("-" unaryPower) | power
        power       ->  unary ("**" power)?
        unary       ->  ("-" unary) | primary
        primary     ->  NUMBER | IDENTIFIER | ("(" expression ")")
  
  The top-down recursive way of parsing leads to the construction of an abstract syntax tree (AST) when parsing each line of codes (or tokens). Namely, an AST is generated for each statement of the BASIC code (if there were no error).

  The parser might throw an error if it could not parse a line of code into a valid statement. 

* **Interpreter**

  The statements ultimately enter *interpreter*, which executes each statement in a proper order based on the evaluation of the expressions. 

  The interpreter throws any error it meets with when executing the statements. For example, 0 cannot power 0 and cannot act as a divisor.


## BASIC Interpreter Guidelines

To get familiar with the UI of BASIC Interpreter and make better use of it, please go through the following guidelines.

The UI for the interpreter has three display boxes: one for the original BASIC code, another for the execution result of the program, the third for the abstract syntax tree.


At the bottom is a command line for user input. The valid input are shown as follows:

* **BASIC Program**
  
  * ***BASIC statement***

    Add a BASIC line of code to the code box by entering a valid BASIC statement with its line number at the beginning. It will be inserted into the code box at the proper position.

  * ***n***
  
    Delete a BASIC line of code from the code box by entering its line number.  If the line of code that corresponds to the input number does not exist, the command will be simply ignored.
    <br/>

* **BASIC Commands**
  
  * **LET *var* = *expr***

       Declare a global variable. 

  * **INPUT *var***

       Read a user input as the value of a global variable (only integer is valid).

  * **PRINT *expr***

       Print a global variable to the result box. 

   *Note that the BASIC commands will NOT print abstract syntax trees into the AST box.*
   <br/>

* **Control Commands**

   * **LOAD**
     
       Load a BASIC file from the local explorer. The program loaded will be displayed in a correct order (sorted by line number) in the code box.

   * **RUN**
     
       Run the program displayed in the code box. Note that a BASIC file needs to be loaded before running. 
       
       The execution result of the program as well as the error message will be printed to the result box. Also, the abstract syntax trees will be displayed in the AST box.

       After running the program, the local variables declared in the program will be cleared, but the global variables declared via BASIC commands will still remain.

   * **CLEAR**
     
       Clear the screen as well as the global variables declared via BASIC commands.

   * **HELP**
     
       Show the help information.

   * **QUIT**
     
       Quit BASIC Interpreter.

Besides, there are four buttons placed below the display boxes as well:
* LOAD: Same as `LOAD` command.
* RUN: Same as `RUN` command.
* SAVE: Save the BASIC program to local.
* CLEAR: Same as `CLEAR` command.
                            
