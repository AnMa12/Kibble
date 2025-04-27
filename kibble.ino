#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Stepper.h>

// Replace with your network credentials
const char* ssid = "digi-y234";
const char* password = "fe4-234-g54-532";

// Web server running on port 80
WebServer server(80);

// Stepper motor setup
const int stepsPerRevolution = 2038;
Stepper myStepper(stepsPerRevolution, 4, 6, 5, 7);

// HTML webpage
const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-CAM Stepper Control</title>
</head>
<body>
  <h1>ESP32-CAM Stream + Stepper Motor</h1>
  <img src="/stream" style="width:100%;max-width:400px;">
  <br><br>
  <button onclick="fetch('/move')">Move Stepper Motor</button>
</body>
</html>
)rawliteral";

// Setup LED flash if needed
void setupLedFlash(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

// Function to handle the root path "/"
void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

// Handle stepper move command
void handleMove() {
  myStepper.setSpeed(10); // 10 RPM
  myStepper.step(-stepsPerRevolution); // Rotate one revolution
  server.send(200, "text/plain", "Stepper Moved!");
}

// Start camera server (MJPEG stream)
void startCameraServer() {
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  
  // MJPEG stream
  server.on("/stream", HTTP_GET, []() {
    WiFiClient client = server.client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    server.sendContent(response);

    while (1) {
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }

      response = "--frame\r\n";
      response += "Content-Type: image/jpeg\r\n\r\n";
      server.sendContent(response);
      server.sendContent((const char *)fb->buf, fb->len);
      server.sendContent("\r\n");

      esp_camera_fb_return(fb);

      if (!client.connected()) {
        break;
      }
      delay(100); // Adjust to control frame rate
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  server.handleClient();
}
