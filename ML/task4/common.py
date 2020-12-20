import numpy as np
import pickle  # For model dump
import scipy.misc  # For imread only


class ViolaJonesFramework:
    def __init__(self):
        self.alphas = []
        self.clfs = []

    def train(self, training, rounds):
        weights = np.zeros(len(training))
        training_data = []

        for x in range(len(training)):
            training_data.append((cummulative_sum(training[x][0]), training[x][1]))
            weights[x] = 1.0 / (len(training))

        features = self.build_features(training_data[0][0].shape)
        X, y = self.apply_features(features, training_data)

        # Select top contrast features with some euristics:)
        indices = sorted(range(len(X)), key=lambda i: np.sum(X[i]), reverse=True)[:int(len(X) / 16.6)]
        X = X[indices]
        features = features[indices]

        for t in range(rounds):
            weights = weights / np.linalg.norm(weights)
            haar_classifiers = self.train_haar_classifier(X, y, features, weights)
            clf, error, accuracy = self.select_best(haar_classifiers, weights, training_data)
            beta = error / (1.0 - error)
            for i in range(len(accuracy)):
                weights[i] = weights[i] * (beta ** (1 - accuracy[i]))
            alpha = np.log(1.0 / beta)
            self.alphas.append(alpha)
            self.clfs.append(clf)

    def train_haar_classifier(self, X, y, features, weights):
        total_pos, total_neg = 0, 0
        for w, label in zip(weights, y):
            if label == 1:
                total_pos += w
            else:
                total_neg += w

        classifiers = []
        for index, feature in enumerate(X):
            applied_feature = sorted(zip(weights, feature, y), key=lambda x: x[1])

            pos_seen, neg_seen = 0, 0
            pos_weights, neg_weights = 0, 0
            min_error, best_feature, best_threshold, best_polarity = float('inf'), None, None, None
            for w, f, label in applied_feature:
                error = min(neg_weights + total_pos - pos_weights, pos_weights + total_neg - neg_weights)
                if error < min_error:
                    min_error = error
                    best_feature = features[index]
                    best_threshold = f
                    best_polarity = 1 if pos_seen > neg_seen else -1

                if label == 1:
                    pos_seen += 1
                    pos_weights += w
                else:
                    neg_seen += 1
                    neg_weights += w

            clf = HaarClassifier(best_feature[0], best_feature[1], best_threshold, best_polarity)
            classifiers.append(clf)
        return classifiers

    def build_features(self, image_shape):
        height, width = image_shape
        features = []
        for w in range(4, width, 2):
            for h in range(2, height, 2):
                i = 0
                while i + w < width:
                    j = 0
                    while j + h < height:
                        immediate = Rectangle(i, j, w, h)

                        right = Rectangle(i + w, j, w, h)
                        if i + 2 * w < width:
                            features.append(([right], [immediate]))
                        bottom = Rectangle(i, j + h, w, h)
                        if j + 2 * h < height:
                            features.append(([immediate], [bottom]))

                        right_2 = Rectangle(i + 2 * w, j, w, h)
                        if i + 3 * w < width:
                            features.append(([right], [right_2, immediate]))
                        bottom_2 = Rectangle(i, j + 2 * h, w, h)
                        if j + 3 * h < height:
                            features.append(([bottom], [bottom_2, immediate]))

                        bottom_right = Rectangle(i + w, j + h, w, h)
                        if i + 2 * w < width and j + 2 * h < height:
                            features.append(([right, bottom], [immediate, bottom_right]))

                        j += 1
                    i += 1
        return np.array(features)

    def select_best(self, classifiers, weights, training_data):
        best_clf, best_error, best_accuracy = None, float('inf'), None
        for clf in classifiers:
            error, accuracy = 0, []
            for data, w in zip(training_data, weights):
                correctness = abs(clf.classify(data[0]) - data[1])
                accuracy.append(correctness)
                error += w * correctness
            error = error / len(training_data)
            if error < best_error:
                best_clf, best_error, best_accuracy = clf, error, accuracy
        return best_clf, best_error, best_accuracy

    def apply_features(self, features, training_data):
        X = np.zeros((len(features), len(training_data)))
        y = np.array(list(map(lambda data: data[1], training_data)))
        i = 0
        for positive_regions, negative_regions in features:
            feature = lambda ii: sum([pos.sum(ii) for pos in positive_regions]) - sum(
                [neg.sum(ii) for neg in negative_regions])
            X[i] = list(map(lambda data: feature(data[0]), training_data))
            i += 1
        return X, y

    def classify(self, image):
        total = 0
        ii = cummulative_sum(image)
        for alpha, clf in zip(self.alphas, self.clfs):
            total += alpha * clf.classify(ii)
        return 1 if total >= 0.5 * sum(self.alphas) else 0

    def dump(self, filename):
        with open(filename + ".pkl", 'wb') as f:
            pickle.dump(self, f)

    @staticmethod
    def load(filename):
        with open(filename + ".pkl", 'rb') as f:
            return pickle.load(f)


class HaarClassifier:
    def __init__(self, positive_regions, negative_regions, threshold, polarity):
        self.positive_regions = positive_regions
        self.negative_regions = negative_regions
        self.threshold = threshold
        self.polarity = polarity

    def feature(self, ii):
        return sum([pos.sum(ii) for pos in self.positive_regions]) - \
               sum([neg.sum(ii) for neg in self.negative_regions])

    def classify(self, ii):
        return 1 if self.polarity * self.feature(ii) < self.polarity * self.threshold else 0


class Rectangle:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    def sum(self, ii):
        return ii[self.y + self.height][self.x + self.width] + \
               ii[self.y][self.x] - \
               ii[self.y + self.height][self.x] - \
               ii[self.y][self.x + self.width]


def cummulative_sum(image):
    ii = np.zeros(image.shape)
    s = np.zeros(image.shape)
    for y in range(len(image)):
        for x in range(len(image[y])):
            s[y][x] = s[y - 1][x] + image[y][x] if y - 1 >= 0 else image[y][x]
            ii[y][x] = ii[y][x - 1] + s[y][x] if x - 1 >= 0 else s[y][x]
    return ii


def read_file(fname):
    image = scipy.misc.imread(fname)
    assert image.shape == (26, 40, 3)
    return image[8:-8, 6:-6, 0]  # Trim unnecessary boundaries
