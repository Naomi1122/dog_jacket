from flask import Flask, request, jsonify
import json
import numpy as np 
from joblib import  load  

from flask_cors import CORS

with open('./model.pkl', 'rb') as model_file:
    model = load(model_file)

app = Flask(__name__)

@app.route('/predict', methods=['POST'])
def predict():
    data = request.json  # get JSON data request by POST
    arr_string  = data['data']
    arr = json.loads(arr_string)
    # load item to float
    float_array = [float(item) for item in arr]
    array = np.array(float_array)
    # data pre-processing
    prediction = model.predict(array.reshape(1, -1))
    predict = prediction[0]
    print(predict)
    if predict == 0.0:
        accuracy = "94.37%"
        result = 'standing'
    if predict == 1.0:
        accuracy = "99.71%"
        result = 'sitting'
    if predict == 2.0:
        accuracy = "70.34%"
        result = 'walking'
    if predict == 3.0:
        accuracy = "99.79%"
        result = 'lying'
    if predict == 4.0:
        accuracy = "84.74%"
        result = 'running'
    return jsonify(
        {
        "code":200,
        "message":"请求成功",
        "result":{
            'accuracy' : accuracy,
            'prediction': result,
        }
        })  # return prediction
if __name__ == '__main__':
    CORS(app)
    app.run(port=8000,debug = True)