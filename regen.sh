#!/bin/bash

pushd lib/Cpp-rest-openapi-client
npx @openapitools/openapi-generator-cli generate -i ../../openapi.yaml -g cpp-restsdk -o .
popd
git apply generated-code.patch