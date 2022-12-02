#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t bitswap(uint8_t x);

class interrupts_off {
public:
    interrupts_off() {
        was = SREG;
        cli();
    }
    ~interrupts_off() {
        SREG = was;
    }
    uint8_t was;
};

void uart_write(uint8_t x)
{
    interrupts_off const uninterrupted{};

    auto const swapped = bitswap(x);
    auto const lo_dr = (swapped >> 2) | 0x80;
    auto const hi_dr = (swapped << 3) | 0x07;

    asm volatile(
        "out %[usidr], %[lo_dr]\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usidr], %[hi_dr]\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        "out %[usicr], %[shift]\n\t"        
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"

        :
        :   [usicr] "I" (_SFR_IO_ADDR(USICR)),
            [usidr] "I" (_SFR_IO_ADDR(USIDR)),
            [ddrb] "I" (_SFR_IO_ADDR(DDRB)),
            [lo_dr] "r" (lo_dr),
            [hi_dr] "r" (hi_dr),
            [shift] "r" (_BV(USIWM0) | _BV(USICLK))
    );
}

int main(void)
{
    // Adjust OSCCAL to get as close to an accurate 16 MHz as possible.
    // Set CKOUT fuse to put the CPU clock on a pin for measurement.
    OSCCAL -= 2;

    USIDR = 0xFF;
    USICR = _BV(USIWM0);
    DDRB |= _BV(PB1);

    while (true) {
        uart_write('h');
        uart_write('e');
        uart_write('l');
        uart_write('l');
        uart_write('o');
        _delay_ms(10);
    }
}
