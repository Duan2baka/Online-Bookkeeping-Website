from django.urls import path
from . import views

urlpatterns=[
    path('',views.index),
    path('venue_temp_data',views.venue_temp_data,name='venue_temp_data'),
]