	DATASET_PATH=/Users/pashatyl/docs/libsvm/datasets/spambase.data.shuffled
	TRAIN_PATH=/Users/pashatyl/docs/libsvm/datasets/train
	TEST_PATH=/Users/pashatyl/docs/libsvm/datasets/test

	head -3450 $DATASET_PATH > $TRAIN_PATH
	tail -1151 $DATASET_PATH > $TEST_PATH

