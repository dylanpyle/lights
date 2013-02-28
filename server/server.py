import time
import BaseHTTPServer
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

PORT_NUMBER = 1337

class LightServer(BaseHTTPServer.BaseHTTPRequestHandler):
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
      ser.write(s.path[1:] + '\n')

if __name__ == '__main__':
  server_class = BaseHTTPServer.HTTPServer
  httpd = server_class(('0.0.0.0', PORT_NUMBER), LightServer)
  print time.asctime(), "Begin."
  try:
    httpd.serve_forever()
  except KeyboardInterrupt:
    pass
  httpd.server_close()
  print time.asctime(), "End."
