use arduino_nano33iot::hal;
use hal::adc::Adc;
use hal::prelude::*;
use hal::gpio::v2::pin::*;
use hal::hal::digital::v2::{OutputPin, ToggleableOutputPin, InputPin};
use super::error::PinError;

pub struct AnalogPin<C> {
    pub pin: C,
}

pub trait AnalogInput {
    fn read(&mut self, adc: &mut Adc<atsamd21g::ADC>) -> Result<u16, PinError>;
}

macro_rules! is_analog_input {
    ($d_id:ident, $port:ident) => {
        impl<'a, I, M> AnalogPin<Pin<I, M>>
        where I: PinId, M: PinMode {
            pub fn $d_id(pin: Pin<I, M>) -> AnalogPin<Pin<I, Alternate<B>>> {
                AnalogPin{pin: pin.into_alternate()}
            }
        }

        impl AnalogInput for AnalogPin<Pin<$port, AlternateB>> {
            fn read(&mut self, adc: &mut Adc<atsamd21g::ADC>) -> Result<u16, PinError> {
                match adc.read(&mut self.pin) {
                    Ok(val) => Ok(val),
                    Err(_) => Err(PinError::ReadError)
                }
            }
        }
    };
}

macro_rules! get_input {
    ($stone:expr, $port:ident) => {
        base::components::analog_pins::AnalogPin::$port($stone.pins.$port)
    };
}
pub(crate) use get_input;

is_analog_input!(a0, PA02);
is_analog_input!(a1, PB02);
is_analog_input!(a2, PA11);
is_analog_input!(a3, PA10);
is_analog_input!(sda, PB08);
is_analog_input!(scl, PB09);
is_analog_input!(a6, PA09);
is_analog_input!(a7, PB03);
is_analog_input!(aref, PA03);