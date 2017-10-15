
C++ Code Style Guide
====================

General
-------

### Indents

We use tabulators (`\t`) equals to 4 spaces width.



### Braces

Same line as name and arguments declaration.

```cpp
void some(int a, int b) const {
    if(a > b) {
		//...
	} else {
		//...
	}
	//...
}
```



### Naming Style

* Local variables: `cammelCase`.  

`float moveDistance = Vector<>::getDistance<float>(pos1, pos2);`

* Namespaces: `PascalCase`.
* Classes: `PascalCase`.  
* Class field: `cammelCase`.  
* Class constants: `PascalCase`.  
* Constants: `UPPER_SNAKE`. 
* Types: 
	- `lower_snake` for primitive (i.e. enums, simple structs), 
	- Of course `PascalCase` for normal classes. 
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
			//...
			
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
			//...
			
			/// Functions
			static Event loadFromFile(const string& filepath);
			//...
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

With space before and after condition/statement:
```cpp
if (some == condition) {
	//...
}
for (auto it = characters.begin(); it != characters.end(); it++) {
	//...
}
```

#### Expressions and function parameters

Without space inside braces: `(x + y) * z`.

#### Templates

There is no space before and inside sharp braces (`<>`). 
Notice that template code must be keep in `.h` files.  

```
template<typename T>
class Kappa {
	//...
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

Pointer star and reference sign should be connected with typename.

```cpp
typename* foo;
typename& bar;
```



### Oneline conditional statements

```cpp
if ()
    //...
else
    //...

for (/*...*/)
	//...
```

There is exception - for nested oneliners.

```cpp
for (/*...*/) {
	if()
		//...
}
```





Files
-----

### Extansions 

* Source: `.cpp`
* Header: `.h`
* Docs: `.md`

#### Encoding

Every file should use `UTF-8` encoding.



### Structure

```
include/						; Includes/headers
    Szczur/						; For scoping as <Szczur/...>
        *.h
src/							; Sources/implementation
    Szczur/						; Just for fancy and symetry to headers :P
        *.cpp
docs/							; Documentation
    *.md						; Common docs, i.e. StyleGuide.md ;)
	Szczur/
		*.md
		*.*						; For example there could be aslo images.
```





Headers
-------

### Include guards

```cpp
#pragma once
```



### Order of includes

1. Corresponding header file (if `.cpp`)
2. C++ system files
3. Other libraries' .h files
4. Your project's .h files

#### Example

```cpp
#include <CorrespondingHeader.h>

#include <iostream>
#include <ctime>
#include <string>

#include <SFML/System.hpp>

#include <AnotherProjectHeader.h>
```



### Using `using`

In headers you can't use `using` in file scope, but in templates and inline functions.



### Forward Declarations

Avoid using forward declarations where possible. 



### Inline Functions

Use functions `inline` only when they are small, say, 10 lines or fewer; or describe why you use it.



### Namespaces

Namespaces subdivide the global scope into distinct, named scopes, and so are useful for preventing name collisions in the global scope.

```cpp
namespace X {
    namespace Y {
        // ...
    }
}
```

#### Naming

Our common namespace is `rat::` for Szczur.



### Descriptions of function

In headers files you should describe the function, at least what it does.

```cpp
/// Does foo
/// angle : alpha
/// index : index in main table
void Foo(float angle, int index);
```



### Classes

#### Description

Aslo classes should be described about what it is providing.

```
/// Provide functions related with player character controlling.
class PlayerController {
	//...
}
```

#### Inheritance

Space before and after colon.

```cpp
class DerivedClass : public BaseClass {
    //...
}
```

#### Order of fields and methods

* friend declarations
 
* public fields
* protected fields
* private fields

* public methods
* protected methods  
* private methods

#### Private and protected fields

Add `_` in front of the field's name e.g.

```cpp
public:
	typename _publicVarName;
private:
    typename _privateVarName;
protected:
	typename _protectedVarName;
```





Sources
-------

### Order of includes

Same as in headers.

### Implementation only headers

In implementation there can be more headers than in headers - which are used only by implementation.

### Using `using`

You should not use `using namespace`, but you can use `using` even in file scope (since it's source file which should not be included anywhere else).





Tips
----

#### Use `final` with classes that cannot be inherited.  

```cpp
final class NoMoreInherits : public Inheirtable {/*...*/}
```

#### Use `override` with functions/methods that are inherited.  

```cpp
class Base {
public:
	virtual void foo();
	//...
}
class Derived : public Base {
	void foo() override;
	//...
}
```

#### Use `const` with functions/methods that do not change value of fields as specifier.

```
Vector<int> getWindowPosition() const;
```

#### Use `const T&` if function/methods do not change passed variables. 

```cpp
template<T>
T rotateTo(const Vector<T> pos);
```

#### Use `struct` for `enum`s to scope its options.

```
struct imagetype {
	enum Enum {
		Standard = 0,
		Nice = 1,
		NotNice = -1
	};
};
//...
imagetype::Enum varname;
//...
varname = imagetype::Nice;
```

#### Good practice is write every dependents of implementation with includes 

Even if you still want to specify namespace i.e. `std::string` - is both possibble ;)

```
#include <CorrespondingHeader.h>
using rat::MyClass;

#include <iostream>
using std::cout;
using std::endl;
#include <ctime>
using std::time_t;
using std::time;
#include <string>
using std::string;

#include <SFML/System.hpp>
using sf::sleep;

#include <AnotherProjectHeader.h>
using rat::OtherClass;
```




