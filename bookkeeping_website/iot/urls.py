from django.urls import path
from . import views
urlpatterns = [
    path('', views.index),
    #path('import', views.import_excel),
]