from django.shortcuts import render
#from . import iot_mqtt
from .models import Event
import xlrd
# Create your views here.

def table(request):
    tmp=dict(request.POST)
    objects = Event.objects.all()
    print(type(tmp))
    cnt1=0
    cnt2=0
    dic = {'Temperature' : 0 , 'Humidity' : 0 , 'Light' : 0 , 'Sound' : 0}
    venue_list_str = ['W311-H1','W311-H2','W311-H3','W311A','W311B','W311D-Z1','W311D-Z2']
    for i in venue_list_str:
        if not i in tmp:
            print(i)
            objects = objects.exclude(node_loc=i)
    for fi,se in tmp.items():
        if fi == 'csrfmiddlewaretoken':
            continue
        if fi[0] == 'W':
            #print(type(fi))
            print('')
            cnt1=cnt1+1
        else:
            dic[fi] = 1
            cnt2=cnt2+1
    if not cnt1:
        #messages.error(request, 'You must choose at least ONE venue!')
        return render(request,'dashboard/errorpage.html',{'value' : 8})
    if not cnt2:
        return render(request,'dashboard/errorpage.html',{'value' : 9})
    
    dic['objects'] = objects
    return render(request, 'iot/table.html', dic)

def home(request):
    return render(request,'iot/home.html')

def filter(request):
    return render(request,'iot/filter.html')

'''
    tmp=dict(request.POST)
    #print(type(tmp))
    #print(tmp)
    cnt1=0
    cnt2=0
    ret = {'venue_list' : [], 'attribute_list' : [], 'Temperature' : 0 , 'Humidity' : 0 , 'Light' : 0 , 'Sound' : 0, 'W311-H1' : 0, 'W311-H2' : 0, 'W311-H3' : 0, 'W311A' : 0,'W311B' : 0,'W311D-Z1' : 0,'W311D-Z2':0}
    for fi,se in tmp.items():
        if fi == 'csrfmiddlewaretoken':
            continue
        if fi[0] == 'W':
            ret['venue_list'].append(fi)
            ret[fi] = 1
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
'''

'''
def import_excel(self, request):
    excel_file = request.FILES.get('excel_file', '')
    file_type = excel_file.name.split('.')[1]
    if file_type in ['xlsx', 'xls']:
        data = xlrd.open_workbook(filename=None, file_contents=excel_file.read())
        tables = data.sheets()
        for table in tables:
            rows = table.nrows
            try:
                with transaction.atomic():
                    for row in range(1, rows):
                        row_values = table.row_values(row)
                        DataTable.objects.create(
                            number=row_values[0],
                            name=row_values[1],
                            linkman=row_values[2],
                            phone=row_values[3],
                            hyperlink=row_values[4],
                            remarks=row_values[5])
            except:
                return restful.error(message='解析excel文件或者数据插入错误！')
        return restful.success()
    else:
        return restful.error(message='上传文件类型错误！')
'''