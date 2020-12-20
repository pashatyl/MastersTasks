import re

from bs4 import BeautifulSoup


class Processor:
    @staticmethod
    def clear_anchor(url: str):
        idx = url.find('#')
        if idx == -1:
            return url
        return url[:idx]

    @staticmethod
    def get_page_links(page: BeautifulSoup):
        return [x for x in
                (Processor.clear_anchor(p["href"]) for p in page.find_all("a", href=True))
                if len(x) > 0]

    @staticmethod
    def parse(html):
        soup = BeautifulSoup(html.text, 'lxml')
        redirect = soup.find('span', attrs={'class': 'mw-redirectedfrom'})
        url = None
        if redirect is not None:
            m = re.match('.*wgPageName\":\"([^\"]*)\"', str(soup.script))
            url = m.group(1)
        return str(soup.body), Processor.get_page_links(soup), url
