use arduino_nano33iot::hal::adc::Adc;
use crate::base::prelude::*;

// Describes TMP36 sensor connected to 3.3V
// https://cdn.sparkfun.com/datasheets/Sensors/Temp/TMP35_36_37.pdf

const VOLTAGE_OFFSET: f32 = 0.5;
const MAX_VOLTAGE: f32 = 3300.0; // mV
const MAX_SIGNAL: f32 = 1023.0;
const MAX_TEMP: f32 = 125.0; // Celsius
const MIN_TEMP: f32 = -40.0; // Celsius
const TEMP_RANGE: f32 = MAX_TEMP - MIN_TEMP;

macro_rules! from {
    ($stone:expr, $pin: ident) => {{
        let pin = analog_pins::get_input!($stone, $pin);
        sensors::TemperatureSensor::new(pin)
    }};
}
pub(crate) use from;

pub struct TemperatureSensor<C> {
    pub pin: C
}

impl<C> TemperatureSensor<C> {
    pub fn new(pin: AnalogPin<C>) -> TemperatureSensor<AnalogPin<C>> {
        TemperatureSensor{pin: pin}
    }
}

fn translate_temp_f(temp: f32) -> f32 {
    let actual_voltage_mhz = (temp / MAX_SIGNAL) * MAX_VOLTAGE;

    // Now we'll convert the voltage to degrees Celsius.
    // This formula comes from the temperature sensor datasheet:

    let degrees_c = (actual_voltage_mhz - 500.0) / 10.0;

    // While we're at it, let's convert degrees Celsius to Fahrenheit.
    // This is the classic C to F conversion formula:

    let degrees_f = degrees_c * (9.0/5.0) + 32.0;

    return degrees_f;
}

impl<C> TemperatureSensor<C> where C: AnalogInput {
    pub fn get_temperature_f(&mut self, adc: &mut Adc<atsamd21g::ADC>) -> Result<f32, PinError> {
        return match self.pin.read(adc) {
            Ok(val) => Ok(translate_temp_f(val as f32)),
            Err(_) => Err(PinError::ReadError)
        }
    }
}
