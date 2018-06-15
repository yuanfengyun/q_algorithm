import logging
logging.getLogger().setLevel(logging.INFO)
import mxnet as mx
import numpy as np

data = [[1],[2],[3],[4],[5],[6],[7],[8],[9],[10]]
label = [1,2,3,4,5,6,7,8,9,10]

batch_size = 32
ntrain = int(data.shape[0]*0.8)
#训练数据集迭代器
train_iter = mx.io.NDArrayIter(data[:ntrain, :], label[:ntrain], batch_size, shuffle=True)
#验证数据集迭代器
val_iter = mx.io.NDArrayIter(data[ntrain:, :], label[ntrain:], batch_size)

#定义网络
net = mx.sym.Variable('data')
#输入层
net = mx.sym.FullyConnected(net, name='fc1', num_hidden=1)
net = mx.sym.Activation(net, name='relu1', act_type="relu")

#隐藏层

#输出层
#net = mx.sym.FullyConnected(net, name='fc2', num_hidden=26)
#net = mx.sym.SoftmaxOutput(net, name='softmax')
mx.viz.plot_network(net)

train_iter.reset()

#创建模型
mod = mx.mod.Module(symbol=net,
                    context=mx.cpu(),
                    data_names=['data'],
                    label_names=['softmax_label'])

#训练
mod.fit(train_iter,
        eval_data=val_iter,
        optimizer='sgd',
        optimizer_params={'learning_rate':0.1},
        eval_metric='acc',
        num_epoch=8)

#预测
y = mod.predict(val_iter)
assert y.shape == (4000, 26)

#评估
score = mod.score(val_iter, ['acc'])
print("Accuracy score is %f" % (score[0][1]))