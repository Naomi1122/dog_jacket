import * as tf from '@tensorflow/tfjs-node';
import fs from 'fs';

// 读取CSV文件并解析特征值和目标标签
function readCSVFile(filePath) {
  const fileContent = fs.readFileSync(filePath, 'utf-8');
  const lines = fileContent.split('\n');
  const features = [];
  const labels = [];

  for (let i = 1; i < lines.length; i++) {
    const values = lines[i].trim().split(',').map(parseFloat);
    features.push(values.slice(0, values.length - 1)); // 去掉最后一个值（目标标签）
    labels.push(values[values.length - 1]);
  }

  return { features, labels };
}

// 加载CSV数据并解析特征值和目标标签
const csvFilePath = './dog_dataset.csv';
const { features, labels } = readCSVFile(csvFilePath);

// 数据预处理
const xs = tf.tensor2d(features);
const ys = tf.tensor1d(labels, 'int32');

// 构建CNN模型
const model = tf.sequential();
model.add(tf.layers.reshape({ targetShape: [6, 1, 1], inputShape: [6] }));
model.add(tf.layers.conv2d({ kernelSize: 3, filters: 16, activation: 'relu' }));
model.add(tf.layers.maxPooling2d({ poolSize: [2, 1] }));
model.add(tf.layers.flatten());
model.add(tf.layers.dense({ units: 64, activation: 'relu' }));
model.add(tf.layers.dense({ units: 5, activation: 'softmax' })); // 5个类别

// 编译模型
model.compile({
  optimizer: tf.train.adam(),
  loss: 'sparseCategoricalCrossentropy',
  metrics: ['accuracy']
});

// 训练模型
const batchSize = 32;
const epochs = 10;

model.fit(xs.reshape([features.length, 6, 1, 1]), ys, {
  batchSize,
  epochs,
  shuffle: true,
  callbacks: tf.node.tensorBoard('logs')
}).then(info => {
  console.log('Final accuracy', info.history.acc);
});

// 保存模型
const modelSavePath = '../model';
await model.save(`file://${modelSavePath}`);
console.log(`Model saved to ${modelSavePath}`);
