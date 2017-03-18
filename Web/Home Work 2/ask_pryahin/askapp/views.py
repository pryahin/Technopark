from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from cgi import parse_qsl, escape

# Create your views here.

@csrf_exempt
def getpost(request):
    response =  "Post:" \
                "<form method='post'>" \
                "<input type='text' name = 'text'>" \
                "<input type='submit' value='Submit'>" \
                "</form>"
    if request.method == 'POST':
        response += "<h1>Post data:</h1>"
        response += "Value = %s <br>" % (request.POST.get('text'))
    elif request.method == 'GET':
        if request.GET:
            response += "<h1>Get data:</h1>"
            for key,value in request.GET.items():
                if len(value) == 1:
                    response += "%s = %s <br>" % (key, value[0])
                else:
                    response += "%s = %s <br>" % (key, value)
    return HttpResponse(response, content_type="text/html", status=200)

