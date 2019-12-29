from flask import Flask, request, jsonify, render_template
import sqlite3

app = Flask(__name__)

data_count = 0

@app.route('/eqdata', methods=['POST'])
def data_reader():
    global data_count
    data_count += 1
    print(data_count)
    res = request.get_json()
    conn =  sqlite3.connect('readings_database.db')
    cur = conn.cursor()
    cur.execute('insert into readings_table values(:Id, :x, :y, :z, :ax, :ay, :az, :orientation)',{'Id':None, 'x':res['raw']['x'], 'y':res['raw']['y'], 'z':res['raw']['z'], 'ax':res['acclro']['ax'], 'ay':res['acclro']['ay'], 'az':res['acclro']['az'], 'orientation':res['orientation']})
    conn.commit()
    cur.close()
    conn.close()
    return jsonify({"success": True}), 202

@app.route('/')
def index():
    conn = sqlite3.connect('readings_database.db')
    cur = conn.cursor()
    cur.execute('select * from readings_table order by id desc LIMIT 1;')
    row = cur.fetchone()
    cur.close()
    conn.close()
    return render_template('index.html', data = row)
    
