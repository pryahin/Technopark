"""ask_pryahin URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from django.conf import settings
from . import views

import django.views.defaults

def handler404(request):
    response = render_to_response('404.html', {}, context_instance=RequestContext(request))
    response.status_code = 404
    return response

urlpatterns = [
               url(r'^$', views.index, name='index'),
               url(r'^hot/', views.hot, name='hot'),
               url(r'^tag/(?P<tag>\w+)/?$', views.questions_by_tag, name='questions_by_tag'),
               url(r'^question/(?P<id>\d+)/?$', views.question, name='question'),
               url(r'^login/', views.login, name='login'),
               url(r'^signup/', views.signup, name='signup'),
               url(r'^ask/', views.ask, name='ask'),

               url(r'^settings/', views.settings, name='settings'),
]

