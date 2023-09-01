<template>
    <div class="main">
        <!-- <h1>Newest Document Data from Firestore</h1>
        <ul>
            <p>
                {{ this.users }}
            </p>
        </ul>
        <ul>
            <p>
                {{ result }}
            </p>
        </ul> -->
    </div>
</template>

<script>
import * as tf from '@tensorflow/tfjs'
import * as tfvis from '@tensorflow/tfjs-vis'
import { reactive, ref } from "vue";
import { getClass } from "../Api/api.js";
import pitch_type from '../data.js';

//import predict from '../prediction.js';
import { query, collection, getDocs, doc, orderBy, limit, onSnapshot, limitToLast } from "firebase/firestore"
import db from '../firebase/init.js'
// import socket from './socket.js'
import moment from 'moment';

export default {
    data() {
        return {
            users: [-1.58,8.52,-4.52,0,0.01,-0.01],
            //reshape:[],
            prediction: '',
            //prediction: null,
            //result: null,
            currentDate: '2023-08-23',
            currentTime: '',
        }
    },
    created() {
        //this.keyDown();
        //window.addEventListener("keydown", this.KeyDown, true);
        //this.trainModel()
        this.predictSample();
        //this.getUsers()
        this.getCurrentDate()
        //this.predictModel()

        setInterval(() => {
            this.getUsers()
            //this.trainModel()
            this.getCurrentDate()
            //this.predictSample();
        }, 60000);

        // socket.on('predictResult', (result) => {
        //     plotPredictResult(result);
        //     console.log(123)
        // });
    },
    methods: {
        // 键盘按键事件
        // keyDown() {
        //     // that = c
        //     //监听键盘按钮
        //     document.onkeydown = (event) => {
        //         var e = event || window.event;
        //         var keyCode = e.keyCode || e.which;
        //         console.log(e, e.key, 'sddd')
        //         switch (e.key) {
        //             case 'i':
        //                 this.$emit('changeName', 'sitting')
        //                 break;
        //             case 'w':
        //                 this.$emit('changeName', 'walking')
        //                 break;
        //             case 'r':
        //                 this.$emit('changeName', 'running')
        //                 break;
        //             case "s":
        //                 this.$emit('changeName', 'standing')
        //                 break;
        //             case "l":
        //                 this.$emit('changeName', 'lying')
        //                 break;
        //             default:
        //                 break;
        //         }
        //         if (e && e.preventDefault) {
        //             e.preventDefault();
        //         } else {
        //             window.event.returnValue = false;
        //         }
        //     }
        // },
        async trainModel() {


            let numTrainingIterations = 3;
            for (var i = 0; i < numTrainingIterations; i++) {
                console.log(`Training iteration : ${i + 1} / ${numTrainingIterations}`);
                await pitch_type.model.fitDataset(pitch_type.trainingData, { epochs: 2 });
                console.log('accuracyPerClass', await pitch_type.evaluate(true));
            }


            console.log('training complete');
            
            const sample = [-1.58,8.52,-4.52,0,0.01,-0.01];
            this.prediction = await pitch_type.predictSample(this.users);
            this.$emit('changeName', this.prediction);

        },
        async predictSample() {
            const res = await getClass(this.users||[-1.58,8.52,-4.52,0,0.01,-0.01]);
            //console.log(res)
            //result.value = res.data.result.prediction;
            this.prediction = res.data.result.prediction
            this.$emit('changeName','sitting')
        },

        async getUsers() {
            const currentDate = moment(new Date()).format('YYYY-MM-DD');
            const currentTime = moment(new Date()).format('HH:mm');

            onSnapshot(doc(db, currentDate, currentTime), (snap) => {
                this.users = []
                this.users.push(snap.data().AX)
                this.users.push(snap.data().AY)
                this.users.push(snap.data().AZ)
                this.users.push(snap.data().GX)
                this.users.push(snap.data().GY)
                this.users.push(snap.data().GZ)
                console.log(snap.data())
            })

        },
        getCurrentDate() {
            this.date = moment(new Date()).format('YYYY-MM-DD, hh:mm');
            this.currentDate = moment(new Date()).format('YYYY-MM-DD');
            this.currentTime = moment(new Date()).format('HH:mm');
        }
        //async predictModel() {
        // const sample = [5.47, 0.56, -7.84, 0, 0.02, -0.01];
        //this.prediction = await pitch_type.predictSample(sample);
        //console.log(this.users)
        //}
    }
}

// onSnapshot(
            //     // use 'query()' instead of a reference
            //     query(collection(db,'2023-08-19'), limitToLast(1)),
            //     doc(db, currentDate, currentTime)
            //     (snap) => {
            //         this.users = []
            //         snap.forEach((doc) => {
            //             this.users.push(doc.data().AX)
            //             this.users.push(doc.data().AY)
            //             this.users.push(doc.data().AZ)
            //             this.users.push(doc.data().GX)
            //             this.users.push(doc.data().GY)
            //             this.users.push(doc.data().GZ)
            //         })
            //     })
            // limit the ordered users to 1
            //const q = query(collection(db, '2023-08-19'), limit(1))
            // const querySnap = await getDocs(collection(db, this.currentDate, this.currentTime), limit(1));

            // querySnap.forEach((doc) => {

</script>
  
<style>
.main {
    padding: 100px;
}
</style>
