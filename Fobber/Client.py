import sys, SQL_Handler
from PyQt5.QtWidgets import QDialog, QApplication, QTableWidgetItem
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
    print(conn, data)
    if conn is not None:
        form.close()
        form2.show()
        form2.logoutButton.clicked.connect(mainMenu)
        form2.sqlTable.setColumnCount(len(data))
        form2.sqlTable.setRowCount(len(list(data.values())[0]))
        form2.sqlTable.setHorizontalHeaderLabels([d for d in data.keys()])
        for col, key in enumerate(data.keys()):
            for row, item in enumerate(data[key]):
                itemWidget = QTableWidgetItem(str(item))
                form2.sqlTable.setItem(row, col, itemWidget)
    print(data)

def mainMenu():
    form2.close()
    form2.destroy()
    form.open()
    form.show()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    count = 0
    form = MainWindow()
    form.show()
    form.loginButton.clicked.connect(connection)
    form2 = SQLWindow()

    sys.exit(app.exec_())



