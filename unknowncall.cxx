#include <cstdio>
#include <functional>
#include <memory>

class Fooclass {
public:
  int method1(int x);
	// method declared here only for checking. It can be absent
  int method2(int x, int y);
};
  
// Unknown method signature
typedef int (Fooclass::*UnknownMethod)(int, int);

int Fooclass::method1(int x) { 
	return x * 100; 
}

int Fooclass::method2(int x, int y) { 
	return x * 100 + y; 
}

int main(int argc, char *argv[]) {
  const ssize_t offset = 0x20;

  Fooclass instance{};

	// Call method just to check result
  int res_explicit = instance.method2(2, 20);
  printf("Result of explicit call %d\n", res_explicit);
  assert(res_explicit == 220);

  // Method pointer to Fooclass::method1
  auto method1_ptr = &Fooclass::method1;
  // Cast to pointer (undefined behaviour)
  void *raw1_ptr = (void *&)method1_ptr;
  // Shift pointer
  void *raw2_ptr = (char *)raw1_ptr + offset;

  UnknownMethod unknown_ptr = nullptr;
  // Replace method pointer with a shifted pointer (undefined behaviour)
  memcpy(&unknown_ptr, &raw2_ptr, sizeof(void *));
  printf("Address of method1 %p, method2 %p\n", raw1_ptr, raw2_ptr);

	// Old weird syntax for method pointer calls
  int res_offset = ((instance).*(unknown_ptr))(2, 20);
  printf("Result of offset call %d\n", res_offset);
  assert(res_offset == 220);

#if __cplusplus > 201703L
  // More sane call by `std::invoke` in C++17
  res_offset = std::invoke(unknown_ptr, instance, 2, 20);
  printf("Result of offset call %d\n", res_offset);
  assert(res_offset == 220);
#endif
}
