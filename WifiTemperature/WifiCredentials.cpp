#include "WifiCredentials.h"

WifiCredentials::WifiCredentials(char* credentialsCoded) {
  int inputStringLength = strlen(credentialsCoded);

  if (inputStringLength > 0) {
    int decodedLength = Base64.decodedLength(credentialsCoded, inputStringLength);
    char decodedString[decodedLength];

    Base64.decode(decodedString, credentialsCoded, inputStringLength);
    char* ssid = strtok(decodedString, ":");
    if (ssid != NULL)
      _ssid = strdup(ssid);

    char* password = strtok(NULL, ":");
    if (password != NULL)
      _password = strdup(password);

    bool userNullOrEmpty = _ssid == NULL ? true : strlen(_ssid) == 0;
    bool passwordNullOrEmpty =  _password  == NULL ? true : strlen(_password) == 0;

    _isValid = !userNullOrEmpty && !passwordNullOrEmpty;
  }
  else {
    _isValid = false;
  }
}

bool WifiCredentials::IsValid() const {
  return _isValid;
}

char* WifiCredentials::SSID() const {
  return _ssid;
}

char* WifiCredentials::Password() const {
  return _password;
}
