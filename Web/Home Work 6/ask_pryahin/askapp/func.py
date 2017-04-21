from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger
from askapp.models import AnswerLike, QuestionLike
import numpy as np

def pagination(request, data, count, page):
    paginator = Paginator(data, count)
    get = request.GET.get('page')
    if get:
        page = int(get)
    try:
        paginator_datas_list = paginator.page(int(page))
    except PageNotAnInteger:
        data_list = paginator.page(1)
    except EmptyPage:
        paginator_datas_list = paginator.page(paginator.num_pages)
    return paginator_datas_list

def randomTags(Tag):
    tags = []
    for i in range(Tag.objects.count()):
        tags.append(str(Tag.objects.get(id=i + 1)))
    np.random.shuffle(tags)
    return tags[:10]

def randomUsers(User):
    users = []
    for i in range(User.objects.count()):
        users.append(str(User.objects.get(id=i + 1)))
    np.random.shuffle(users)
    return users[:5]

def getQuestionPageLikes(question_, answers, User):
    likes = []
    dislikes = []
    for ans in answers:
        try:
            query = AnswerLike.objects.get(answer=ans, author=User)
        except:
            query = None
        if query is not None:
            if query.rating == 1:
                likes.append(ans.id)
            else:
                dislikes.append(ans.id)
    try:
        query = QuestionLike.objects.get(question=question_, author=User)
        if query.rating == 1:
            islike=1
        else:
            islike=-1
    except:
        islike=0;

    result = [likes, dislikes, islike]

    return result

def getQuestionListLikes(questions, User):
    likes = []
    dislikes = []

    for question in questions:
        try:
            query = QuestionLike.objects.get(question=question, author=User)
        except:
            query = None

        if query is not None:
            if query.rating == 1:
                likes.append(question.id)
            else:
                dislikes.append(question.id)
    result = [likes, dislikes]
    return result
