import copy


class Node(object):
    def __init__(self, value_, next_=None):
        assert not isinstance(next_, Node) or next_ is not None
        assert value_ is not None
        self._value = value_
        self._next = next_

    def __iter__(self):
        result = copy.copy(self)
        yield result
        while result._next is not None:
            result._value = result._next.value
            result._next = result._next.next
            yield result

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, value_):
        self._value = value_

    @property
    def next(self):
        return self._next

    @next.setter
    def next(self, next_):
        self._next = next_


def flatten_linked_list(node: Node, next_: Node = None):
    nodes = []
    for e in node:
        if isinstance(e.value, Node):
            nodes += flatten_linked_list(e.value, e.next)
            continue
        if e.next is None and next_ is not None:
            e.next = next_
            nodes.append(e.value)
            break
        nodes.append(e.value)

    return nodes


if __name__ == '__main__':
    r2 = Node(3, Node(Node(19, Node(25)), Node(12)))
    r3 = flatten_linked_list(r2)
    for i in r3:
        print(i)
