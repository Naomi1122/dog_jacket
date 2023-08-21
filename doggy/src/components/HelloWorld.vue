<script setup>
import { useI18n } from 'vue-i18n'


defineProps({
  msg: {
    type: String,
    default: '',
  },

})

const { t } = useI18n();

</script>

<script>
// the relevant methods
import { collection, getDocs, query, limit, onSnapshot, doc } from "firebase/firestore"
// the firestore instance
import db from '../firebase/init.js'
import moment from 'moment'



export default {
  //created: 
  // function () {
  //   this.moment = moment;
  //   this.date = moment(new Date()).format('YYYY-MM-DD');
  //   return date
  // },
  // setup() {
  //  let todaysDate = new Date();
  // },
  // computed: {
  //   currentDateTime() {
  //     return moment(new Date()).format('MMMM Do YYYY, hh:mm');
  //   }
  // },
  
  data() {
    return {
      temp: '',
      bat: '',
      date:''
    }
  },
  created() {
    this.getdata()
    this.getCurrentDate()
    setTimeout(() => {
            this.getdata()
            this.getCurrentDate()
        }, 60000);
  },
  methods: {
    async getdata() {
      const currentDate = moment(new Date()).format('YYYY-MM-DD');

      onSnapshot(doc(db, '2023-08-18', '00:00:08'), (snap) => {
        this.temp = snap.data().temp
        this.bat = snap.data().battery_Capacity
      })
    },
    getCurrentDate() {
      this.date = moment(new Date()).format('YYYY-MM-DD, hh:mm');
    }
  },
  setup() {
    let todaysDate = new Date();
    // 其他初始化逻辑
  },
  computed: {
    currentDateTime() {
      return moment(new Date()).format('MMMM Do YYYY, hh:mm');
    }
  }

  }    
</script>

<template>
  <div class="text-center text-md box">
    <h1 class="mainTitle">{{ msg }}</h1>
    <p>
      {{t('Temp')}}
      ：{{ temp }}
    </p>
    <p>
      {{t('Bat')}}
      ：{{ bat }}
    </p>
    <p>
      {{ date }}
    </p>
    <!-- <p>
      {{t('recomended-ide')}}
      <a href="https://code.visualstudio.com/" target="_blank">VSCode</a>
      +
      <a
        href="https://marketplace.visualstudio.com/items?itemName=octref.vetur"
        target="_blank"
      >
        Vetur
      </a>
      or
      <a href="https://github.com/johnsoncodehk/volar" target="_blank">Volar</a>
      ({{t('if-using')}}
      <code>&lt;script setup&gt;</code>)
    </p>

    <p>{{t('See')}} <code>README.md</code> {{t('more-info')}}</p>
    <p class="mb-10">
      <a href="https://vitejs.dev/guide/features.html" target="_blank">
        Vite Docs
      </a>
      |
      <a href="https://v3.vuejs.org/" target="_blank">Vue 3 Docs</a>
    </p> -->
  </div>
</template>

<style scoped>
a {
  @apply text-cyan-400 hover:text-cyan-500 transition-all ease-out duration-100;
}
</style>

<style scoped>
label {
  margin: 0 0.5em;
  font-weight: bold;
}

code {
  @apply text-xs font-mono bg-yellow-200 text-yellow-700 rounded px-0.5 py-0.5;
}
@media (min-width: 760px) {
  .mainTitle {
    font-size: 40px !important;
  }
}
.mainTitle{
  font-weight: bold;
  font-size: 27px;
}
</style>
