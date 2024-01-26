# Necessary changes

- timestamp for Message model needs to be of int64_t type, otherwise it overflows
- Payload_data needs m_Json to store data
