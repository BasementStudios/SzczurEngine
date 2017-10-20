
# C++ Code Style Guide


## General
---

### Indents

We use tabulators (`\t`) equal to the width of 4 spaces.



### Braces

In the same line as the expression.

```cpp
void foo(int a, int b) const {
    if (a > b) {
		// ...
	}
    else {
		// ...
	}
	// ...
}
```



### Naming Style

* Local variables: `cammelCase`.  
* Namespaces: `PascalCase`.
* Classes and Structs: `PascalCase`.  
* Class field: `cammelCase`.  
* Class constants: `PascalCase`.  
* Constants: `UPPER_SNAKE`.
* Functions and methods: `cammelCase`.  
* Templates: `T` or `PascalCase`.

```cpp
/// Namespaces
namespace Szczur {
	namespace Core {
		/// Classes
		class Event {
		public:
			/// Constants
			const static std::string BLOCK_START = "{";
			// ...

			/// Types
			struct level {
				enum Enum {
					Null = 0,
					GameObject,
					Sprite,
					Character,
					Player
				};
			};

			/// Fields
			Event::level::Enum level;
			// ...

			/// Functions
			static Event loadFromFile(const std::string& filepath);
			// ...
			/// Methods
			bool unload();
			/// Templates
			template<typename T>
			bool execute(/*...*/);
		}
	}
}
```



### Spacing

#### Conditional

With a space before and after the condition/statement:

```cpp
if (some == condition) {
	// ...
}

for (auto it = characters.begin(); it != characters.end(); it++) {
	// ...
}
```



#### Expressions and function parameters

No spaces between brackets and parameters/variables : `(x + y) * z`.



#### Templates

No spaces before nor inside the angle brackets (`<>`).  
Template code must be kept in `.h` files.  

```cpp
template<typename T>
class Kappa {
	// ...
}
```



### Variable declaration

Each declaration separately.

```cpp
int foo;
int bar;
float d;
```



### Pointers and references

Pointer's astisks and reference's ampersands should be placed next to typename.

```cpp
typename* foo;
typename& bar;
```



### Oneline conditional statements

```cpp
if ()
    // ...
else
    // ...

for (/*...*/)
	// ...
```

There is an exception – for nested oneliners.

```cpp
for (/*...*/) {
	if()
		// ...
}
```



## Files
---

### Extansions

* Source files: `.cpp`
* Header files: `.h`
* Docs: `.md`



### Encoding

Every file should use `UTF-8` encoding.



### Structure

```
include/                    ; Includes/headers
    Szczur/                 ; For scoping as <Szczur/...>
        *.h
src/                        ; Sources/implementation
    Szczur/                 ; Just for fancy and symetry to headers :P
        *.cpp
docs/                       ; Documentation
    *.md                    ; Common docs, i.e. StyleGuide.md ;)
    Szczur/
        *.md
        *.*                 ; For example there could be aslo images.
```



## Header files
---

### Include guards

```cpp
#pragma once
```



### Order of includes

1. Corresponding header file (if `.cpp`)
2. C++ system files
3. Other libraries' .h files
4. Your project's .h files



```cpp
#include "CorrespondingHeader.h"

#include <iostream>
#include <ctime>
#include <string>

#include <SFML/System.hpp>

#include "../AnotherLocation/SomeHeader.h"
#include "SomeOtherLocalHeader.h"
```



### Using `using`

Do not use `using` keyword in the global namespace nor the header files.



### Forward Declarations

Avoid using forward declarations where possible.



### Inline Functions

Use `inline` functions only when they are small e.g. 10 lines or less; or describe why you use it.



### Namespaces

Namespaces are useful because they allows to divide the global scope in named scopes which prevents functions/variables' names collisions.

```cpp
namespace X {
    namespace Y {
        // ...
    }
}
```



### Naming

Our common namespace is `rat::` that stands for `Szczur`.



### Descriptions of function

In header files describe the function, at least what it does.

```cpp
/// Does foo
/// angle : alpha
/// index : index in main table
void Foo(float angle, int index);
```



## Classes
---

### Description

Describe classes about what they provide.

```cpp
/// Provide methods related with player character controlling.
class PlayerController {
	// ...
}
```



### Inheritance

Put spaces before and after the colon.

```cpp
class DerivedClass : public BaseClass {
    // ...
}
```



### Order of fields and methods

* friendship declarations


* public fields
* protected fields
* private fields


* public methods
* protected methods  
* private methods



### Private and protected fields

Add `_` in front of the private and protected field's name.

```cpp
private:
    typename _privateVarName;
protected:
    typename _protectedVarName;
```



## Source files
---

### Order of includes

Same as in headers.



### Using `using`

Do not use `using` in the global namespace although you can use it in source files.



## Tips
---

#### Use `final` specifier with classes that cannot be inherited.  

```cpp
class DerivedClass final : public BaseClass {/*...*/}
```

#### Use `override` specifier with methods that are inherited.  

```cpp
class BaseClass {
public:
	virtual void foo();
	// ...
}

class DerivedClass : public BaseClass {
	void foo() override;
	// ...
}
```



#### Use `const` keyword with functions and methods that do not change the value of attributes.

```cpp
Vector2<int> getWindowPosition() const;
```



#### Use `const T&` if the passed objects are read-only and to avoid copying it.

```cpp
template<typename T>
T distanceTo(const Vector2<T>& pos);
```



#### Use `enum class` instead of `enum` to scope it's values.

```cpp
enum class Imagetype : int {
	Standard = 0,
	Nice = 1,
	NotNice = -1
};
// ...
Imagetype enum_var;
// ...
enum_var = Imagetype::Nice;
// ...
auto var = static_cast<std::underlying_type<Imagetype>::type>(enum_var);
```
