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
    var html = '';
    for(let i = 0; i < 1000; i++) {
      html += '<p>hello awtk</p>\n';
    }
    response.writeHead(200, {'Content-Type': 'text/html'})
    setTimeout(function() {
      response.end(`\ndata: ${html} \ndelay: ${delay}\n`);
    }, delay);
  }
})

const port = 8000
const host = '127.0.0.1'
server.listen(port, host)

console.log(`Listening at http://${host}:${port}`)

