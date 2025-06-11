# meSP2::Docs : Math submodule

## Vectors

**meSP2** implements vectors as points in eucledian linear space of multidimensional set of elements. Non-eucledian and non-linear spaces are not considered there.

Although linear algebra _does not_ limit vector definition to a set of _real numbers in cartesian coordinate system_ $\mathbb{R}^d$ ( and library is able to perform linear operations upon vectors in, say, functional space ) and **meSP2** allows usage of different spaces, we will use vectors as points in such system, as used in most games.

**Note**: _This implementation is used for convenient vector usage with quake's format, rather writing yet another linear algebra library. When searching, I haven't seen good yet compact library for doing geometrical things ( including non-traditional linear spaces, e.g. color spaces )._

### Brief

Vector is declared as pair of dimension count and element type:

```cpp
// Creating vector of floats, default for quake 3
const mesp2::vector<3, float> = {1., 2., 3.};

// Creating vector of doubles
const mesp2::vector<4, double> = {1., 2., 3., 4.};

// Float is default type for vectors
mesp2::vector<2> v;

v::value_type == float; // true
```

First 4 coordinates can be accessed through their common names:

```cpp
// For 4-dimensional vector
mesp2::vector<4> a;

a.x = 0.f;
a.z += a.x + a.y;
a.w *= 2.0f;

// For 2-dimensional vector
mesp2::vector<2> b;

b.x = b.y = 1.0f;

b.z = 0.0f; // CE: field 'z' is undefined for 2-dimensional vectors
```

### Arithmetic

Basic arithmetic could be performed through known operators you may know from `GLSL`:

```cpp
mesp2::vector<3> a, b, c; // assume they're initialized

// Adding and subtracting
c = b - a;
a += b;

// Initializing for next exmaple to explain it later
a = {1.5f, 3.0f, 4.0f};
b = {2.0f, 2.0f, 1.0f};

// This is component-by-component multiplication and *NOT*
// a dot product. For given above vectors resuling vector
// c will be: {3.0f, 6.0f, 4.0f}
c = a * b;
```

Scalar operations with vectors are also defined:

```cpp
mesp2::vector<3> a = {1.0f, 2.0f, 3.0f};

// Vector can be multiplied by scalar
a *= 2.0f; // a = {2.0f, 4.0f, 6.0f}

// Scalar can be added to a vector
a += 1.0f; // a = {3.0, 5.0f, 6.0f}
```

### Operations

#### `mesp2::dot<n>`

Calculates vector DOT product, assuming vectors are in cartesian coordinate system. Defined for $n$-dimensional vectors.

```cpp
// 6-dimensional vectors
mesp2::vector<6> a = {2.f, .5f, .25f, 1.f, 0.f, 3.f},
                 b = {1.f, 5.f, 2.f, 1.f, 19.f, 10.f};

std::cout << mesp2::dot(a, b) << "\n"; // 36
```

#### `mesp2::length<n>`

Calculates length for vector $a$. Defined for $n$-dimensional vectors.

```cpp
mesp2::vector<5> a = {4, 1, 1, 1, 9},
                 b = {13, 0, 0, 0, 0};

std::cout << mesp2::length(a) << "\n"  // 4
          << mesp2::length(b) << "\n"; // 13
```

#### `mesp2::distance<n>`

Calculates distance between vectors $a$ and $b$. Equivalent to length of vector $a - b$.

#### `mesp2::normalized<n>`

Returns normalized copy of provided vector ( so it's length is equal to 1 ). Defined for $n$-dimensional vectors.

```cpp
mesp2::vector<5> a = {4, 1, 1, 1, 9},
                 b = {13, 0, 0, 0, 0};

std::cout << mesp2::normalized(a) << "\n"  // {1, 0.25, 0.25, 0.25, 2.25}
          << mesp2::normalized(b) << "\n"; // {1, 0, 0, 0, 0}
```

#### `mesp2::cross<3>`

Returns cross product of two 3d vectors.

_**!**_: Defined for $3$-dimensional vectors only.

```cpp
mesp2::vector<3> a = {2, 0, 0},
                 b = {1, 0, 3};

std::cout << mesp2::cross(a, b) << "\n"; // {0, -6, 0}
```
