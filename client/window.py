from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QTextEdit
from PyQt5.QtCore import QThread
from client import Client
from logger import logger
from settings_window import SettingsWindow


class ClientWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.request_label = QLabel('Request:')
        self.request_line_edit = QLineEdit()
        self.result_label = QLabel('Result:')
        self.result_text = QLabel()
        self.send_button = QPushButton('Send')
        self.open_settings_button = QPushButton('Open Settings')
        self.set_widgets()
        self.send_button.clicked.connect(self.send_request)
        self.open_settings_button.clicked.connect(self.open_settings_window)
        client = Client()
        self.producer = client.set_producer()
        self.consumer = client.set_consumer(self.update_label)
        self.thread = QThread()
        self.consumer.moveToThread(self.thread)
        self.thread.started.connect(self.consumer.check_queue)
        self.thread.start()

    def set_widgets(self):
        layout = QVBoxLayout()
        layout.addWidget(self.request_label)
        layout.addWidget(self.request_line_edit)
        layout.addWidget(self.result_label)
        layout.addWidget(self.result_text)
        layout.addWidget(self.send_button)
        layout.addWidget(self.open_settings_button)
        self.setLayout(layout)

    def send_request(self):
        self.result_text.clear()
        self.producer.send_message(self.request_line_edit.text())
        self.consumer.ready_consume()

    def update_label(self, message):
        logger.info(message)
        self.result_text.setText(message)

    def open_settings_window(self):
        self.settings_window = SettingsWindow()
        self.settings_window.show()
