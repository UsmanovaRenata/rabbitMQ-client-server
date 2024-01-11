import pika
from PyQt5.QtCore import pyqtSignal, QObject, QTimer
from config import config
from logger import logger
import protocol_pb2 as pb


class Client(QObject):

    def __init__(self):
        super().__init__()
        self.connectionParams = pika.ConnectionParameters(
            host=config.get('CLIENT', 'host'),
            port=config.get('CLIENT', 'port'),
            virtual_host='/',
            credentials=pika.PlainCredentials(config.get('CLIENT', 'username'), config.get('CLIENT', 'password'))
        )
        try:
            self.id = str(id(self))
            self.connection = pika.BlockingConnection(self.connectionParams)
            self.channel = self.connection.channel()
            self.exchange = "client"
            self.channel.exchange_declare(exchange=self.exchange,
                                          exchange_type="direct")
            self.queue_name = "queue"
            self.channel.queue_declare(queue=self.queue_name)
            self.channel.queue_bind(exchange=self.exchange, queue=self.queue_name)
            logger.info("Start client")
        except Exception as e:
            logger.critical(e)

    def set_producer(self):
        producer = Producer(self.connection, self.queue_name, self.exchange, self.id)
        return producer

    def set_consumer(self, callback):
        consumer = Consumer(self.connection, self.id)
        consumer.message_received.connect(callback)
        return consumer


class Producer:
    def __init__(self, connection, queue, exchange, id):
        self.channel = connection.channel()
        self.queue_name = queue
        self.exchange = exchange
        self.id = id

    def send_message(self, msg):
        try:
            request = pb.Request()
            request.id = self.id
            logger.info(self.id)
            request.req = int(msg)
            self.channel.basic_publish(exchange=self.exchange, routing_key=self.queue_name, body=request.SerializeToString())
            logger.info("Message sent")
        except Exception as e:
            logger.critical(e)


class Consumer(QObject):
    message_received = pyqtSignal(str)

    def __init__(self,connection, queue):
        super().__init__()
        self.connection = connection
        self.channel = connection.channel()
        self.queue_name = queue
        self.readyConsume = False

    def ready_consume(self):
        self.readyConsume = True
        logger.info("Consume ready")

    def check_queue(self):
        if self.readyConsume:
            try:
                method_frame, header_frame, body = self.channel.basic_get(queue=self.queue_name, auto_ack=True)
                if method_frame:
                    response = pb.Response()
                    response.ParseFromString(body)
                    num = response.res
                    logger.info(num)
                    self.message_received.emit(str(num))
            except pika.exceptions.ChannelClosedByBroker:   
                self.message_received.emit("wait..")
                self.channel = self.connection.channel()
            except Exception as e:
                logger.critical(e)
        QTimer.singleShot(100, self.check_queue)
