from django.shortcuts import render
from django.http import JsonResponse
from django.core import serializers
from iot.models import Event
from iot.models import Venue_Event
from .forms import NameForm
# Create your views here.

def index(request):
    return render(request, 'data/index.html')

def venue_temp_data(request):
    objs = Venue_Event.objects.all()
    data = serializers.serialize('json', objs)
    return JsonResponse(data, safe=False)

def result(request):
    return render(request, 'data/result.html')