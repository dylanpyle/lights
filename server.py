import time
import BaseHTTPServer
import serial

ser = serial.Serial('/dev/ttyUSB1', 9600)

HOST_NAME = '0.0.0.0'
PORT_NUMBER = 1337

class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
    def do_GET(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
        if(s.path == '/'):
            for line in open('index.html'):
                s.wfile.write(line)
        else:
            s.wfile.write("Loaded %s" % s.path)
            ser.write(s.path[1:])

if __name__ == '__main__':
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)
