let janus = null;
let pluginHandle = null;
//const server = "http://192.168.1.45:8088/janus"; // Remplace par ton serveur Janus
//const server = "http://10.42.0.1:8088/janus"; // Remplace par ton serveur Janus
const server = "http://" + window.location.hostname + ":8088/janus";
function receiveList(result, streamid) {
  if (result["list"]) {
    let list = result["list"];
    Janus.log("Got a list of available streams:", list);
    let bodyVideo = { request: "watch", id: streamid };
    pluginHandle.send({ message: bodyVideo });
  }
}
function janusAttachSuccess(plugin, streamid) {
  pluginHandle = plugin;
  Janus.log(
    "Plugin attached! (" +
      pluginHandle.getPlugin() +
      ", id=" +
      pluginHandle.getId() +
      ")",
  );
  pluginHandle.send({
    message: { request: "list" },
    success: function (result) {
      receiveList(result, streamid);
    },
  });
}

function janusOnMessage(msg, jsep) {
  Janus.debug(" ::: Got a message :::", msg);
  let result = msg["result"];
  if (jsep) {
    Janus.debug("Handling SDP as well...", jsep);
    pluginHandle.createAnswer({
      jsep: jsep,
      tracks: [{ type: "data" }],
      success: function (jsep) {
        Janus.debug("Got SDP!", jsep);
        let body = { request: "start" };
        pluginHandle.send({ message: body, jsep: jsep });
      },
      error: function (error) {
        Janus.error("WebRTC error:", error);
      },
    });
  }
}
function janusOnRemoteTrack(track, mid, on, metadata) {
  Janus.debug(
    "Remote track (mid=" +
      mid +
      ") " +
      (on ? "added" : "removed") +
      (metadata ? " (" + metadata.reason + ") " : "") +
      ":",
    track,
  );
  if (track.kind === "video") {
    let video = document.getElementById("remoteVideo");
    if (video.readyState <= 2 || video.paused || video.currentTime <= 0) {
      let streamVideo = new MediaStream([track]);
      Janus.log("Created remote video stream:", streamVideo);
      Janus.attachMediaStream(video, streamVideo);
      janus2 = new Janus({
        server: server,
        success: function () {
          janus2.attach({
            plugin: "janus.plugin.streaming",
            success: function (plugin) {
              janusAttachSuccess(plugin, 11);
            },
            onmessage: janusOnMessage,
            onremotetrack: janusOnRemoteTrack,
            onerror: janusOnError,
          });
        },
      });
    }
  }
  if (track.kind === "audio") {
    let audio = document.getElementById("remoteAudio");
    let streamAudio = new MediaStream([track]);
    Janus.log("Created remote audio stream:", streamAudio);
    Janus.attachMediaStream(audio, streamAudio);
  }
}

function janusOnError() {
  Janus.log("Error");
}
Janus.init({
  debug: "all",
  callback: function () {
    janus = new Janus({
      server: server,
      success: function () {
        janus.attach({
          plugin: "janus.plugin.streaming",
          success: function (plugin) {
            janusAttachSuccess(plugin, 10);
          },
          onmessage: janusOnMessage,
          onremotetrack: janusOnRemoteTrack,
          onerror: janusOnError,
        });
      },
    });
  },
});
const selectElement = document.getElementById("resolution-select");

selectElement.addEventListener("change", function () {
  const selectedValue = this.value;
  console.log("Résolution sélectionnée :", selectedValue);

  // Exemple : on peut séparer largeur et hauteur
  const [width, height] = selectedValue.split("x").map(Number);
  // Tu peux ajouter ton propre traitement ici
  console.log(`Largeur : ${width}, Hauteur : ${height}`);
  document.getElementById("remoteVideo").width = width;
  document.getElementById("remoteVideo").height = height;
});
