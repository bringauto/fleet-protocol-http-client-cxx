#!/bin/bash

pushd lib/bringauto-fleet-http-client-generated
npx @openapitools/openapi-generator-cli generate -i ../../openapi/openapi.yaml -g cpp-restsdk -o . --package-name bringauto-fleet-http-client-generated
popd
git apply openapi/generated-code.patch