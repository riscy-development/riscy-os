# Style guide

Please adhere to the following style guide when writing code for the RISCy Kernel.

## DRY

**Don't Repeat Yourself.** <br>
**Don't Repeat Yourself.** <br>
**Don't Repeat Yourself.**

Its easy to copy-paste. It's also easy to duplicate bugs that way. Just define a `static` functions, even if it is five lines long and takes four arguments. 
The compiler is smart and will inline it appropriately.

## Language

We're using **C 23** and **C++ 20**. Go crazy, you've got all the new features.

For example, annotate your fallthrough switch cases explicitly:
```c
switch (x) {
    case 1:
        a = 5;
        [[fallthrough]];
    case 2:
        foo();
        break;
}
```

## Formatting

This is handled by `clang-format`. Just run `make format` and your code will become pretty.

Some special cases are listed below.

### Single-line `if` statements

Feel free to drop the branches on single-line `if` statements. We avoid bugs such as the infamous
Apple one by having a code formatter.

### Multi-line `if` conditions

The formatter has trouble with the following type of code

```c
if (my_really_long_function_name(
        with, lots, of, arguments, with, really, long_, names
    )) {
    do_a();
    then_do_b();
}
```

So format your code like this to help it out:
```c
bool ok = my_really_long_function_name(
    with, lots, of, arguments, with, really, long_, names
);
if (ok) {
    do_a();
    then_do_b();
}
```

### Constants

Please use `constexpr` for `c` and `cpp` file constants, now that it exists in C and C++.

For header constants, use `#define` macros or `enum`s.

## Types

### Return values

If returning a constant, please explicitly cast it:
```c
int
foo()
{
    return (int)0;
}
```

### Pointers and numbers

Sometimes we (unfortunately) have to cast pointers to numbers. Please do so as follows:

```c
    void* ptr;
    uint32_t val;

    ptr = (void*)(uintptr_t)val;
    val = (uint32_t)(uintptr_t)ptr;
```

### Integers

Please favor strictly-defined (`uintptr_t`, `size_t`, etc.) types when possible. When exact sizes are required, use fixed-width (`uintXX_t`/`intXX_t`) types.
However, if a general-purpose integer is needed with no strict requirements on width, use `word_t` and `uword_t`, which is fixed to a native
word type.

On 32-bit or higher systems, `word_t` is guaranteed to be at least 32 bits. On smaller systems, it will be fixed to the width of a register.

To put it simply:
> When I say uint8_t I'm saying this better be exactly 8 bits or the code is WRONG, because of some standard or register width, etc.
>
> — @kjhayes
