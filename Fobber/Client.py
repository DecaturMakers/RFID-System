import sys, SQL_Handler
from PyQt5.QtWidgets import QDialog, QApplication, QTableWidgetItem, QMessageBox
import mainwindow, sqlwindow

class MainWindow(QDialog, mainwindow.Ui_FobberWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)

class SQLWindow(QDialog, sqlwindow.Ui_SQLWindow):
    def __init__(self, parent=None):
        super(SQLWindow, self).__init__(parent)
        self.setupUi(self)

def connection():
    conn, data = SQL_Handler.connectToDatabase('fobs', form.UsernameField.text(), form.PasswordField.text(), 'localhost')
    if conn is not None:
        buildTable()
        form.close()
        form2.show()
        form2.logoutButton.clicked.connect(mainMenu)
        form2.refreshButton.clicked.connect(lambda: buildTable())
        form2.validationButton.clicked.connect(lambda: saveChanges())
    else:
        Q = QMessageBox()
        Q = QMessageBox.information(Q, 'Error', 'Connection failed.',
                                    QMessageBox.Ok)

def mainMenu():
    form2.close()
    form2.destroy()
    form.open()
    form.show()

def buildTable():
    cconn, data = SQL_Handler.connectToDatabase('fobs', form.UsernameField.text(), form.PasswordField.text(), 'localhost')
    form2.sqlTable.clear()
    SQL_Handler.getAndSetData(cconn)
    form2.sqlTable.setColumnCount(len(data))
    form2.sqlTable.setRowCount(len(list(data.values())[0]))
    form2.sqlTable.setHorizontalHeaderLabels(sorted([d for d in data.keys()]))
    form2.sqlTable.resizeRowsToContents()
    form2.sqlTable.resizeColumnsToContents()
    for col, key in enumerate(sorted(data.keys())):
        for row, item in enumerate(data[key]):
            itemWidget = QTableWidgetItem(str(item))
            form2.sqlTable.setItem(row, col, itemWidget)

def saveChanges():
    conn, data = SQL_Handler.connectToDatabase('fobs', form.UsernameField.text(), form.PasswordField.text(), 'localhost')
    cursor = conn.cursor()
    for col in range(0,len(data.keys())):
        for row in range(0,len(list(data.values())[0])):
            text = form2.sqlTable.item(row,col).text()
            keys = sorted(list(data.keys()))[col]
            text2 = data.get(sorted(list(data.keys()))[col])[row]
            fob_id = form2.sqlTable.item(row,3).text()
            if text != text2:
                print('UPDATE fobs SET {} = "{}" WHERE fob_id = "{}"'.format(str(keys), str(text), str(fob_id)))
                cursor.execute('UPDATE fobs SET {} = "{}" WHERE fob_id = "{}"'.format(str(keys), str(text), str(fob_id)))
                conn.commit()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    form.loginButton.clicked.connect(connection)
    form2 = SQLWindow()
    sys.exit(app.exec_())