from copy import deepcopy


def optimize(X_, D, F, C, n_iterations, X_bounds=(-2, 2)):
    X_copy = deepcopy(X_)
    losses = []
    min_loss = np.inf
    X_min = None
    gen_points = [deepcopy(X_copy)]
    for j in tqdm(range(n_iterations)):
        generation_losses = []
        for i in range(len(X_copy)):
            x = X_copy[i]
            w = mutation(X_copy, F)
            w = crossover(x, w, C)
            for w_i in w:
                if w_i < X_bounds[0] or w_i > X_bounds[1]:
                    w = random.sample(X_copy, 1)[0]
                    break
            w_loss = rosenbrock_loss(w, D)
            x_loss = rosenbrock_loss(x, D)
            gen_loss = min(w_loss, x_loss)
            generation_losses.append(gen_loss)
            if w_loss <= x_loss:
                X_copy[i] = w
            if gen_loss < min_loss:
                min_loss = gen_loss
                X_min = X_copy[i]
        gen_points.append(deepcopy(X_copy))
        losses.append(np.mean(generation_losses))
    return X_min, losses, gen_points
