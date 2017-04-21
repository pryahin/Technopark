from django.shortcuts import render, render_to_response, get_object_or_404
from django.contrib.auth.models import User
from askapp.models import *
from askapp.func import *
# Create your views here.

def index(request, page='1'):
    questions = pagination(request, Question.objects.newest(), 20, page)
    questions.paginator.baseurl = "/"
    return render_to_response('index.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'questions' : questions, 'isAuthorized' : True})

def hot(request, page='1'):
    questions = pagination(request, Question.objects.hot(), 20, page)
    questions.paginator.baseurl = "/hot/"
    return render_to_response('index.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'questions' : questions, 'isAuthorized' : True})

def questions_by_tag(request, tag, page='1'):
    tag_questions = Question.objects.byTag(tag)
    questions = pagination(request, tag_questions, 20, page)
    questions.paginator.baseurl = "/tag/" + tag + "/"
    return render_to_response('question_by_tag.html', {'tag': tag, 'questions': questions, 'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized': True})

def questions_by_user(request, user, page='1'):
    user_ = get_object_or_404(User, username=user)
    user_questions = Question.objects.byUser(user)
    questions = pagination(request, user_questions, 20, page)
    questions.paginator.baseurl = "/user/" + user + "/"
    return render_to_response('question_by_user.html', {'user': user, 'questions': questions, 'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized': True})

def question(request, id):
    question_ = get_object_or_404(Question, pk=id)
    answers = question_.answer_set.all()
    return render_to_response('question.html', {'question': question_, 'answers': answers, 'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized': True})


###########################
def login(request):
    return render_to_response('login.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized' : 0 })

def signup(request):
    return render_to_response('signup.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized' : 0})

def ask(request):
    return render_to_response('ask.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized' : True})

def settings(request):
    return render_to_response('settings.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'isAuthorized' : True})
