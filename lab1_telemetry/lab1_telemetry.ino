#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "nketiaaa";
const char* password = "password";

// AWS IoT Endpoint
const char* AWS_IOT_ENDPOINT = "874841217409-ats.iot.eu-north-1.amazonaws.com";

// MQTT Topic
const char* AWS_IOT_TOPIC = "esp32/telemetry";

// DHT22 Configuration
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Root CA Certificate (Amazon Root CA 1)
const char* CACERT = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/JaO54vB4ikPmIjZbyjANBgkqhkiG9w0BAQSF\n" \
"ADASMQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMQwwCgYDVQQLEw1BdGEx\n" \
"b24yV2F0Y0RQQSAxMB4XDTE5MDIyNnAwMDAwMFoXDTI0MDIyNjAwMDAwMFowSTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEMMAoGA1UECxMDQXRhMDIyV2F0\n" \
"Y0RQQSAxMREwDwYDVQQDEwhBbWF6b24gVFIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQC9v3K9vCDQSAMXMB4XDT2E1MDJnYjNjY2J1NDEXMDEN4MDEXMzYw\n" \
"MAKGA1UEChMVDXNzMvM3DzanBGVvA0ZanBvYjEZMCARQoADAgECAQOCggEBALJ4gHnXj\n" \
"b3qQg8EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHnXjb3qQg8E\n" \
"ca9HgF8fwV7y14h29J1o91ghYP1hA4EvrAIThtCgQ3p0qsQTqNro8vo3BSmHgFZm\n" \
"906II8c+6zf1rtN4S4Wi3e5djgDYzK/oi2pVKVuRF4fn9tBb6dXENqO5L/qw\n" \
"IFAgBhRrQgLkM+a/sRXmPUDG3hKKH0Vj4utVp+UnnMjbu1hnep4mjucAwahmaRwa6\n" \
"VOUjw5HS5Nz/@eguLX8tdHA114gk957EWv67c4cx8JjGKLHd+rcdqsQ08p8kDi1L\n" \
"93FcXmn/6pUCyzikr1A4b9v7LWIbxccvOF34gFlD5yHISY/QCB/IIDEgEw+OyQt\n" \
"jgSub3rIag@CaWEAAAnCNEAMwDYDVRT0AQN/BAUwAWEB/zA0BgNVHREBAf8EBAMCPgAw\n" \
"AYYWHQYDVR0OBBYEFEQY/EVIQyZIU07LWlQuQcFmcx7IQtGoIM48GCqSq6ISB3DQEbcXUA\n" \
"A4IBAQCY8jdaQzChGsV2UsggNiMOrUu6r41KSipOB/G/wkju8yKGX9rbxerDI\n" \
"USPMCCjjmCXPI6T53iHTFiUJRU6adTrCC2qjEHZERxhlb1I6jjt/mxv8twdQ1wJs\n" \
"N+gD563pyAaCbxy8Mwy7Vu33PquXHeeE6V/Uq2V8vit096LXfVKm1JbYK8J9@vv\n" \
"o/uFQJvmtVT8QtPHRh8jrdkPShCA2XV4cdFyQzR1blD/wgjIcJmApzyM2FoS1QSXU\n" \
"SMsI+yMRQ+HDKX3Iow1XdJxgJkLK642W4UwtBV8bob2x3NDd2ZwLnoQdGexeGAdbkpy\n" \
"rqXRFboQnoZsGQ4qSWtp46B5QuvvD5\n" \
"-----END CERTIFICATE-----\n";

// Thing Certificate (.pem.crt)
const char* CERTIFICATE = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUWcDAxJ1jTC0tvqr031OHJ7B8PnMwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI2MTAxNTIzMTAy
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALS4tNDT/SZQ7uR4ru5n
3QcYvrL+CA+mMQbOtXZsalW2UwUNRlv2mDks8NB96Cgvzdced5XIhaahZLaEzsFO
Kz1lUMn3W2RagJJO2PAcT7F9hYjSIivu0d41irVHggtjVwhYSln6zZK4kqAm0F8Z
rNq3kwT+Pz/B+j8hpgAvvRfe6AR3+AfUDi3BU9XlakjWnf+puRsaxbsHwWgjVfKm
qY9K8x3i2Z1zhJ/buLDqjOyK4NyvWczdvDz1PT7+ZuYxIszOSvu+OSHYi0xprkIg
t2p1kqp+zNeKCLO+b6JVulVF3j2iy+JLZj29pFeGukB8ZZ4NsgUgFdEd3hC0pKPi
kscCAwEAAaNgMF4wHwYDVR0jBBgwFoAU7Y7r7OUNgm80y3zQq75jMdOv63wwHQYD
VR0OBBYEFL962Eus+Mv6EIdDt02ZWK8KSN1vMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCq5rPJDWrWb2j0dnJzS8QFHA2S
5+rCRVCAzi6salbS330rRNVYPAVkvqIvLbIXzhZqkolxrIgze0EXtNEVeiVoFa08
b8iW3oy6h2CurH/KXOL+Xwr3HPbGKzM1Yxbo9wbQ4qU6xzF5UteTYy+ewo23riIS
D6bLWfzFc/Ni4i3l5SU3vLWULMXpympPl+DWmId2KcNKWpXepYyDXCvAwGl0regP
AEs+QxHpZBNBpgTuUDS2LaLGYmM0WyX9v91gh9aPrwWJstIGxJNDdu2i3xt+XCdO
Wh+9EiVDcPoCbai2z8pQzguYGF/Igl8l7f+tT6+XWqonkI/18pIUtzus2wqC\n" \
"-----END CERTIFICATE-----\n";

