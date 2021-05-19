# HttpResponse is used to pass the information back to view 
from django.http import HttpResponse 
from django.shortcuts import render 
import json
import pyrebase
from django.views.generic import TemplateView
from collections import OrderedDict
import itertools
import datetime

config={ 
    "apiKey": "AIzaSyAktkuyITarzohCCFJVuDxBbp-22jYWEQ8",
    "authDomain": "waterquality-67c37.firebaseapp.com",
    "projectId": "waterquality-67c37",
    "databaseURL": "https://waterquality-67c37-default-rtdb.firebaseio.com/",
    "storageBucket": "waterquality-67c37.appspot.com",
    "messagingSenderId": "160810570175",
    "appId": "1:160810570175:web:252a415d80a846b7e4510a",
    "measurementId": "G-0B13Y9ZGVX"
} 
#initializing pyrebase 
firebase=pyrebase.initialize_app(config) 
authe = firebase.auth() 
database=firebase.database() 
mydate = datetime.datetime.now()


# Defining functions which will receive request and perform task depending upon function definition
def paani2(request):
    temperature = database.child('DHT11').child('Temperature').order_by_key().limit_to_last(1).get().val()
    humidity = database.child('DHT11').child('Humidity').order_by_key().limit_to_last(1).get().val()
    temperature = [value for key, value in temperature.items()]
    humidity = [value for key, value in humidity.items()]
    context = {
        'temperature':temperature[0],
        'humidity':humidity[0]
    }
    return render(request,"paani2.html", context=context)
    
    # return HttpResponse(f'{temperature[0]} <br> {humidity[0]}')
    
def pGRAPH(request):
    temperature = database.child('DHT11').child('Temperature').get()
    humidity = database.child('DHT11').child('Humidity').get()
    # List of all the previous temperature, humidity data
    temperature = [value.val() for value in temperature.each()]
    humidity = [value.val() for value in humidity.each()]
    #plotting data
    # The chartConfig dict contains key-value pairs data for chart attribute
    chart_size = 22
    humidity = humidity[int(len(humidity)-chart_size):]
    temperature = temperature[int(len(temperature)-chart_size):] 
    data_qs = []
    for (a, b) in itertools.zip_longest(humidity, temperature, fillvalue=-1): 
        month = mydate.strftime("%b")
        data = f'"Month":"{month}","Humidity":{a},"Temperature":{b}'
        data_qs += [data]
    context = {
        'data':data_qs
    }
    return render(request,"PaaniGraphs.html", context=context)
    
    # return HttpResponse(f'{temperature} <br> {humidity}')
    
    
def pCONTACT(request):
    return render(request,"PaaniContact.html")

