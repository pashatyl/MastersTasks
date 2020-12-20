import os
import threading
import time

from LRUCache import LRUCache


# TODO: Replace with smart lock impl. Now all mutation operations works synchronously
class ThreadSafeLRUCache(LRUCache):
    def __init__(self, size, clean_period, cold_enabled=False, ttl=1):
        super().__init__(size, ttl)
        self._lock = threading.Lock()
        cleanup_impl = self.__file_backup_cleaner__ if cold_enabled else super().remove_object
        self.cleanup_daemon_enabled = True
        self.cleanup_daemon = \
            threading.Thread(target=self.__cleaner__, args=(cleanup_impl, clean_period,), daemon=True)
        self.cleanup_daemon.start()

    def __file_backup_cleaner__(self, key):
        path = 'storage/'
        if not os.path.exists(path):
            os.mkdir(path)
        with open(path + key, "w") as f:  # File will be truncated
            f.write(f'{self._data[key].object} ttd: {self._data[key].expiration} time: {time.time()}')
            super().remove_object(key)

    def __cleaner__(self, cleanup_impl, clean_period):
        while self.cleanup_daemon_enabled:
            time.sleep(clean_period)
            with self._lock:
                t = time.time()
                keys = [k for k, v in self._data.items() if v.expiration <= t]
                for k in keys:
                    cleanup_impl(k)

    def add(self, key, value, ttl=None):
        with self._lock:
            super().add(key, value, ttl)

    def remove_object(self, key):
        with self._lock:
            super().remove_object(key)

    def clear(self):
        with self._lock:
            super().clear()

    def disable_cleanup_daemon(self):
        self.cleanup_daemon_enabled = False
