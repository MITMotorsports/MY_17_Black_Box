#ifndef XBEE_MANAGER_H
#define XBEE_MANAGER_H

class Xbee_Manager {
  public:
    XbeeManager();
    void check_For_Message();
    bool send_Message();
  private:
    //string? getDataFormat();
};

#endif
