#![feature(proc_macro_hygiene, decl_macro)]

#[macro_use] extern crate rocket;

#[get("/sensor/<name>")]
fn get_sensor_data(name: String) -> String {
  "[16, 19, 21.2, 20.2, 19.2, 18.7]".to_string()
}

fn main() {
  println!("Hello, world!");
  rocket::ignite()
    .mount("/greenhouse", routes![get_sensor_data])
    .launch();
}
