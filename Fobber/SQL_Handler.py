import pymysql

def connectToDatabase(db, user, psswrd, host):
    try:
        return pymysql.connect(
            db=db,
            user=user,
            passwd=psswrd,
            host=host
        )
    except pymysql.err.OperationalError:
        return None


def retrieveTable(conn):
    if type(conn) is not None:
        print('Not None')