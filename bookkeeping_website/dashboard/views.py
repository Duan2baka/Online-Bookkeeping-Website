from django.shortcuts import render,redirect
from django.http import JsonResponse
from django.core import serializers
from iot.models import Event as e1
#import tkinter.messagebox
#from tkinter import *
#from django.contrib import messages
import json
# Create your views here.

def index(request):
    return render(request,'dashboard/index.html')

def temp_data(request):
    objs = e1.objects.all()
    data = serializers.serialize('json', objs)
    return JsonResponse(data, safe=False)

def errorpage(request):
    return render(request,'dashboard/errorpage.html')

def result(request):
    tmp=dict(request.POST)
    #print(type(tmp))
    #print(tmp)
    cnt1=0
    cnt2=0
    ret = {'venue_list' : [], 'attribute_list' : [], 'Temperature' : 0 , 'Humidity' : 0 , 'Light' : 0 , 'Sound' : 0}
    for fi,se in tmp.items():
        if fi == 'csrfmiddlewaretoken':
            continue
        if fi[0] == 'W':
            ret['venue_list'].append(fi)
            cnt1=cnt1+1
        else:
            ret['attribute_list'].append(fi)
            ret[fi] = 1
            cnt2=cnt2+1
    if not cnt1:
        #messages.error(request, 'You must choose at least ONE venue!')
        return render(request,'dashboard/errorpage.html',{'value' : 1})
    if not cnt2:
        return render(request,'dashboard/errorpage.html',{'value' : 2})
    ret['venue'] = json.dumps(ret['venue_list'])
    ret['attribute'] = json.dumps(ret['attribute_list'])
    return render(request,'dashboard/result.html',ret)