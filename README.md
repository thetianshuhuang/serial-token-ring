# Serial Ring Network

## Protocol
Clients should be connected via UART at 115200 baud in a ring topology. Packets should consist of at least 3 bytes:
```
| 0xAA | SRC | DST | SIZE | ---MSG--- | CHK |
```
### 0xAA
Mesage begin header; all packets must begin with 0xAA.

This is to minimize the impact of packets with misplaced SIZE values; if a previous packet dropped two bytes, without a message begin byte to check, the size byte would be interpreted as the source, and the first byte of the message would be interpreted as the SIZE value. If the message began with 0xFF or another large number, this could cause cascades of dropped packets.

### SRC
Source address. Each client should have a unique 8-bit source address obtained either from EEPROM or by some form of hardware. Using a voltage divider with varying resistor values is suggested.

### DST
Destination address. Packets should not be forwarded by their destination. 0x00 is reserved for peer discovery and broadcast.

### SIZE
Size of the message. Since SIZE is an 8-bit value, message sizes are limited to 255 bytes. Null messages are possible, and should be interpreted and forwarded as appropriate by the network driver.

### MSG
Message contents. Since a SIZE identifier is used to specify the message length, null characters are allowed in the message.

The first byte in peer discovery messages should be reserved for an application identifier.

### CHK
8-bit checksum. Computed as the XOR of all bytes in the message, except for the message initializer.

## Suggested implementation

Since this protocol is designed to operate on embedded systems, impelmentations should be platform-independent where possible. Two functions should be defined:

```
update:
	Read input buffer, and append to the function buffer
	If no message is in the input buffer, discard all bytes until 0xAA is recieved.
	Check to see if the message has been finished. If not, return a null string.
	Compute the checksum of the message; if it does not match the recieved checksum, discard the message
	Terminate the message if SRC or DST match the device address
	Forward the message if not terminated
	Return the recieved message if SRC[7] = 1 (broadcast) or DST matches the device address.
	Return a null string

send:
	Send 0xAA
	Begin keeping track of all sent bytes for the checksum
	Send SRC, DST, SIZE, MSG
	Send CHK
```

It is suggested that this module should be initialized with a send function and a recieve function as function pointers in order to maximize interoperability.

The user is responsible for calling update at least once every 500us either in a main loop, through a periodic interrupt, or through a UART buffer interrupt.
