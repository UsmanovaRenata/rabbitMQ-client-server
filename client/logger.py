import logging
from config import config

logger = logging.getLogger(__name__)

def logging_setting():
    level = config.get('LOGGING','level')
    logger.setLevel(level)
    logfile = config.get('LOGGING', 'file')
    file_handler = logging.FileHandler(logfile)
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(funcName)s - %(message)s')
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
