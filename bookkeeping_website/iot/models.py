from django.db import models

# Create your models here.

class Event(models.Model):
    node_id = models.CharField(max_length=3)
    node_loc = models.CharField(max_length=10)
    temp = models.DecimalField(max_digits=5, decimal_places=2)
    humid = models.DecimalField(max_digits=5, decimal_places=2)
    light = models.IntegerField()
    sound = models.DecimalField(max_digits=5, decimal_places=2)
    date_created = models.DateTimeField(auto_now_add=True)
    def __str__(self):
        return 'Event #{}'.format(self.id)