function doRain() {
  const rainContainer = document.querySelector(".rain-container"); // Sélectionner le conteneur de pluie
  if (effetPluie) {
    const rainCount = 1000; // Nombre de gouttes de pluie

    for (let i = 0; i < rainCount; i++) {
      const rainDrop = document.createElement("div");
      rainDrop.className = "rain";
      rainDrop.style.left = Math.random() * 100 + "vw";
      rainDrop.style.animationDuration = Math.random() * 10 + 1.5 + "s"; // Durée d'animation aléatoire
      rainContainer.appendChild(rainDrop); // Ajouter la goutte de pluie au conteneur
    }
  } else {
    rainContainer.innerHTML = "";
  }
}
let effetPluie = true;
doRain();
