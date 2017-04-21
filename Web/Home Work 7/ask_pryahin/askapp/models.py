from django.db import models
from django.contrib.auth.models import User, UserManager
from django.utils import timezone
from django.db.models import Sum

from askapp import comet

# Create your models here.

class QuestionManager(models.Manager):
    def hot(self):
        return self.order_by('-rating')

    def newest(self):
        return self.order_by('-date')

    def byTag(self, input_tag):
        return self.filter(tags__text__iexact = input_tag)

    def byUser(self, user_name):
        return self.filter(author__username__iexact = user_name)



class ProfileManager(models.Manager):
    def getName(self, user_name):
        return self.filter(user__username = user_name)
    def getUsername(self, id):
        return self.filter(user__id = id)

class QuestionLikeManager(models.Manager):
    def filter_by_question(self, question):
        return self.filter(question=question)

    def isliked(self, question, user):
        return self.filter(question=question, author=user)

    def sum_likes(self, question):
        return self.filter_by_question(question).aggregate(sum=Sum('rating'))['sum']

    def add_or_update(self, author, question, value):
        new = self.update_or_create(author=author,question=question,defaults={'rating': value})

        question.rating = self.sum_likes(question)
        question.save()
        return new

class AnswerLikeManager(models.Manager):
    def filter_by_user(self, answer, user):
        return self.get(answer=answer, author=user)

    def filter_by_answer(self, answer):
        return self.filter(answer=answer)

    def sum_likes(self, answer):
        return self.filter_by_answer(answer).aggregate(sum=Sum('rating'))['sum']

    def add_or_update(self, author, answer, value):
        new = self.update_or_create(author=author,answer=answer,defaults={'rating': value})

        answer.rating = self.sum_likes(answer)
        answer.save()
        return new

class AnswerManager(models.Manager):
    def create(self, **kwargs):
        ans = super(AnswerManager, self).create(**kwargs)

        comet.comet_send_message(
            str(ans.question.id),
            ans.id,
            ans.author.username,
            ans.author.profile.avatar.url,
            ans.text,
            ans.question.isVote,
            ans.question.author.id,
            ans.question.count_answers
        )
        return ans

# class TagManager(models.Manager):
#     def getPopularTags(self):
#         return self.order_by(Count('question'))

class Tag(models.Model):
    class Meta:
        verbose_name = u'Тег'
        verbose_name_plural = u'Теги'
    
    text = models.CharField(max_length=15, verbose_name='Tag')
    
    def __str__(self):
        return self.text

class Question(models.Model):
    class Meta:
        verbose_name = u'Вопрос'
        verbose_name_plural = u'Вопросы'

    author =  models.ForeignKey(User, verbose_name=u'Пользователь')
    title = models.CharField(max_length = 100, verbose_name=u'Заголовок')
    text = models.TextField(verbose_name=u'Текст')
    rating = models.IntegerField(default = 0, verbose_name=u'Рейтинг')
    count_answers = models.IntegerField(default = 0, verbose_name=u'Количество ответов')
    tags = models.ManyToManyField(Tag, verbose_name=u'Тег')
    isVote = models.BooleanField(default = True, verbose_name=u'Голосование')
    date = models.DateTimeField(default = timezone.now, verbose_name=u'Дата создания')

    objects = QuestionManager()

    def __str__(self):
        return self.text

class Answer(models.Model):
    class Meta:
        verbose_name = u'Ответ'
        verbose_name_plural = u'Ответы'

    author = models.ForeignKey(User, verbose_name=u'Пользователь')
    question = models.ForeignKey(Question, verbose_name=u'Вопрос')
    text = models.TextField(verbose_name=u'Текст')
    rating = models.IntegerField(default = 0, verbose_name=u'Рейтинг')
    is_correct = models.BooleanField(default = False, verbose_name=u'Корректность')
    date = models.DateTimeField(default = timezone.now, verbose_name=u'Дата создания')

    objects = AnswerManager()

class Profile(models.Model):
    class Meta:
        verbose_name=u'Профиль'
        verbose_name_plural=u'Профили'
    
    user = models.OneToOneField(User, verbose_name=u'Пользователь')
    avatar = models.ImageField(upload_to='avatars')
    
    objects = ProfileManager()
    
    def __str__(self):
        return self.user

class QuestionLike(models.Model):
    class Meta:
        verbose_name = u'Рейтинг вопроса'
        verbose_name_plural = u'Рейтинг вопросов'
    
    question = models.ForeignKey(Question, default = False, verbose_name=u'Вопрос')
    author = models.ForeignKey(User, verbose_name=u'Пользователь')
    rating = models.IntegerField(default = 0, db_index=True, verbose_name=u'Рейтинг')

    objects = QuestionLikeManager()

    def __str__(self):
        return self.rating

class AnswerLike(models.Model):
    class Meta:
        verbose_name = u'Рейтинг ответа'
        verbose_name_plural = u'Рейтинг ответов'
    
    answer = models.ForeignKey(Answer, default = False, verbose_name=u'Ответ')
    author = models.ForeignKey(User, verbose_name=u'Пользователь')
    rating = models.IntegerField(default = 0, db_index=True, verbose_name=u'Рейтинг')

    objects = AnswerLikeManager()
    
    def __str__(self):
        return self.rating
