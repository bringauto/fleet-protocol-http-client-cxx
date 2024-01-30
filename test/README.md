# Fleet HTTP Client - TESTS

Google Tests

## Requirements

[gtest](https://github.com/google/googletest)

## Build
```
mkdir -p _build_tests && cd _build_tests
cmake ../ -DBRINGAUTO_TESTS=ON
make
```

## Run
```
./fleethttpclient_tests
```