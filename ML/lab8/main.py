import numpy as np
import pandas as pd
from scipy.spatial import distance


def split_dataset(df, n):
    train_df = df.sample(frac=n)
    test_df = df.loc[~df.index.isin(train_df.index)]
    return train_df[[0, 1, 2, 3]].values, train_df[4].values, \
           test_df[[0, 1, 2, 3]].values, test_df[4].values


def knn_predict(x, y, object, k, dist_alg=distance.cosine):
    dists = []
    for vect in x:
        dist = dist_alg(vect, object)
        dists.append(dist)
    dists = np.array(dists)
    top_k_inds = np.argsort(dists)[k]
    top_k_classes = y[top_k_inds]
    return np.unique(top_k_classes)[-1]


def test_model(df, n, k, dist_alg):
    x, y, X, Y = split_dataset(df, n)
    acc = 0
    for elem in zip(X, Y):
        if knn_predict(x, y, elem[0], k, dist_alg) == elem[1]:
            acc += 1
    return acc / len(X)


if __name__ == '__main__':
    df = pd.read_csv('iris.data.csv', header=None)
    acc = test_model(df, n=0.8, k=5, dist_alg=distance.sqeuclidean)
    print('Accuracy =', acc)
