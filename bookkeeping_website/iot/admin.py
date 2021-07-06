from django.contrib import admin
from .models import Event
from .models import Venue_Event

# Register your models here.

admin.site.register(Event)
admin.site.register(Venue_Event)