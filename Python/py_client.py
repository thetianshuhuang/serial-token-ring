# pc_client.py
# python client for interacting with the serial token ring protocol
#
# For use with the Serial Token Ring communication protocol
# https://github.com/thetianshuhuang/serial-token-ring


import serial
import time


class token_ring_client():

    def __init__(self, port):

        """
        Class initialization

        Parameters
        ----------
        port
            Port that the serial device is connected to.
        """

        self.connect_device(port, 115200, 1, 1)

        self.MSG = ""
        self.SRC = 0
        self.DST = 0
        self.broadcast = False
        self.current_index = 0
        self.remaining_bytes = 0
        self.recieving_packet = False

    # copied from serial-vis/serial_lib/base_device.py
    def connect_device(self, path, baudrate, rx_timeout, tx_timeout):

        """
        Attempt to connect a serial device.
        Returns
        -------
        bool
            Success (True) or failure (False).
        """

        # open serial interface
        try:
            # initialize device
            self.device = serial.Serial(
                path,
                baudrate,
                rx_timeout,
                tx_timeout)
            print("Device connected: " + path + "\n")
            # flush potentially incomplete commands from buffer
            self.device.flushInput()

            # return success
            return(True)

        except serial.serialutil.SerialException:
            # limit the error message to once every 2.5 seconds.
            if(self.next_time < time.time()):
                print("Serial device " +
                      path +
                      " not found. Trying again.")

                self.next_time = time.time() + 2.5

            # return failure
            return(False)

    def update_network(self):

        """
        Update network function, as described by the protocol.

        Returns
        -------
        byte array
            Recieved values
        """

        if(self.device.inWaiting() > 0):
            if(self.recieving_packet):

                # first byte == SRC
                if(self.current_index == 0):
                    # set SRC
                    self.SRC = self.device.read(1)
                    # check for broadcast
                    if(self.char_buffer & 0x80):
                        self.broadcast = True
                    else:
                        self.broadcast = False

                # second byte == DST if broadcast not set
                elif(self.current_index == 1):
                    # skip DST
                    if(self.broadcast):
                        self.current_index += 1
                    # read DST
                    else:
                        self.DST = self.device.read(1)

                # third byte == SIZE
                elif(self.current_index == 2):
                    self.remaining_bytes = ord(self.device.read(1))

                # fourth byte and beyod == message
                elif(self.remaining_bytes > 0):
                    self.MSG += self.device.read(1)
                    self.remaining_bytes -= 1

                # checksum
                else:
                    self.CHK = self.device.read(1)
                    self.recieving_packet = False
                    # process packet here

                self.current_index += 1

            elif(self.device.read(1) == 0xAA):
                self.recieving_packet = True

        return("")
