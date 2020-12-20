from copy import deepcopy, copy
from itertools import chain
from typing import Dict, Any, Set, Optional, Union, Tuple, Type


class DependencyHelper:
    def __init__(self):
        self.graph: Dict[Any, Set[Any]] = dict()

    def add(self, lead: Any, slave: Optional[Any] = None):
        if lead not in self.graph:
            self.graph[lead] = set()
        if slave is not None:
            if slave not in self.graph:
                self.graph[slave] = set()
            self.graph[lead].add(slave)

    def update(self, other: 'DependencyHelper'):
        for lead, slaves in other.graph.items():
            if slaves:
                for slave in slaves:
                    self.add(lead, slave)
            else:
                self.add(lead)

    def __iadd__(self, other: Union[Tuple[Any, Any], 'DependencyHelper']) -> 'DependencyHelper':
        if isinstance(other, DependencyHelper):
            self.update(other)
        elif isinstance(other, tuple):
            self.add(*other)
        return self

    def remove(self, lead: Any, slave: Any):
        if lead in self.graph:
            self.graph[lead].remove(slave)

    def __isub__(self, other: Union[Tuple[Any, Any], 'DependencyHelper']) -> 'DependencyHelper':
        if isinstance(other, DependencyHelper):
            for lead, slaves in other.graph.items():
                for slave in slaves:
                    self.remove(lead, slave)
        elif isinstance(other, tuple):
            self.remove(*other)
        return self

    def copy(self) -> 'DependencyHelper':
        return deepcopy(self)

    def get_dependent(self, lead) -> Tuple[Any, ...]:
        return tuple(self.graph.get(lead, []))

    def has_cycle_dependency(self) -> bool:
        graph = copy(self.graph)
        while True:
            vertex_set = set(graph).intersection(chain.from_iterable(graph.values()))
            sub_graph = {k: vertex_set & vs for k, vs in graph.items()
                         if k in vertex_set and vertex_set & vs}
            if sub_graph == graph:
                break
            else:
                graph = sub_graph
        return True if graph else False

    def __bool__(self) -> bool:
        return not self.has_cycle_dependency()


class PriorityHelper(DependencyHelper):
    def enumerate_priorities(self) -> Dict[Any, int]:
        if self.has_cycle_dependency():
            raise ValueError("Cannot create priority queue because of cycle dependency")
        inverted_graph = {k: set() for k in self.graph.keys()}
        for lead, slaves in self.graph.items():
            for slave in slaves:
                inverted_graph[slave].add(lead)

        priorities = {k: len(inverted_graph.keys()) for k in inverted_graph.keys()}
        for slave, leads in inverted_graph.items():
            if len(leads) == 0:
                priorities[slave] = 0

        def traversal(node, cur_priority):
            priorities[node] = min(cur_priority, priorities[node])
            for n in self.graph[node]:
                traversal(n, cur_priority + 1)

        for node in priorities.keys():
            if priorities[node] == 0:
                traversal(node, 0)

        return priorities


def example1():
    dependency_helper = DependencyHelper()
    dependency_helper.add(1, 2)
    dependency_helper += (2, 1)
    assert not dependency_helper
    dependency_helper.add(2, 3)
    dependency_helper.remove(2, 1)
    assert dependency_helper


def example2():
    ph = PriorityHelper()
    ph.add('a', 'b')
    ph.add('a', 'c')
    ph.add('d', 'c')
    ph.add('e')
    print(ph.enumerate_priorities())


def example3():
    ph = PriorityHelper()
    ph.add('a', 'b')
    ph.add('b', 'c')
    ph.add('c', 'd')
    ph.add('d', 'e')
    ph.add('f', 'c')
    ph.add('f', 'd')
    ph.add('g', 'h')
    ph.add('g', 'i')
    ph.add('g', 'j')
    ph.add('i', 'j')
    ph.add('k')
    print(ph.enumerate_priorities())

    ph2 = ph.copy()
    ph2.add('e', 'a')
    try:
        print(ph2.enumerate_priorities())
    except ValueError as e:
        print(str(e))


if __name__ == '__main__':
    example1()
    example2()
    example3()
