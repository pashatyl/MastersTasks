import sqlite3
from src.Helpers import chunks


class Database:
    def __init__(self):
        self.__conn = sqlite3.connect('crawler.db')
        self.__c = self.__conn.cursor()
        self.__contains_cache = set()

    def contains(self, key):
        if key in self.__contains_cache:
            return True
        self.execute(f'SELECT 1 FROM processed WHERE id=?;', (key,))
        return self.__c.rowcount > 0

    def add(self, key: set):
        self.__contains_cache = self.__contains_cache.union(key)
        self.execute_batch(f'INSERT OR REPLACE INTO processed VALUES (?)', map(lambda x: (x,), key))

    def new_set(self, links: set):
        links -= self.__contains_cache
        rows = []
        for ls in chunks(list(links), 50):
            question_marks = ', '.join('?' for _ in ls)
            self.__c.execute(f'''SELECT id 
                            FROM processed
                            WHERE id IN ({question_marks})''', list(ls))
            rows += [x[0] for x in self.__c.fetchall()]
        return links - set(rows)

    def save_path(self, key, path):
        self.execute(f'INSERT OR REPLACE INTO pages VALUES (?, ?)', (key, path))

    def execute(self, query, *params):
        self.__c.execute(query, *params)

    def execute_batch(self, query: str, params):
        self.__c.executemany(query, params)

    def commit(self):
        self.__conn.commit()

    def rollback(self):
        self.__conn.rollback()
