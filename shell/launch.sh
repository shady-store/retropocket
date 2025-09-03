#!/bin/bash
sudomdp="retro1234!"
# Fichier déclencheur
TRIGGER_FILE="/home/retropocket/sdcard/.reset"
RESET_DIRECTORY="/home/retropocket/original/retropocket"
RESET_NETPLAN="/home/retropocket/original/retropocket/network/wifi.yaml"

SOURCE_DIR="/tmp/retropocket"
DEST_DIR="/home/retropocket/"

# Vérifier si le dossier existe
if [ -d "$SOURCE_DIR" ]; then
  echo "Le dossier $SOURCE_DIR existe. Déplacement en cours..."
  mv "$SOURCE_DIR" "$DEST_DIR"
  echo "Déplacement terminé."
else
  echo "Le dossier $SOURCE_DIR n'existe pas."
fi

# Vérifier si le fichier déclencheur existe
if [ -f "$TRIGGER_FILE" ]; then
  echo "Fichier reset $TRIGGER_FILE trouvé. Copie en cours..."

  # Créer le dossier destination s'il n'existe pas
  mkdir -p "$DEST_DIR"/retropocket

  # Copier le contenu du dossier source vers la destination
  cp -r "$RESET_DIRECTORY"/* "$DEST_DIR"/retropocket

  exec echo $sudomdp | sudo -S cp $RESET_NETPLAN /etc/netplan
  exec echo $sudomdp | sudo -S netplan apply

  echo "Copie terminée."

  echo "Suppression du fichier reset"
  rm $TRIGGER_FILE
else
  echo "Fichier $TRIGGER_FILE introuvable. Aucune action effectuée."
fi

echo "Démarrage bluetooth"
exec echo $sudomdp | sudo -S /home/retropocket/blue.sh &

echo "Démarrage retroarch"
exec /usr/bin/retroarch >/dev/tty1 2>&1 &

echo "Démarrage streaming ffmpeg vidéo"
exec echo $sudomdp | sudo -S ffmpeg -device /dev/dri/card0 -f kmsgrab -i - -r 30 -vcodec h264_rkmpp -f rtp rtp://localhost:8004 >/dev/tty2 2>&1 &

echo "Démarrage streaming ffmpeg audio"
exec ffmpeg -f pulse -i alsa_output.platform-hdmi-sound.stereo-fallback.monitor -acodec libopus -b 96000 -f rtp rtp://localhost:5004 >/dev/tty3 2>&1 &
cd /home/retropocket/retropocket/back/ && exec echo $sudomdp | sudo -S ./retropocket-back >/dev/tty4 2>&1 &
