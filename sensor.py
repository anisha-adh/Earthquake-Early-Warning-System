import time, requests, random, json

url = "http://127.0.0.1:5000/eqdata"
data_count = 0
while(1):
    x = round(random.uniform(-1,1),2)
    y = round(random.uniform(-1,1),2)
    z = round(random.uniform(-1,1),2)
    ax = round(random.uniform(-1,1),2)
    ay = round(random.uniform(-1,1),2)
    az = round(random.uniform(-1,1),2)
    orientation = ["XYZ", "XZY", "YZX", "ZYX", "ZXY"]
    data = {
            "raw":
            {
                "x":x, 
                "y":y, 
                "z":z
            },
            "acclro":
            {
                "ax":ax,
                "ay":ay,
                "az":az
            },
            "orientation":random.choice(orientation)
            }
    try:
        requests.post(url, json=data)
        data_count += 1
        print(data_count)
    except:
        print("Post Error")
    #time.sleep(1)
    
