# Generating client code

To regenerate the Cpp-rest-openapi-client lib, use the `regen.sh` script (npm, git and [OpenAPI Generator](https://openapi-generator.tech/docs/installation/) are required). It uses the openapi.yaml file placed in this directory (openapi) as an input. The most up-to-date version can be found in the [fleet-protocol-http-api project](https://github.com/bringauto/fleet-protocol-http-api).

## Necessary changes

The generated code does not work properly without some adjustments:

- timestamp for Message model needs to be changed to int64_t, otherwise it overflows
- functions listCommands, listStatuses and availableCars need their since parameter changed to int64_t
- Payload_data needs m_Json added to it to store data
- the Payload_data header generates with a non existent include

This is taken care of by the `regen.sh` script. Changes to the input specification might make it not work, so be careful when generating from new openapi.yaml files.