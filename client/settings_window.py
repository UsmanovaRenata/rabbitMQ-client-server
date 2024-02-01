from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QTextEdit
from config import config
from logger import logger

class SettingsWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.config = config
        self.server_host_label = QLabel('Server Host:')
        self.server_host_line_edit = QLineEdit(self.config.get('CLIENT', 'host'))

        self.server_port_label = QLabel('Server Port:')
        self.server_port_line_edit = QLineEdit(self.config.get('CLIENT', 'port'))

        self.exchange_name_label = QLabel('Exchange Name:')
        self.exchange_name_line_edit = QLineEdit(self.config.get('EXCHANGE', 'name'))

        self.exchange_type_label = QLabel('Exchange Type:')
        self.exchange_type_line_edit = QLineEdit(self.config.get('EXCHANGE', 'type'))

        self.queue_name_label = QLabel('Queue Name:')
        self.queue_name_line_edit = QLineEdit(self.config.get('QUEUE', 'name'))

        self.client_username_label = QLabel('Username:')
        self.client_username_line_edit = QLineEdit(self.config.get('CLIENT', 'username'))

        self.client_password_label = QLabel('Password:')
        self.client_password_line_edit = QLineEdit(self.config.get('CLIENT', 'password'))

        self.logging_level_label = QLabel('Logging Level:')
        self.logging_level_line_edit = QLineEdit(self.config.get('LOGGING', 'level'))

        self.logging_file_label = QLabel('Log File:')
        self.logging_file_line_edit = QLineEdit(self.config.get('LOGGING', 'file'))
        self.save_settings_button = QPushButton('Save Settings')
        self.save_settings_button.clicked.connect(self.save_settings)
        self.set_widgets()

    def set_widgets(self):
        layout = QVBoxLayout()
        layout.addWidget(self.server_host_label)
        layout.addWidget(self.server_host_line_edit)
        layout.addWidget(self.server_port_label)
        layout.addWidget(self.server_port_line_edit)
        layout.addWidget(self.exchange_name_label)
        layout.addWidget(self.exchange_name_line_edit)
        layout.addWidget(self.exchange_type_label)
        layout.addWidget(self.exchange_type_line_edit)
        layout.addWidget(self.queue_name_label)
        layout.addWidget(self.queue_name_line_edit)
        layout.addWidget(self.client_username_label)
        layout.addWidget(self.client_username_line_edit)
        layout.addWidget(self.client_password_label)
        layout.addWidget(self.client_password_line_edit)
        layout.addWidget(self.logging_level_label)
        layout.addWidget(self.logging_level_line_edit)
        layout.addWidget(self.logging_file_label)
        layout.addWidget(self.logging_file_line_edit)
        layout.addWidget(self.save_settings_button)
        self.setLayout(layout)

    def save_settings(self):
        try:
            self.config.set('CLIENT', 'host', self.server_host_line_edit.text())
            self.config.set('CLIENT', 'port', self.server_port_line_edit.text())
            self.config.set('CLIENT', 'username', self.client_username_line_edit.text())
            self.config.set('CLIENT', 'password', self.client_password_line_edit.text())

            self.config.set('EXCHANGE', 'name', self.exchange_name_line_edit.text())
            self.config.set('EXCHANGE', 'type', self.exchange_type_line_edit.text())
            self.config.set('QUEUE', 'name', self.queue_name_line_edit.text())

            self.config.set('LOGGING', 'level', self.logging_level_line_edit.text())
            self.config.set('LOGGING', 'file', self.logging_file_line_edit.text())

            with open('config.ini', 'w') as config_file:
                self.config.write(config_file)
            logger.info("Settings saved successfully.")
        except Exception as e:
            logger.critical(f"Error saving settings: {e}")
        self.close()