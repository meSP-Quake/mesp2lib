# meSP2::Docs : Math submodule

## Matrices

### How it was done in Quake

Quake seems not to be using matrices much, as it doesn't even
have it's own dedicated type. In fact, you can see matrices only in `UI` code, with two exceptions in `qmath` and `cgame`.

> The reason why Quake is not using matrices may hide in CPU capabilities back at the time.
> Intel's MMX ( **M**atrix **M**ath e**X**tensions ) were targeting only
> integer operations, not so helpful with floats.
>
> Multiplication, inversion, determinant calculation and others
> were heavy operations, taking $`\mathcal{O}\left(n^3\right)`$ operations
> per $`n \times n`$ matrix.
> Just rotating, scaling and shifting vector were $`\mathcal{O}\left(n\right)`$ at worst, which is a lot faster.

However, currently CPUs are powerful enough to handle matrix math flawlessly. Now it's golden instrument for solving variety of different
tasks, even finding $`n`$-th Fibonacci number for $`\mathcal{O}\left(\mathrm{log}\;n\right)`$ ( against naive solution for $`\mathcal{O}\left(n\right)`$ ).

### How to use them

In computer graphics, matrices used for vector rotation, scaling and
shift. With them, transforming vector becomes simple. Two transforms could be combined with matrix multiplication. It feels easy.

However, despite it's the most frequent use of matrix, they have other usage cases. You can use it for color filters, or for solving linear equations and more.

### Usage examples

Let's try to do first matrix math with vector rotation:

```cpp
static constexpr float degrees = 30.0f;

mesp2::vector<2> vec = {0, 1};

// mesp2::matrix<rows, columns>
mesp2::matrix<2, 2> transform =
    mesp2::rotationMatrixPlane<2, float>(degrees * M_PI / 180.0, 0, 1);

// Now rotate vector clockwise by 30 degrees
std::cout << transform * vec << "\n"; // { 0.5, 0.866025 }

// Rotate vector counterclockwise by 30 degrees
std::cout << vec * transform << "\n"; // { -0.5, 0.866025 }
```

What have we done?

With `mesp2::rotationMatrixPlane` we've created matrix, which rotates vector within axis aligned one plane.

> [!IMPORTANT]
> Rotating vector within a plane is more correct statement than _"vector rotation around axis"_.
> Only in three dimensions we have 3 rotation planes, as well as 3 rotation axis.
> Fourth dimension brings us 6 rotation planes, while two dimensional space has only one.

With this note understood, the secret behind last parameters `0, 1` is revealed. They're just coordinate indexes for rotation plane definition.

Operation `transform * vec` applies transform to the vector, so in this case we get rotated vector. Depending on vector placement ( `transform * vec` or `vec * transform` ), the rotation will be clockwise or counterclockwise.

However, this is not the only case:

```cpp
static constexpr float degrees = 30.0f;

mesp2::vector<2> vec = {0, 1};
mesp2::matrix<2, 2> transform =
    mesp2::rotationMatrixPlane<2, float>(degrees * M_PI / 180.0, 0, 1);

transform *= 3.0; // now vector will be scaled by 3

std::cout << transform * vec << "\n"; // { 1.5, 2.59808 }
```

We've not just only rotated the vector, but also scaled it. For shifting, we need to
use fake third dimension, where third coordinate is awlays set to 1:

```cpp
static constexpr float fictional = 1.0f;
static constexpr float degrees = 30.0f;

mesp2::vector<3> vec = {0, 1, fictional};
mesp2::matrix<3, 3> transform =
    mesp2::rotationMatrixPlane<3, float>(degrees * M_PI / 180.0, 0, 1);

transform *= 3.0;      // scale vector by 3
transform[2][2] = 1.0; // for fictional; should always be 1

// shift vector by (1.0, 2.0)
transform[2][0] = 1.0;
transform[2][1] = 2.0;

std::cout << transform * vec << "\n"; // { 2.5, 4.59808, 1 }
```

### Operations

#### `matrix.fill(initialValue)`

Fills matrix with specified value.

#### `matrix.getDeterminant()`

**Complexity**:

- For `float` and `double`: $`\mathcal{O}\left(n^3\right)`$
- For other types: $`\mathcal{O}\left(n!\right)`$

Calculates matrix's determinant.

#### `matrix.getTrace()`

**Complexity**: $`\mathcal{O}\left(n\right)`$

Calculates matrix's trace.

#### `matrix.makeInverse()`

**Complexity**:

- For `float` and `double`: $`\mathcal{O}\left(n^3\right)`$
- For other types: $`\mathcal{O}\left(n^2n!\right)`$

Creates inverse matrix and returns its copy. Does not mutate matrix.

#### `matrix.makeTransposed()`

Creates transposed matrix and returns its copy. Does not mutate matrix.

#### `static matrix<n, n, T>::identity()`

Creates identity matrix and returns it.

#### `mesp2::rotationMatrix2d<T>(angle)`

Creates $2 \times 2$ rotation matrix.

#### `mesp2::rotationMatrix3d<T>(angles[3])`

Creates $3 \times 3$ rotation matrix from given Euler angles.

#### `mesp2::rotationMatrixPlane<N, T>(angle, dim1, dim2)`

Creates $N \times N$ rotation matrix among plane, passing through `dim1` axis and `dim2` axis.

#### `mesp2::binpow(matrix, power)`

**Complexity**: $`\mathcal{O}\left(n^3 \mathrm{log}\left(m\right)\right)`$

- for matrix $`n \times n`$ and power $`m`$.

Raises matrix to power with binary power raising algorithm and returns result copy. Does not mutate matrix.

Usage example:

```cpp
// Finding n-th fibonacci number for O(logn)

// Special matrix for finding fibonacci numbers
mesp2::matrix<2, 2, int64_t> base = {0, 1, 1, 1};

// Print out first ten fibonacci numbers
for (int pow = 1; pow < 10; ++pow) {
    // Each is calculated individually, without using previous results
    mesp2::matrix<2, 2, int64_t> fib = mesp2::binpow(base, pow);

    // n-th number is stored at last row and last column
    std::cout << fib[1][1] << "\n";
}

// Finding 10000000000-th fibonacci number. Try this against naive soluition
// and measure time.
std::cout << mesp2::binpow(base, 1e10)[1][1] << "\n";
// Naive solution: 2.263697s
// Binpow: 0.000001s
```
