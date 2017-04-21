from cgi import parse_qsl, escape

def application(environ, start_response):
    output = [b"Post:",
              b"<form method='post'>"
              b"<input type='text' name = 'text'>"
              b"<input type='submit' value='Submit'>"
              b"</form>"]

    if environ['REQUEST_METHOD'] == 'POST':
        request=environ['wsgi.input'].read()
        dict_value = dict(parse_qsl(request))
        output.append(b"<h1>Post data:</h1>")
        output.append(bytes("Value = %s <br>" % (dict_value[b"text"]).decode("UTF-8"), encoding="UTF-8"))

    if environ['REQUEST_METHOD'] == 'GET':
        if environ['QUERY_STRING'] != '':
            dict_value = parse_qsl(environ['QUERY_STRING'])
            output.append(b"<h1>Get data:</h1>")
            for key,value in dict_value:
                if len(value) == 1:
                    output.append(bytes("%s = %s <br>" % (key, value[0]), encoding="UTF-8"))
                else:
                    output.append(bytes("%s = %s <br>" % (key, value), encoding="UTF-8"))

    output_len = sum(len(line) for line in output)
    start_response('200 OK', [('Content-type', 'text/html'), ('Content-Length', str(output_len))])
    return output
