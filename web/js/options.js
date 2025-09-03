const toggle = document.getElementById("effetPluieSlider");
toggle.addEventListener("change", function () {
  if (this.checked) {
    effetPluie = true;
  } else {
    effetPluie = false;
  }
  doRain();
});
const toggleCrt = document.getElementById("effetCrtSlider");
toggleCrt.addEventListener("change", function () {
  document.getElementById("rootContainer").classList.toggle("crt");
});
