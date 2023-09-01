<template>
  <el-cascader :options="options" @change="handleChange" />
</template>
<script setup>

const emit = defineEmits(['emitName']);
//options = {dataArray};
const currentDate = moment(new Date()).subtract(1, 'days').format('YYYY-MM-DD');
const options = [
  {
    value: currentDate,
    label: currentDate,
    children: [
      {
        value: '12:00',
        label: '12:00',
        children: [
          {
            value: 'sitting',
            label: 'sitting',
          }
        ],
      },
      {
        value: '13:00',
        label: '13:00',
        children: [
          {
            value: 'walking',
            label: 'walking',
          }
        ],
      },
      {
        value: '14:00',
        label: '14:00',
        children: [
          {
            value: 'running',
            label: 'running',
          }
        ],
      },
      {
        value: '15:00',
        label: '15:00',
        children: [
          {
            value: 'standing',
            label: 'standing',
          }
        ],
      },
      {
        value: '16:00',
        label: '16:00',
        children: [
          {
            value: 'lying',
            label: 'lying',
          }
        ],
      },
    ],
  }
]
const handleChange = (value) => {
  emit('emitName', value)
}

</script>
<script>

// import { collection, onSnapshot } from 'firebase/firestore'
// import db from '../firebase/init.js'


// export default {
//   data() {
//     return {
//       users: []
//     }
//   },
//   mounted() {
//     onSnapshot(collection(db, '2023-08-18'), (snap) => {

// snap.forEach((doc) => {
//   this.users.push(doc.data())
// })
// })
//     }
//   }
// //console.log(users);
import moment from 'moment';

export default {
  data() {
    return {
      dataArray: []
    };
  },
  created() {
    this.generateDataArray();
  },
  methods: {
    generateDataArray() {
      const yesterdayDate = moment().subtract(1, 'days').format('YYYY-MM-DD');
      const currentHour = moment().format('HH');

      const options = ['standing', 'walking', 'lying', 'sitting', 'running'];

      const dateItem = {
        value: yesterdayDate,
        label: yesterdayDate,
        children: []
      };

      for (let i = 0; i <= currentHour; i++) {
        const label = moment().set('hour', i).format('HH');
        const value = i % options.length;
        dateItem.children.push({ value: options[value], label });
      }

      this.dataArray.push(dateItem);
    }
  }
};
</script>
