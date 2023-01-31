#!/usr/bin/python3

from PIL import Image
import urllib3
from io import BytesIO


minmax_loc_sw_ne = [2.273327959701419, 48.83729998022318, 2.2824849374592304,48.84390999097377]
bbox = [round(minmax_loc_sw_ne[i], 2)-0.01 if i < 2 else round(minmax_loc_sw_ne[i],2)+0.01 for i in range(4)]

url_base = "https://api.mapbox.com/styles/v1/"
uname = "mapbox/"
styleid = "streets-v11/"
how = "static/"
bbox_str = f"{[float('%.2f' % elem) for elem in bbox ]}/"
size = "800x600@2x"
token = "?access_token=pk.eyJ1IjoicXptYW4iLCJhIjoiY2xkamZvbG52MHl6cDN4bDFwajluZWowNyJ9.5G2V_B3bq-E0TgoWCyjlYA"

url_req = url_base+uname+styleid+how+bbox_str+size+token
print(url_req)

http = urllib3.PoolManager()
resp = http.request("GET", url_req)

img = Image.open(BytesIO(resp.data))
img.save("test.png")