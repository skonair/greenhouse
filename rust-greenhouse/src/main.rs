#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use] extern crate rocket;

#[get("/sensors")]
fn all_sensor_data() -> String {
  "                {
                    sensor: 'Top',
                    type: 'Temperature',
                    icon: 'home',
                    data: [24.1, 24.5, 24.7, 24.8, 25.1, 25.5],
                    currentValue: 25.5,
                    unit: '°C'
                },
                {
                    sensor: 'Middle',
                    type: 'Temperature',
                    icon: 'home',
                    data: [20.1, 21.3, 22.1, 22.3, 23.2, 23.4],
                    currentValue: 23.4,
                    unit: '°C'
                },
                {
                    sensor: 'Bottom',
                    type: 'Temperature',
                    icon: 'home',
                    data: [16, 19, 21.2, 20.2, 19.2, 18.7],
                    currentValue: 18.7,
                    unit: '°C'
                },
                {
                    sensor: 'Hum Layer 1',
                    type: 'Humidity',
                    icon: 'local_bar',
                    data: [16, 19, 21.2, 20.2, 19.2, 18.7],
                    currentValue: 18.7,
                    unit: '%'
                }".to_string()
}

#[get("/sensor/<_name>")]
fn get_sensor_data(_name: String) -> String {
                format!("{{
                    sensor: '{sensor_name}',
                    type: 'Temperature',
                    icon: 'home',
                    data: [24.1, 24.5, 24.7, 24.8, 25.1, 25.5],
                    currentValue: 25.5,
                    unit: '°C'
                }}", sensor_name = _name)
}

fn main() {
  println!("Hello, world!");
  rocket::ignite()
    .mount("/greenhouse", routes![all_sensor_data, get_sensor_data])
    .launch();
}
