from django.core.management.base import BaseCommand, CommandError
from django.contrib.auth.models import User
from django.contrib.auth.hashers import make_password
from askapp.models import Profile

from faker import Factory

import requests
from django.core.files.base import ContentFile

class Command(BaseCommand):
    help = 'Fill users'

    def add_arguments(self, parser):
        parser.add_argument('--number',
                action='store',
                dest='number',
                default=5,
                help='Number of count users to add'
        )

    def handle(self, *args, **options):
        fake = Factory.create()

        number = int(options['number'])

        for i in range(0, number):
            profile = fake.simple_profile()
            u = User()
            
            u.username = profile['username']
            u.first_name = fake.first_name()
            u.last_name = fake.last_name()
            u.email = profile['mail']
            u.password = make_password('Qwerty123456')
            u.is_active = True
            u.is_superuser = False 
            u.save() 

            up = Profile()
            up.user = u
            
            image_url = 'http://api.adorable.io/avatars/100/%s.png' % u.username
            response = requests.get(image_url)

            up.avatar.save('%s.png' % u.username, ContentFile(response.content), save=True)

            up.save()

            self.stdout.write('[%d] added user %s' % (u.id, u.username))