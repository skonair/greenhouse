# coding: utf-8
import SimpleHTTPServer
import SocketServer
import fileinput
import subprocess

PORT = 9999
lastline = "ll"

def do_GET(self):
    line = subprocess.check_output(['tail', '-n2', "../data/test_t1.txt" ]).splitlines()[0]
    words = line.split()
    temperature =  '%e' % float(words[3])
    humidity =  '%e' % float(words[5])
    brightness =  '%e' % float(words[7])
    bme_temperature =  '%e' % float(words[9])
    bme_pressure =  '%e' % float(words[11])
    bme_altitude =  '%e' % float(words[13])
    bme_humidity =  '%e' % float(words[15])
    self.send_response(200)
    # self.send_header('Content-type', 'application/json')           
    self.send_header('Content-type', 'text/plain')           
    self.end_headers()
    self.wfile.write("# TYPE greenhouse_temperature gauge\n");
    self.wfile.write("greenhouse_temperature " + temperature + " \n" );
    self.wfile.write("# TYPE greenhouse_humidity gauge\n");
    self.wfile.write("greenhouse_humidity " + humidity + " \n" );
    self.wfile.write("# TYPE greenhouse_brightness gauge\n");
    self.wfile.write("greenhouse_brightness " + brightness + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_temperature gauge\n");
    self.wfile.write("greenhouse_bme_temperature " + bme_temperature + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_pressure gauge\n");
    self.wfile.write("greenhouse_bme_pressure " + bme_pressure + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_altitude gauge\n");
    self.wfile.write("greenhouse_bmw_altitude " + bme_altitude + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_humidity gauge\n");
    self.wfile.write("greenhouse_bme_humidity " + bme_humidity + " \n" );

Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
Handler.do_GET = do_GET
httpd = SocketServer.TCPServer(("", PORT), Handler)
httpd.serve_forever()


