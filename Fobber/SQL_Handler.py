import pymysql
from PyQt5.QtWidgets import QMessageBox
import Client

conn = None
data = {}
def connectToDatabase(db, user, psswrd, host):
    try:
        global conn
        conn = pymysql.connect(
            db=db,
            user=user,
            passwd=psswrd,
            host=host
        )
        global data
        data = getAndSetData(conn)
        return conn, data
    except pymysql.err.OperationalError:
        Q = QMessageBox()
        Q = QMessageBox.information(Q, 'Error', 'Your username and/or password was incorrect. Please try again.', QMessageBox.Ok)
        return None, None

def getAndSetData(conn):
    data = {}
    cursor = conn.cursor()
    cursor.execute('SHOW columns FROM fobs')
    col = [c[0] for c in cursor.fetchall()]
    for column in col:
        cursor.execute('SELECT %s FROM fobs' % column)
        l = [row[0] for row in cursor.fetchall()]
        data[column] = l
    return data
