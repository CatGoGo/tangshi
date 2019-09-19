# -*- coding: utf-8 -*-
from flask import Flask, escape, request
import sqlite3
import json
app = Flask(__name__)
def getone(sql):
    conn = sqlite3.connect('tangshi.db')
    cursor = conn.cursor()
    cursor.execute(sql)
    row = cursor.fetchall()[0]
    cursor.close()
    conn.close()
    id = row[0]
    title = row[1]
    author = row[2]
    txt = row[3].replace('\n','') #.rstrip('\n') 无效不知道为什么
    return id,title,author,txt
def random():
    sql = "select * from poem ORDER BY RANDOM() limit 1"
    return getone(sql)
def id(id):
    sql = "select * from poem where id={0}".format(id)
    return getone(sql)
def text():
    id,title,author,txt=random()
    txt = txt.replace('，',' ').replace('。',' ').replace('？',' ').replace('！',' ')
    return "{0}  作者  {1}  {2}".format(title,author,txt)
@app.route('/json')
def tojson():
    jsonData = []
    result = {}
    result['id'], result['title'],result['author'],result['txt'] = random()
    jsonData.append(result)
    jsondatar=json.dumps(jsonData,ensure_ascii=False)
    out = jsondatar[1:len(jsondatar)-1]
    return out
@app.route('/text/ansi')
def gb2312():
    return text().encode("ansi")
@app.route('/text/utf8')
def utf8():
    return text()
if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000)
