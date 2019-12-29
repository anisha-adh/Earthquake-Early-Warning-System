import numpy as np
from matplotlib import pyplot as plt
import sqlite3
conn = sqlite3.connect("readings_database.db")
cur = conn.cursor()
'''
cur.execute("""CREATE TABLE IF NOT EXISTS readings_table(
        Id INTEGER PRIMARY KEY AUTOINCREMENT,
        x real,
        y real,
        z real,
        ax real,
        ay real,
        az real,
        orientation text
        )""")
cur.close()
conn.close()
'''
data = cur.execute("select x,y,z from readings_table where id<100")
new_data = np.array(data.fetchall())
cur.close()
conn.close()
plt.plot(new_data[:,0])
plt.plot(new_data[:,1])
#plt.plot(new_data[:,2])
plt.show()
