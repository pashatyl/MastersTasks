import time


def profile(func):
    def wrapper():
        start_time = time.perf_counter()  # default_timer replaced because of it's performance
        r = func()
        print(time.perf_counter() - start_time)
        return r

    return wrapper


class timer:
    def __enter__(self):
        self.start_time = time.perf_counter()
        return self

    def __exit__(self, *exc):
        print((time.perf_counter() - self.start_time))
