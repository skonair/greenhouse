#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use] extern crate rocket;
#[macro_use] extern crate serde_derive;

use rocket_contrib::json::{Json};

#[derive(Serialize, Deserialize)]
struct SensorData {
  sensor: String,
  sensortype: String,
  icon: String,
  data: Vec<f32>,
  currentValue: f32,
  unit: String
}

#[get("/sensors", format = "json")]
fn all_sensor_data() -> Json<Vec<SensorData>> {
  Json(vec![
    SensorData {
      sensor: "Top Temp".to_string(),
      sensortype: "Temperature".to_string(),
      icon: "home".to_string(),
      data: vec![24.1, 24.5, 24.7, 24.8, 25.1, 25.5],
      currentValue: 25.5,
      unit: "°C".to_string() },
    SensorData {
      sensor: "Top Humi".to_string(),
      sensortype: "Humidity".to_string(),
      icon: "local_bar".to_string(),
      data: vec![28.1, 27.5, 26.7, 25.8, 24.1, 23.5],
      currentValue: 23.5,
      unit: "%".to_string() },
    SensorData {
      sensor: "Bottom Temp".to_string(),
      sensortype: "Temperature".to_string(),
      icon: "home".to_string(),
      data: vec![22.1, 21.5, 24.7, 28.8, 25.1, 21.5],
      currentValue: 21.5,
      unit: "°C".to_string() },
    SensorData {
      sensor: "Bottom Humi".to_string(),
      sensortype: "Humidity".to_string(),
      icon: "local_bar".to_string(),
      data: vec![22.1, 21.5, 26.7, 23.8, 21.1, 20.5],
      currentValue: 50.5,
      unit: "%".to_string() }
  ])
}
  
#[get("/sensor/<_name>")]
fn get_sensor_data(_name: String) -> Json<SensorData> {
  Json(SensorData {
    sensor: _name,
    sensortype: "Temperature".to_string(),
    icon: "home".to_string(),
    data: vec![24.1, 24.5, 24.7, 24.8, 25.1, 25.5],
    currentValue: 25.5,
    unit: "°C".to_string() })
}

fn main() {
  println!("Hello, world!");
  rocket::ignite()
    .mount("/greenhouse", routes![all_sensor_data, get_sensor_data])
    .launch();
}
