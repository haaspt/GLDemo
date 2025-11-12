# Unit Tests

This directory contains unit tests for the GLDemo project using Google Test (GTest).

## Running Tests

### Using CMake/CTest (Recommended)
```bash
cd build
ctest
```

### Running the test executable directly
```bash
cd build
./tests
```

### Running specific tests
```bash
cd build
./tests --gtest_filter=Vector3Test.*          # Run all Vector3 tests
./tests --gtest_filter=TransformTest.Perspective  # Run specific test
```

## Test Structure

- `test_vector.cpp` - Tests for Vector2, Vector3, and Vector4 classes
- `test_transform.cpp` - Tests for Transform class and matrix operations

## Adding New Tests

1. Add your test to the appropriate test file (or create a new one)
2. Follow the Google Test naming convention: `TEST(TestSuiteName, TestName)`
3. Rebuild: `cd build && make tests`
4. Run: `./tests`

## Example Test

```cpp
TEST(Vector3Test, MyNewTest) {
    Vector3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}
```

