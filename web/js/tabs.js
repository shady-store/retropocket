function openTab(tabName, clickedButton) {
  // 1. Masquer tous les contenus
  const tabContents = document.querySelectorAll(".tab-content");
  tabContents.forEach((content) => content.classList.remove("active"));

  // 2. Retirer la classe active de TOUS les boutons
  const tabButtons = document.querySelectorAll(".tab-button");
  tabButtons.forEach((button) => button.classList.remove("active"));

  // 3. Afficher le bon contenu
  const targetContent = document.getElementById(tabName);
  if (targetContent) targetContent.classList.add("active");

  // 4. Gestion Audio/Vidéo
  const remoteAudio = document.getElementById("remoteAudio");
  const remoteVideo = document.getElementById("remoteVideo");
  if (tabName === "screen") {
    remoteAudio.play();
    remoteVideo.play();
  } else {
    remoteVideo.pause();
    remoteAudio.pause();
  }

  // 5. Ajouter la classe active au bouton cliqué (Simple et efficace !)
  if (clickedButton) {
    clickedButton.classList.add("active");
  }
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
