#!/usr/bin/env python2
import sys
import os
from common import *

folder = sys.argv[-2]
model = sys.argv[-1]

examples = []
for fname in os.listdir(os.path.join(folder, 'cars')):
    neg_feature = read_file(os.path.join(folder, 'cars', fname))
    examples.append([neg_feature, 0])
for fname in os.listdir(os.path.join(folder, 'faces')):
    pos_feature = read_file(os.path.join(folder, 'faces', fname))
    examples.append([pos_feature, 1])

vj = ViolaJonesFramework()
vj.train(examples, 200)  # 200 is mostly for additional guaranty
vj.dump(model)
