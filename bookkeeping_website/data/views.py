from django.shortcuts import render
from django.http import JsonResponse
from django.core import serializers
from iot.models import Event
from iot.models import Venue_Event
from .forms import NameForm
import json
from datetime import datetime
# Create your views here.

def index(request):
    return render(request, 'data/index.html')

def convert_to_date(str):
    tmp_str=str[0:4]+'-'+str[5:7]+'-'+str[8:10]+'-'+str[11:13]+'-'+str[14:16]
    return datetime.strptime(tmp_str,'%Y-%m-%d-%H-%M')


def venue_temp_data(request):
    objs = Venue_Event.objects.all()
    data = serializers.serialize('json', objs)
    event_obj = Event.objects.all()
    event_data = serializers.serialize('json', event_obj)
    #print(data)
    #print('---------------------------------------------')
    tmp=json.loads(data)
    #print(tmp)
    event_tmp=json.loads(event_data)
    for i in tmp:
        tmp[i['pk']-1]['fields']['start']=i['fields']['Date']+' '+i['fields']['Time_st']
        tmp[i['pk']-1]['fields']['end']=i['fields']['Date']+' '+i['fields']['Time_ed']
        tmp[i['pk']-1]['fields']['id']=i['pk']
        #print(tmp[i['pk']-1]['fields']['Venue'])
        tmp[i['pk']-1]['fields']['Venue']=tmp[i['pk']-1]['fields']['Venue'].upper()
        #print(tmp[i['pk']-1]['fields']['Venue'])
        #print(convert_to_date(tmp[i['pk']-1]['fields']['start']))
        st=convert_to_date(tmp[i['pk']-1]['fields']['start'])
        ed=convert_to_date(tmp[i['pk']-1]['fields']['end'])
        for j in event_tmp:
            if convert_to_date(j['fields']['date_created'])>=st and  convert_to_date(j['fields']['date_created'])<=ed:
                tmp[i['pk']-1]['fields']['Temperature']=j['fields']['temp']
                tmp[i['pk']-1]['fields']['Humidity']=j['fields']['humid']
                tmp[i['pk']-1]['fields']['Light']=j['fields']['light']
                tmp[i['pk']-1]['fields']['Sound']=j['fields']['sound']
                break
    data=json.dumps(tmp)
    #print(data)
    return JsonResponse(data, safe=False)

def result(request):
    tmp=dict(request.POST)
    #print(len(dict(request.POST)['end_time'][0]))
    print(tmp)
    if len(dict(request.POST)['end_time'][0]) and len(dict(request.POST)['end_date'][0]) and len(dict(request.POST)['start_time'][0]) and len(dict(request.POST)['start_date'][0]):
        ret={}
        ret['start']=convert_to_date(tmp['start_date'][0] + '-' + tmp['start_time'][0])
        ret['end']=convert_to_date(tmp['end_date'][0] + '-' + tmp['end_time'][0])
        print(ret['start']<=ret['end'])
        print(ret['start'])
        print(ret['end'])
        print(type(ret['start']))
        if ret['start']<=ret['end']:
            ret['venue']=tmp['venue list'][0]
            print(ret)
            return render(request, 'data/result.html',ret)
        else:
            return render(request, 'dashboard/errorpage.html',{'value':7})
    else:
        if not len(dict(request.POST)['start_date'][0]):
            return render(request, 'dashboard/errorpage.html',{'value':3})
        elif not len(dict(request.POST)['start_time'][0]):
            return render(request, 'dashboard/errorpage.html',{'value':4})
        elif not len(dict(request.POST)['end_date'][0]):
            return render(request, 'dashboard/errorpage.html',{'value':5})
        elif not len(dict(request.POST)['end_time'][0]):
            return render(request, 'dashboard/errorpage.html',{'value':6})