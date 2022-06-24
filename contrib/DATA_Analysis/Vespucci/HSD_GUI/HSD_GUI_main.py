import sys

from PySide6.QtWidgets import QApplication
from PySide6 import QtCore

from .MainWindow import MainWindow

def main():
    QApplication.setAttribute(QtCore.Qt.AA_ShareOpenGLContexts)
    app = QApplication(sys.argv)
    
    mainWindow = MainWindow(app)
    mainWindow.show()
    app.setAttribute(QtCore.Qt.AA_Use96Dpi)
    app.exec_()

if __name__ == "__main__":
    main()


