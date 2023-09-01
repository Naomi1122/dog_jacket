<template>
  <div class="canvascontainer threejs">
    <div ref="rendererContainer" id="renderer-container"></div>
  </div>
</template>
  
<script>
let scene;
let model;
import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';
import { ThreeEngine } from './TEngine'

export default {
  name: 'Threejs',
  props: ['name'],
  watch: {
    // listen to task
    name(newQuestion, oldQuestion) {
      console.log(newQuestion, 'new')
      scene.remove(model)
      // load animations
      model.animations.forEach(animationClip => {
        animationClip.tracks.forEach(track => track.dispose());
      });
      model = null;

      this.$nextTick(() => {
        
        let that = this
        const rendererContainer = document.getElementById('renderer-container');
        rendererContainer.appendChild(that.renderer.domElement);
        this.assetLoader.load(that.monkeyUrl.href, function (gltf) {
          model = gltf.scene;
          scene.add(model);
          that.mixer = new THREE.AnimationMixer(model);
          const clips = gltf.animations;
          const clip = THREE.AnimationClip.findByName(clips, name);
          const action = that.mixer.clipAction(clip);
          action.timeScale = 5;
          action.play();
        });
      })

    }
  },
  data() {
    return {
      container: null,
      camera: null,
      controls: null,
      renderer: null,
      mixer: null,
      assetLoader: null,
      monkeyUrl: new URL('../assets/dog.gltf', import.meta.url)
    }
  },
  methods: {
    init() {
      let that = this


      that.renderer = new THREE.WebGLRenderer({ alpha: true });

      that.renderer.setSize(window.innerWidth, window.innerHeight);
      const rendererContainer = document.getElementById('renderer-container');
      that.renderer.domElement.style.position = "absolute";
      that.renderer.domElement.style.zIndex = 0;
      rendererContainer.appendChild(that.renderer.domElement);
      that.renderer.setSize(window.innerWidth, window.innerHeight);

      scene = new THREE.Scene();
      scene.background = null;


      that.camera = new THREE.PerspectiveCamera(
        5,
        window.innerWidth / window.innerHeight,
        0.1,
        1000
      );

      //renderer.setClearColor(0xA3A3A3);

      const pageid = document.getElementById('pageid');
      const orbit = new OrbitControls(that.camera, pageid);

      that.camera.position.set(-35, 0, 0);
      orbit.target = new THREE.Vector3(0, 0, 0);
      orbit.update();

      //const grid = new THREE.GridHelper(30, 30);
      //scene.add(grid);

      that.assetLoader = new GLTFLoader();
      // let mixer;
      that.assetLoader.load(this.monkeyUrl.href, function (gltf) {
        model = gltf.scene;
        scene.add(model);
        that.mixer = new THREE.AnimationMixer(model);
        const clips = gltf.animations;
        // Play a certain animation
        const clip = THREE.AnimationClip.findByName(clips, that.name);
        const action = that.mixer.clipAction(clip);
        action.timeScale = 5;
        if (action) {
          action.stop();
          action.play();
        }


        // Play all animations at the same time
        //clips.forEach(function(clip) {
        //const action = mixer.clipAction(clip);
        //action.play();
        //});

      }, undefined, function (error) {
        console.error(error);
      });




      const clock = new THREE.Clock();
      function animate() {
        if (that.mixer)
          that.mixer.update(clock.getDelta());
        that.renderer.render(scene, that.camera);
      }

      that.renderer.setAnimationLoop(animate);

      window.addEventListener('resize', function () {
        that.camera.aspect = window.innerWidth / window.innerHeight;
        that.camera.updateProjectionMatrix();
        that.renderer.setSize(window.innerWidth, window.innerHeight);
        //renderer.setSize(500, 500);
      });

    }
  },
  mounted() {
    this.init();
  },

};
</script>
  
<style scoped>
.threejs {
  position: absolute;
  z-index: 0;
  top: 30%
}

.canvasContainer {
  width: 100%;
  height: 100%;
  background-color: antiquewhite;
  margin: 0;
  padding: 0;
}

#renderer-container {
  position: absolute;
  z-index: 0;
  width: 100%;
  height: 100%;
}

#renderer-container canvas {
  position: absolute;
  z-index: 0;
  width: 100%;
  height: 100%;
}
</style>
  