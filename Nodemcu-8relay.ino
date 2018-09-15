#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

MDNSResponder mdns;
const char* ssid = "SSID";
const char* password = "PASSWORD";
ESP8266WebServer server(80);
String webPage = "";

void setup() {
  SPIFFS.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println(": ");
  Serial.println("Searching for WiFi");


  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  String mymac = (WiFi.macAddress());
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  webPage += "<title>BSO's Haustechnik v0.1</title><h1 style=color:green>BSO's UG Haustechnik v0.1</h1>";
  webPage += "<h3 style=color:Blue>Voreinstellungen</h3>";
  webPage += "<p><a href=\"socket00On\"><button>Alles AN</button></a>&nbsp;<a href=\"socket00Off\"><button>Alles AUS</button></a>&nbsp;<a href=\"socket-n\"><button>Nacht</button></a>&nbsp;<a href=\"socket-w\"><button>Arbeit</button></a></p>";
  webPage += "<h3 style=color:Blue>Relay Steuerung&nbsp;<a href=\"socket00R\"><button>Wiederherstellen</button></h3>";
  webPage += "<p><a href=\"socket10On\"><button>ON</button></a>&nbsp;<a href=\"socket10Off\"><button>OFF</button></a>  #10 - Relay-D0 - Servermonitor</p>";
  webPage += "<p><a href=\"socket11On\"><button>ON</button></a>&nbsp;<a href=\"socket11Off\"><button>OFF</button></a>  #11 - Relay-D1 - Bier Lampe</p>";
  webPage += "<p><a href=\"socket12On\"><button>ON</button></a>&nbsp;<a href=\"socket12Off\"><button>OFF</button></a>  #12 - Relay-D2 - HP Mufu</p>";
  webPage += "<p><a href=\"socket13On\"><button>ON</button></a>&nbsp;<a href=\"socket13Off\"><button>OFF</button></a>  #13 - Relay-D3 - Front Rechts</p>";
  webPage += "<p><a href=\"socket15On\"><button>ON</button></a>&nbsp;<a href=\"socket15Off\"><button>OFF</button></a>  #15 - Relay-D5 - Front Mitte</p>";
  webPage += "<p><a href=\"socket16On\"><button>ON</button></a>&nbsp;<a href=\"socket16Off\"><button>OFF</button></a>  #16 - Relay-D6 - Front Links</p>";
  webPage += "<p><a href=\"socket17On\"><button>ON</button></a>&nbsp;<a href=\"socket17Off\"><button>OFF</button></a>  #17 - Relay-D7 - frei</p>";
  webPage += "<p><a href=\"socket14On\"><button>ON</button></a>&nbsp;<a href=\"socket14Off\"><button>OFF</button></a>  #14 - Relay-D8 - frei</p>";
  webPage += "<h3 style=color:red>Tools</h3><form method = 'POST' action = '/update' enctype = 'multipart/form-data'><input type = 'file' name = 'update'><input type = 'submit' value = 'Aktualisieren'></form>";
  webPage += "<p><a href=\"format\"><button>Einstellungen Löschen</button></a>Flash formatieren</p>";
  webPage += mymac;
  webPage += "<h3 style=color:red> About</h3>";
  webPage += "<p>todo: jede menge</p>";
  webPage += "<p>Script Version 0.1 Update 20180915 (c) Bjoern Sommer</p>";

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  pinMode(D4, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(D4, LOW);
  }


  File f10 = SPIFFS.open("/state10.txt", "r");
  if (!f10) {
    Serial.println("#10 File open failed on read.");
    digitalWrite(D0, HIGH);
  } else {
    while (f10.available()) {
      String socket10 = f10.readStringUntil('\n');
      int socket10i = socket10.toInt();
      if (socket10i == 1) {
        digitalWrite(D0, LOW);
        Serial.println("Last State of #10 was On");
      } else {
        digitalWrite(D0, HIGH);
        Serial.println("Last State of #10 was OFF");
      }
    }
    f10.close();
  }

  File f11 = SPIFFS.open("/state11.txt", "r");
  if (!f11) {
    Serial.println("#11 File open failed on read.");
    digitalWrite(D1, HIGH);
  } else {
    while (f11.available()) {
      String socket11 = f11.readStringUntil('\n');
      int socket11i = socket11.toInt();
      if (socket11i == 1) {
        digitalWrite(D1, LOW);
        Serial.println("Last State of #11 was On");
      } else {
        digitalWrite(D1, HIGH);
        Serial.println("Last State of #11 was OFF");
      }
    }
    f11.close();
  }

  File f12 = SPIFFS.open("/state12.txt", "r");
  if (!f12) {
    Serial.println("#12 File open failed on read.");
    digitalWrite(D2, HIGH);
  } else {
    while (f12.available()) {
      String socket12 = f12.readStringUntil('\n');
      int socket12i = socket12.toInt();
      if (socket12i == 1) {
        digitalWrite(D2, LOW);
        Serial.println("Last State of #12 was On");
      } else {
        digitalWrite(D2, HIGH);
        Serial.println("Last State of #12 was OFF");
      }
    }
    f12.close();
  }

  File f13 = SPIFFS.open("/state13.txt", "r");
  if (!f13) {
    Serial.println("#13 File open failed on read.");
    digitalWrite(D3, HIGH);
  } else {
    while (f13.available()) {
      String socket13 = f13.readStringUntil('\n');
      int socket13i = socket13.toInt();
      if (socket13i == 1) {
        digitalWrite(D3, LOW);
        Serial.println("Last State of #13 was On");
      } else {
        digitalWrite(D3, HIGH);
        Serial.println("Last State of #13 was OFF");
      }
    }
    f13.close();
  }

  File f14 = SPIFFS.open("/state14.txt", "r");
  if (!f14) {
    Serial.println("#14 File open failed on read.");
    digitalWrite(D8, HIGH);
  } else {
    while (f14.available()) {
      String socket14 = f14.readStringUntil('\n');
      int socket14i = socket14.toInt();
      if (socket14i == 1) {
        digitalWrite(D8, LOW);
        Serial.println("Last State of #14 was On");
      } else {
        digitalWrite(D8, HIGH);
        Serial.println("Last State of #14 was OFF");
      }
    }
    f14.close();
  }

  File f15 = SPIFFS.open("/state15.txt", "r");
  if (!f15) {
    Serial.println("#15 File open failed on read.");
    digitalWrite(D5, HIGH);
  } else {
    while (f15.available()) {
      String socket15 = f15.readStringUntil('\n');
      int socket15i = socket15.toInt();
      if (socket15i == 1) {
        digitalWrite(D5, LOW);
        Serial.println("Last State of #15 was On");
      } else {
        digitalWrite(D5, HIGH);
        Serial.println("Last State of #15 was OFF");
      }
    }
    f15.close();
  }

  File f16 = SPIFFS.open("/state16.txt", "r");
  if (!f16) {
    Serial.println("#16 File open failed on read.");
    digitalWrite(D6, HIGH);
  } else {
    while (f16.available()) {
      String socket16 = f16.readStringUntil('\n');
      int socket16i = socket16.toInt();
      if (socket16i == 1) {
        digitalWrite(D6, LOW);
        Serial.println("Last State of #16 was On");
      } else {
        digitalWrite(D6, HIGH);
        Serial.println("Last State of #16 was OFF");
      }
    }
    f16.close();
  }

  File f17 = SPIFFS.open("/state17.txt", "r");
  if (!f17) {
    Serial.println("#17 File open failed on read.");
    digitalWrite(D7, HIGH);
  } else {
    while (f17.available()) {
      String socket17 = f17.readStringUntil('\n');
      int socket17i = socket17.toInt();
      if (socket17i == 1) {
        digitalWrite(D7, LOW);
        Serial.println("Last State of #17 was On");
      } else {
        digitalWrite(D7, HIGH);
        Serial.println("Last State of #17 was OFF");
      }
    }
    f17.close();
  }

  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });

  server.on("/socket10On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D0, LOW);
    File f10 = SPIFFS.open("/state10.txt", "w");
    f10 = SPIFFS.open("/state10.txt", "w");
    if (!f10) {
      Serial.println("#10 file open failed on update.");
    } else {
      f10.println("1");
      f10.close();
      Serial.println("written #10-1");
    }
    delay(500);
  });
  server.on("/socket10Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D0, HIGH);
    File f10 = SPIFFS.open("/state10.txt", "w");
    f10 = SPIFFS.open("/state10.txt", "w");
    if (!f10) {
      Serial.println("#10 file open failed on update.");
    } else {
      f10.println("0");
      f10.close();
      Serial.println("written #10-0");
    }
    delay(500);
  });

  server.on("/socket11On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D1, LOW);
    File f11 = SPIFFS.open("/state11.txt", "w");
    f11 = SPIFFS.open("/state11.txt", "w");
    if (!f11) {
      Serial.println("#11 file open failed on update.");
    } else {
      f11.println("1");
      f11.close();
      Serial.println("written #11-1");
    }
    delay(500);
  });
  server.on("/socket11Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D1, HIGH);
    File f11 = SPIFFS.open("/state11.txt", "w");
    f11 = SPIFFS.open("/state11.txt", "w");
    if (!f11) {
      Serial.println("#11 file open failed on update.");
    } else {
      f11.println("0");
      f11.close();
      Serial.println("written #11-0");
    }
    delay(500);
  });

  server.on("/socket12On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D2, LOW);
    File f12 = SPIFFS.open("/state12.txt", "w");
    f12 = SPIFFS.open("/state12.txt", "w");
    if (!f12) {
      Serial.println("#12 file open failed on update.");
    } else {
      f12.println("1");
      f12.close();
      Serial.println("written #12-1");
    }
    delay(500);
  });
  server.on("/socket12Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D2, HIGH);
    File f12 = SPIFFS.open("/state12.txt", "w");
    f12 = SPIFFS.open("/state12.txt", "w");
    if (!f12) {
      Serial.println("#12 file open failed on update.");
    } else {
      f12.println("0");
      f12.close();
      Serial.println("written #12-0");
    }
    delay(500);
  });

  server.on("/socket13On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D3, LOW);
    File f13 = SPIFFS.open("/state13.txt", "w");
    f13 = SPIFFS.open("/state13.txt", "w");
    if (!f13) {
      Serial.println("#13 file open failed on update.");
    } else {
      f13.println("1");
      f13.close();
      Serial.println("written #13-1");
    }
    delay(500);
  });
  server.on("/socket13Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D3, HIGH);
    File f13 = SPIFFS.open("/state13.txt", "w");
    f13 = SPIFFS.open("/state13.txt", "w");
    if (!f13) {
      Serial.println("#13 file open failed on update.");
    } else {
      f13.println("0");
      f13.close();
      Serial.println("written #13-0");
    }
    delay(500);
  });

  server.on("/socket14On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D8, LOW);
    File f14 = SPIFFS.open("/state14.txt", "w");
    f14 = SPIFFS.open("/state14.txt", "w");
    if (!f14) {
      Serial.println("#14 file open failed on update.");
    } else {
      f14.println("1");
      f14.close();
      Serial.println("written #14-1");
    }
    delay(500);
  });
  server.on("/socket14Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D8, HIGH);
    File f14 = SPIFFS.open("/state14.txt", "w");
    f14 = SPIFFS.open("/state14.txt", "w");
    if (!f14) {
      Serial.println("#14 file open failed on update.");
    } else {
      f14.println("0");
      f14.close();
      Serial.println("written #14-0");
    }
    delay(500);
  });

  server.on("/socket15On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D5, LOW);
    File f15 = SPIFFS.open("/state15.txt", "w");
    f15 = SPIFFS.open("/state15.txt", "w");
    if (!f15) {
      Serial.println("#15 file open failed on update.");
    } else {
      f15.println("1");
      f15.close();
      Serial.println("written #15-1");
    }
    delay(500);
  });
  server.on("/socket15Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D5, HIGH);
    File f15 = SPIFFS.open("/state15.txt", "w");
    f15 = SPIFFS.open("/state15.txt", "w");
    if (!f15) {
      Serial.println("#15 file open failed on update.");
    } else {
      f15.println("0");
      f15.close();
      Serial.println("written #15-0");
    }
    delay(500);
  });
  server.on("/socket16On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D6, LOW);
    File f16 = SPIFFS.open("/state16.txt", "w");
    f16 = SPIFFS.open("/state16.txt", "w");
    if (!f16) {
      Serial.println("#16 file open failed on update.");
    } else {
      f16.println("1");
      f16.close();
      Serial.println("written #16-1");
    }
    delay(500);
  });
  server.on("/socket16Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D6, HIGH);
    File f16 = SPIFFS.open("/state16.txt", "w");
    f16 = SPIFFS.open("/state16.txt", "w");
    if (!f16) {
      Serial.println("#16 file open failed on update.");
    } else {
      f16.println("0");
      f16.close();
      Serial.println("written #16-0");
    }
    delay(500);
  });

  server.on("/socket17On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D7, LOW);
    File f17 = SPIFFS.open("/state17.txt", "w");
    f17 = SPIFFS.open("/state17.txt", "w");
    if (!f17) {
      Serial.println("#17 file open failed on update.");
    } else {
      f17.println("1");
      f17.close();
      Serial.println("written #17-1");
    }
    delay(500);
  });
  server.on("/socket17Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D7, HIGH);
    File f17 = SPIFFS.open("/state17.txt", "w");
    f17 = SPIFFS.open("/state17.txt", "w");
    if (!f17) {
      Serial.println("#17 file open failed on update.");
    } else {
      f17.println("0");
      f17.close();
      Serial.println("written #17-0");
    }
    delay(500);
  });


  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      WiFiUDP::stopAll();
      Serial.printf("Update: %s\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield();
  });

  server.on("/socket00On", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D7, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D8, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D0, LOW);
    delay(500);
  });
  server.on("/socket00Off", []() {
    server.send(200, "text/html", webPage);
    digitalWrite(D7, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D8, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D0, HIGH);
    delay(500);
  });

  server.on("/socket00R", []() {
    server.send(200, "text/html", webPage);
    File f10 = SPIFFS.open("/state10.txt", "r");
    if (!f10) {
      Serial.println("#10 File open failed on read.");
      digitalWrite(D0, HIGH);
    } else {
      while (f10.available()) {
        String socket10 = f10.readStringUntil('\n');
        int socket10i = socket10.toInt();
        if (socket10i == 1) {
          digitalWrite(D0, LOW);
          Serial.println("Last State of #10 was On");
        } else {
          digitalWrite(D0, HIGH);
          Serial.println("Last State of #10 was OFF");
        }
      }
      f10.close();
    }

    File f11 = SPIFFS.open("/state11.txt", "r");
    if (!f11) {
      Serial.println("#11 File open failed on read.");
      digitalWrite(D1, HIGH);
    } else {
      while (f11.available()) {
        String socket11 = f11.readStringUntil('\n');
        int socket11i = socket11.toInt();
        if (socket11i == 1) {
          digitalWrite(D1, LOW);
          Serial.println("Last State of #11 was On");
        } else {
          digitalWrite(D1, HIGH);
          Serial.println("Last State of #11 was OFF");
        }
      }
      f11.close();
    }

    File f12 = SPIFFS.open("/state12.txt", "r");
    if (!f12) {
      Serial.println("#12 File open failed on read.");
      digitalWrite(D2, HIGH);
    } else {
      while (f12.available()) {
        String socket12 = f12.readStringUntil('\n');
        int socket12i = socket12.toInt();
        if (socket12i == 1) {
          digitalWrite(D2, LOW);
          Serial.println("Last State of #12 was On");
        } else {
          digitalWrite(D2, HIGH);
          Serial.println("Last State of #12 was OFF");
        }
      }
      f12.close();
    }

    File f13 = SPIFFS.open("/state13.txt", "r");
    if (!f13) {
      Serial.println("#13 File open failed on read.");
      digitalWrite(D3, HIGH);
    } else {
      while (f13.available()) {
        String socket13 = f13.readStringUntil('\n');
        int socket13i = socket13.toInt();
        if (socket13i == 1) {
          digitalWrite(D3, LOW);
          Serial.println("Last State of #13 was On");
        } else {
          digitalWrite(D3, HIGH);
          Serial.println("Last State of #13 was OFF");
        }
      }
      f13.close();
    }

    File f14 = SPIFFS.open("/state14.txt", "r");
    if (!f14) {
      Serial.println("#14 File open failed on read.");
      digitalWrite(D8, HIGH);
    } else {
      while (f14.available()) {
        String socket14 = f14.readStringUntil('\n');
        int socket14i = socket14.toInt();
        if (socket14i == 1) {
          digitalWrite(D8, LOW);
          Serial.println("Last State of #14 was On");
        } else {
          digitalWrite(D8, HIGH);
          Serial.println("Last State of #14 was OFF");
        }
      }
      f14.close();
    }

    File f15 = SPIFFS.open("/state15.txt", "r");
    if (!f15) {
      Serial.println("#15 File open failed on read.");
      digitalWrite(D5, HIGH);
    } else {
      while (f15.available()) {
        String socket15 = f15.readStringUntil('\n');
        int socket15i = socket15.toInt();
        if (socket15i == 1) {
          digitalWrite(D5, LOW);
          Serial.println("Last State of #15 was On");
        } else {
          digitalWrite(D5, HIGH);
          Serial.println("Last State of #15 was OFF");
        }
      }
      f15.close();
    }

    File f16 = SPIFFS.open("/state16.txt", "r");
    if (!f16) {
      Serial.println("#16 File open failed on read.");
      digitalWrite(D6, HIGH);
    } else {
      while (f16.available()) {
        String socket16 = f16.readStringUntil('\n');
        int socket16i = socket16.toInt();
        if (socket16i == 1) {
          digitalWrite(D6, LOW);
          Serial.println("Last State of #16 was On");
        } else {
          digitalWrite(D6, HIGH);
          Serial.println("Last State of #16 was OFF");
        }
      }
      f16.close();
    }

    File f17 = SPIFFS.open("/state17.txt", "r");
    if (!f17) {
      Serial.println("#17 File open failed on read.");
      digitalWrite(D7, HIGH);
    } else {
      while (f17.available()) {
        String socket17 = f17.readStringUntil('\n');
        int socket17i = socket17.toInt();
        if (socket17i == 1) {
          digitalWrite(D7, LOW);
          Serial.println("Last State of #17 was On");
        } else {
          digitalWrite(D7, HIGH);
          Serial.println("Last State of #17 was OFF");
        }
      }
      f17.close();
    }
    delay(500);
  });

  server.on("/format", []() {
    server.send(200, "text/html", webPage);
    Serial.println("Please wait 30 secs for SPIFFS to be formatted");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    delay(500);
  });
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}
