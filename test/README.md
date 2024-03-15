# Fleet HTTP Client - TESTS

Google Tests

## Requirements

[gtest](https://github.com/google/googletest)

The tests try to connect to the api on localhost:8080. Nothing should run on port 8080, failed connection is expected.

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