<template>
    <div id="app">
    </div>
  </template>
  
  <script>
    import moment from 'moment'

  export default {
    data() {
    return {
      date: '',
      time: ''
    }
  },
        created: function () {
    this.moment = moment;
    this.date = moment(new Date()).format('YYYY-MM-DD');
    return date
  },
  setup() {
   let todaysDate = new Date();
  },
}

  </script>