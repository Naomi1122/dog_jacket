import axios from "axios";

const service = axios.create({
    baseURL: 'http://localhost:8000',
    timeout: 5000
})
// const getClass = (data)=>{
//     return service.post('/predict',{data: JSON.stringify(data)})
// }
export const getClass = (data)=>{
    return service.post('/predict',{data: JSON.stringify(data)})
}