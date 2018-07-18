




# C++ Code Style Guide

These rules are not strictly mandatory in some cases, but you should follow them.



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

### Namespaces

For namespaces we do not have indentation.

```cpp
namespace rat
{
//...
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

### Constructors

There should be new line for construction list. Remember also the order of variables to avoid construction reorder warning. 

```cpp
SomeClass::SomeClass(int foo, int bar)
:	foo(foo),
	bar(bar * 2)
{
	//...
}
```



## Files

### Extensions

* Source files: `.cpp` - compiled as compiler units.
* Header files: `.hpp` - code documentation and templates.
* Documents: `.md` - external documentation.

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
		Utility/
			*.cpp
			*.hpp
```



## Header files

### Include guards

```cpp
#pragma once
```

### Order of file

1. Corresponding header file (if `.cpp`)
2. Note about file and authors
3. C++ system files
4. Other libraries' forward declarations
5. Other libraries' .hpp files
6. Your project's forward declarations
7. Your project's .hpp files
8. Your code
9. Templates implementation at the end (if `.hpp`)

```cpp
// Use quotation marks to include the corresponding header file
#include "CorrespondingHeader.hpp"

/** @file CorrespondingHeader.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

// Use angle brackets to include C++ libraries
// Good practice is to mark what you need from certain includes if name couldn't tell that
#include <iostream> // cout, setw
#include <ctime>
#include <string>

// Use angle brackets to include 3rd-party libraries
// Avoid using global includes (i.e. use `SFML/Graphics/Sprite.hpp` instead of `SFML/Graphics.hpp` if you want use `Sprite`)
namespace sf {
	class RenderWindow;
}
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

// Use quotation marks to include another SzczurEngine headers
#include "Szczur/Foo/Bar.hpp"

// If the headers are a part of the same module use the relative path
#include "Bar.hpp"
```

### Using `using`

Do not use `using` keyword in the global namespace nor the header files. You can use them inside implementations and classes (type aliases).

### Forward Declarations

Use where posible to speed up the compilation.

### Inline Functions

Novadays, compiler knows better when inline your function even if you put it in `.cpp` file (C++ LTO), but you can still use `inline` functions only when they are small e.g. 10 lines or less; or describe why you use it.

### Naming

Our common namespace is `rat::` that stands for `Szczur`. There is also `sf3d::` for SFML 3D extensions of `sf::` - SFML library..


