from django.core.management.base import BaseCommand, CommandError

from askapp.models import Question, Tag

from random import choice, randint
import os

class Command(BaseCommand):
    help = 'Creates fake tags'

    def add_arguments(self, parser):
        parser.add_argument('--number',
                action='store',
                dest='number',
                default=3,
                help='Number of tags for a question'
        )

    def handle(self, *args, **options):
        tags = [
                'javascript', 'java', 'php', 'android', 'jquery', 'python',
                'html', 'css', 'c++', 'ios', 'mysql',  'sql',
                'iphone', 'angularjs', 'regexp', 'mail.ru',
                'technopark', 'WEB', 'gunicorn', 'nginx',
                'apache', 'django', 'valgrind', 'PHP',
                'HTML'
                ]

        for tag in tags:
            if len(Tag.objects.filter(text=tag)) == 0:
                t = Tag()

                t.text = tag 
                t.save()

        number = int(options['number'])

        tags = Tag.objects.all()

        for q in Question.objects.all():
            self.stdout.write('question [%d]' % q.id)
            if len(q.tags.all()) < number:
                for i in range(0, number - len(q.tags.all())):
                    t = choice(tags)

                    if t not in q.tags.all():
                        q.tags.add(t)