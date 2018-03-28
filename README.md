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
```
| broadcast [1] | ---SRC [7]--- |
```
Source address. Each client should have a unique 7-bit source address obtained either from EEPROM or by some form of hardware. Using a voltage divider with varying resistor values is suggested.

If the broadcast bit is 1, the packet is a broadcast packet, and is forwarded by all clients except for the sender. If the broadcast bit is 0, the next bit contains the destination address.

### DST (Optional)
If the broadcast bit in the SRC byte is set, the device with address DST should not forward the packet.

### SIZE
Size of the message. Since SIZE is an 8-bit value, message sizes are limited to 255 bytes. Null messages are possible, and should be interpreted and forwarded as appropriate by the network driver.

### MSG
Message contents. The first byte should contain identifying information for the message, and should not be 0xAA or 0x00. 0x00 is reserved for peer discovery; 0xAA is reserved to avoid potential confusion for a packet initializer.

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
	Send SRC, DST as appropriate
	Send SIZE
	Send MSG
	Send CHK
```

It is suggested that this module should be initialized with a send function and a recieve function as function pointers in order to maximize interoperability.

The user is responsible for calling update at least once every 500us either in a main loop, through a periodic interrupt, or through a UART buffer interrupt.
