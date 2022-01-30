<template>
<div id = "app">
  <canvas id = "myCanvas" width="700" height="700"></canvas>
  <button class="button button1" v-on:click="getMessage(1)">origin image earth</button>
  <button  class="button button1" v-on:click="getMessage(2)">origin image nature </button>
  <button  class="button button1" v-on:click="getMessage(3)">origin image nature_2 </button>
  <button class="button button1" v-on:click="getMessage(4)">origin image Fall</button>
  <button class="button button1" v-on:click="clearCanvas()">clear</button>
  <button class="button button1" v-on:click="drawimage">draw image</button>
</div>
</template>
<style>
.button {
  background-color: #13e2e8; /* Green */
  border: none;
  color: white;
  padding: 16px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  -webkit-transition-duration: 0.4s; /* Safari */
  transition-duration: 0.4s;
  cursor: pointer;

}
.button1 {
  background-color: white;
  color: black;
  border: 2px solid #0ceac9;
}

.button1:hover {
  background-color: #13e2e8;
  color: white;
}
</style>
<script>
import axios from "axios";
export default {
  name:'App',
  data(){
    return{
      data:{
        image_array:{
          image_array: [],
          height: {
            type: Number,
            default:0
          },
          width: {
            type: Number,
            default:0
          },

        }
      }

    };
  },
  methods:{
    clearCanvas(){
      var cxt=document.getElementById("myCanvas").getContext("2d");
      cxt.clearRect(0,0,700,700);
    },
    drawimage(){
      console.log(this.data.height)
      if (this.data.height ===undefined){
        alert("empty image!")
      }
      var c = document.getElementById("myCanvas");
      var ctx = c.getContext("2d");
      let i,j
      for(i=0;i<this.data.width;i++){
        for(j=0;j<this.data.height;j++){
          ctx.fillStyle = "rgb("+this.data.image_array[j][i][0]+","+this.data.image_array[j][i][1]+","+this.data.image_array[j][i][2]+")"
          ctx.fillRect(i,j,1,1)
        }
      }
    },
    getMessage(x){
      let url = 'http://127.0.0.1:5000/get/';
      url +=`${x}`
      axios.get(url).then((res)=>{
        this.data = res.data
        console.log(this.data)
      }).catch((error)=>{
        console.error(error)
      })
    }
  },


}
</script>

<style>

</style>
