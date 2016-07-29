import sys

from PyQt5.QtWidgets import QTextEdit, QWidget, QDialog, QApplication, QPushButton

import output
class MainWindow(QDialog, output.Ui_FobberWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)

if __name__ == '__main__':

    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    sys.exit(app.exec_())



