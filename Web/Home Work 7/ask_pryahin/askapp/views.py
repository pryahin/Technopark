from django.shortcuts import render, render_to_response, get_object_or_404
from django.contrib.auth.models import User
from askapp.models import *
from askapp.func import *
from askapp.forms import *

from django.contrib import auth
from django.http import HttpResponse, HttpResponseRedirect, Http404
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse

import json

# Create your views here.

def index(request, page='1'):
    questions = pagination(request, Question.objects.newest(), 20, page)
    questions.paginator.baseurl = "/"
    likes = getQuestionListLikes(questions, request.user)
    return render(request, 'index.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'questions' : questions, 'likes':likes[0], 'dislikes':likes[1]})

def hot(request, page='1'):
    questions = pagination(request, Question.objects.hot(), 20, page)
    questions.paginator.baseurl = "/hot/"
    return render(request, 'index.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'questions' : questions})

def questions_by_tag(request, tag, page='1'):
    tag_questions = Question.objects.byTag(tag)
    questions = pagination(request, tag_questions, 20, page)
    questions.paginator.baseurl = "/tag/" + tag + "/"
    likes = getQuestionListLikes(questions, request.user)
    return render(request, 'question_by_tag.html', {'tag': tag, 'questions': questions, 'tags': randomTags(Tag), 'users': randomUsers(User), 'likes':likes[0], 'dislikes':likes[1]})

def questions_by_user(request, user, page='1'):
    user_ = get_object_or_404(User, username=user)
    user_questions = Question.objects.byUser(user)
    questions = pagination(request, user_questions, 20, page)
    questions.paginator.baseurl = "/user/" + user + "/"
    likes = getQuestionListLikes(questions, request.user)
    return render(request, 'question_by_user.html', {'user': user, 'questions': questions, 'tags': randomTags(Tag), 'users': randomUsers(User), 'likes':likes[0], 'dislikes':likes[1]})

def question(request, id):
    question_ = get_object_or_404(Question, pk=id)
    answers = question_.answer_set.all()
    
    likes=getQuestionPageLikes(question_,answers,request.user)

    if request.method == "POST":
        form=AnswerForm(request.POST)
        if form.is_valid():
            a = form.save(question_, request.user)
            return HttpResponseRedirect('%s#comment%d' % (reverse('question', kwargs={'id': question_.id}), a.id))
    else:
        form=AnswerForm()
    return render(request, 'question.html', {'question': question_, 'answers': answers, 'tags': randomTags(Tag), 'users': randomUsers(User), 'form': form, 'likes': likes[0], 'dislikes': likes[1], 'islike': likes[2]})


###########################
def login(request):
    redirect = request.GET.get('continue', '/')
    if request.user.is_authenticated():
        return HttpResponseRedirect(redirect)
    if request.method == "POST":
        form = LoginForm(request.POST)
        if form.is_valid():
            auth.login(request, form.cleaned_data['user'])
            return HttpResponseRedirect(redirect)
    else:
        form = LoginForm()
    return render(request, 'login.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'form': form})

@login_required
def logout(request):
    redirect = request.GET.get('continue', '/')
    auth.logout(request)
    return HttpResponseRedirect(redirect)


def signup(request):
    if request.user.is_authenticated():
        return HttpResponseRedirect('/')
    if request.method == "POST":
        form = SignupForm(request.POST, request.FILES)
        if form.is_valid():
            user = form.save()
            auth.login(request, user)
            return HttpResponseRedirect('/')
    else:
        form = SignupForm()
    return render(request, 'signup.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'form': form})

@login_required
def settings(request):
    data = {
        'first_name': request.user.first_name,
        'last_name': request.user.last_name,
        'email': request.user.email
    }
    if request.method == "POST":
        form = ProfileEditForm(user=request.user, initial=data, data=request.POST, files=request.FILES)
        if form.is_valid():
            user = form.save()
    else:
        form = ProfileEditForm(user=request.user, initial=data)
    return render(request, 'settings.html', {'tags': randomTags(Tag), 'users': randomUsers(User), 'form' : form})

@login_required
def ask(request):
    if request.method == "POST":
        form = AskForm(request.POST)
        if form.is_valid():
            q = form.save(request.user)
            return HttpResponseRedirect(reverse('question', kwargs={'id': q.id}))
            # return HttpResponseRedirect('/')
    else:
        form = AskForm()
    return render(request, 'ask.html', {'tags': randomTags(Tag), 'users': randomUsers(User),'form': form})

@login_required
def ajax_question_like(request, id):
    if request.method == "POST":
        user = request.user
        value = int(request.POST.get('value'))
        question = get_object_or_404(Question, pk=id)
        # if QuestionLike.objects.isliked(question,user) not is None:

        if value == 1:
            like = QuestionLike.objects.add_or_update(user, question, 1)
            content_dis = "<span class=\"page-like-img dislikebtn\"/>"
            content_like = "<span class=\"page-like-img islikebtn\"/>"
        elif value == -1:
            like = QuestionLike.objects.add_or_update(user, question, -1)
            content_dis = "<span class=\"page-like-img isdislikebtn\"/>"
            content_like = "<span class=\"page-like-img likebtn\"/>"

        content = {'like': content_like, 'dislike': content_dis}
        ratingid = '#qRating' + str(question.id)
        response = {'ratingid': ratingid, 'rating': like[0].question.rating, 'content': content, 'qid': request.POST.get('qid')}

        return HttpResponse(json.dumps(response), content_type='application/json')
    raise Http404()

@login_required
def ajax_answer_like(request, id):
    if request.method == "POST":
        user = request.user
        value = int(request.POST.get('value'))
        answer = get_object_or_404(Answer, pk=id)
        # if QuestionLike.objects.isliked(question,user) not is None:

        if value == 1:
            like = AnswerLike.objects.add_or_update(user, answer, 1)
            content_dis = "<span class=\"page-like-img dislikebtn\"/>"
            content_like = "<span class=\"page-like-img islikebtn\"/>"
        elif value == -1:
            like = AnswerLike.objects.add_or_update(user, answer, -1)
            content_dis = "<span class=\"page-like-img isdislikebtn\"/>"
            content_like = "<span class=\"page-like-img likebtn\"/>"

        content = {'like': content_like, 'dislike': content_dis}
        ratingid = '#aRating' + str(answer.id)
        response = {'ratingid': ratingid, 'rating': like[0].answer.rating, 'content': content, 'aid': request.POST.get('aid')}

        return HttpResponse(json.dumps(response), content_type='application/json')
    raise Http404()

@login_required
def ajax_answer_correct(request, id):
    if request.method == "POST":
        user = request.user
        answer = get_object_or_404(Answer, pk=id)
        # if QuestionLike.objects.isliked(question,user) not is None:

        if answer.question.author == user:
            answer.is_correct = True
            answer.question.isVote = False
            answer.save()
            answer.question.save()

            content = {'new': "<div class=\"answer-correct\"> Лучший ответ!</div>"}
            response = {'result': 'true', 'content': content, 'aid': request.POST.get('aid')}
        else:
            response = {'result': 'false'}

        return HttpResponse(json.dumps(response), content_type='application/json')
    raise Http404()
