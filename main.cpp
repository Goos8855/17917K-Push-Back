#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/widgets/lv_btn.h"
#include "liblvgl/widgets/lv_label.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "liblvgl/lvgl.h"
#include "pros/screen.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup frontLeft({7,8});
pros::MotorGroup frontRight({-19,-20});
pros::MotorGroup backLeft({9,10});
pros::MotorGroup backRight({-17,-18});
pros::MotorGroup Intake({11});

//initialize home gui
void init_gui(){
	lv_obj_t* autoBtn = lv_btn_create(lv_scr_act());
  lv_obj_align(autoBtn, NULL, 10, 5);
  
  lv_obj_t* autoLabel = lv_label_create(autoBtn);
  lv_label_set_text(autoLabel, "Auto Controls");
}

void initialize() {
	lvgl_init();
	init_gui();
}

void disabled() {}


void competition_initialize() {}


void autonomous() {}

void opcontrol() {
  while (true) {
    int Y = master.get_analog(ANALOG_LEFT_Y);
    int X = master.get_analog(ANALOG_LEFT_X);
    int R = master.get_analog(ANALOG_RIGHT_X);

    frontLeft.move(Y + X + R);
    frontRight.move(Y - X - R);
    backLeft.move(Y - X + R);
    backRight.move(Y + X - R);

    Intake.move(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));


    pros::delay(10);
  }
}
