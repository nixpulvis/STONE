#![no_std]
#![no_main]

use panic_halt as _;

mod base;
use base::prelude::*;
use base::{entry, Stone};
use base::components::{digital_pins, analog_pins, usb_logger};

#[entry]
fn main() -> ! {
    let mut stone = Stone::take();
    let usb_logger = usb_logger::from!(stone);

    let mut built_in_led = indicators::built_in_led::from!(stone);
    let mut ext_led = indicators::external_led::from!(stone, d10);
    let mut temp_sensor = sensors::temperature::from!(stone, a0);
    let mut msg_buf = msg_buffer::buffer_of_size!(100);

    // Just give some time for the serial monitor to start listening...
    stone.delay.delay_ms(500u32);
    msg_buffer::log_to!(usb_logger, "Initialized!!!\n");

    loop {
        built_in_led.blink(&mut stone.delay).unwrap();
        ext_led.blink(&mut stone.delay).unwrap();

        let temp_val = temp_sensor.get_temperature_f(&mut stone.adc).unwrap_or(0f32);
        if temp_val > 70.0 {
            ext_led.set_high().unwrap();
        } else {
            ext_led.set_low().unwrap();
        }

        msg_buffer::log_to_fmt!(msg_buf, usb_logger, "Temperature: {} degrees fahrenheit\n", temp_val);

        stone.delay.delay_ms(200u32);
    }
}
