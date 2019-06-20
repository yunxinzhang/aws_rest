from flask import Flask, jsonify, request
app = Flask(__name__)
def calc_lixi(lendMoney, payPerMonth, nMonth):	
    print("_1_")
    for k in range(1, 10000):
        zhexian = 0
        for n in range(1, nMonth+1):
            zhexian += payPerMonth / pow(1+k/10000.0, n)
        if zhexian <= lendMoney:
            return pow(k/10000.0 + 1,12)
    return -1

@app.route('/', methods=['GET', 'POST'])
def index():
    print(type(request))
    print("get >>", request.args)
    for k,v in request.args.items():
        print(k, v)
    return "eg: http://54.219.159.131:5000/calc?money=10000&pay=878.33&month=12"

@app.route('/calc', methods=['GET', 'POST'])
def calc():
    print(type(request))
    print("get >>", request.args)
    kv = {}
    for k,v in request.args.items():
        print(k, v)
        kv[k] = v
    try:
        print("try")
        m = float(kv["money"])
        print(m)
        p = float(kv["pay"])
        print(p)
        n = int(kv["month"])
        print(n)
        lixi = calc_lixi(m, p, n)
        print(lixi)
        return "answer :" + str(lixi)
    except:
        return "eg: http://54.219.159.131:5000/calc?money=10000&pay=878.33&month=12"

if __name__ == '__main__':
    app.run(debug=True, host='172.31.4.61', port=5000)
