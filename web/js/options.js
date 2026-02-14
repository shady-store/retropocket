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
// Fonction pour appliquer la traduction
async function applyTranslations(lang) {
  try {
    const response = await fetch(`./locales/${lang}.json`);
    if (!response.ok) throw new Error(`Could not load ${lang}.json`);
    const translations = await response.json();

    document.querySelectorAll("[data-i18n]").forEach((el) => {
      const key = el.getAttribute("data-i18n");
      if (translations[key]) {
        // Si c'est un bouton avec une icône, on essaie de préserver l'icône
        if (el.innerText.includes("↻") || el.innerText.includes("⏻")) {
          const icon = el.innerText.split(" ")[0];
          el.innerText = `${icon} ${translations[key].replace(/^[↻⏻]\s*/, "")}`;
        } else {
          el.innerText = translations[key];
        }
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
