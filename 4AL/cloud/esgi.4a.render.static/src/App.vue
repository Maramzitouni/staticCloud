<script setup>
import { RouterView } from 'vue-router'
import MenuComponent from '@/components/MenuComponent.vue'
</script>

<template>
  <div class="bg-white w-2/3 mx-auto my-20 p-10 rounded-2xl shadow flex flex-col">
    <MenuComponent/>

    <RouterView/>
  </div>
</template>

<style scoped>
header {
  line-height: 1.5;
  max-height: 100vh;
}

nav {
  width: 100%;
  font-size: 12px;
  text-align: center;
  margin-top: 2rem;
}

@media (min-width: 1024px) {
  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }

  nav {
    text-align: left;
    margin-left: -1rem;
    font-size: 1rem;

    padding: 1rem 0;
    margin-top: 1rem;
  }
}
</style>
