# C++ snippets


## Unknown call

Let's assume `Fooclass::method1` is present in our header but `Fooclass::method2` is missing.

Check the `nm` output and deduce offset from `method` (it is `0x20` for the example)

    % c++ -std=c++17 unknowncall.cxx -g -o unknowncall
    % nm unknowncall | grep method
    0000000100003b60 T __ZN8Fooclass7method1Ei
    0000000100003b80 T __ZN8Fooclass7method2Eii

Now specify the offset in `unknowncall.cxx` and call `method2` even without declaring it in header.

You have been warned! It is not [allowed by standard](https://isocpp.org/wiki/faq/pointers-to-members#cant-cvt-memfnptr-to-voidptr) and can easily format your SSD.  It won't work with virtual methods. Static methods and non-static methods work the same way. Checked with clang 14.

# License

BSD 3-Clause

