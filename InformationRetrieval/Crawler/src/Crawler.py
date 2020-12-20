import re
import time
from logging import *

from src.Graph import Graph
from src.Saver import Saver
from src.Queue import Queue
from src.Processor import Processor
from src.Downloader import Downloader
from src.Database import Database


class Crawler:
    def __init__(self,
                 queue: Queue,
                 processor: Processor,
                 downloader: Downloader,
                 graph: Graph,
                 saver: Saver,
                 db: Database,
                 host: str):
        self.__graph = graph
        self.__downloader = downloader
        self.__queue = queue
        self.__proc = processor
        self.__saver = saver
        self.__db = db
        self.__host = host

    def start(self):
        log(INFO, 'Started')
        while True:
            try:
                for url in self.__queue.get_consumer():
                    log(INFO, url)
                    print(1)
                    page = self.__downloader.load(url, self.__host)

                    body, links, true_url = self.__proc.parse(page)
                    old_url = None
                    if true_url is not None:
                        old_url = url
                        url = true_url

                    links = set(filter(Crawler.is_internal, links))
                    if url in links:
                        links.remove(url)
                    unwatched_links = self.__db.new_set(links)

                    for l in unwatched_links:
                        self.__queue.push(l)

                    if self.article_url(url):
                        key = Crawler.get_key(url)
                        path = self.__saver.save(key, body)
                        self.__db.save_path(key, path)
                        self.__graph.add(key, map(Crawler.get_key, filter(self.article_url, links)))
                        if old_url is not None:
                            self.__graph.add(old_url, [url])

                    self.__db.add(unwatched_links)
                    self.__queue.ack()
                    self.__db.commit()
            except Exception as e:
                log(ERROR, 'Exception: ', e, 'url: ' + url, 'unwatched_links: ' + str(unwatched_links))
                self.__db.rollback()
                time.sleep(30)

    PATH_PREFIX = '/wiki'

    @staticmethod
    def is_internal(path: str):
        return path.startswith(Crawler.PATH_PREFIX)

    @staticmethod
    def get_key(path: str):
        return path[len(f'{Crawler.PATH_PREFIX}/'):]

    @staticmethod
    def article_url(link: str):
        return link.startswith(Crawler.PATH_PREFIX) \
               and re.search(r':(?!_)', link) is None \
               and link != f'{Crawler.PATH_PREFIX}/Main_Page'
