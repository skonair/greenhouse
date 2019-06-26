extern crate nrf24l01;

use std::time::Duration;
use std::thread::sleep;

use nrf24l01::{RXConfig, NRF24L01, OperatingMode};

fn main() {
    let config = RXConfig {
        channel: 0x00,
        pipe0_address: *b"1Node",
        ..Default::default()
    };

    println!("Config pipe0_address: {:x?}", config.pipe0_address);
    let mut device = NRF24L01::new(25, 0).unwrap();
    device.configure(&OperatingMode::RX(config)).unwrap();
    device.flush_input().unwrap();
    device.listen().unwrap();
    println!("Start loop...");
    loop {
        if device.data_available().unwrap() {
            device
                .read_all(|packet| {
                    println!("Received {:?} bytes", packet.len());
                    println!("Payload {:?}", packet);
                })
                .unwrap();
        }
        sleep(Duration::from_millis(500));
    }
}
