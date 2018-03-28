# Serial Token Ring Network

## Protocol
Clients should be connected via UART at 115200 baud in a token-ring topology. Packets should consist of at least 3 bytes:
```
| 0xAA | SRC | <DST> | SIZE | ---MSG--- | CHK |
```
### 0xAA
Mesage begin header; all packets must begin with 0xAA.

This is to minimize the impact of packets with misplaced SIZE values; if a previous packet dropped two bytes, without a message begin byte to check, the size byte would be interpreted as the source, and the first byte of the message would be interpreted as the SIZE value. If the message began with 0xFF or another large number, this could cause cascades of dropped packets.

### SRC
Source 