import requests

from src.Crawler import Crawler
from src.Database import Database
from src.Downloader import Downloader
from src.Graph import Graph
from src.Processor import Processor
from src.Queue import Queue
from src.Saver import Saver


if __name__ == '__main__':
    BASE_HOST = 'https://simple.wikipedia.org'
    queue = Queue('127.0.0.1:9092')
    db = Database()
    # db.execute('delete from processed;')
    # db.execute('delete from edges;')
    # db.execute('delete from pages;')
    # db.commit()
    # queue.clear()
    # queue.push('/wiki/Main_Page')
    # queue.flush()
    crawler = Crawler(
        queue, Processor(), Downloader(), Graph(db), Saver('D:\\Crawler'),
        db, BASE_HOST)
    crawler.start()
