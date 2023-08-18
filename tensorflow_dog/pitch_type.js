const tf = require('@tensorflow/tfjs');

// const fs = require('fs');
// const path = require('path');
// const Papa = require('papaparse');

// const csvFilePath = path.join(__dirname, 'dog_dataset.csv'); // CSV 数据集文件路径
// const trainCsvFilePath = path.join(__dirname, 'train_dataset.csv'); // 训练集保存路径
// const testCsvFilePath = path.join(__dirname, 'test_dataset.csv'); // 测试集保存路径

// const testRatio = 0.2; // 测试集占比

// // 读取 CSV 文件内容
// const csvData = fs.readFileSync(csvFilePath, 'utf8');

// // 解析 CSV 数据
// const parsedData = Papa.parse(csvData, { header: true });

// // 随机打乱数据
// const shuffledData = parsedData.data.sort(() => Math.random() - 0.5);

// // 划分数据集
// const numExamples = shuffledData.length;
// const numTestExamples = Math.round(numExamples * testRatio);

// const testDataset = shuffledData.slice(0, numTestExamples);
// const trainDataset = shuffledData.slice(numTestExamples);

// // 将数据集保存回到 CSV 文件中
// const trainCsv = Papa.unparse(trainDataset, { header: true });
// const testCsv = Papa.unparse(testDataset, { header: true });

// fs.writeFileSync(trainCsvFilePath, trainCsv, 'utf8');
// fs.writeFileSync(testCsvFilePath, testCsv, 'utf8');

// console.log('数据集分割和保存完成！');

// util function to normalize a value between a given range.
function normalize(value, min, max) {
  if (min === undefined || max === undefined) {
    return value;
  }
  return (value - min) / (max - min);
}

// data can be loaded from URLs or local file paths when running in Node.js.
const TRAIN_DATA_PATH = '/Users/lissie/Desktop/DoggyDynamic/tensorflow_dog/dog_dataset.csv';
const TEST_DATA_PATH = '/Users/lissie/Desktop/DoggyDynamic/tensorflow_dog/train_dataset.csv';

// Constants from training data
const AX_MIN = -18.885;
const AX_MAX = 18.065;
const AY_MIN = -152.463;
const AY_MAX = -86.374;
const AZ_MIN = -15.5146078412997;
const AZ_MAX = 9.974;
const GX_MIN = -48.0287647107959;
const GX_MAX = 30.592;
const GY_MIN = 9.397;
const GY_MAX = 49.18;
const GZ_MIN = -49.339;
const GZ_MAX = 2.95522851438373;
// const START_SPEED_MIN = 59;
// const START_SPEED_MAX = 104.4;

const NUM_PITCH_CLASSES = 5;
const TRAINING_DATA_LENGTH = 968;
const TEST_DATA_LENGTH = 242;

// Converts a row from the CSV into features and labels.
// Each feature field is normalized within training data constants
const csvTransform =
    ({xs, ys}) => {
      const values = [
        normalize(xs.ax, AX_MIN, AX_MAX),
        normalize(xs.ay, AY_MIN, AY_MAX),
        normalize(xs.az, AZ_MIN, AZ_MAX), normalize(xs.gx, GX_MIN, GX_MAX),
        normalize(xs.gy, GY_MIN, GY_MAX), normalize(xs.gz, GZ_MIN, GZ_MAX),
        //normalize(xs.start_speed, START_SPEED_MIN, START_SPEED_MAX),
        //xs.left_handed_pitcher
      ];
      return {xs: values, ys: ys.Type};
    }

const trainingData =
    tf.data.csv(TRAIN_DATA_PATH, {columnConfigs: {Type: {isLabel: true}}})
        .map(csvTransform)
        .shuffle(TRAINING_DATA_LENGTH)
        .batch(100);

// Load all training data in one batch to use for evaluation
const trainingValidationData =
    tf.data.csv(TRAIN_DATA_PATH, {columnConfigs: {Type: {isLabel: true}}})
        .map(csvTransform)
        .batch(TRAINING_DATA_LENGTH);

// Load all test data in one batch to use for evaluation
const testValidationData =
    tf.data.csv(TEST_DATA_PATH, {columnConfigs: {Type: {isLabel: true}}})
        .map(csvTransform)
        .batch(TEST_DATA_LENGTH);

        const model = tf.sequential();
        model.add(tf.layers.dense({units: 250, activation: 'relu', inputShape: [8]}));
        model.add(tf.layers.dense({units: 175, activation: 'relu'}));
        model.add(tf.layers.dense({units: 150, activation: 'relu'}));
        model.add(tf.layers.dense({units: NUM_PITCH_CLASSES, activation: 'softmax'}));
        
        model.compile({
          optimizer: tf.train.adam(),
          loss: 'sparseCategoricalCrossentropy',
          metrics: ['accuracy']
        });

// Returns pitch class evaluation percentages for training data
// with an option to include test data
async function evaluate(useTestData) {
    let results = {};
    await trainingValidationData.forEachAsync(pitchTypeBatch => {
      const values = model.predict(pitchTypeBatch.xs).dataSync();
      const classSize = TRAINING_DATA_LENGTH / NUM_PITCH_CLASSES;
      for (let i = 0; i < NUM_PITCH_CLASSES; i++) {
        results[pitchFromClassNum(i)] = {
          training: calcPitchClassEval(i, classSize, values)
        };
      }
    });
  
    if (useTestData) {
      await testValidationData.forEachAsync(pitchTypeBatch => {
        const values = model.predict(pitchTypeBatch.xs).dataSync();
        const classSize = TEST_DATA_LENGTH / NUM_PITCH_CLASSES;
        for (let i = 0; i < NUM_PITCH_CLASSES; i++) {
          results[pitchFromClassNum(i)].validation =
              calcPitchClassEval(i, classSize, values);
        }
      });
    }
    return results;
  }
  
  async function predictSample(sample) {
    let result = model.predict(tf.tensor(sample, [1,sample.length])).arraySync();
    var maxValue = 0;
    var predictedPitch = 4;
    for (var i = 0; i < NUM_PITCH_CLASSES; i++) {
      if (result[0][i] > maxValue) {
        predictedPitch = i;
        maxValue = result[0][i];
      }
    }
    return pitchFromClassNum(predictedPitch);
  }
  
  // Determines accuracy evaluation for a given pitch class by index
  function calcPitchClassEval(pitchIndex, classSize, values) {
    // Output has 7 different class values for each pitch, offset based on
    // which pitch class (ordered by i)
    let index = (pitchIndex * classSize * NUM_PITCH_CLASSES) + pitchIndex;
    let total = 0;
    for (let i = 0; i < classSize; i++) {
      total += values[index];
      index += NUM_PITCH_CLASSES;
    }
    return total / classSize;
  }
  
  // Returns the string value for Baseball pitch labels
  function pitchFromClassNum(classNum) {
    switch (classNum) {
      case 0:
        return 'Standing';
      case 1:
        return 'Sitting';
      case 2:
        return 'Walking';
      case 3:
        return 'lying';
      // case 4:
      //   return 'Slider';
      // case 5:
      //   return 'Changeup';
      // case 6:
      //   return 'Curveball';
      default:
        return 'Unknown';
    }
  }
  
  module.exports = {
    evaluate,
    model,
    pitchFromClassNum,
    predictSample,
    testValidationData,
    trainingData,
    TEST_DATA_LENGTH
  }