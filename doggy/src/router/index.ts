import { createRouter, createWebHistory } from 'vue-router';

const routes = [
  {
    path: '/',
    name: 'index',
    component: () => import('/@/pages/Index.vue')
  },
  {
    path: '/About',
    name: 'About',
    component: () => import('/@/pages/About.vue')
  },
  {
    path: '/Review',
    name: 'Review',
    component: () => import('/@/pages/Review.vue')
  },
];

export const Router = createRouter({
  //scrollBehavior: () => ({ left: 0, top: 0 }),
  history: createWebHistory(),
  routes,
});
