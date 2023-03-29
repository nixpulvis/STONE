macro_rules! buffer_of_size {
    ($size: expr) => {
        [0u8; $size]
    };
}
pub(crate) use buffer_of_size;

macro_rules! log_to {
    ($logger: expr, $base: expr) => {
        $logger.log($base);
    };
}
pub(crate) use log_to;

macro_rules! log_to_fmt {
    ($msg_buf: expr, $logger: expr, $base: expr, $($args:tt),*) => {{
        $msg_buf.fill(0x00);
        display_utils::collect_str(&mut $msg_buf, format_args!($base, $($args),*)).unwrap();
        $logger.log(&core::str::from_utf8(&$msg_buf).unwrap());
    }};
}
pub(crate) use log_to_fmt;