// Private Key (.pem.key)
const char* PRIVATE_KEY = 
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAtLi00NP9JlDu5Hiu7mfdBxi+sv4ID6YxBs61dmxqVbZTBQ1G\\\n" \
"W/aYOSzw0H3oKC/N1x53lciFpqFktoTOwU4rPWVQyfdbZFqAkk7Y8BxPsX2FiNIi\\\n" \
"K+7R3jWKtUeCC2NXCFhKWfrNkriSoCbQXxms2reTBP4/P8H6PyGmAC+9F97oBHf4\\\n" \
"B9QOLcFT1eVqSNad/6m5GxrFuwfBaCNV8qapj0rzHeLZnXOEn9u4sOqM7Irg3K9Z\\\n" \
"zN28PPU9Pv5m5jEizM5K+745IdiLTGmuQiC3anWSqn7M14oIs75volW6VUXePaLL\\\n" \
"4ktmPb2kV4a6QHxlng2yBSAV0R3eELSko+KSxwIDAQABAoIBAG+ygb9YZuyZtS1t\\\n" \
"EeG0PD44oxUKQX06oK6ucCXDmG1lhSZoMAaDAU7FYgna5/R+p2DAKfVfvIti0Val\\\n" \
"+wwq/jqyNh6+TbRW44SKSzcMqIzxIemQxyIUN4551UN58PyM56v2te2rQAfhIG7v\\\n" \
"znYY7mc4cxvw8LK9d6gxaeJPnzXyu9kTUE9lKDnIv53IDqW/ig15FfajwceqfEfc\\\n" \
"TWyu8fl2JnH0uHXwxhTxB0YnWbm/xHXQdR82jVFi+JPZFiUwtC8O+K5hUCMH2CNW\\\n" \
"s6bak1cWMAMZy5tuGFIVT2RZnpNJh61sOAyAwsrfSPOrgPYl1ZYQeFjvNoQ6Ho71\\\n" \
"k2UgXhECgYEA2a7VaqLsVLrjgrp6BigU2LpVpQnFsluQNCXTarbF0+Ob65023PPm\\\n" \
"lfoUA20r7b/FLrTJHRoxXFf2072tqFrtTdNIPn6nv4PMRQJTktHyRO2vIIh/drbf\\\n" \
"N3DmoTpvEF4OtP/l9A1Oys5Qso7OrlLmWE181MKC+WEyet0+XHgaNGUCgYEA1IhT\\\n" \
"u4arlc9vua6AdZ+CvFPu5d6vaW4tq7T0SFwlZy5RgXqcHvnboR1v26Uobs6Rms/u\\\n" \
"W9hhfcNx9J3vSgaaCphz9mKbWDUY6EPMhduopodiGgxljW7V5X8tMYgdQCzIl47W\\\n" \
"VDGB1psb+Rv59fL6erPqrqjwKPeRrPy0SsH/ybsCgYEA2Sy8by8saTwrT3mL1CiY\\\n" \
"48DDQWyGeHMxY1V5n7XJmKQ0ItUurzIGkwp0fRamBQSWTQrAuvAu3tpDvuw2HQja\\\n" \
"SBLKuM7TvRZY7WBs0MX01x/q4KJNyLqZ6E4TSjdqJwIv2hST7vB3843ypLIlMXYz\\\n" \
"qVVlfT6yPOrj2gBw/wAz5KECgYEAhjZY+jE/JenLGizyflTu0FMIqC0KBMBJIwew\\\n" \"xAk8rX34hpWRBZavdX0LaF9RuzfDh0oLsgBhAHRuABXESLEGke5YPJ1kkD1nrzUx\\\n" \
"cM6YICGad+ygLMJzXOwE9CSKpD4jpmHTQ3PIFGf7MELUhNaeCuKXLxkMIDLG0cD7\\\n" \
"ccI074UCgYACnEE5S4Y2PhYNv5cy8QunWIDCdFV3/ivMfmD1bDYltTV9WMxJBqIx\\\n" \
"s5UZWZ3xPNcs/G95NCsSJ3spnWv7KKGSXQVY90wTRS6QZn7FxX2Ow0vv35J/wXQZ\\\n" \
"W7PGc25QFiNhEKqAn5gQwY4OSvpXiApnVB6kZqanSwSGQ7uESzJYcw==\n" \
"-----END RSA PRIVATE KEY-----\n";

WiFiClientSecure net;
PubSubClient client(net);

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
}

void connectAWS() {
  net.setCACert(CACERT);
  net.setCertificate(CERTIFICATE);
  net.setPrivateKey(PRIVATE_KEY);

  client.setServer(AWS_IOT_ENDPOINT, 8883);

  Serial.print("Connecting to AWS IoT Core...");
  while (!client.connect("ESP32_Thing")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to AWS IoT Core!");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  connectWiFi();
  
  // Sync time via NTP (required for TLS validation)
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
  connectAWS();
}

unsigned long lastMillis = 0;

void loop() {
  client.loop();

  if (millis() - lastMillis > 5000) {
    lastMillis = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String payload = "{";
    payload += "\"device_id\": \"ESP32_01\",";
    payload += "\"temperature_c\": " + String(t, 2) + ",";
    payload += "\"humidity_percent\": " + String(h, 2);
    payload += "}";

    Serial.print("Publishing: ");
    Serial.println(payload);

    client.publish(AWS_IOT_TOPIC, payload.c_str());
  }
}
