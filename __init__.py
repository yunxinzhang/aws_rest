from flask import Flask, render_template, request, jsonify
import ctypes
import pymysql
import json
import sys
conn = pymysql.connect(host='172.31.9.93', user='xxxzyx', password='0909xxxS', database = 'users_ops', charset='utf8')
cursor = conn.cursor()
app = Flask(__name__)

@app.route('/hello')
def hello():
    print("hello request")
    return '{ "A" : "Hi there, how ya doin?" }'

@app.route('/')
def homepage():
    ip = request.remote_addr
    print(ip)
    sql = "insert into user_visit(ip, time) values(%s, now())"
    cursor.execute(sql, (ip,))
    conn.commit()
    return render_template("main.html")

@app.route('/calc_lixi')
def calc_lixi():
    return render_template("calc_lixi.html")

@app.route('/ipo_info')
def get_info():
    corps = []
    with open("/var/www/FlaskApp/FlaskApp/ipo_info.txt", "r") as f:
        txt = f.read()
    corps = json.loads(txt)
    return render_template("ipo.html", corps = corps)

sdk = ctypes.CDLL('/var/www/FlaskApp/FlaskApp/sdk.so')

@app.route('/sudoku')
def sudoku():
    return render_template('sudoku.html')

import os
@app.route('/solve_sudoku', methods = ['GET', 'POST'])
def solve_sudoku():
    egm = request.args.get("question")
#    idx = request.args.get("query")
    path = './sdk_result/' + str(egm) 
    ans = 'x'
 #   print("-->", idx)
    print("是否存在", os.path.exists(path), path);
    if os.path.exists( path ):
        print("read")
        with open( path, 'r' ) as f:
            ans = f.read()
        return jsonify(answer = ans)        
#    if idx == 1:   #字符串?
#        return  jsonify(answer = "今天算累了，明天帮你算")
    # 没有答案时的处理
    with open("matrix.txt", "w") as f:
        print("write")
        egm =''.join(list(egm))
        f.write(str(egm))
    s = sdk.solveSDK()  
    print("s",s);
    if s == 1:
        with open(path, "r") as f:
            ans = f.read()
    elif s == -1:
        ans = '无解'
    elif s == -3:
        ans = '太有深度'
    else:
        ans = "不想做这个题"
    return jsonify(answer = ans)

import os
import random
@app.route("/sudoku_question", methods=['GET', 'POST'])
def sudoku_question():
    files = os.listdir('./sdk_result/')
    idx = random.randint(0, len(files)-1)
    return jsonify(question = str(files[idx]))

if __name__ == "__main__":
    app.run(host="172.31.9.93", port=80, debug=True)
    #app.run()
