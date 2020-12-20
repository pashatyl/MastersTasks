#!/usr/bin/env python2
import sys

from common import *

model = sys.argv[-2]
fname = sys.argv[-1]

vj = ViolaJonesFramework.load(model)

print vj.classify(read_file(fname))
