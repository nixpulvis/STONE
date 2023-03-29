use crate::base::prelude::*;
use crate::base::components::digital_pins::DigitalPin;
use arduino_nano33iot::hal::delay::Delay;
use crate::base::components::digital_pins::*;
use crate::base::components::error::PinError;

macro_rules! from {
    ($stone:expr) => {{
        let pin = digital_pins::push_pull_output!($stone, led_sck);
        indicators::BuiltInLED::new(pin)
    }};
}
pub(crate) use from;

pub struct BuiltInLED<C> {
    pub pin: DigitalPin<C>
}

impl<C> BuiltInLED<C> where C: ToggleableOutputPin + OutputPin {
    pub fn new(pin: DigitalPin<C>) -> BuiltInLED<C> {
        BuiltInLED{pin: pin}
    }

    pub fn blink(&mut self, delay: &mut Delay) -> Result<(), PinError> {
        let mut result = self.pin.toggle();

        if result.is_ok() { delay.delay_ms(1000u16); }
        if result.is_ok() { result = self.pin.toggle(); }
        if result.is_ok() { delay.delay_ms(1000u16); }
        return result;
    }

    pub fn toggle(&mut self) -> Result<(), PinError> {
        return self.pin.toggle();
    }

    pub fn set_high(&mut self) -> Result<(), PinError> {
        return self.pin.set_high();
    }

    pub fn set_low(&mut self) -> Result<(), PinError> {
        return self.pin.set_low();
    }
}