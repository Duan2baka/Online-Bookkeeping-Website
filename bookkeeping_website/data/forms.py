from django.forms import ModelForm
from django import forms

class NameForm(forms.Form):
    st_time = forms.DateField(label='Start time')
    ed_time = forms.DateField(label='End time')