#!/bin/bash

# 1. Chargement et nettoyage de la configuration
CONF="/boot/retropocket.conf"
if [ -f "$CONF" ]; then
  sed 's/\r$//' "$CONF" >/tmp/rp.conf && source /tmp/rp.conf
else
  echo "ERREUR : Fichier de config introuvable !" && exit 1
fi

echo "--- Initialisation Retropocket (Dual Watchdog Mode) ---"

# 2. Optimisations Hardware
for i in {0..3}; do
  echo "$SUDOMDP" | sudo -S sh -c "echo $GOVERNOR > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_governor"
done
echo "$SUDOMDP" | sudo -S sh -c "echo $GOVERNOR > /sys/devices/platform/fde60000.gpu/devfreq/fde60000.gpu/governor"
echo "$SUDOMDP" | sudo -S sh -c "echo $GOVERNOR > /sys/devices/platform/dmc/devfreq/dmc/governor"
echo "$SUDOMDP" | sudo -S iw dev "$WIFI_INTERFACE" set power_save off
echo "$SUDOMDP" | sudo -S blockdev --setra $READ_AHEAD "$DISK_DEV"
echo "$SUDOMDP" | sudo -S chmod 777 /dev/tty*

# 3. Watchdog VIDEO (KMSGrab)
video_watchdog() {
  echo "Watchdog Vidéo activé."
  while true; do
    # On cherche le ffmpeg qui utilise kmsgrab
    if ! pgrep -f "ffmpeg.*kmsgrab" >/dev/null; then
      echo "[Watchdog Video] Relancement..."
      echo "$SUDOMDP" | sudo -S ffmpeg -f kmsgrab -device "$VIDEO_DEVICE" -framerate "$VIDEO_FPS" -i - \
        -vcodec h264_rkmpp -g "$VIDEO_GOP" -b:v "$VIDEO_BITRATE" \
        -maxrate "$VIDEO_BITRATE" -bufsize "$VIDEO_BUFSIZE" \
        -f rtp "rtp://$VIDEO_DEST" >/dev/tty2 2>&1
    fi
    sleep 2
  done
}

# 4. Watchdog AUDIO (Pulse/Opus)
audio_watchdog() {
  echo "Watchdog Audio activé."
  while true; do
    # On cherche le ffmpeg qui utilise pulse
    if ! pgrep -f "ffmpeg.*pulse" >/dev/null; then
      echo "[Watchdog Audio] Relancement..."
      export PULSE_LATENCY_MSEC="$AUDIO_LATENCY"
      ffmpeg -use_wallclock_as_timestamps 1 \
        -f pulse -i "$AUDIO_SOURCE" \
        -acodec libopus -application lowdelay -frame_duration "$AUDIO_FRAME_DUR" \
        -ar "$AUDIO_SAMPLERATE" -b:a "$AUDIO_BITRATE" \
        -af "aresample=async=1:min_hard_comp=0.01" \
        -f rtp "rtp://$AUDIO_DEST" >/dev/tty3 2>&1
    fi
    sleep 2
  done
}

# 5. Lancement des services en arrière-plan
echo "Démarrage Bluetooth..."
echo "$SUDOMDP" | sudo -S "$BLUETOOTH_SCRIPT" &

echo "Démarrage Backend d'administration..."
cd "$(dirname "$BACKEND_BIN")"
echo "$SUDOMDP" | sudo -S "$BACKEND_BIN" >/dev/tty4 2>&1 &

# 6. Activation des Watchdogs
video_watchdog &
audio_watchdog &

# 7. Lancement de l'interface principale (Bloquant)
echo "Démarrage de l'interface..."
"$EMU_BIN" >/dev/tty1 2>&1

echo "L'interface a été quittée. Nettoyage..."
pkill -f ffmpeg
