use arduino_nano33iot as bsp;

use atsamd21g::{NVMCTRL, PM, SYSCTRL, TC5, USB, NVIC};
use bsp::{hal, Pins};
use hal::adc::Adc;
use hal::delay::Delay;
use hal::clock::GenericClockController;
use hal::pac::{CorePeripherals, Peripherals};

pub struct Stone {
    /// Power manager
    pub (crate) pm: PM,
    // Timer Counter 5
    pub (crate) tc5: TC5,
    /// USB
    pub (crate) usb: USB,
    /// Delay Timer
    pub (crate) delay: Delay,
    /// System control
    pub (crate) sysctrl: SYSCTRL,
    /// Non-volitile memory controller
    pub (crate) nvmctrl: NVMCTRL,
    // Nested vector interrupt controller
    pub (crate) nvic: NVIC,
    /// Clock
    pub (crate) clock: GenericClockController,
    /// Pins
    pub (crate) pins: Pins,
    // ADC (Analog -> Digital Converter)
    pub (crate) adc: Adc<atsamd21g::ADC>,
}

impl Stone {
    pub fn take() -> Stone {
        let mut peripherals = Peripherals::take().unwrap();
        let core = CorePeripherals::take().unwrap();

        let mut clocks = GenericClockController::with_internal_32kosc(
            peripherals.GCLK,
            &mut peripherals.PM,
            &mut peripherals.SYSCTRL,
            &mut peripherals.NVMCTRL);

        let mut adc = Adc::adc(peripherals.ADC, &mut peripherals.PM, &mut clocks);
        // Nano 33 IOT has a 10bit resolution (0 to 1023)
        adc.resolution(bsp::hal::adc::Resolution::_10BIT);
        // Set reference to AREF pin (~3.3V) and 1x gain, matching analogReference(AR_EXTERNAL)
        // https://github.com/arduino/ArduinoCore-samd/blob/master/cores/arduino/wiring_analog.c#L105
        adc.reference(bsp::hal::adc::Reference::AREFA);
        adc.gain(bsp::hal::adc::Gain::_1X);
        adc.prescaler(bsp::hal::adc::Prescaler::DIV512);

        let pins = bsp::Pins::new(peripherals.PORT);
        let delay = Delay::new(core.SYST, &mut clocks);

        Stone {
            pm: peripherals.PM,
            usb: peripherals.USB,
            delay: delay,
            tc5: peripherals.TC5,
            sysctrl: peripherals.SYSCTRL,
            nvmctrl: peripherals.NVMCTRL,
            nvic: core.NVIC,
            clock: clocks,
            pins: pins,
            adc: adc,
        }
    }
}