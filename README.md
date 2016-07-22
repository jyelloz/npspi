# npspi

control neopixels by misusing the SPI protocol functionality on FTDI FT232
serial adapters.

This is quite similar to the Python library
[Adafruit NeoPixel FTDI](https://github.com/tdicola/Adafruit_NeoPixel_FTDI)
except it is written in plain C.

It only has 5 functions and 1 opaque typedef so it should be quite easy to
build FFI-based bindings to other languages.
