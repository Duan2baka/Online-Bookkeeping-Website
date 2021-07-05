from django.shortcuts import render
from . import iot_mqtt
from .models import Event
# Create your views here.

def index(request):
    objects = Event.objects.all()
    dic = {'objects' : objects}
    return render(request, 'iot/index.html', dic)