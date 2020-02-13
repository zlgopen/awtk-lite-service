const http = require('http')

const server = http.createServer(function(request, response) {
  let delay = Math.random() * 3000;
  console.log(request.method);
  if (request.method == 'POST' || request.method == 'PUT') {
    var body = ''
    request.on('data', function(data) {
      body += data
    })
    request.on('end', function() {
      response.writeHead(200, {'Content-Type': 'text/html'})
      setTimeout(function() {
        response.end(`\ndata: ${body} \ndelay: ${delay}\n`);
      }, delay);
    })
  } else if (request.method == 'DELETE') {
    response.writeHead(200, {'Content-Type': 'text/html'})
    setTimeout(function() {
      response.end(`\ndelete done \ndelay: ${delay}\n`);
    }, delay);
  } else {
    var html = '<p>hello awtk</p>\n';
    var done = 'done\n';
    var n = 100000;
    var size = n * html.length + done.length;

    response.writeHead(200, {'Content-Type': 'text/html', 'Content-Length': size})

    for(let i = 0; i < n; i++) {
      response.write(html);
    }

    setTimeout(function() {
      response.end(done);
    }, delay);
  }
})

const port = 8000
const host = '127.0.0.1'
server.listen(port, host)

console.log(`Listening at http://${host}:${port}`)

