# GPS NMEA 0183 Parser

Platform independent GPS NMEA parser for embedded systems.

<h3>Read first: <a href="http://docs.majerle.eu/projects/gps-nmea-parser/">Documentation</a></h3>

## Features

* Written in ANSI C99
* Platform independent, easy to use
* Built-in support for 4 GPS statements
    * ``GPGGA`` or ``GNGGA``: GPS fix data
    * ``GPGSA`` or ``GNGSA``: GPS active satellites and dillusion of position
    * ``GPGSV`` or ``GNGSV``: List of satellites in view zone
    * ``GPRMC`` or ``GNRMC``: Recommended minimum specific GPS/Transit data
* Optional ``float`` or ``double`` floating point units
* Low-level layer is separated from application layer, thus allows you to add custom communication with GPS device
* Works with operating systems
* Works with different communication interfaces
* User friendly MIT license

## Contribute

Fresh contributions are always welcome. Simple instructions to proceed::

1. Fork Github repository
2. Respect [C style & coding rules](https://github.com/MaJerle/c-code-style) used by the library
3. Create a pull request to develop branch with new features or bug fixes

Alternatively you may:

1. Report a bug
2. Ask for a feature request

## Test

To build the code and run basic tests on your host::

        cd examples
        make test
