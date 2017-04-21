from django.core.management.base import BaseCommand, CommandError

from django.contrib.auth.models import User
from askapp.models import Question, QuestionLike, Answer, AnswerLike

from random import choice, randint

class Command(BaseCommand):
    help = 'Creates likes'

    def add_arguments(self, parser):
        parser.add_argument('--number',
                action='store',
                dest='number',
                default=20,
                help='Number of likes'
        )

    def handle(self, *args, **options):
        number = int(options['number'])

        users = User.objects.all()[1:]
        questions = Question.objects.all()
        answers = Answer.objects.all()


        for q in questions:
            self.stdout.write('question [%d]' % q.id)
            for i in range(0, randint(1,number)):
                QuestionLike.objects.add_or_update(author=choice(users),question=q,value=choice([-1, 1]))

        for ans in answers:
            self.stdout.write('answer [%d]' % ans.id)
            for i in range(0, randint(1,number)):
                AnswerLike.objects.add_or_update(author=choice(users),answer=ans,value=choice([-1, 1]))