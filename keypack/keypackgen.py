# key = 256kB
# keypack= 256 keys
# name format yy-order-key
import os
import random
import datetime

key_path = 'E:\\keypath'

year = int(datetime.datetime.now().year) % 100
# keysize = 0xff * 1024 *1024
def keygen(key):
    out = open(key, 'wb')
    random.seed(datetime.datetime.utcnow().timestamp())
    for i in range(0,0x100):
            out.write(random.randbytes(1024))
    out.close()

for i in range(10000,10001):
    keyname = ('%02d%05d' %(year,i))
    keyfullpath = os.path.join(key_path, keyname)
    # print(keyname)
    keygen(keyfullpath)