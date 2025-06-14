// Copyright 2021 CLOBOT Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MYAHRS_ROS2_DRIVER__MYAHRS_ROS2_DRIVER_HPP_
#define MYAHRS_ROS2_DRIVER__MYAHRS_ROS2_DRIVER_HPP_

#include <myahrs_ros2_driver/myahrs_plus.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
#include <std_msgs/msg/float64.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <unistd.h>
#include <memory>
#include <string>

using std::placeholders::_1;

namespace WithRobot
{
class MyAhrsDriverForROS : public rclcpp::Node, iMyAhrsPlus
{
public:
  MyAhrsDriverForROS(std::string port, int baud_rate);

  ~MyAhrsDriverForROS();

  bool initialize();

  inline void get_data(SensorData & data)
  {
    LockGuard _l(lock_);
    data = sensor_data_;
  }

  inline SensorData get_data()
  {
    LockGuard _l(lock_);
    return sensor_data_;
  }

  void publish_topic(int sensor_id);

  tf2::Quaternion Euler2Quaternion(float roll, float pitch, float yaw);

private:
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_data_raw_pub_,
    imu_data_pub_;
  rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr imu_mag_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr imu_temperature_pub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;

  Platform::Mutex lock_;
  SensorData sensor_data_;
  bool publish_tf_;
  std::string parent_frame_id_;
  std::string frame_id_;
  
  double linear_acceleration_stddev_;
  double angular_velocity_stddev_;
  double magnetic_field_stddev_;
  double orientation_stddev_;

  void OnSensorData(int sensor_id, SensorData data);

  void OnAttributeChange(int sensor_id, std::string attribute_name, std::string value);
};
}  // namespace WithRobot
#endif  // MYAHRS_ROS2_DRIVER__MYAHRS_ROS2_DRIVER_HPP_
