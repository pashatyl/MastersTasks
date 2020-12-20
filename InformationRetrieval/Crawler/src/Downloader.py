from urllib.parse import urljoin

import requests


class Downloader:
    @staticmethod
    def load(url: str, host: str = ''):
        return requests.get(urljoin(host, url))
