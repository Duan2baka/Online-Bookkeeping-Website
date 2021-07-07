from django.shortcuts import render
from django.http import JsonResponse
from django.core import serializers
from iot.models import Event
# Create your views here.

def index(request):
    return render(request,'dashboard/index.html')

def temp_data(request):
    objs = Event.objects.all()
    data = serializers.serialize('json', objs)
    return JsonResponse(data, safe=False)

def result(request):
    print(request.POST)
    return render(request,'dashboard/result.html')