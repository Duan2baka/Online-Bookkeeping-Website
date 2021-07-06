from django.db import models

# Create your models here.

class Event(models.Model):
    node_id = models.CharField(max_length=3)
    node_loc = models.CharField(max_length=10)
    temp = models.DecimalField(max_digits=5, decimal_places=2)
    humid = models.DecimalField(max_digits=5, decimal_places=2)
    light = models.DecimalField(max_digits=5, decimal_places=2)
    sound = models.DecimalField(max_digits=5, decimal_places=2)
    date_created = models.DateTimeField(auto_now_add=True)
    def __str__(self):
        return 'Event #{}'.format(self.id)

class Venue_Event(models.Model):
    Venue = models.CharField(max_length = 10)
    Date = models.DateField()
    Time_st = models.TimeField()
    Time_ed = models.TimeField()
    Event = models.CharField(max_length = 10)
    Instructor = models.CharField(max_length = 20)
    def __str__(self):
        return 'Venue Event #{}'.format(self.id)