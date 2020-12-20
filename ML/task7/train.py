from libsvm.python import svmutil
import pandas as pd
import numpy as np
import os
import json
from tqdm import tqdm

LIBSVM_PATH = os.path.abspath('/Users/pashatyl/Documents/Study/ML/task7/libsvm')
R = 5


def preproc_data(data_path: str, out_path: str = ''):
    print(f'Preprocessing {data_path}')
    if not out_path:
        out_path = data_path + '_preproc'
    df = pd.read_csv(data_path, header=None)
    with open(out_path, 'w') as f:
        for ind, items in df.iterrows():
            data = items.to_list()
            features = " ".join(f'{i + 1}:{d}' for i, d in enumerate(data[:-1]) if d > 0)
            f.write(f'{int(data[-1])} {features}\n')
    print(f'Saved to {out_path}')
    return out_path


def scale_data(train_data_path: str, test_data_path: str):
    print('Scaling dataset')
    out_train_path = f'{train_data_path}_scaled'
    out_test_path = f'{test_data_path}_scaled'
    svm_scale_path = os.path.join(LIBSVM_PATH, 'svm-scale')
    os.system(f'{svm_scale_path} -l 0 -u 1 -s range {train_data_path} > {out_train_path}')
    os.system(f'{svm_scale_path} -r range {test_data_path} > {out_test_path}')
    return out_train_path, out_test_path


def train(y_train: np.array, X_train: np.array, y_test: np.array, X_test: np.array, degree: int = 3, c: int = 2):
    model = svmutil.svm_train(y_train, X_train, f'-t 1 -d {degree} -c {c} -q -r {R}')
    p_label, p_acc, p_val = svmutil.svm_predict(y_test, X_test, model, '-q')
    acc, mse, _ = p_acc
    svmutil.svm_save_model(f'./models/m_d{degree}_c{c}', model)
    return acc, mse


def kfold(X_train: np.array, y_train: np.array, k: int = 10):
    X_split = np.vsplit(X_train.todense(), k)
    y_split = np.split(y_train, k)
    for i in range(k):
        X_val, y_val = X_split[i], y_split[i]
        X_t = np.vstack([x for j, x in enumerate(X_split) if j != i])
        y_t = np.hstack([x for j, x in enumerate(y_split) if j != i])
        yield X_t, y_t, X_val, y_val


def grid_search(X_train: np.array, y_train: np.array, degree_range: range, c_range: list):
    results = []
    for d in degree_range:
        res = []
        for c in tqdm(c_range):
            errors = []
            for X_t, y_t, X_val, y_val in kfold(X_train, y_train):
                acc, mse = train(y_t, X_t, y_val, X_val, degree=d, c=c)
                errors.append(100 - acc)
            e = np.mean(errors)
            res.append({'c': c, 'err': e, 'std': np.std(errors)})
        results.append({
            'd': d,
            'res': res
        })
    return results


if __name__ == '__main__':
    train_path = '/Users/pashatyl/docs/libsvm/datasets/train'
    test_path = '/Users/pashatyl/docs/libsvm/datasets/test'
    train_path = preproc_data(train_path)
    test_path = preproc_data(test_path)
    train_path, test_path = scale_data(train_path, test_path)

    y_train, X_train = svmutil.svm_read_problem(train_path, return_scipy=True)

    y_test, X_test = svmutil.svm_read_problem(test_path, return_scipy=True)

    deg = 15
    #c_range = [2 ** i for i in range(-deg, (deg + 1), 2)]
    c_range = [2048]
    degree_range = range(1, 5)
    #res = grid_search(X_train, y_train, degree_range=degree_range, c_range=c_range)

    res = []
    for d in degree_range:
        model = svmutil.svm_train(y_train, X_train, f'-t 1 -d {d} -c {c_range[3]} -q -r {R}')
        p_label, p_acc, p_val = svmutil.svm_predict(y_test, X_test, model, '-q')
        res.append({'d': d, 'err': 100 - p_acc[0], 'v_cnt': model.l})

    with open('results.json', 'w') as f:
        json.dump(res, f, indent=2)
