#!/bin/bash

pushd lib/bringauto-fleet-http-client-generated
npx @openapitools/openapi-generator-cli generate -i ../../openapi.yaml -g cpp-restsdk -o . --package-name bringauto-fleet-http-client-generated
popd
git apply generated-code.patch