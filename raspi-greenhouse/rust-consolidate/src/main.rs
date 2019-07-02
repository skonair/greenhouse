#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use] extern crate rocket;
#[macro_use] extern crate serde_derive;

extern crate chrono;

use rocket_contrib::json::{Json};


use std::collections::HashMap;
use std::fs::File;
use std::io;
use std::io::BufRead;
use std::io::BufReader;
use std::io::BufWriter;
use std::io::Write;
use std::time::{Duration, SystemTime};

use chrono::{DateTime, NaiveDate, NaiveDateTime, NaiveTime, Utc};
use chrono::format::ParseError;
use chrono::offset::TimeZone;
use itertools::Itertools;

#[derive(Serialize, Deserialize, Debug)]
struct SensorData {
    sensor: String,
    sensor_type: String,
    icon: String,
    data: Vec<f32>,
    current_value: f32,
    unit: String
}

#[derive(Debug)]
struct Measure {
    chunk: i32,
    value: f32,
}

fn get_data(sensor: &str) -> Vec<f32> {
    let now = Utc::now();
    println!("Now is {:?}", now);

    let duration = 1440;
    let delta = 5;
    let max_chunks = 288;

    let filename = format!("./data/test_{}.txt", sensor);
    let f = File::open(filename).unwrap();
    let file = BufReader::new(&f);
    let mut ve: Vec<Measure> = vec![];
    for (num, line) in file.lines().enumerate() {
        let l = line.unwrap();

        let v: Vec<&str> = l.split(" ").collect();
        let sdt = format!("{} {}", v[0], v[1]);
        let result = v[3].to_string().parse::<f32>().unwrap();

        let custom = NaiveDateTime::parse_from_str(&sdt, "%Y-%m-%d %T");
        let utc_custom = DateTime::<Utc>::from_utc(custom.unwrap(), Utc);
        let duration = now.signed_duration_since(utc_custom);
        let chunk_no: i32 = (duration.num_minutes() / delta) as i32;

        let measure = Measure { chunk: chunk_no, value: result };
        ve.push(measure);
    }

    let x: HashMap<i32, f32> = ve.iter()
        .group_by(|m| m.chunk)
        .into_iter()
        .map(|(ge0, group)| (ge0, mean(group.collect())))
        .collect::<HashMap<i32, f32>>();

    println!("{:?}", x);

    let mut last_v = 20.0 as f32;
    let range: Vec<f32> = (0..max_chunks).map(|i| {
        let idx = max_chunks - i;
        let v = x.get(&idx).cloned().unwrap_or(last_v);
        last_v = v;
        v
    }).collect();

    range

}


fn mean(numbers: Vec<&Measure>) -> f32 {
    let sum: f32 = numbers.iter().map(|m| m.value).sum();
    sum as f32 / numbers.len() as f32
}

#[get("/sensors", format = "json")]
fn all_sensor_data() -> Json<Vec<SensorData>> {
    let data = get_data(&"t1");

    Json(vec![
        SensorData {
            sensor: "t1".to_string(),
            sensor_type: "Temperature".to_string(),
            icon: "home".to_string(),
            data: data,
            current_value: 25.5, // data.last().cloned().unwrap_or(0.0 as f32),
            unit: "°C".to_string()
        }
    ])
}

#[get("/sensor/<_name>")]
fn get_sensor_data(_name: String) -> Json<SensorData> {
    let data = get_data(_name.as_str());

    Json(SensorData {
        sensor: _name,
        sensor_type: "Temperature".to_string(),
        icon: "home".to_string(),
        data: data,
        current_value: 25.5, // data.last().cloned().unwrap_or(0.0 as f32),
        unit: "°C".to_string()
    })
}

fn main() {
    println!("Greenhouse rocket launched!");
    rocket::ignite()
        .mount("/greenhouse", routes![all_sensor_data, get_sensor_data])
        .launch();
}