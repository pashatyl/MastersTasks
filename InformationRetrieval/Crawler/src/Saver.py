import gzip
import hashlib
import os


class Saver:
    def __init__(self, root: str):
        if not os.path.exists(root):
            raise ValueError(f'Root {root} does not exists')
        self.__root = root
        self.__exists_cache = set()

    def save(self, key, page):
        tree, name = self.tail(key)
        folder = os.path.join(self.__root, tree)
        if tree not in self.__exists_cache:
            os.makedirs(folder, exist_ok=True)
            self.__exists_cache.add(tree)
        path = os.path.join(folder, name) + '.html.gz'
        with open(path, "wb") as f:
            f.write(gzip.compress(bytes(page, 'utf-8')))
        return path

    def tail(self, key):
        hash_link = hashlib.md5(key.encode()).hexdigest()
        return os.path.join(hash_link[0], hash_link[1], hash_link[2]), hash_link
