from django.contrib.auth import authenticate
from django.contrib.auth.models import User
from django import forms

from django.contrib.auth.hashers import make_password
from askapp.models import Profile, Question, Tag, Answer

import urllib, requests, re

from django.core.files.base import ContentFile
from django.forms import Field
from django.utils.translation import ugettext_lazy


Field.default_error_messages = {
    'required': ugettext_lazy(u'Это поле необходимо заполнить!'),
}

class LoginForm(forms.Form):
    login = forms.CharField(
        widget=forms.TextInput(
            attrs={'class': 'form-control', 'placeholder': 'Login', }
        ),
        max_length=30,
        label=u'Логин'
    )
    password = forms.CharField(
        widget=forms.PasswordInput(
            attrs={'class': 'form-control', 'placeholder': '*******', }
        ),
        label=u'Пароль'
    )

    def clean(self):
        data = self.cleaned_data
        try:
            username_in_db = User.objects.get(username__iexact=data.get('login', ''))
            user = authenticate(username=username_in_db, password=data.get('password', ''))

            if user.is_active:
                data['user'] = user
            else:
                raise forms.ValidationError(u'Данный пользователь не активен')
        except:
            raise forms.ValidationError(u'Указан неверный логин или пароль')

class SignupForm(forms.Form):
    username = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=30,
        label=u'Логин'
    )
    first_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=30,
        label=u'Имя'
    )
    last_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=30,
        label=u'Фамилия'
    )
    email = forms.EmailField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=254, 
        label=u'Email',
        error_messages={'invalid':'Введен некорректный email адресс!'}
    )
    password = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'form-control form-control-danger'}),
        min_length=6,
        label=u'Пароль',
        error_messages={'min_length':'Пароль должен быть не менее 6 символов(Введено: %(show_value)d)'}
    )
    password_repeat = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'form-control form-control-danger'}),
        required=False,
        min_length=6,
        label=u'Повторите пароль',
        error_messages={'min_length':'Пароль должен быть не менее 6 символов(Введено: %(show_value)d)'}

    )
    avatar = forms.FileField(
        widget=forms.ClearableFileInput(attrs={'class': 'custom-file-input'}),
        required=False,
        label=u'Загрузите фото'
    )

    def clean_username(self):
        username = self.cleaned_data.get('username', '')

        try:
            u = User.objects.get(username__iexact=username)
            raise forms.ValidationError(u'Такой пользователь уже существует')
        except User.DoesNotExist:
            return username

    def clean_email(self):
        email = self.cleaned_data.get('email', '')
        try:
            u = User.objects.get(email__iexact=email)
            raise forms.ValidationError(u'Данная почта уже зарегистрирована!')
        except User.DoesNotExist:
            return email

    def clean_password_repeat(self):
        pass1 = self.cleaned_data.get('password', '')
        pass2 = self.cleaned_data.get('password_repeat', '')

        if pass1 != pass2:
            raise forms.ValidationError(u'Пароли не совпадают')

    def save(self):
        data = self.cleaned_data
        password = data.get('password')
        u = User()

        u.username = data.get('username')
        u.password = make_password(password)
        u.email = data.get('email')
        u.first_name = data.get('first_name')
        u.last_name = data.get('last_name')
        u.is_active = True
        u.is_superuser = False
        u.save()

        up = Profile()
        up.user = u

        if data.get('avatar') is None:
            image_url = 'http://api.adorable.io/avatars/100/%s.png' % u.username
            response = requests.get(image_url)

            up.avatar.save('%s.png' % u.username, ContentFile(response.content), save=True)
        else:
            avatar = data.get('avatar')
            up.avatar.save('%s_%s' % (u.username, avatar.name), avatar, save=True)

        up.save()

        return authenticate(username=u.username, password=password)

