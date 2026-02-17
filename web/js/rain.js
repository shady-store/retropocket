function doRain(currentTheme) {
  const rainContainer = document.querySelector(".rain-container");
  if (!rainContainer) return;

  // Nettoyage systématique avant de recréer les éléments
  rainContainer.innerHTML = "";

  // Si l'utilisateur a désactivé l'effet via le slider
  if (!effetPluie) return;

  if (currentTheme === "win95") {
    // --- MODE STARFIELD WINDOWS 95 ---
    const logoCount = 345;

    for (let i = 0; i < logoCount; i++) {
      const logo = document.createElement("div");
      logo.className = "win-logo-particle";

      // Calcul de la trajectoire (Projection vers l'extérieur)
      const angle = Math.random() * Math.PI * 2;
      const distance =
        window.innerWidth > window.innerHeight
          ? window.innerWidth
          : window.innerHeight;

      const tx = Math.cos(angle) * distance + "px";
      const ty = Math.sin(angle) * distance + "px";
      //const rot = Math.random() * 720 - 360 + "deg"; // Rotation aléatoire entre -360 et 360
      const rot = 0 + "deg";

      logo.style.setProperty("--tx", tx);
      logo.style.setProperty("--ty", ty);
      logo.style.setProperty("--rot", rot);

      // Durée et délai pour un flux continu
      logo.style.animationDuration = Math.random() * 3.5 + 1.5 + "s";
      logo.style.animationDelay = Math.random() * 4 + "s";

      rainContainer.appendChild(logo);
    }
  } else if (currentTheme === "cyberpunk") {
    const lineCount = 30;
    for (let i = 0; i < lineCount; i++) {
      const line = document.createElement("div");
      line.className = "cyber-grid-particle";
      line.style.width = Math.random() * 200 + 50 + "px";
      line.style.top = Math.random() * 100 + "vh";
      line.style.left = "-200px";
      line.style.animationDuration = Math.random() * 2 + 1 + "s";
      line.style.animationDelay = Math.random() * 3 + "s";
      rainContainer.appendChild(line);
    }
  } else if (currentTheme === "aero") {
    const bubbleCount = 20;
    for (let i = 0; i < bubbleCount; i++) {
      const bubble = document.createElement("div");
      bubble.className = "aero-bubble";

      const size = Math.random() * 70 + 20 + "px";
      bubble.style.width = size;
      bubble.style.height = size;
      bubble.style.left = Math.random() * 100 + "vw";
      bubble.style.bottom = "-100px";

      // Vitesse de montée aléatoire
      bubble.style.animation = `floatUp ${Math.random() * 12 + 8}s ease-in infinite`;
      bubble.style.animationDelay = Math.random() * 5 + "s";
      bubble.style.opacity = Math.random() * 0.3 + 0.1;

      rainContainer.appendChild(bubble);
    }
  } else if (currentTheme === "aero-dark") {
    const bubbleCount = 15; // Moins de bulles pour rester élégant
    for (let i = 0; i < bubbleCount; i++) {
      const bubble = document.createElement("div");
      bubble.className = "aero-bubble";

      const size = Math.random() * 100 + 40 + "px";
      bubble.style.width = size;
      bubble.style.height = size;
      bubble.style.left = Math.random() * 100 + "vw";
      bubble.style.bottom = "-150px";

      // Montée très lente et vaporeuse
      bubble.style.animation = `floatUp ${Math.random() * 15 + 10}s ease-out infinite`;
      bubble.style.opacity = Math.random() * 0.4 + 0.1;

      rainContainer.appendChild(bubble);
    }
  } else {
    // --- MODE PLUIE MATRIX ---
    const rainCount = 1000;
    for (let i = 0; i < rainCount; i++) {
      const rainDrop = document.createElement("div");
      rainDrop.className = "rain";
      rainDrop.style.left = Math.random() * 100 + "vw";
      rainDrop.style.animationDuration = Math.random() * 8 + 2 + "s";
      rainDrop.style.animationDelay = Math.random() * 5 + "s";
      rainContainer.appendChild(rainDrop);
    }
  }
}

// N'oublie pas d'appeler doRain() dans ta fonction changeTheme(themeName) :
// function changeTheme(themeName) {
//    currentTheme = themeName;
//    ... chargement CSS ...
//    doRain();
// }
//function doRain() {
//  const rainContainer = document.querySelector(".rain-container"); // Sélectionner le conteneur de pluie
//  if (effetPluie) {
//    const rainCount = 1000; // Nombre de gouttes de pluie
//
//    for (let i = 0; i < rainCount; i++) {
//      const rainDrop = document.createElement("div");
//      rainDrop.className = "rain";
//      rainDrop.style.left = Math.random() * 100 + "vw";
//      rainDrop.style.animationDuration = Math.random() * 10 + 1.5 + "s"; // Durée d'animation aléatoire
//      rainContainer.appendChild(rainDrop); // Ajouter la goutte de pluie au conteneur
//    }
//  } else {
//    rainContainer.innerHTML = "";
//  }
//}
let effetPluie = true;
//doRain();
