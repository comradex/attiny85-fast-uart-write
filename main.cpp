#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

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

    auto const ddrb_do_on = DDRB | _BV(PB1) | _BV(PB2);
    auto const ddrb_do_off = ddrb_do_on & ~(_BV(PB1) | _BV(PB2));

    // No interrupts. I'm clocking out by cycle counting.
    // Using three wire mode.
    USICR = _BV(USIWM0);

    asm volatile(
        "out %[usidr], %[lo_dr]\n\t"

        "out %[ddrb], %[ddrb_do_on]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "nop\n\t"
        "out %[usidr], %[hi_dr]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[usicr], %[clk]\n\t"        
        "out %[usicr], %[clk_shift]\n\t"

        "out %[ddrb], %[ddrb_do_off]\n\t"
        :
        :   [usicr] "I" (_SFR_IO_ADDR(USICR)),
            [usidr] "I" (_SFR_IO_ADDR(USIDR)),
            [ddrb] "I" (_SFR_IO_ADDR(DDRB)),
            [ddrb_do_on] "r" (ddrb_do_on),
            [ddrb_do_off] "r" (ddrb_do_off),
            [lo_dr] "r" (lo_dr),
            [hi_dr] "r" (hi_dr),
            [clk] "r" (_BV(USIWM0) | _BV(USITC)),
            [clk_shift] "r" (_BV(USIWM0) | _BV(USITC) | _BV(USICLK))
    );

    USICR = 0x00;
}

int main(void)
{
    // Adjust OSCCAL to get as close to an accurate 16 MHz as possible.
    // Set CKOUT fuse to put the CPU clock on a pin for measurement.
    OSCCAL -= 2;

    // Set pull-ups for when the USI is disabled.
    PORTB |= _BV(PB1) | _BV(PB2);

    while (true) {
        uart_write('h');
        uart_write('e');
        uart_write('l');
        uart_write('l');
        uart_write('o');
        _delay_ms(10);
    }
}