class ProfileEditForm(forms.Form):
    def __init__(self, user, initial, data=None, files=None):
        self.user = user
        super(ProfileEditForm, self).__init__(data=data, files=files, initial=initial)

    first_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger',}),
        max_length=30,
        label=u'Имя',
    )
    last_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=30,
        label=u'Фамилия',
    )
    email = forms.EmailField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        max_length=254, 
        label=u'Email',
        error_messages={'invalid':'Введен некорректный email адресс!'},
    )
    password_old = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'form-control form-control-danger'}),
        min_length=6,
        label=u'Старый пароль',
        error_messages={'min_length':'Пароль должен быть не менее %(limit_value)d символов(Введено: %(show_value)d)'},
        required=False
    )
    password = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'form-control form-control-danger'}),
        min_length=6,
        label=u'Новый пароль',
        error_messages={'min_length':'Пароль должен быть не менее %(limit_value)d символов(Введено: %(show_value)d)'},
        required=False
    )
    password_repeat = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'form-control form-control-danger'}),
        min_length=6,
        label=u'Повторите пароль',
        error_messages={'min_length':'Пароль должен быть не менее %(limit_value)d символов(Введено: %(show_value)d)'},
        required=False
    )
    avatar = forms.FileField(
        widget=forms.ClearableFileInput(attrs={'class': 'custom-file-input'}),
        label=u'Загрузите фото',
        required=False
    )

    def clean_email(self):
        cur_email = self.user.email.lower()
        email = self.cleaned_data.get('email', '').lower()
        if cur_email != email:
            try:
                u = User.objects.get(email__iexact=email)
                raise forms.ValidationError(u'Данная почта уже зарегистрирована!')
            except User.DoesNotExist:
                return email
        return email

    def clean_password_old(self):
        pass_old = self.cleaned_data.get('password_old', '')
        if pass_old != '':
            if not self.user.check_password(pass_old):
                raise forms.ValidationError(u'Неправильно введен старый пароль!')

    def clean_password_repeat(self):
        pass1 = self.cleaned_data.get('password', '')
        pass2 = self.cleaned_data.get('password_repeat', '')

        if pass1 != pass2:
            raise forms.ValidationError(u'Пароли не совпадают')

    def save(self):
        data = self.cleaned_data
        user = self.user
        user.first_name = data.get('first_name')
        user.last_name = data.get('last_name')
        user.email = data.get('email')

        pass1 = self.cleaned_data.get('password', '')
        if pass1 != '':
            user.set_password(pass1)

        user.save()

        up = user.profile
        up.info = data.get('info')

        if data.get('avatar') is not None:
            avatar = data.get('avatar')
            up.avatar.save('%s_%s' % (user.username, avatar.name), avatar, save=True)

        up.save()

        return self

class AskForm(forms.Form):
    title = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'form-control form-control-danger'}),
        min_length=3,
        label=u'Заголовок',
        error_messages={'min_length': u'Заголовок должен быть не менее %(limit_value)d символов', 'required': 'Вопрос должен иметь заголовок!'},
    )
    question = forms.CharField(
        widget=forms.Textarea(attrs={'class': 'form-control form-control-danger'}),
        min_length=3,
        label=u'Текст',
        error_messages={'min_length': u'Вопрос должен быть не менее %(limit_value)d символов', 'required': 'Введите вопрос!'},
    )
    tags = forms.CharField(
        widget=forms.TextInput(attrs={'class':'form-control'}),
        label=u'Теги(через пробел)',
        required=False
    )

    def save(self, user):
        data = self.cleaned_data
        title=data.get('title','')
        text=data.get('question','')
        _tags=data.get('tags','')

        q = Question()    
        q.title = title
        q.text = text
        q.author = user
        q.save()

        # q = Question.objects.create(title=title,text=text,author=user)

        if _tags!='':
            tags = re.split('\s+', _tags)
            print(tags)
            for tag in tags:
                print(tag)
                t = Tag.objects.get_or_create(text=tag)[0]
                q.tags.add(t)

        return q

class AnswerForm(forms.Form):
    answer = forms.CharField(
        widget=forms.Textarea(attrs={'class': 'form-control', 'rows': '3', 'placeholder': 'Введите свой ответ здесь...'}),
        min_length=2,
        label=u'Текст',
        error_messages={'min_length': u'Ответ должен быть не менее %(limit_value)d символов', 'required': 'Введите ответ!'},
    )

    def save(self, question, user):
        data = self.cleaned_data
        answer = data.get('answer','')

        # ans = Answer()
        # ans.author = user
        # ans.question = question
        # ans.text = answer

        # ans.save()

        ans = question.answer_set.create(text=answer, author=user)

        question.count_answers += 1
        question.save()

        # ans = question.answer_set.create(text=ans.text, author=ans.author)

        return ans

