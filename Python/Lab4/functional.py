from abc import ABC, abstractmethod


class BoundedMeta(type):
    instances = 0

    def __new__(mcs, name, bases, attrs, max_instance_count=1):
        mcs.max_instance_count = max_instance_count
        return super().__new__(mcs, name, bases, attrs)

    def __call__(cls, *args, **kwargs):
        if cls.max_instance_count is not None and cls.instances == cls.max_instance_count:
            raise TypeError(f"Trying to create {cls.instances + 1} but limit is {cls.max_instance_count}")
        cls.instances += 1
        return super().__call__(*args, **kwargs)


class BoundedBase(ABC):
    instances = 0

    @classmethod
    @abstractmethod
    def get_max_instance_count(cls):
        pass

    def __new__(cls, *args, **kwargs):
        max_instance_count = cls.get_max_instance_count()
        if max_instance_count is not None and cls.instances == max_instance_count:
            raise TypeError(f"Trying to create {cls.instances + 1} but limit is {max_instance_count}")
        cls.instances += 1


def smart_function():
    smart_function.times = getattr(smart_function, 'times', 0) + 1
    return smart_function.times
