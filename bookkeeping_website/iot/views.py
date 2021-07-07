from django.shortcuts import render
#from . import iot_mqtt
from .models import Event
import xlrd
# Create your views here.

def index(request):
    objects = Event.objects.all()
    dic = {'objects' : objects}
    return render(request, 'iot/index.html', dic)
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