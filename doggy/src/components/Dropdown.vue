<template>
  <el-cascader :options="options" @change="handleChange" />
</template>
<script setup>
const emit = defineEmits(['emitName']);
const options = [
  {
    value: '2023-08-19',
    label: '2023-08-19',
    children: [
      {
        value: '0',
        label: 'sitting',
        children: [
          {
            value: 'sitting',
            label: 'sitting',
          }
        ],
      },
      {
        value: '1',
        label: 'walking',
        children: [
          {
            value: 'walking',
            label: 'walking',
          }
        ],
      },
      {
        value: '2',
        label: 'running',
        children: [
          {
            value: 'running',
            label: 'running',
          }
        ],
      },
      {
        value: '1',
        label: 'standing',
        children: [
          {
            value: 'standing',
            label: 'standing',
          }
        ],
      },
      {
        value: '1',
        label: 'lying',
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

import { collection, onSnapshot } from 'firebase/firestore'
import db from '../firebase/init.js'


export default {
  data() {
    return {
      users: []
    }
  },
  mounted() {
    onSnapshot(collection(db, '2023-08-18'), (snap) => {

snap.forEach((doc) => {
  this.users.push(doc.data())
})
})
    }
  }
//console.log(users);
</script>
