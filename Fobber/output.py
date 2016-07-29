from PyQt5 import QtCore, QtGui, QtWidgets, QtSql
import pymysql

class Ui_FobberWindow(object):
    def connectToDatabase(self):
        try:
            conn = pymysql.connect(
                db='test3',
                user=self.UsernameField.text(),
                passwd=self.PasswordField.text(),
                host='localhost'
            )
            c = conn.cursor()
            c.execute("SELECT * FROM text3")
            for row in c:
                print(c)
            c.close()
            self.username = self.UsernameText.text()
            self.password = self.PasswordText.text()
        except pymysql.err.OperationalError:
            self.tryAgain = QtWidgets.QMessageBox.question(self, 'Error', 'Your username and/or password was incorrect. Please try again.',
                                             QtWidgets.QMessageBox.Ok)

    def setupUi(self, FobberWindow):
        FobberWindow.setObjectName("FobberWindow")
        FobberWindow.resize(800, 370)
        font = QtGui.QFont()
        font.setBold(False)
        font.setWeight(50)
        FobberWindow.setFont(font)
        self.username = ''
        self.password = ''
        self.centralwidget = QtWidgets.QWidget(FobberWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.MakersLogo = QtWidgets.QLabel(self.centralwidget)
        self.MakersLogo.setGeometry(QtCore.QRect(309, 2, 182, 172))
        self.MakersLogo.setText("")
        self.MakersLogo.setPixmap(QtGui.QPixmap("decatur_makers_logo.jpeg"))
        self.MakersLogo.setObjectName("MakersLogo")
        self.UsernameText = QtWidgets.QLabel(self.centralwidget)
        self.UsernameText.setGeometry(QtCore.QRect(10, 190, 120, 20))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.UsernameText.setFont(font)
        self.UsernameText.setScaledContents(False)
        self.UsernameText.setWordWrap(False)
        self.UsernameText.setObjectName("UsernameText")
        self.PasswordText = QtWidgets.QLabel(self.centralwidget)
        self.PasswordText.setGeometry(QtCore.QRect(10, 260, 120, 20))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.PasswordText.setFont(font)
        self.PasswordText.setScaledContents(False)
        self.PasswordText.setWordWrap(False)
        self.PasswordText.setObjectName("PasswordText")
        self.UsernameField = QtWidgets.QLineEdit(self.centralwidget)
        self.UsernameField.setGeometry(QtCore.QRect(10, 220, 300, 30))
        self.UsernameField.setObjectName("UsernameField")
        self.PasswordField = QtWidgets.QLineEdit(self.centralwidget)
        self.PasswordField.setGeometry(QtCore.QRect(10, 290, 300, 30))
        self.PasswordField.setText("")
        self.PasswordField.setFrame(True)
        self.PasswordField.setEchoMode(QtWidgets.QLineEdit.Password)
        self.PasswordField.setObjectName("PasswordField")
        self.loginButton = QtWidgets.QPushButton(self.centralwidget)
        self.loginButton.setGeometry(QtCore.QRect(10, 330, 140, 30))
        self.loginButton.setObjectName("loginButton")
        self.loginButton.clicked.connect(self.connectToDatabase)
        self.retranslateUi(FobberWindow)
        QtCore.QMetaObject.connectSlotsByName(FobberWindow)

    def retranslateUi(self, FobberWindow):
        _translate = QtCore.QCoreApplication.translate
        FobberWindow.setWindowTitle(_translate("FobberWindow", "Fobber v0.0.1"))
        self.UsernameText.setText(_translate("FobberWindow", "Username:"))
        self.PasswordText.setText(_translate("FobberWindow", "Password:"))
        self.loginButton.setText(_translate("FobberWindow", "Login"))
