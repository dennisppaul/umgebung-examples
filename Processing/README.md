# *Processing* Examples in *Umfeld*

⚠️ this is a Work in Progress: the plan is to eventually port *all* of the *Processing* examples to *Umfeld* ⚠️

## Differences between *Processing* and *Umfeld*

although *Umfeld* is heavily inspired by *Processing*, there are some differences that you should be aware of when porting your code from *Processing* to *Umfeld*. this document lists the most important differences.

### `@umfeld(include)`

to use *Umfeld* the header file `Umfeld.h` must be included. this is done with the `#include "Umfeld.h"` directive at the top. in *Processing* a similar effect is achieved with `import`. additionally, *Umfeld* uses the namespace `umfeld` to avoid name clashes with other libraries. this means that all *Umfeld* functions and classes must be prefixed with `umfeld::` or the `using namespace umfeld;` directive must be used.

this is a simple example of how to include *Umfeld* in your code:

```c++
#include "Umfeld.h"
using namespace umfeld;
```

### `@umfeld(color_range)`

the color model is RGBA and the color range in *Umfeld* is set to `[0.0...1.0]` where `0.0` is black and `1.0` is white. whereas in *Processing* the color range is set to `[0...255]` by default. in *Processing* range and color model can be changed with `colorMode()`. there are currently no plans to support `colorMode()` in *Umfeld*.

### `@umfeld(arrays)`

arrays in *Umfeld* ( i.e in C/C++ ) are declared slightly differently than in *Processing* ( i.e in Java ). in *Processing*, arrays are declared with the `float[]` syntax, while in *Umfeld* they are declared with the `float*` syntax.

arrays in C/C++ is a big topic. there a quite a few different approaches to arrays in C/C++. the example above is a so-called C-style arrays, which are simple and efficient, but also have some limitations. for example, they do not have a built-in `.length` property, so you need to keep track of the length yourself. this is different from *Processing*, where arrays have a built-in length property. most C++ programmers use the `std::vector` class from the C++ Standard Library, which is a dynamic array that can grow and shrink in size. it has a built-in `.size()` method to get the length of the array and elements can be accessed with square brackets `[]`. this is similar to *Processing* arrays, but with some differences in syntax.

```c++
float* myCArray = new float[10]; // C-style array
std::vector<float> myVector;     // C++ vector
```
