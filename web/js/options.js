const toggle = document.getElementById("effetPluieSlider");
toggle.addEventListener("change", function () {
  if (this.checked) {
    effetPluie = true;
  } else {
    effetPluie = false;
  }
  const savedTheme = localStorage.getItem("preferred-theme") || "matrix";
  doRain(savedTheme);
});
const toggleCrt = document.getElementById("effetCrtSlider");
toggleCrt.addEventListener("change", function () {
  document.getElementById("rootContainer").classList.toggle("crt");
});
// Fonction pour appliquer la traduction
async function applyTranslations(lang) {
  try {
    const response = await fetch(`./locales/${lang}.json`);
    if (!response.ok) throw new Error(`Could not load ${lang}.json`);
    const translations = await response.json();

    document.querySelectorAll("[data-i18n]").forEach((el) => {
      const key = el.getAttribute("data-i18n");
      if (translations[key]) {
        el.innerText = translations[key];
      }
    });

    // On change l'attribut lang du HTML pour le SEO/Accessibilité
    document.documentElement.lang = lang;
  } catch (err) {
    console.error("i18n Error:", err);
  }
}

// Fonction globale pour changer de langue
window.changeLanguage = function (lang) {
  localStorage.setItem("preferred_lang", lang);
  applyTranslations(lang);
};

// Au chargement initial
document.addEventListener("DOMContentLoaded", () => {
  const savedLang = localStorage.getItem("preferred_lang") || "fr";
  applyTranslations(savedLang);
});

// Support HTMX : Re-traduire après un swap de contenu
document.body.addEventListener("htmx:afterSwap", () => {
  const currentLang = localStorage.getItem("preferred_lang") || "fr";
  applyTranslations(currentLang);
});
function changeTheme(themeName) {
  const themeLink = document.getElementById("dynamic-theme");

  if (themeName === "matrix") {
    // Pour Matrix, on vide le lien dynamique (le CSS de base reprend le dessus)
    themeLink.href = "";
  } else {
    // Pour les autres, on charge le fichier correspondant
    themeLink.href = `css/theme-${themeName}.css`;
  }

  // Sauvegarde pour le prochain chargement
  localStorage.setItem("preferred-theme", themeName);
  doRain(themeName);
}

// Au chargement de la page
window.addEventListener("DOMContentLoaded", () => {
  const savedTheme = localStorage.getItem("preferred-theme") || "matrix";
  document.getElementById("theme-selector").value = savedTheme;
  changeTheme(savedTheme);
});
