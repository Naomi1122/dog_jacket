import * as tf from '@tensorflow/tfjs'

// 构建CNN模型
function createModel() {
  // 构建CNN模型
function createModel() {
    const model = tf.sequential();
    model.add(tf.layers.conv2d({ inputShape: [6, 1, 1], kernelSize: 3, filters: 16, activation: 'relu' }));
    model.add(tf.layers.maxPooling2d({ poolSize: 2, strides: 2 }));
    model.add(tf.layers.flatten());
    model.add(tf.layers.dense({ units: 64, activation: 'relu' }));
    model.add(tf.layers.dense({ units: 4, activation: 'softmax' }));
    return model;
  }
  
  // 数据预处理
  function preprocessData(data) {
    const xs = data.map(entry => [entry.AX, entry.AY, entry.AZ, entry.GX, entry.GY, entry.GZ]);
    const ys = data.map(entry => entry.Type);
    const xsTensor = tf.tensor2d(xs);
    const ysTensor = tf.oneHot(tf.tensor1d(ys, 'int32'), 4); // 4 classes (0 to 3)
    return { xs: xsTensor, ys: ysTensor };
  }
  
  // 加载并预处理数据
  const rawData = [
    { AX: 2.73, AY: 2.15, AZ: -8.95, GX: 0, GY: 0.02, GZ: -0.01, Type: 0 },
    { AX: -9.41, AY: 2.29, AZ: -1.43, GX: 0, GY: 0.01, GZ: -0.01, Type: 1 },
    { AX: 2.81, AY: 1.69, AZ: -9.04, GX: 0, GY: 0.02, GZ: -0.01, Type: 3 },
    { AX: -9.41, AY: 2.21, AZ: -1.53, GX: -0.01, GY: 0.02, GZ: -0.01, Type: 1 },
    { AX: 2.82, AY: 1.67, AZ: -9.05, GX: 0, GY: 0.01, GZ: -0.01, Type: 3 },
  ];
  const { xs, ys } = preprocessData(rawData);
  
  // 构建和训练模型
  const model = createModel();
  model.compile({ optimizer: 'adam', loss: 'categoricalCrossentropy', metrics: ['accuracy'] });
  model.fit(xs.reshape([5, 6, 1, 1]), ys, { epochs: 10 });
  
  // 保存模型
  const modelSavePath = './model.json';
  model.save(`file://${modelSavePath}`)
    .then(() => console.log(`Model saved to ${modelSavePath}`))
    .catch(err => console.error('Error saving model:', err));
  
}

// 加载模型
async function loadModel() {
  const modelSavePath = './model.json';
  const model = await tf.loadLayersModel(`file://${modelSavePath}`);
  return model;
}

// 提供预测接口
async function predict(inputData) {
  const model = await loadModel();
  const { xs: input } = preprocessData([inputData]);
  const predictions = model.predict(input.reshape([1, 6, 1, 1])).arraySync();
  const predictedClass = predictions[0].indexOf(Math.max(...predictions[0]));
  return predictedClass;
}

export default { predict };
