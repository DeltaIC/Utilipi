#!/usr/bin/env python

"""
================================================
ABElectronics IO Pi | Digital I/O Write Demo

Requires python smbus to be installed
For Python 2 install with: sudo apt-get install python-smbus
For Python 3 install with: sudo apt-get install python3-smbus

run with: python demo_iowrite.py
================================================

This example uses the write_pin and write_port methods to switch the pins
on and off on the I/O bus.

"""
from __future__ import absolute_import, division, print_function, \
                                                    unicode_literals
import time

try:
    from MCP23017 import MCP23017
except ImportError:
    print("Failed to import MCP23017 from python system path")
    print("Importing from parent folder instead")
    try:
        import sys
        sys.path.append('..')
        from MCP23017 import MCP23017
    except ImportError:
        raise ImportError(
            "Failed to import library from parent folder")


def main():
    '''
    Main program function
    '''

    # Create an instance of the IOPi class with an I2C address of 0x20
    iobus = MCP23017(0x24)

    # We will write to the pins 9 to 16 so set port 1 to be outputs turn off
    # the pins
    iobus.set_port_direction(1, 0x00)
    iobus.write_port(1, 0x00)

    while True:

        time.sleep(0.1)
        iobus.write_pin(16, 1)

        time.sleep(0.1)
        iobus.write_pin(16, 0)

        # repeat until the program ends

if __name__ == "__main__":
    main()
