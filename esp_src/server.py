from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/eqdata', methods=['GET','POST'])
def getData():
    content = request.get_json(silent=True)
    #print(content);
    raw = content['raw'];
    acc = content['acclr'];
    orient = {'orientation': content['orientation']};

    dlist = [raw,acc,orient];

    for mydict in dlist:
        placeholders = ', '.join(['%s'] * len(mydict))
        # print(mydict.keys());
        columns = ', '.join("`" + str(x).replace('/', '_') + "`" for x in mydict.keys())
        values = ', '.join("'" + str(x).replace('/', '_') + "'" for x in mydict.values())
        sql = "INSERT INTO %s ( %s ) VALUES ( %s );" % ('mytable', columns, values)

        f = open("/home/deaddemocracy/Desktop/earthquake_prav/data.sql", "a")
        f.write(sql + '\n')

    return 'JSON posted';

if __name__ == '__main__':
    app.run(host="192.168.100.108",port = 5000, debug=True)

