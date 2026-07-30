import http.server
import subprocess
import tempfile
import os

PORT = 8000

class CompilerHandler(http.server.BaseHTTPRequestHandler):

    def do_GET(self):
        if self.path == '/' or self.path == '/index.html':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            with open('index.html', 'rb') as f:
                self.wfile.write(f.read())
        else:
            self.send_response(404)
            self.end_headers()

    def do_POST(self):
        if self.path == '/compile':
            length = int(self.headers.get('Content-Length', 0))
            source_code = self.rfile.read(length).decode('utf-8')

            # Source code ta ekta temporary .mc file e save kori,
            # karon amader compiler ekta file path expect kore
            with tempfile.NamedTemporaryFile(mode='w', suffix='.mc', delete=False) as f:
                f.write(source_code)
                temp_path = f.name

            try:
                # Already-build kora ./compiler executable ta chalai,
                # ar tar output (stdout + stderr) dhore rakhi
                result = subprocess.run(
                    ['./compiler', temp_path],
                    capture_output=True,
                    text=True,
                    timeout=5
                )
                output = result.stdout + result.stderr
                if output.strip() == '':
                    output = '(no output)'
            except Exception as e:
                output = f'Error running compiler: {e}'
            finally:
                os.unlink(temp_path)

            self.send_response(200)
            self.send_header('Content-type', 'text/plain; charset=utf-8')
            self.end_headers()
            self.wfile.write(output.encode('utf-8'))
        else:
            self.send_response(404)
            self.end_headers()

    def log_message(self, format, *args):
        # Terminal e onek beshi log ashe na, sudhu essential ta dekhabo
        pass


if __name__ == '__main__':
    server = http.server.HTTPServer(('localhost', PORT), CompilerHandler)
    print(f'Server running at http://localhost:{PORT}')
    print('Press Ctrl+C to stop.')
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print('\nServer stopped.')
