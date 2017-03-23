#include "IMU_Manager.h"

IMU_Manager::IMU_Manager(){
  declination = DEFAULT_DECLINATION;
  }

void IMU_Manager::initialize(){
  imu.settings.device.commInterface = IMU_MODE_SPI;
  imu.settings.device.mAddress = LSM9DS1_M_CS;
  imu.settings.device.agAddress = LSM9DS1_AG_CS;
  imu.begin();
}

void IMU_Manager::update_declination(int d){
  declination = d;
}

data_log_t IMU_Manager::get_accel(){
  data_log_t output;
  if(imu.accelAvailable()){
    imu.readAccel();
    output.type = BB_ACCEL;
    output.data_length = 12; //4 bytes for each axis
    uint8_t data[output.data_length];
    uint8_t tmp[4];
    float_to_int(imu.calcAccel(imu.ax), &tmp[0]);
    copy_into(tmp, 4, data, 0);
    float_to_int(imu.calcAccel(imu.ay), &tmp[0]);
    copy_into(tmp, 4, data, 4);
    float_to_int(imu.calcAccel(imu.az), &tmp[0]);
    copy_into(tmp, 4, data, 8);
    output.data = data;
  }else{
    output.data_length = 0;
  }
  return output;
}

data_log_t IMU_Manager::get_gyro(){
  data_log_t output;
  if(imu.gyroAvailable()){
    imu.readGyro();
    output.type = BB_GYRO;
    output.data_length = 12; //4 bytes for each axis
    uint8_t data[output.data_length];
    uint8_t tmp[4];
    float_to_int(imu.calcGyro(imu.gx), &tmp[0]);
    copy_into(tmp, 4, data, 0);
    float_to_int(imu.calcGyro(imu.gy), &tmp[0]);
    copy_into(tmp, 4, data, 4);
    float_to_int(imu.calcGyro(imu.gz), &tmp[0]);
    copy_into(tmp, 4, data, 8);
    output.data = data;
  }else{
    output.data_length = 0;
  }
  return output;
  }

data_log_t IMU_Manager::get_mag(){
  data_log_t output;
  if(imu.magAvailable()){
    imu.readMag();
    output.type = BB_MAG;
    output.data_length = 12; //4 bytes for each axis
    uint8_t data[output.data_length];
    uint8_t tmp[4];
    float_to_int(imu.calcMag(imu.mx), &tmp[0]);
    copy_into(tmp, 4, data, 0);
    float_to_int(imu.calcMag(imu.my), &tmp[0]);
    copy_into(tmp, 4, data, 4);
    float_to_int(imu.calcMag(imu.mz), &tmp[0]);
    copy_into(tmp, 4, data, 8);
    output.data = data;
  }else{
    output.data_length = 0;
  }
  return output;
  }
