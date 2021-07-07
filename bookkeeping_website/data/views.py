from django.shortcuts import render
from iot.models import Event
from iot.models import Venue_Event
# Create your views here.

def index(request):
    return render(request,'data/index.html')

def venue_temp_data(request):
    objs = Venue_Event.objects.all()
    data = serializers.serialize('json', objs)
    return JsonResponse(data, safe=False)