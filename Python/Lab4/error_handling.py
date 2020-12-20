import logging
import sys
import time
from contextlib import contextmanager
from functools import wraps
from typing import Type, Optional, Callable, Tuple, Union

formatter = logging.Formatter(fmt='[%(asctime)s][%(levelname)s] - %(message)s\n', datefmt='%d/%m/%Y %H:%M:%S')
handler = logging.StreamHandler(stream=sys.stdout)
handler.setFormatter(formatter)
logger = logging.Logger('ErrorHandlerLogger', level=logging.DEBUG)
logger.addHandler(handler)


def handle_error(
        re_raise: bool = True,
        log_traceback: bool = True,
        exc_type: Union[Type[Exception], Tuple[Type[Exception]]] = Exception,
        tries: Optional[int] = 1,
        delay: float = 0,
        backoff: float = 1,
):
    def handle_error_decorator(func: Callable):
        @wraps(func)
        def func_wrapper(*args, **kwargs):
            _delay = delay
            for attempt in range(1, tries + 1):
                try:
                    return func(*args, **kwargs)
                except exc_type as e:
                    if attempt == tries:
                        if log_traceback:
                            logger.exception(str(e))
                        if re_raise:
                            raise e
                    else:
                        time.sleep(_delay)
                _delay *= backoff

        return func_wrapper

    return handle_error_decorator


@contextmanager
def handle_error_context(
        re_raise: bool = True,
        log_traceback: bool = True,
        exc_type: Union[Type[Exception], Tuple[Type[Exception]]] = Exception,
):
    @handle_error(re_raise, log_traceback, exc_type, tries=1, delay=0, backoff=1)
    def func():
        yield

    func()
