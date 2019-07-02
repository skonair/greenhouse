extern crate chrono;

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
use std::ptr::hash;
use chrono::offset::TimeZone;
use itertools::Itertools;

#[derive(Debug)]
struct Measure {
    chunk: i64,
    value: f32,
}

fn main() {
    let mut writer = BufWriter::new(io::stdout());
    let now = Utc::now();
    writeln!(writer, "Now is {:?}", now);

    let duration = 1440;
    let delta = 5;
    let max_chunks = 288;

    let f = File::open("./data/test_t1.txt").unwrap();
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
        let chunk_no = duration.num_minutes() / delta;

        let measure = Measure { chunk: chunk_no, value: result };
        ve.push(measure);
    }

    let x = ve.iter()
        .group_by(|m| m.chunk)
        .into_iter()
        .map(|(ge0, group)| (ge0, mean(group.collect())))
        .collect::<Vec<(i64, f32)>>();

    println!("{:?}", x);



}


fn mean(numbers: Vec<&Measure>) -> f32 {
    let sum: f32 = numbers.iter().map(|m| m.value).sum();
    sum as f32 / numbers.len() as f32
}
