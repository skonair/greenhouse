#![feature(plugin)]
#![plugin(rocket_codegen)]

extern crate rocket;

#[get("/sensor/<name>")]
#[response(status = 200, content_type = "json")]
fn get_sensor_data(name: String) -> String {
  "[16, 19, 21.2, 20.2, 19.2, 18.7]"
}

fn main() {
  println!("Hello, world!");
  rocket::ignite()
    .mount("/greenhouse", routes![get_sensor_data])
    .launch();
}
