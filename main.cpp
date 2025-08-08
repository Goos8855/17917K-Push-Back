#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lemlib/pose.hpp"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/widgets/lv_btn.h"
#include "liblvgl/widgets/lv_label.h"
#include "pros/abstract_motor.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "liblvgl/lvgl.h"
#include "pros/motors.h"
#include "pros/rotation.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cstddef>
#include <cstdio>
#include <string>
#include "liblvgl/lvgl.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup leftDT({8,-9,-10}, pros::MotorGearset::blue); 
pros::MotorGroup rightDT({-18,19,20}, pros::MotorGearset::blue);
pros::Imu IMU(7);
pros::Rotation yOdom(17);

lemlib::Drivetrain drivetrain(
    &leftDT,
    &rightDT,
    12,
    lemlib::Omniwheel::NEW_325,
    360,
    2
);

lemlib::TrackingWheel verticalTrackingWheel(&yOdom,  lemlib::Omniwheel::NEW_2,0);

lemlib::OdomSensors sensors(
    &verticalTrackingWheel,
    nullptr,
    nullptr,
    nullptr,
    &IMU
);

lemlib::ControllerSettings lateralController(
    10,
    0,
    3,
    3,
    1,
    100,
    3,
    500,
    20
);

lemlib::ControllerSettings angularController{
    2,
    0,
    10,
    3,
    1,
    100,
    3,
    500,
    0
};

lemlib::Chassis chassis(
    drivetrain,
    lateralController,
    angularController,
    sensors
);

lv_obj_t* posLabel;

//initialize home gui
void init_gui(){
//	lv_obj_t* autoBtn = lv_btn_create(lv_scr_act());
//  lv_obj_align(autoBtn, NULL, 10, 5);
  
//  lv_obj_t* autoLabel = lv_label_create(autoBtn);
//  lv_label_set_text(autoLabel, "Auto Controls");

    posLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(posLabel, "X:0.00, Y:0.00, T:0.00");
    lv_obj_align(posLabel, NULL, 10, 10);
}

void updatePose(void*){
    while(true){
        lemlib::Pose pose = chassis.getPose();

        char buf[50];
        sprintf(buf, "X: %.2f, Y: %.2f, T: %.2f", pose.x, pose.y, pose.theta);

        lv_label_set_text(posLabel, buf);
        pros::delay(50);
    }
}

void initialize() {
	lvgl_init();
	init_gui();

    chassis.calibrate();

    pros::Task  ui_update(updatePose, nullptr, "UI Update Task");

}

void disabled() {}


void competition_initialize() {}


void autonomous() {}

void opcontrol() {
    
pros::Controller controller(pros::E_CONTROLLER_MASTER);

  while (true) {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)*-1;
    int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    chassis.curvature(leftY, leftX);

    pros::delay(10);
  }
}
