from error_handling import handle_error, handle_error_context
from functional import BoundedMeta, BoundedBase, smart_function


def example1():
    @handle_error(re_raise=False)
    def some_function():
        x = 1 / 0

    some_function()
    print(1)


def example2():
    @handle_error(re_raise=False, exc_type=TypeError)
    def some_function():
        x = 1 / 0

    some_function()
    print(1)


def example3():
    def random():
        return 0.5

    @handle_error(re_raise=True, tries=3, delay=0.5, backoff=2)
    def some_function():
        print('some_function called')
        if random() < 0.75:
            x = 1 / 0

    some_function()


def example4():
    with handle_error_context(log_traceback=True, exc_type=ValueError):
        raise ValueError()


def example5():
    class C(metaclass=BoundedMeta, max_instance_count=2):
        pass

    c1 = C()
    c2 = C()
    try:
        c3 = C()
    except TypeError:
        print('everything works fine!')
    else:
        print('something goes wrong!')


def example6():
    class D(BoundedBase):
        @classmethod
        def get_max_instance_count(cls):
            return 1

    d1 = D()
    try:
        d2 = D()
    except TypeError:
        print('everything works fine!')
    else:
        print('something goes wrong!')


def example7():
    for real_call_count in range(1, 10):
        result = smart_function()
        assert result == real_call_count, f"Function was called {real_call_count} times but returned {result}"
    print('everything works fine!')


if __name__ == '__main__':
    for f in [
        example1,
        example2,
        example3,
        example4,
        example5,
        example6,
        example7,
    ]:
        try:
            f()
        except Exception as e:
            print(f'Raised {type(e)}: {e}')
        finally:
            print(f"{f.__name__} finished")
            print('--------')
