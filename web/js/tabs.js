function openTab(tabName) {
  // Masquer tous les contenus des onglets
  const tabContents = document.querySelectorAll(".tab-content");
  tabContents.forEach((content) => {
    content.classList.remove("active");
  });

  // Retirer la classe active de tous les boutons
  const tabButtons = document.querySelectorAll(".tab-button");
  tabButtons.forEach((button) => {
    button.classList.remove("active");
  });

  // Afficher le contenu de l'onglet sélectionné
  document.getElementById(tabName).classList.add("active");

  const remoteAudio = document.getElementById("remoteAudio");
  const remoteVideo = document.getElementById("remoteVideo");
  if (tabName === "screen") {
    remoteAudio.play();
    remoteVideo.play();
  } else {
    remoteVideo.pause();
    remoteAudio.pause();
  }

  // Ajouter la classe active au bouton correspondant
  const activeButton = Array.from(tabButtons).find(
    (button) =>
      button.textContent === tabName.charAt(0).toUpperCase() + tabName.slice(1),
  );
  activeButton.classList.add("active");
}
function toggleInputs() {
  const apMode = document.querySelector(
    'input[name="mode"][value="AP"]',
  ).checked;
  const ssidInput = document.getElementById("ssid");
  const passwordInput = document.getElementById("password");
  const ipaddrInput = document.getElementById("ipaddr");

  if (apMode) {
    ssidInput.disabled = true; // Désactiver les champs
    passwordInput.disabled = true;
    ipaddr.disabled = true;
    ssidInput.value = "retropocket";
    passwordInput.value = "retro1234!";
    ipaddr.value = "10.42.0.1";
  } else {
    ssidInput.disabled = false; // Activer les champs
    passwordInput.disabled = false;
    ipaddr.disabled = false;
    ssidInput.value = "";
    passwordInput.value = "";
    ipaddr.value = "";
  }
}

// Appeler la fonction au chargement pour définir l'état initial
document.addEventListener("DOMContentLoaded", toggleInputs);
