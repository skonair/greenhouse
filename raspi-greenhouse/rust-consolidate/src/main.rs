extern crate chrono;

use chrono::{DateTime, NaiveDate, NaiveDateTime, NaiveTime, Utc};
use chrono::format::ParseError;

use std::collections::HashMap;
use std::fs::File;
use std::io;
use std::io::BufReader;
use std::io::BufRead;
use std::io::BufWriter;
use std::io::Write;
use std::time::{Duration, SystemTime};



fn main() {   
  let mut writer = BufWriter::new(io::stdout());
  let now = Utc::now();
  writeln!(writer, "Now is {:?}", now);

  let duration = 1440;
  let delta = 5;
  let max_chunks = 288;

  let f = File::open("./data/test.txt").unwrap();
  let file = BufReader::new(&f);
  for (num, line) in file.lines().enumerate() {
    let l = line.unwrap();

    let mut split = l.split(" ");
    let v: Vec<&str> = split.collect();
    let mut iter = v.chunks(2);

    let dt = iter.next().unwrap();
    writeln!(writer, "date found {}", dt[0]);
    writeln!(writer, "time found {}", dt[1]);
    let sdt = format!("{} {}", dt[0], dt[1]);
    writeln!(writer, "Sdt is -{}-", &sdt);

    let custom = NaiveDateTime::parse_from_str(&sdt, "%Y-%m-%d %T");
    writeln!(writer, "Custom value is {:?}", custom);

    let utcCustom = DateTime::<Utc>::from_utc(custom.unwrap(), Utc);

    let duration = now.signed_duration_since(utcCustom);
    writeln!(writer, "Duration: {:?}", duration);
    writeln!(writer, "As whole minutes: {:?}", duration.num_minutes());

    let chunkNo = duration.num_minutes() / delta;
    writeln!(writer, "chunkNo is {}", chunkNo);

    if (chunkNo > max_chunks) {
      // skip because too old
    } else {
      // add to hashmap 
    }

    for elem in iter {
      writeln!(writer, "Key {}, Value {}", elem[0], elem[1]);
    }

  }       
}
