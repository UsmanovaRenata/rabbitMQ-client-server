from logger import logger, logging_setting
import sys
from window import ClientWindow, QApplication

if __name__ == '__main__':
    logging_setting()
    logger.info("Start of the program")
    app = QApplication(sys.argv)
    window = ClientWindow()
    window.show()
    sys.exit(app.exec_())
