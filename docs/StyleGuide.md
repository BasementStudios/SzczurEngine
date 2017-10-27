
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



### Naming

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
			/// Member Constants
			const static std::string BLOCK_START = "{";
			// ...

			/// Types
			enum class TargetType {
				Null = 0,
				GameObject,
				Sprite,
				Character,
				Player
			};

			/// Member Variables
			Event::TargetType level;
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

With a space before and after the condition/statement.  
With an `else` statement in the new line.

```cpp
if (foo == bar) {
	// ...
}
else {
	// ...
}

for (auto it = foo.begin(); it != foo.end(); it++) {
	// ...
}
```



#### Expressions and function parameters

No spaces between brackets and parameters/variables.  
Use spaces between operators.

 ```cpp
 int a = (x + y) * z;
 std::cout << "Hello World" << std::endl;
 ```



#### Templates

No spaces neither before nor inside angle brackets (`<>`).  
Template implementation code must be kept in `.tpp` files.  

```cpp
template<typename T>
class Foo {
	// ...
}
```



### Variable declaration

Each declaration separately.

```cpp
int foo;
int bar;
```



### Pointers and references

Pointer's astisks and reference's ampersands should be placed next to typename.

```cpp
int* foo;
int& bar;
```



### Oneline conditional statements

```cpp
if (/* ... */)
	// ...
else
	// ...

for (/* ... */)
	// ...
```

There is an exception – for nested oneliners.

```cpp
for (/* ... */) {
	if ()
		// ...
}
```



## Files
---

### Extensions

* Source files: `.cpp`
* Header files: `.hpp`
* Template files: `.tpp`
* Docs: `.md`



### Encoding

Every file should use `UTF-8` encoding.



### Structure

```
src/
	Szczur/						; Scoping, i.e. <Szczur/...>
		Core/					; Core Engine, e.g. Rendering, Memory Management
			*.cpp
			*.hpp
			*.tpp
		Modules/				; Scoping, i.e. <Szczur/Modules/...>
			ExampleModule/			; Example module directory
				*.cpp
				*.hpp
				*.tpp
		Utils/					; Utility classes and functions
			*.cpp
			*.hpp
			*.tpp
docs/							; Documentation
	*.md						; Common docs, e.g. StyleGuide.md
	Szczur/
		*.md
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
3. Other libraries' .hpp files
4. Your project's .hpp files
4. Template file (if `.hpp`)



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



### Namespaces

Namespaces are useful because they allows to divide the global scope in named scopes which prevents name collisions.

```cpp
namespace Foo {
	namespace Boo {
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
/// Does foo.
class Foo {
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


* public members
* protected members
* private members


* public methods
* protected methods  
* private methods


#### Define and initialize member variables in the order of member declaration

##### Reason

To minimize confusion and errors. That is the order in which the initialization happens (independent of the order of member initializers).

##### Example, bad

```cpp
class Foo {
	int m1;
	int m2;
public:
	Foo(int x) :m2{x}, m1{++x} { }   // BAD: misleading initializer order
	// ...
};

Foo x(1); // surprise: x.m1 == x.m2 == 2
```

##### Enforcement

(Simple) A member initializer list should mention the members in the same order they are declared.

###### Source: [C++ Core Guidelines – C.47](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-order)


### Private and protected fields

Add a *underscore* in front of the private and protected field's name.

```cpp
private:
	typename _privateFoo;
protected:
	typename _protectedBar;
```



### Initializer List

Left a *colon* in the same line as a declaration.

```cpp
Foo(int x, int y, const std::string& name) :
	_x(x), _y(y), _name(name) {
	// ...
}
```



### Default constructor

Don't define a default constructor that only initializes data members; use in-class member initializers instead

##### Reason

Using in-class member initializers lets the compiler generate the function for you. The compiler-generated function can be more efficient.

##### Example, bad

```cpp
class Foo { // BAD: doesn't use member initializers
	std::string s;
	int i;
public:
	X1() :s{"default"}, i{1} { }
	// ...
};
```

##### Example

```cpp
class Bar {
	std::string s = "default";
	int i = 1;
public:
	// use compiler-generated default constructor
	// ...
};
```

##### Enforcement

(Simple) A default constructor should do more than just initialize member variables with constants.

###### Source: [C++ Core Guidelines – C.45](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-default)



## Source files
---

### Order of includes

Same as in headers.



### Using `using`

Do not use `using` in the global namespace although you can use it in source files.



## Tips
---

### Use `final` specifier with classes that cannot be inherited

```cpp
class DerivedClass final : public BaseClass {/* ... */}
```

### Use `override` specifier with methods that are inherited.  

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



### Use `const` keyword with functions and methods that do not change the value of attributes

```cpp
Vector2<int> getWindowPosition() const;
```



### Use `const T&` if the passed objects are read-only and to avoid copying it

```cpp
template<typename T>
T distanceTo(const Vector2<T>& pos);
```



### Use `enum class` instead of `enum` to scope it's values

```cpp
enum class Imagetype : int {
	Standard = 0,
	Nice = 1,
	NotNice = -1
};
// ...
Imagetype foo;
// ...
foo = Imagetype::Nice;
// ...
auto bar = static_cast<std::underlying_type_t<Imagetype>>(foo);
```



### Where there is a choice, prefer default arguments over overloading

##### Reason

Default arguments simply provide alternative interfaces to a single implementation. There is no guarantee that a set of overloaded functions all implement the same semantics. The use of default arguments can avoid code replication.

##### Note

There is a choice between using default argument and overloading when the alternatives are from a set of arguments of the same types. For example:

```cpp
void print(const std::string& s, format f = {});
```

as opposed to

```cpp
void print(const std::string& s);  // use default format
void print(const std::string& s, format f);
```

There is not a choice when a set of functions are used to do a semantically equivalent operation to a set of types. For example:

```cpp
void print(const char&);
void print(int);
```

###### Source: [C++ Core Guidelines – F.51](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rf-default-args)



### Keep the number of function arguments low

##### Reason

Having many arguments opens opportunities for confusion. Passing lots of arguments is often costly compared to alternatives.

##### Discussion

The two most common reasons why functions have too many parameters are:

 * Missing an *abstraction*. There is an abstraction missing, so that a compound value is being
 passed as individual elements instead of as a single object that enforces an invariant. This not only expands the parameter list, but it leads to errors because the component values are no longer protected by an enforced invariant.

 * Violating *"one function, one responsibility."* The function is trying to do more than one job and should probably be refactored.

###### Source: [C++ Core Guidelines – I.23](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-nargs)



### Never transfer ownership by a raw pointer (T*) or reference (T&)

##### Reason

If there is any doubt whether the caller or the callee owns an object, leaks or premature destruction will occur.

##### Example

Consider:

```cpp
X* compute(args)    // don't
{
	X* res = new X{};
	// ...
	return res;
}
```

Who deletes the returned X? The problem would be harder to spot if compute returned a reference. Consider returning the result by value (use move semantics if the result is large):

```cpp
std::vector<double> compute(args)  // good
{
	std::vector<double> res(10000);
	// ...
	return res;
}
```

**Alternative:** Pass ownership using a *"smart pointer"*, such as *unique_ptr* (for exclusive ownership) and *shared_ptr* (for shared ownership). However, that is less elegant and often less efficient than returning the object itself, so use smart pointers only if reference semantics are needed.

###### Source: [C++ Core Guidelines – I.11](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Ri-raw)
