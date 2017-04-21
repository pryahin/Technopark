from django.core.management.base import BaseCommand, CommandError

from django.contrib.auth.models import User
from askapp.models import Question

from random import choice, randint
from faker import Factory

class Command(BaseCommand):
    help = 'Fill questions'

    def add_arguments(self, parser):
        parser.add_argument('--number',
                action='store',
                dest='number',
                default=50,
                help='Number of questions to add'
        )

    def handle(self, *args, **options):
        fake = Factory.create()

        number = int(options['number'])

        users = User.objects.all()[1:]
        for i in range(0, number):
            q = Question()

            q.title = fake.sentence(nb_words=randint(2, 4), variable_nb_words=True)
            q.text = fake.paragraph(nb_sentences=randint(4, 17), variable_nb_sentences=True)
            q.author = choice(users)
            # q.rating = randint(0,1000)
            q.save()
