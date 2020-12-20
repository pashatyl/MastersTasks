import numpy as np
np.random.seed(42)


def dcg_score(relevs, p):
    score = 0
    for position, relev in enumerate(relevs[:p]):
        score += (2.0 ** relev - 1) / np.log2(position + 2)
    return score


def idcg10_single_doc(ref_doc_id, ground_truth):
    return dcg_score(sorted(ground_truth[ref_doc_id].values(), reverse=True), 10)


def dcg10_single_doc(ref_doc_id, candidates_doc_ids, ground_truth):
    assert len(set(candidates_doc_ids)) == len(candidates_doc_ids), "All candidates must be different"
    return dcg_score([ground_truth[ref_doc_id].get(doc_id, 0.0) for doc_id in candidates_doc_ids], 10)


def ndcg10_single_doc(ref_doc_id, candidates_doc_ids, ground_truth):
    # Полученный вашим алгоритмом dcg10
    dcg = dcg10_single_doc(ref_doc_id, candidates_doc_ids, ground_truth)

    # Идеальный dcg10 для этого документа
    # чтобы ускорить, стоит предпросчитать ответы
    idcg = idcg10_single_doc(ref_doc_id, ground_truth)

    return dcg / idcg if idcg > 1e-10 else 0.0


def ndcg10_stats(title, doc_to_candidates, ground_truth):
    scores = []
    for doc_id, candidates in list(doc_to_candidates.items())[:10]:
        scores.append(ndcg10_single_doc(doc_id, candidates, ground_truth))
    print(title)
    print('  Average:\t', np.mean(scores))
    print('  Median: \t', np.median(scores))
    print('  Std Dev:\t', np.std(scores))
    print('  Minimum:\t', np.min(scores))
    print('  Maximum:\t', np.max(scores))