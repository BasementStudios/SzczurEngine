
# C++ Code Style Guide



## General



### Indents

We use tabulators (`\t`) equal to the width of 4 spaces.



### Braces

In the next line for the code and data structures (functions, `class`, `struct`, `enum`, `namespace`).

In the same line for the expressions (including lambda) and conditions (`if`, `while`, `for`, `[](){}`).

```cpp
// Good
void foo(int a, int b) const
{
	if (a > b) {
		// ...
	}
	else {
		// ...
	}
}

// Bad
void foo(int a, int b) const {
	if (a > b)
	{
		// ...
	}
	else
	{
		// ...
	}
}
```



### Naming

* Local variables: `cammelCase`
* Namespaces: one word in `lower_snake`
* Classes and structs: `PascalCase`
* Functions and methods: `cammelCase`
* Public class field: `cammelCase`
* Private/protected class field: `_cammelCase`
* Class aliases: `PascalCase_t`
* Macros: `UPPER_SNAKE`
* Templates: `T` or `PascalCase`



### Expressions and function parameters

No spaces between brackets and parameters/variables.  
Use spaces between operators.

```cpp
// Good
int a = (x + y) * z;
std::cout << "Hello World" << std::endl;

// Bad
int a=(x+y)*z;
std::cout<<"Hello World"<<std::endl;
```



### Variable declaration

Each declaration separately.

```cpp
// Good
int foo;
int bar;

// Bad
int foo, bar;
```



### Pointers and references

Pointer's astisks and reference's ampersands should be placed next to typename.

```cpp
// Good
int* foo;
int& bar;

// Bad
int *foo;
int &bar;
```



## Files



### Extensions

* Source files: `.cpp`
* Header files: `.hpp`
* Template files: `.tpp`
* Docs: `.md`



### Encoding

Every file should use `UTF-8` encoding.



### Structure

```
docs/
	*.md
src/
	Szczur/
		Modules/
			ExampleModule/
				*.cpp
				*.hpp
				*.tpp
		Utility/
			*.cpp
			*.hpp
			*.tpp
```



## Header files



### Include guards

```cpp
#pragma once
```



### Order of includes

1. Corresponding header file (if `.cpp`)
2. C++ system files
3. Other libraries' .hpp files
4. Your project's .hpp files
5. Template file at the end (if `.hpp`)



```cpp
// Use quotation marks to include the corresponding header file
#include "CorrespondingHeader.hpp"

// Use angle brackets to include C++ libraries
#include <iostream>
#include <ctime>
#include <string>

// Use angle brackets to include 3rd-party libraries
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Use quotation marks to include another SzczurEngine headers
#include "Szczur/Foo/Bar.hpp"

// If the headers are a part of the same module use the relative path
#include "Bar.hpp"
```



### Using `using`

Do not use `using` keyword in the global namespace nor the header files.



### Forward Declarations

Avoid using forward declarations where possible.



### Inline Functions

Use `inline` functions only when they are small e.g. 10 lines or less; or describe why you use it.



### Naming

Our common namespace is `rat::` that stands for `Szczur`.
