import numpy as np
import sys
from matplotlib import pyplot as plt
import matplotlib.animation as animation
import sqlite3

fig = plt.figure()

def animate(i):
    conn = sqlite3.connect("readings_database.db")
    cur = conn.cursor()
    values = int(sys.argv[1])
    command = sys.argv[2]
    data = cur.execute(command)
    new_data = np.array(data.fetchall())
    cur.close()
    conn.close()

    idx = 321
    title = ['X value', 'Y value', 'Z value', 'ax value', 'ay value', 'az value']
    #fig = plt.figure()
    #fig.tight_layout()
    for i in range(values):
        ax = fig.add_subplot(idx+i);
        ax.title.set_text(title[i])
        ax.clear()
        ax.plot(new_data[:,i])
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.subplots_adjust(left=0.1, bottom=0.1, right=0.9, top=0.9, wspace=0.2, hspace=0.5)
plt.show()

