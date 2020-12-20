import hashlib
import itertools
import random
import re
import tarfile
from collections import defaultdict
from copy import copy
from statistics import mean
import collections
import bz2
from chardet import detect
from nltk import ngrams
from ndcg import ndcg10_stats

NGRAM_SIZE = 4


def parse_single_result(note):
    return note.split('=')


def parse_ground_truth_line(line):
    items = line.decode().strip().split('\t')
    doc_id = items[0]
    result = [parse_single_result(item) for item in items[1:]]
    return doc_id, {key: float(value) for key, value in result}


def assert_correct_ground_truth(ground_truth):
    for doc_id in ground_truth:
        for other_id in ground_truth[doc_id]:
            assert ground_truth[other_id][doc_id] == ground_truth[doc_id][other_id]


def read_ground_truth(path: str):
    bz_file = bz2.BZ2File(path)
    items = (parse_ground_truth_line(line) for line in bz_file.readlines())
    ground_truth = dict(items)

    assert_correct_ground_truth(ground_truth)
    return ground_truth


def preprocess_document(document: str):
    document = document.lower()
    document = re.sub('[\\n\\r\\t\s+]', ' ', document)
    return document


def decode(b):
    encoding = detect(b)['encoding']
    return b.decode(encoding)


def read_collection(path: str):
    docs = {}
    with tarfile.open(path, 'r:bz2') as tf:
        for ti in tf:
            document_bytes = tf.extractfile(ti).read()
            docs[ti.name] = decode(document_bytes)
    return docs


def invert_dict(data: dict):
    res = defaultdict(list)
    for name, content in data.items():
        res[content].append(name)
    return res


def group_duplicates(docs):
    # returns grouped by same hash {hash: [doc1, doc2, ...]}
    docs_hashed = map_dict(lambda x: hashlib.sha256(x.encode("utf-8")).digest(), docs)
    return invert_dict(docs_hashed)


def show_groups_statistics(groups: dict):
    groups_sizes = list(map(len, groups.values()))
    print(f"Max size: {max(groups_sizes)}")
    print(f"Min size: {min(groups_sizes)}")
    print(f"Average size: {mean(groups_sizes)}")


def exclude_full_duplicates(groups, ground_truth):
    gt = copy(ground_truth)
    for documents in groups.values():
        for combination in itertools.product(documents, repeat=2):
            if combination[0] == combination[1]:
                continue
            assert (ground_truth[combination[0]][combination[1]] != 1,
                    f'Wrong pair: {combination[0]} and {combination[1]}')
            gt[combination[0]].pop(combination[1])
    return [v[0] for v in groups.values()], gt


def map_dict(func, data: dict):
    return {k: func(v) for k, v in data.items()}


def generate_hash_functions(n):
    return [universal_hashing() for _ in range(n)]


def universal_hashing():
    def rand_prime():
        while True:
            p = random.randrange(2 ** 32, 2 ** 34, 2)
            if all(p % n != 0 for n in range(3, int((p ** 0.5) + 1), 2)):
                return p

    m = 2 ** 32 - 1
    p = rand_prime()
    a = random.randint(0, p)
    if a % 2 == 0:
        a += 1
    b = random.randint(0, p)

    def h(x):
        return ((a * x + b) % p) % m

    return h


def get_min_hashes(b: str, hash_funcs):
    ng = list(ngrams(b.split(' '), NGRAM_SIZE))
    return [min(h(hash(e)) for e in ng) for h in hash_funcs]


def score_similarity(index_table, name, hashes):
    length = len(hashes)
    similars = []
    for i, (hash, table) in enumerate(zip(hashes, index_table)):
        similars += table[hash]
    counter = collections.Counter(filter(lambda x: x != name, similars))
    return {k: v / length for (k, v) in dict(counter).items()}


def min_hash_index_table(size, doc_hash_dict):
    # returns list (for each hash func) of dicts {min_hash: name}
    hash_value_maps = [defaultdict(list) for _ in range(size)]
    for k, v in doc_hash_dict.items():
        for i, val in enumerate(v):
            hash_value_maps[i][val].append(k)
    return hash_value_maps


def build_lookup_table(docs, hash_count):
    hash_funcs = generate_hash_functions(hash_count)

    # {name: [min_hashes]}
    min_hashes_for_docs = map_dict(lambda v: get_min_hashes(v, hash_funcs), docs)

    index_table = min_hash_index_table(hash_count, min_hashes_for_docs)
    tops = {}
    for name, hashes in min_hashes_for_docs.items():
        similarity_sorted = sorted(score_similarity(index_table, name, hashes).items(), key=lambda x: -x[1])
        tops[name] = [key for key, value in similarity_sorted]
    return tops


if __name__ == '__main__':
    ground_truth = read_ground_truth('/Users/pashatyl/Downloads/ground_truth.tsv.bz2')
    docs = read_collection('/Users/pashatyl/Downloads/collection.tar.bz2')

    docs = map_dict(preprocess_document, docs)
    groups = group_duplicates(docs)
    show_groups_statistics(groups)

    cleared_docs, ground_truth = exclude_full_duplicates(groups, ground_truth)
    # {name: text}
    cleared_docs = {k: v for k, v in docs.items() if k not in cleared_docs}
    for i in [1, 10, 50, 100]:
        table = build_lookup_table(cleared_docs, i)
        ndcg10_stats(f"Ndcg@10: {i}", table, ground_truth)
