import json
import time
import urllib.request
from  bs4 import BeautifulSoup
import socket
socket.setdefaulttimeout(60)
class IpoCorp:
    def __init__(self, name, link, nemp, ninc, domain, des, dtime):
        self.name = name
        self.link = link
        self.nemp = nemp
        self.ninc = ninc
        self.domain = domain
        self.des = des
        self.dtime = dtime
def IpoCorpJson(obj):
    return { "name" : obj.name,
             "link" : obj.link,
             "nemp" : obj.nemp,
             "ninc" : obj.ninc,
             "domain" : obj.domain,
             "des"  : obj.des,
             "dtime" : obj.dtime
            }
    
path = 'https://www.iposcoop.com/ipo-calendar/'
h =   {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20100101 Firefox/23.0'}  
h = { 'User-Agent' :'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:67.0) Gecko/20100101 Firefox/67.0',
     'Accept-Language' : 'zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2'}
r =   urllib.request.Request(url=path, headers=h) 

cnt = 0

while(True):
    cnt = cnt + 1
    try:
        ht =  urllib.request.urlopen(r).read() 
        ss =  BeautifulSoup(ht, "html.parser")
        table = ss.find(name="table", attrs={"class" : "standard-table ipolist"})
        tbody = table.find(name="tbody")
        corps = []
        for tr in tbody.find_all("tr"):
            name, link, nemp, ninc, des, domain, dtime = '', '', '', '', '', '',''
            tds = tr.find_all("td")
            name = tds[0].find("a").get_text()
            link = tds[0].find("a").get("href")
            dtime = tds[7].get_text()
            #print(link)
            cpr = urllib.request.Request(url=link, headers=h)
            cph = urllib.request.urlopen(cpr).read()
            cps = BeautifulSoup(cph, "html.parser")
            ctable = cps.find("table", attrs={"class":"ipo-table"})
            ctrs = ctable.find_all("tr")
            des = ctrs[1].find_all("strong")[-1].get_text()
            domain = ctrs[2].find_all("td")[-1].get_text()
            nemp = ctrs[3].find_all("td")[-1].get_text()
            link = ctrs[8].find_all("td")[-1].get_text()
            ninc = ctrs[13].find_all("td")[-1].get_text()
            print("1")
            corp = IpoCorp(name,link,nemp,ninc,domain,des, dtime)
            corps.append(corp)
           # corps.append(json.dumps(corp, default=IpoCorpJson))
        with open("ipo_info.txt", "w") as f:
            print("w")
            f.write(str(json.dumps(corps, default=IpoCorpJson)))
        #print("sleep")
        time.sleep(7200)
        #print("wakeup")
    except:
        print("there is some errors")
