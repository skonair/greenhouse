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
    self.send_response(200)
    # self.send_header('Content-type', 'application/json')           
    self.send_header('Content-type', 'text/plain')           
    self.end_headers()
    self.wfile.write("# TYPE greenhouse_temperature gauge\n");
    self.wfile.write("greenhouse_temperature " + words[3] + " \n" );
    self.wfile.write("# TYPE greenhouse_humidity gauge\n");
    self.wfile.write("greenhouse_humidity " + words[5] + " \n" );
    self.wfile.write("# TYPE greenhouse_brightness gauge\n");
    self.wfile.write("greenhouse_brightness " + words[7] + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_temperature gauge\n");
    self.wfile.write("greenhouse_bme_temperature " + words[9] + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_pressure gauge\n");
    self.wfile.write("greenhouse_bme_pressure " + words[11] + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_altitude gauge\n");
    self.wfile.write("greenhouse_bmw_altitude " + words[13] + " \n" );
    self.wfile.write("# TYPE greenhouse_bme_humidity gauge\n");
    self.wfile.write("greenhouse_bme_humidity " + words[15] + " \n" );

Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
Handler.do_GET = do_GET
httpd = SocketServer.TCPServer(("", PORT), Handler)
httpd.serve_forever()


