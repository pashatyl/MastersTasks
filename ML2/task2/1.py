from sklearn import linear_model
from sklearn.datasets import load_boston
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split
import numpy as np

boston = load_boston()
TEST_SIZE = 0.2


def pierson(x, y):
    return np.corrcoef(x, y)[0][1]


def filter(features, lables, n: int = 5):
    return np.argsort(list(map(lambda x: np.abs(pierson(x, lables)), features.T)))[-1:-n - 1:-1]


def greedy_forward_selection(data, lables, metric, n: int = 5):
    top = []
    features = set(range(data.shape[1]))
    for _ in range(n):
        res = [(f, metric(data[:, top + [f]], lables)) for f in features]
        best = min(res, key=lambda x: x[1])[0]
        top.append(best)
        features.remove(best)
    return top


def greedy_backward_selection(data, lables, metric, n: int = 5):
    top = set(range(data.shape[1]))
    for _ in range(data.shape[1] - n):
        res = [(f, metric(data[:, list(top - {f})], lables)) for f in top]
        best = min(res, key=lambda x: x[1])[0]
        top.remove(best)
    return list(top)


def linear_regression_metric(features, lables):
    X_train, X_test, y_train, y_test = train_test_split(features, lables, test_size=TEST_SIZE, random_state=0)
    reg = LinearRegression().fit(X_train, y_train)
    return mean_squared_error(reg.predict(X_test), y_test)


def lasso_selection(features, lables, k: float = 1e-4, n: int = 5, step: float = 0.001):
    alpha = None
    min_mse = np.inf

    for _alpha in np.arange(step, 10, step):
        lasso = linear_model.Lasso(alpha=_alpha)
        X_train, X_test, y_train, y_test = train_test_split(features, lables, test_size=TEST_SIZE, random_state=0)
        lasso.fit(X_train, y_train)
        cur_selected_features = np.abs(lasso.coef_) > k
        selected_count = np.sum(cur_selected_features)
        if selected_count > n or selected_count == 0:
            continue
        mse = mean_squared_error(lasso.predict(X_test), y_test)
        if mse <= min_mse:
            min_mse = mse
            alpha = _alpha

    if alpha is None:
        raise Exception(f'Optimal featurese not found. Check k={k} n={n} step={step}')

    lasso = linear_model.Lasso(alpha=alpha)
    lasso.fit(features, lables)
    return list(np.argsort(np.abs(lasso.coef_))[-1:-n - 1:-1])


if __name__ == '__main__':
    data = np.array(boston['data'])
    target = np.array(boston['target'])

    N = 5
    print(filter(data, target, N))
    print(greedy_forward_selection(data, target, linear_regression_metric, N))
    print(greedy_backward_selection(data, target, linear_regression_metric, N))
    print(lasso_selection(data, target, k=1e-4, n=N))
