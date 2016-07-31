from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_SQLWindow(object):
    def setupUi(self, SQLWindow):
        SQLWindow.setObjectName("FobberWindow")
        SQLWindow.resize(800, 370)
        font = QtGui.QFont()
        font.setBold(False)
        font.setWeight(50)
        SQLWindow.setFont(font)
        self.centralwidget = QtWidgets.QWidget(SQLWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.sqlTable = QtWidgets.QTableWidget(self.centralwidget)
        self.sqlTable.setGeometry(QtCore.QRect(10, 20, 782, 321))
        self.sqlTable.setObjectName("sqlTable")
        self.validationButton = QtWidgets.QPushButton(self.centralwidget)
        self.validationButton.setGeometry(QtCore.QRect(10, 340, 93, 28))
        self.validationButton.setObjectName("validationButton")
        self.logoutButton = QtWidgets.QPushButton(self.centralwidget)
        self.logoutButton.setGeometry(QtCore.QRect(210, 340, 93, 28))
        self.logoutButton.setObjectName("logoutButton")
        self.refreshButton = QtWidgets.QPushButton(self.centralwidget)
        self.refreshButton.setGeometry(QtCore.QRect(110, 340, 93, 28))
        self.refreshButton.setObjectName("refreshButton")
        self.retranslateUi(SQLWindow)
        QtCore.QMetaObject.connectSlotsByName(SQLWindow)

    def retranslateUi(self, FobberWindow):
        _translate = QtCore.QCoreApplication.translate
        FobberWindow.setWindowTitle(_translate("FobberWindow", "Fobber v0.0.1"))
        self.validationButton.setText(_translate("FobberWindow", "Save Changes"))
        self.logoutButton.setText(_translate("FobberWindow", "Log Out"))
        self.refreshButton.setText(_translate("FobberWindow", "Refresh"))

