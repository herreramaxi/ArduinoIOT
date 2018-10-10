#ifndef WifiCredentials_h
#define WifiCredentials_h

#include "Arduino.h"
#include "Base64.h"

class WifiCredentials
{
  public:
    WifiCredentials(char* credentialsCoded);

    bool IsValid() const;
    char* SSID() const;
    char* Password() const;

  private:
    char* _ssid;
    char* _password;
    bool _isValid;
};
#endif
