# Serial Token Ring Network

## Protocol
Clients should be connected via UART at 115200 baud in a token-ring topology.

### Packets
Packets should consist of at least 3 bytes.
```
| 0xAA | SRC | <DST> | SIZE | ---MSG--- | CHK |
```