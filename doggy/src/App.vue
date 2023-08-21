<script>
import Sidebar from '/@/components/Sidebar.vue';
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'App',
});
</script>
<template>
  <div class="app">
    <sidebar class="sidebar" />
    
	<div id="page">
		<router-view />
	</div>
  </div>
  
</template>
<style lang="scss">
.sidebar{
	position:absolute;
	z-index:200;
}
:root {
	--primary: #4ade80;
	--primary-alt: #22c55e;
	--grey: #64748b;
	--dark: #1e293b;
	--dark-alt: #334155;
	--light: #f1f5f9;
	--sidebar-width: 300px;
}

* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
	font-family: 'Fira sans', sans-serif;
}

html,body{
	width:100%;
	height:100%;
	overflow:hidden
}

body {
	background: var(--light);
}

button {
	cursor: pointer;
	appearance: none;
	border: none;
	outline: none;
	background: none;
}


#app {
	width:100%;
	height:100%;
	
}

.app {
	width:100%;
	height:100%;
	display: flex;

	 main {
		flex: 1 1 0;
		padding: 2rem;

		@media (max-width: 1024px) {
			padding-left: 6rem;
		}
	} 
}
</style>