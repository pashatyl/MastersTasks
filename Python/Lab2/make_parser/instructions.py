import os
import re
from abc import ABC, abstractmethod
from itertools import product, chain
from operator import itemgetter
from random import random
from time import sleep
from typing import Iterable

from dependency_helper import PriorityHelper


class Instruction(ABC):
    def __init__(self,
                 pattern: str,
                 separator: str):
        self.pattern = pattern
        self.separator = separator

    @abstractmethod
    def parse(self, filepath: str):
        folder, file = os.path.split(filepath)
        result = []

        with open(filepath, 'r', encoding='utf-8') as f:
            source = f.read()
        files_all = re.findall(self.pattern, source, re.S)
        for files in files_all:
            files = files.strip().split(self.separator)
            result.extend(os.path.abspath(os.path.join(folder, file.strip())) for file in files)
        return result


class IncludeCommand(Instruction):
    def __init__(self,
                 pattern: str,
                 separator: str,
                 extension: str):
        super().__init__(pattern, separator)
        self.extension = extension

    def parse(self, filepath: str):
        files = super().parse(filepath)
        return [f for f in files if f.endswith(self.extension)]


class PeerdirCommand(Instruction):
    def __init__(self,
                 pattern: str,
                 separator: str,
                 extension: str):
        super().__init__(pattern, separator)
        self.extension = extension

    def parse(self, filepath: str):
        folders = super().parse(filepath)
        files = []
        for folder in folders:
            folder_files = [os.path.abspath(os.path.join(folder, file)) for file in os.listdir(folder)]
            files.extend(f for f in folder_files if os.path.isfile(f))
        return [f for f in files if f.endswith(self.extension)]


class Build:
    def __init__(self, root_folder: str, makefile_name: str = 'make'):
        self.root = root_folder
        self.makefile = makefile_name

        self.peerdir = PeerdirCommand(r'PEERDIR\((.*?)\)', '\n', ".c")
        self.include = IncludeCommand(r'INCLUDE\((.*?)\)', '\n', ".c")

        self.graph = PriorityHelper()

    def add_dependencies(self, makefiles: Iterable[str]):
        for makefile in makefiles:
            peerdir_files = self.peerdir.parse(makefile)
            include_files = self.include.parse(makefile)
            for f in chain.from_iterable((peerdir_files, include_files)):
                self.graph.add(f)
            for a, b in product(peerdir_files, include_files):
                self.graph += (a, b)

    def make(self):
        makefiles = []
        for root, dirs, files in os.walk(self.root):
            makefiles.extend(os.path.abspath(os.path.join(root, file)) for file in files if file == self.makefile)

        self.add_dependencies(makefiles)

        queue = sorted(self.graph.enumerate_priorities().items(), key=itemgetter(1))
        for f, _ in queue:
            self.build(f)

    def build(self, filepath: str):
        print(f'Building {filepath}... ', end='')
        sleep(random())
        print('Done!')


def example4():
    b = Build(r'/Users/pashatyl/Documents/Study/Python/Lab2/make_parser/proj')
    b.make()


if __name__ == '__main__':
    example4()
