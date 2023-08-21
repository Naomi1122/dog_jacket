import { createApp } from 'vue';
import '@unocss/reset/tailwind.css'
import 'uno.css'
import { createI18n } from 'vue-i18n';

import App from './App.vue';
import './styles/base.css';

// Router
import { Router } from '/@/router';

// i18n
import messages from '@intlify/vite-plugin-vue-i18n/messages';

import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'

//import axios from 'axios'
//import vueAxios from 'vue-axios'

// import './firebase/init.js'
// import Vuefire from 'vuefire';

const i18n = createI18n({
  locale: 'en',
  messages,
});

const app = createApp(App);

app.use(i18n);

app.use(Router);
app.use(ElementPlus);
// app.use(Vuefire);
//app.use(vueAxios,axios);

app.mount('#app');
