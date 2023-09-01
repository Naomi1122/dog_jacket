import * as tf from '@tensorflow/tfjs'

// util function to normalize a value between a given range.
function normalize(value, min, max) {
  if (min === undefined || max === undefined) {
    return value;
  }
  return (value - min) / (max - min);
}

// data can be loaded from URLs or local file paths when running in Node.js.
const TRAIN_DATA_PATH = 'https://raw.githubusercontent.com/Naomi1122/doggydynamic/main/doggy/src/trainedpy_dataset.csv';
const TEST_DATA_PATH = 'https://raw.githubusercontent.com/Naomi1122/doggydynamic/main/doggy/src/testedpy_dataset.csv';

// Constants from training data
const AX_MIN = -9.61;
const AX_MAX = 8.83;
const AY_MIN = -8.94;
const AY_MAX = 18.85;
const AZ_MIN = -22.82;
const AZ_MAX = 4.47;
const GX_MIN = -5.39;
const GX_MAX = 6.84;
const GY_MIN = -6.40;
const GY_MAX = 2.29;
const GZ_MIN = -4.63;
const GZ_MAX = 3.15;
// const START_SPEED_MIN = 59;
// const START_SPEED_MAX = 104.4;

const NUM_PITCH_CLASSES = 6;
const TRAINING_DATA_LENGTH = 3002;
const TEST_DATA_LENGTH = 751;

// Converts a row from the CSV into features and labels.
// Each feature field is normalized within training data constants
const csvTransform =
  ({ xs, ys }) => {
    const values = [
      normalize(xs.AX, AX_MIN, AX_MAX),
      normalize(xs.AY, AY_MIN, AY_MAX),
      normalize(xs.AZ, AZ_MIN, AZ_MAX), normalize(xs.GX, GX_MIN, GX_MAX),
      normalize(xs.GY, GY_MIN, GY_MAX), normalize(xs.GZ, GZ_MIN, GZ_MAX),
      //normalize(xs.start_speed, START_SPEED_MIN, START_SPEED_MAX),
      //xs.left_handed_pitcher
    ];
    return { xs: values, ys: ys.Type };
  }

const trainingData =
  tf.data.csv(TRAIN_DATA_PATH, { columnConfigs: { Type: { isLabel: true } } })
    .map(csvTransform)
    .shuffle(TRAINING_DATA_LENGTH)
    .batch(32);

// Load all training data in one batch to use for evaluation
const trainingValidationData =
  tf.data.csv(TRAIN_DATA_PATH, { columnConfigs: { Type: { isLabel: true } } })
    .map(csvTransform)
    .batch(TRAINING_DATA_LENGTH);

// Load all test data in one batch to use for evaluation
const testValidationData =
  tf.data.csv(TEST_DATA_PATH, { columnConfigs: { Type: { isLabel: true } } })
    .map(csvTransform)
    .batch(TEST_DATA_LENGTH);

const model = tf.sequential();
model.add(tf.layers.dense({ units: 128, activation: 'relu', inputShape: [6] })); // Increase units
model.add(tf.layers.dense({ units: 64, activation: 'relu' })); 
model.add(tf.layers.dense({ units: 32, activation: 'relu' }));
model.add(tf.layers.dense({ units: 150, activation: 'relu' }));
model.add(tf.layers.dense({ units: NUM_PITCH_CLASSES, activation: 'softmax' }));

model.compile({
  optimizer: tf.train.adam(0.0001),
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
  let result = model.predict(tf.tensor(sample, [1, sample.length])).arraySync();
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
    case 0.0:
      return 'standing';
    case 1.0:
      return 'sitting';
    case 2.0:
      return 'walking';
    case 3.0:
      return 'lying';
    case 4.0:
      return 'running';
    // case 5:
    //   return 'Changeup';
    // case 6:
    //   return 'Curveball';
    default:
      return 'Unknown';
  }
}

// module.exports = {
//   evaluate,
//   model,
//   pitchFromClassNum,
//   predictSample,
//   testValidationData,
//   trainingData,
//   TEST_DATA_LENGTH
// }
export default {
  evaluate,
  model,
  pitchFromClassNum,
  predictSample,
  testValidationData,
  trainingData,
  TEST_DATA_LENGTH
}