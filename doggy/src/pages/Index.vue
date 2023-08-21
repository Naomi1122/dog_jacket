<script setup lang="ts">
import { useI18n } from 'vue-i18n';

import { ref } from 'vue';
import { useTheme } from '/@/composables';
import Threejs from '../components/Threejs.vue'
import Tensorflowjs from '../components/Tensorflowjs.vue'

const { t, availableLocales, locale } = useI18n();

const toggleLocales = () => {
  const locales = availableLocales;
  locale.value =
    locales[(locales.indexOf(locale.value) + 1) % locales.length];
};

const name = ref('walking');
const { toggleDark } = useTheme();

const show = ref(false);

function changeName(e: string) {
  name.value = e
  console.log(e, 'sd')
};

setTimeout(() => {
  show.value = true;
}, 1000);

</script>

<template>
  <div class="container max-w-3xl mx-auto mt-10">
    <div class="container-panel">

      <HelloWorld :msg="t('hello') + ' 👋 ' + t('welcome')" />


      <footer class="text-center">
        <ul class="flex justify-between w-1/3 mx-auto mb-8">
          <li class="cursor-pointer text-2xl">
            <a href="#" @click="toggleLocales" class="footer-link text-cyan-700 hover:text-cyan-500"
              :title="t('toggle_language')">
              <i class="i-ph-translate-bold" />
            </a>
          </li>
          <li class="cursor-pointer text-2xl">
            <a href="#" @click="toggleDark()" class="text-cyan-700 hover:text-cyan-500" :title="t('toggle_theme')">
              <i i="ph-sun dark:ph-moon" />
            </a>
          </li>
          <li class="cursor-pointer text-2xl">
            <a href="https://github.com/Naomi1122/doggydynamic/tree/main" rel="noreferrer" target="_blank"
              class="footer-link text-cyan-700 hover:text-cyan-500" title="Github repo">
              <i class="i-ph-github-logo" />
            </a>
          </li>
        </ul>

        <span class="text-xs">{{ t('made_by') }}
          <a class="footer-link text-cyan-400 hover:text-cyan-500" href="https://github.com/Naomi1122" rel="noreferrer"
            target="_blank">Naomi Cheng</a></span>
      </footer>
    </div>
  </div>
  <Tensorflowjs @changeName="changeName" />
  <div id="pageid">
    <Threejs :name="name" />

  </div>
</template>

<style>
.select {
  padding: 100px;
}

.helloworld {
  position: absolute;
  z-index: 10;
}

a,

.footer-link {
  opacity: 0.8;
}

.container {
  width: 100%;
  height: 30%;
  /* Adjust the height to fit your needs */
  position: absolute;
  /* z-index: 100; */
  left: 55%;
  transform: translate(-50%, 0);
  overflow: hidden
}

.container-panel {
  width: 60%;
  height: 30%;
  /* Adjust the height to fit your needs */
  text-align: center;
  margin: 0 auto;
}

/* Styles specific to Three.js */
.canvas-container {

  width: 100%;
  height: 100%;
  /* Adjust the height to match the container */
  position: relative;
  z-index: -1;
  /* Make sure Three.js is on top */
}

#pageid {
  width: 100%;
  height: 100%;
}
</style>
