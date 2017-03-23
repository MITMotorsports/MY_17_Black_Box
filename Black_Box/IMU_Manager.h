#ifndef IMU_MANAGER_H
#define IMU_MANAGER_H

#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

#include "Data_Types.h"
#include "utils.h"
#include "pins.h"

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DEFAULT_DECLINATION -8.58

/* --- IMU MANAGER ---
*
*/
class IMU_Manager {
  public:
    IMU_Manager();
    void initialize();
    void update_declination(int d);
    data_log_t get_accel();
    data_log_t get_gyro();
    data_log_t get_mag();
  private:
    LSM9DS1 imu;
    int declination;
};

#endif
