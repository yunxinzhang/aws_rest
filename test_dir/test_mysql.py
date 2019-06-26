import pymysql

conn = pymysql.connect(host='172.31.9.93', user='xxxzyx', password='0909xxxS', database = 'users_ops', charset='utf8')
cursor = conn.cursor()
#sql = "select count(*) from user_visit;"
#ret = cursor.execute(sql)
sql= "insert into user_visit(ip, time) values(%s, now())"
ip = '11.11.11.11'
t =  '20000101010101'
s = cursor.execute(sql, (ip,))
conn.commit()
cursor.close()
conn.close()

print(s)
