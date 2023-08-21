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
                {{ this.prediction }}
            </p>
        </ul> -->
    </div>
</template>

<script>
import * as tf from '@tensorflow/tfjs'
import * as tfvis from '@tensorflow/tfjs-vis'
import pitch_type from '../data.js';
import { query, collection, getDocs, orderBy, limit, onSnapshot } from "firebase/firestore"
import db from '../firebase/init.js'
// import socket from './socket.js'

export default {
    data() {
        return {
            users: [],
            //reshape:[],
            prediction: ''
        }
    },
    created() {
        this.keyDown();
        //window.addEventListener("keydown", this.KeyDown, true);
        this.trainModel()
        this.getUsers()
        //this.predictModel()

        setTimeout(() => {
            this.getUsers()
            this.trainModel()
        }, 60000);

        // socket.on('predictResult', (result) => {
        //     plotPredictResult(result);
        //     console.log(123)
        // });
    },
    methods: {
        // 键盘按键事件
        keyDown() {
            // that = c
            //监听键盘按钮
            document.onkeydown = (event) => {
                var e = event || window.event;
                var keyCode = e.keyCode || e.which;
                console.log(e, e.key, 'sddd')
                switch (e.key) {
                    case 'i':
                        this.$emit('changeName', 'sitting')
                        break;
                    case 'w':
                        this.$emit('changeName', 'walking')
                        break;
                    case 'r':
                        this.$emit('changeName', 'running')
                        break;
                    case "s":
                        this.$emit('changeName', 'standing')
                        break;
                    case "l":
                        this.$emit('changeName', 'lying')
                        break;
                    default:
                        break;
                }
                if (e && e.preventDefault) {
                    e.preventDefault();
                } else {
                    window.event.returnValue = false;
                }
            }
        },
        async trainModel() {


            let numTrainingIterations = 10;
            for (var i = 0; i < numTrainingIterations; i++) {
                console.log(`Training iteration : ${i + 1} / ${numTrainingIterations}`);
                await pitch_type.model.fitDataset(pitch_type.trainingData, { epochs: 1 });
                console.log('accuracyPerClass', await pitch_type.evaluate(true));
                //await sleep(TIMEOUT_BETWEEN_EPOCHS_MS);
            }

            //io.emit('trainingComplete', true);
            console.log('training complete');

              //const sample = [5.47,0.56,-7.84,0,0.02,-0.01];
              this.prediction = await pitch_type.predictSample(this.users);
              this.$emit('changeName', this.prediction);
                // console.log({this.prediction});

        },

        async getUsers() {

            onSnapshot(
                // use 'query()' instead of a reference
                query(collection(db, '2023-08-19'), limit(1)),
                (snap) => {
                    this.users = []
                    snap.forEach((doc) => {
                        this.users.push(doc.data().AX)
                        this.users.push(doc.data().AY)
                        this.users.push(doc.data().AZ)
                        this.users.push(doc.data().GX)
                        this.users.push(doc.data().GY)
                        this.users.push(doc.data().GZ)
                    })
                })
            // limit the ordered users to 1
            //const q = query(collection(db, '2023-08-19'), limit(1))
            // const querySnap = await getDocs(collection(db, '2023-08-19'));

            // querySnap.forEach((doc) => {
            //     this.users.push(doc.data().AX)
            //     this.users.push(doc.data().AY)
            //     this.users.push(doc.data().AZ)
            //     this.users.push(doc.data().GX)
            //     this.users.push(doc.data().GY)
            //     this.users.push(doc.data().GZ)
            //     console.log(doc.data())
            // })

            //     socket.on('connect', () => {
            //         console.log('Socket.IO connected.');
            //         socket.emit('predictSample', [-0.56, 2.07, -9.56, -0.02, -0.06, -0.02], (res) => { console.log(res, 111) });

            //     });

            // },
            // plotPredictResult(result) {
            //     console.log(result);
            // }
        },
        //async predictModel() {
           // const sample = [5.47, 0.56, -7.84, 0, 0.02, -0.01];
            //this.prediction = await pitch_type.predictSample(sample);
            //console.log(this.users)
        //}
    }
}



</script>
  
<style>
.main {
    padding: 100px;
}
</style>
