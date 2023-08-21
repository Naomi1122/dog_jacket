<script setup lang="ts">

import { ref } from 'vue';



import { useTheme } from '/@/composables';
//import { Threejs } from 'components/Threejs.vue';
//import Dropdown from '../components/Dropdown.vue';


const { toggleDark } = useTheme();

const show = ref(false);
// 传值给threejs
let name = ref('sitting')
// 接收选取的数据
function getName(value: any) {
  console.log(value[2], 'sdddd')
  name.value = value[2]
};
setTimeout(() => {
  show.value = true;
}, 1000);

</script>
<template>

  <div class="container max-w-3xl mx-auto mt-10">
    <div class="container-panel">
      <footer class="text-center">
        <li class="cursor-pointer text-2xl">
          <a href="#" @click="toggleDark()" class="text-cyan-700 hover:text-cyan-500" title="theme">
            <i i="ph-sun dark:ph-moon" />
          </a>
        </li>

      </footer>
    </div>
  </div>
  <div class="select">
    <Dropdown @emitName="getName" />
  </div>
  <div id="pageid">
    <Threejs :name="name"/>
  </div>
</template>

<style> .select {
   padding: 100px;
 }

 .container {
   width: 100%;
   height: 100%;
   /* Adjust the height to fit your needs */
   position: absolute;
   z-index: 100;
   top: 50%;
   left: 50%;
   transform: translate(-50%, -50%);
   overflow: hidden
 }

 .container-panel {
   width: 60%;
   height: 30%;
   /* Adjust the height to fit your needs */
   text-align: center;
   margin: 0 auto;
 }

 #pageid {
   width: 100%;
   height: 100%;
 }

 .select {
   padding: 100px;
 }
</style>