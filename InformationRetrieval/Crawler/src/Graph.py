from src.Database import Database


class Graph:
    def __init__(self, db: Database):
        self.__db = db

    def add(self, v1, v2s):
        self.__db.execute_batch('INSERT INTO edges VALUES (?, ?)', [(v1, v2) for v2 in v2s])
