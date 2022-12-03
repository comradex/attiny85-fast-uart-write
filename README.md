# ATTiny85 Fast(ish) UART write

A minor waste of time demonstrating using the ATTiny85 USI peripheral to
transmit UART at high rates (2 Mbaud or faster).

There's an [app note][appnote] for that. Their solution is interrupt-based,
which is good for low speed, but bad at high speed. The one-byte USIDR register
means that they must reload the register half way through the UART frame. The
minimum interrupt delay in the ATTiny85 CPU, according to the
[datasheet][datasheet], is 4 cycles; 4 cycles is the whole bit time at 4 Mbaud
(and 16 MHz F_CPU).

Instead, I've written cycle-counted transmits for 16, 8, 4, and 2 Mbaud. My
scope decodes all of them. My USB-to-UART is a CP2104 and only supports 2 Mbaud,
so that's as fast as I've confirmed works with another device.

I manually adjusted OSCCAL for my particular chip. The internal RC oscillator
is not especially accurate from the factory, but it seems stable enough (at
a constant temperature) to stay within the timing margin of my CP2104 at least.
In the scope screenshots below, the blue trace is the clock output from the
ATTiny85, enabled by setting the CKOUT fuse.

![2 MBaud scope trace](./scope-2Mbaud.png)

![4 MBaud scope trace](./scope-4Mbaud.png)

![8 MBaud scope trace](./scope-8Mbaud.png)

![16 MBaud scope trace](./scope-16Mbaud.png)

[appnote]: http://ww1.microchip.com/downloads/en/Appnotes/doc4300.pdf
[datasheet]: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf