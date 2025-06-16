# meSP2::Docs : Math submodule

## Brief

**meSP2** implements vectors as points in eucledian linear space of multidimensional set of elements. Non-eucledian and non-linear spaces are not considered here.

Although linear algebra _does not_ limit vector definition to a set of _real numbers in cartesian coordinate system_ $\mathbb{R}^d$ and **meSP2** allows usage of different spaces, we will use vectors as points in such system, as used in most games.

**Note**: _This implementation is used for convenient vector usage with quake's format, rather writing yet another linear algebra library. When searching, I haven't found good yet compact library for doing geometrical things ( including non-traditional linear spaces, e.g. color spaces )._

## Vectors

### Quake's format

For vectors, Quake uses special types:

```c
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
```

As you can see, special type `vec_t` abstracts vectors from their types, allowing you to change type when needed everywhere.

When in most cases vectors up to 5 dimensions are enough, this scheme of typing sometimes causes repeatable code:

```c
vec_t dot2(vec2_t left, vec2_t right);
vec_t dot3(vec3_t left, vec3_t right);
vec_t dot4(vec4_t left, vec4_t right);
// so on and so forth...
```

Also, basic arithmetic operations definitely don't look like a really simple code:

```c
// This is a bit simplified version of G_BounceMissile for demonstration purposes
void G_BounceMissile( gentity_t *ent, trace_t *trace ) {
    vec3_t  velocity; // assume initialized

    // reflect the velocity on the trace plane
    float dot = DotProduct( velocity, trace->plane.normal );
    VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

    if ( ent->s.eFlags & EF_BOUNCE_HALF ) {
        VectorScale( ent->s.pos.trDelta, 0.65, ent->s.pos.trDelta );
        // check for stop
        if ( trace->plane.normal[2] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 ) {
            return;
        }
    }

    VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
    VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
}
```

From a bit of perspective, it looks somewhat readable, but let's take a look at _mesp2's_ version:

```cpp
// This is a bit simplified version of G_BounceMissile for demonstration purposes
void G_BounceMissile( gentity_t *ent, trace_t *trace ) {
    mesp2::vector<3> velocity; // assume initialized

    // reflect the velocity on the trace plane
    float dot = mesp2::dot( velocity, trace->plane.normal );

    ent->s.pos.trDelta = velocity - trace->plane.normal * 2 * dot;

    if ( ent->s.eFlags & EF_BOUNCE_HALF ) {
        ent->s.pos.trDelta *= 0.65;
        // check for stop
        if ( trace->plane.normal.y > 0.2 && mesp2::length( ent->s.pos.trDelta ) < 40 ) {
            return;
        }
    }

    ent->r.currentOrigin += trace->plane.normal;
    ent->s.pos.trBase = ent->r.currentOrigin;
}
```

Now it's more readable.

With assigning, you know where result is stored:

```cpp
// Before
VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

// After
ent->s.pos.trDelta = velocity - trace->plane.normal * 2 * dot;
```

With overloaded operators, vectors are manipulated the same way numbers do. With named vectors components, such as **x, y, z, w** it's cleaner and easier to perceive code, and other great stuff thanks to **C++**, which you might have seen somewhere already in other geometric libraries.

### meSP2's format

As you can see from examples in previous sections, `mesp2::vector` is a template class with support of any dimensions and possibility to specify type.

Basic declaration:

```cpp
// Float is type by default
mesp2::vector<3, float> vec = {1, 2, 3};

// 24-dimensional vector of int64_t, initialized with ones
mesp2::vector<24, int64_t> vec(1);
```

The type of a vector is obtained with `::value_type`:

```cpp
mesp2::vector<3, float> vec;

vec::value_type == float; // true
```

Arithmetic for vectors came from `GLSL`. Most of behaviour from there works as well:

```cpp
vec + 3; // adding scalar to all vector components
a * b; // component-wise multiplication: {Ax * Bx, Ay * By, ...}
```

as well as common operations, like addition, subtraction, multiplication and other you saw earlier.

When dealing with higher dimensions, vectors have array-like interface for indexing it's components:

```cpp
mesp2::vector<100, bool> a;

// Indexing starts with zero
a[0] = true;
a[5] = false;
a[99] = true;
```

### Matrix math

Vectors are part of matrix math. Strictly speaking, vectors are matrices with one unit dimension.

For given vector $a$, it could be:

- Row-vector:

  $`\overline a = \begin{vmatrix}\begin{array}{ccc}1&2&3\end{array}\end{vmatrix}`$

- Column-vector:

  $`\overline a = \begin{vmatrix}\begin{array}{c}1\\2\\3\end{array}\end{vmatrix}`$

As you may know ( or may not know, in that case I'd recommend you to give a read about linear algebra basics first ), matrices are not commutative by multiplication:

$$AB \ne BA$$

So, depending on your vector placement, result would be different:

```cpp
mesp2::vector<2> vec = {9, 4};
mesp2::matrix<2> mat = {
    7, -2,
    5, 3
};

// vec as row
std::cout << vec * mat << "\n"; // {55, 57}

// vec as column
std::cout << mat * vec << "\n"; // {83, -6}
```

In both cases _meSP2_ cares with vector representation for you, but be careful, when doing math with matrices.

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
