# -*- coding: utf-8 -*-
import sqlite3
import json
conn = sqlite3.connect('tangshi.db')
cursor = conn.cursor()
cursor.execute('select * from poem ORDER BY RANDOM() limit 1')
values = cursor.fetchall()
#print(values)
cursor.close()
conn.close()
jsonData = []
for row in values:
    result = {}
    result['id'] = row[0]
    result['title'] = row[1]
    result['author'] = row[2]
    txt = row[3].replace('\n','') #.rstrip('\n') 无效不知道为什么
    result['txt']= txt

    jsonData.append(result)
jsondatar=json.dumps(jsonData,ensure_ascii=False)
out = jsondatar[1:len(jsondatar)-1]
print(out)