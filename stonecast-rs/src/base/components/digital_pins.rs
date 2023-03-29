use arduino_nano33iot::hal;
use hal::prelude::*;
use hal::gpio::v2::{PA17, Pin, PushPullOutput};
pub use hal::gpio::v2::pin::AnyPin;
pub use hal::hal::digital::v2::{OutputPin, ToggleableOutputPin, InputPin};
use super::error::PinError;

pub struct DigitalPin<C> {
    pub pin: C,
}

pub trait GPIOOutput {
    fn toggle(&mut self) -> Result<(), PinError>;
    fn set_high(&mut self) -> Result<(), PinError>;
    fn set_low(&mut self) -> Result<(), PinError>;
}

pub trait GPIOInput {
    fn is_high(&self) -> Result<bool, PinError>;
    fn is_low(&self) -> Result<bool, PinError>;
}


impl<P> GPIOOutput for DigitalPin<P> where P: ToggleableOutputPin + OutputPin,
{
    fn toggle(&mut self) -> Result<(), PinError> {
        match self.pin.toggle() {
            Ok(_) => Ok(()),
            Err(_) => Err(PinError::WriteError)
        }
    }

    fn set_high(&mut self) -> Result<(), PinError> {
        match self.pin.set_high() {
            Ok(_) => Ok(()),
            Err(_) => Err(PinError::WriteError)
        }
    }

    fn set_low(&mut self) -> Result<(), PinError> {
        match self.pin.set_low() {
            Ok(_) => Ok(()),
            Err(_) => Err(PinError::WriteError)
        }
    }
}

impl<P> GPIOInput for DigitalPin<P> where P: InputPin {
    fn is_high(&self) -> Result<bool, PinError> {
        match self.pin.is_high() {
            Ok(val) => Ok(val),
            Err(_) => Err(PinError::ReadError)
        }
    }

    fn is_low(&self) -> Result<bool, PinError> {
        match self.pin.is_low() {
            Ok(val) => Ok(val),
            Err(_) => Err(PinError::ReadError)
        }
    }
}

macro_rules! is_gpio {
    ($d_id:ident, $port:ident) => {
        impl<M> DigitalPin<M>
        where M: AnyPin {
            pub fn $d_id(pin: M) -> Self {
                DigitalPin{pin: pin}
            }
        }
    };
}

is_gpio!(d2, PB10);
is_gpio!(d3, PB11);
is_gpio!(d4, PA07);
is_gpio!(d5, PA05);
is_gpio!(d6, PA04);
is_gpio!(d7, PA06);
is_gpio!(d8, PA18);
is_gpio!(d9, PA20);
is_gpio!(d10, PA21);
is_gpio!(d11, PA16);
is_gpio!(d12, PA19);
is_gpio!(d13, PA17);
is_gpio!(d14, PA02);
is_gpio!(d15, PB02);
is_gpio!(d16, PA11);
is_gpio!(d17, PA10);
is_gpio!(d18, PB08);
is_gpio!(d19, PB09);
is_gpio!(d20, PA09);
is_gpio!(d21, PA03);
is_gpio!(led_sck, PA17);

macro_rules! push_pull_output {
    ($stone:expr, $port:ident) => {
        base::components::digital_pins::DigitalPin::$port($stone.pins.$port.into_push_pull_output())
    };
}
pub(crate) use push_pull_output;

macro_rules! readable_output {
    ($stone:expr, $port:ident) => {
        base::components::digital_pins::DigitalPin::$port($stone.pins.$port.into_readable_output())
    };
}
pub(crate) use readable_output;

macro_rules! pull_down_input {
    ($stone:expr, $port:ident) => {
        base::components::digital_pins::DigitalPin::$port($stone.pins.$port.into_pull_down_input())
    };
}
pub(crate) use pull_down_input;

macro_rules! pull_up_input {
    ($stone:expr, $port:ident) => {
        base::components::digital_pins::DigitalPin::$port($stone.pins.$port.into_pull_up_input())
    };
}
pub(crate) use pull_up_input;