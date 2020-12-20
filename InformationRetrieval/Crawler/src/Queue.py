import random
from json import loads, dumps

from kafka import KafkaConsumer, KafkaProducer
# import logging
# logging.basicConfig(level=logging.INFO)


class Queue:
    def __init__(self, kafka_host):
        self.__topic = 'crawler'
        common_settings = {
            # 'security_protocol': "SSL"
            # 'api_version': (0, 9, 0, 1)
        }
        self.__consumer = KafkaConsumer(
            self.__topic,
            bootstrap_servers=[kafka_host],
            auto_offset_reset='earliest',
            enable_auto_commit=True,
            group_id='g0',
            value_deserializer=lambda x: loads(x.decode('utf-8')))
        self.__producer = KafkaProducer(
            bootstrap_servers=[kafka_host],
            value_serializer=lambda x: dumps(x).encode('utf-8'))

    def push(self, item):
        self.__producer.send(self.__topic, value=item)

    def ack(self):
        self.__consumer.commit()

    def get_consumer(self):
        res = {}
        while len(res) == 0:
            res = self.__consumer.poll(timeout_ms=100, max_records=100)
        return map(lambda x: x.value, next(iter(res.values())))
        # return map(lambda x: x.value, self.__consumer)

    def clear(self):
        print('Clear!')
        for _ in self.__consumer:
            print(_.value)
            self.__consumer.commit()

    def flush(self):
        self.__producer.flush()
