import time


class Expirable(object):
    def __init__(self, object, ttl: int):
        self._object = object
        self._ttl = ttl or 0
        self._ttd = time.time() + ttl or 0

    @property
    def expiration(self):
        return self._ttd

    def renew(self):
        self._ttd = time.time() + self._ttl or 0

    @property
    def object(self):
        return self._object


class LRUCache(object):
    def __init__(self, size, ttl=1):
        self._size = size
        self._default_ttl = ttl
        self._count = 0
        self._data = {}

    @property
    def count(self):
        return self._count

    def get(self, key):
        return self[key]

    def __getitem__(self, key):
        if self._data[key]:
            self._data[key].renew()

        return self._data[key].object or None

    def add(self, key, object, ttl=None):
        if key not in self._data:
            self._count += 1
            if self._count > self._size:
                self.remove_oldest()

        self._data[key] = Expirable(object, ttl or self._default_ttl)

    def remove_oldest(self):
        key = min(self._data, key=lambda k: self._data[k].expiration)
        self.remove_object(key)

    def contains(self, key):
        return key in self

    def __contains__(self, key):
        return self._data.__contains__(key)

    def remove_object(self, key):
        if self._data[key]:
            self._data.pop(key)
            self._count -= 1

    def clear(self):
        self._data.clear()
        self._count = 0
