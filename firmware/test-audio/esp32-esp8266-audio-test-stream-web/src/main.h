typedef struct {
  char url[96];
  bool isAAC;
  int16_t volume;
  int16_t checksum;
} Settings;

void SaveSettings();
void LoadSettings();

void RedirectToIndex(WiFiClient *client);

// C++11 multiline string constants are neato...
static const char HEAD[] PROGMEM = R"KEWL(
<head>
<title>ESP8266 Web Radio</title>
<script type="text/javascript">
  function updateTitle() {
    var x = new XMLHttpRequest();
    x.open("GET", "title");
    x.onload = function() { document.getElementById("titlespan").innerHTML=x.responseText; setTimeout(updateTitle, 5000); }
    x.onerror = function() { setTimeout(updateTitle, 5000); }
    x.send();
  }
  setTimeout(updateTitle, 1000);
  function showValue(n) {
    document.getElementById("volspan").innerHTML=n;
    var x = new XMLHttpRequest();
    x.open("GET", "setvol?vol="+n);
    x.send();
  }
  function updateStatus() {var x = new XMLHttpRequest();
    x.open("GET", "status");
    x.onload = function() { document.getElementById("statusspan").innerHTML=x.responseText; setTimeout(updateStatus, 5000); }
    x.onerror = function() { setTimeout(updateStatus, 5000); }
    x.send();
  }
  setTimeout(updateStatus, 2000);
</script>
</head>)KEWL";

static const char BODY[] PROGMEM = R"KEWL(
<body>
ESP8266 Web Radio!
<hr>
Currently Playing: <span id="titlespan">%s</span><br>
Volume: <input type="range" name="vol" min="1" max="150" steps="10" value="%d" onchange="showValue(this.value)"/> <span id="volspan">%d</span>%%
<hr>
Status: <span id="statusspan">%s</span>
<hr>
<form action="changeurl" method="GET">
Current URL: %s<br>
Change URL: <input type="text" name="url">
<select name="type"><option value="mp3">MP3</option><option value="aac">AAC</option></select>
<input type="submit" value="Change"></form>
<form action="stop" method="POST"><input type="submit" value="Stop"></form>
</body>)KEWL";