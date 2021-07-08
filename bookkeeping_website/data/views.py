from django.shortcuts import render
from django.http import JsonResponse
from django.core import serializers
from iot.models import Event
from iot.models import Venue_Event
from .forms import NameForm
import json
# Create your views here.

def index(request):
    return render(request, 'data/index.html')

def venue_temp_data(request):
    objs = Venue_Event.objects.all()
    data = serializers.serialize('json', objs)
    #print(data)
    #print('---------------------------------------------')
    tmp=json.loads(data)
    for i in tmp:
        tmp[i['pk']-1]['fields']['start']=i['fields']['Date']+' '+i['fields']['Time_st']
        tmp[i['pk']-1]['fields']['end']=i['fields']['Date']+' '+i['fields']['Time_ed']
        tmp[i['pk']-1]['fields']['id']=i['pk']
        #print(tmp[i['pk']-1]['fields']['start'])
    data=json.dumps(tmp)
    #print(data)
    return JsonResponse(data, safe=False)

def result(request):
    tmp=dict(request.POST)
    #print(dict(request.POST))
    ret={}
    ret['start date']=tmp['start_date']
    ret['start time']=tmp['start_time']
    ret['end date']=tmp['end_date']
    ret['end time']=tmp['end_time']
    ret['venue']=tmp['venue list']
    return render(request, 'data/result.html')