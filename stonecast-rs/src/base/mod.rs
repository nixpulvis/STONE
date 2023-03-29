#![allow(dead_code)]
#![allow(unused_imports)]
#![allow(unused_macros)]

pub mod prelude;

use arduino_nano33iot as bsp;
pub use bsp::entry;

mod stone;
pub use stone::Stone;

pub mod components;
pub mod sensors;
pub mod indicators;

pub mod msg_buffer;