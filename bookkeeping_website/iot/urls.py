from django.urls import path
from . import views
urlpatterns = [
    path('', views.home),
    path('table',views.table),
    path('filter',views.filter)
    #path('import', views.import_excel),
]