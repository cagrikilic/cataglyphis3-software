#include "init_step_one.h"
#include "ui_init_step_one.h"

init_step_one::init_step_one(QWidget *parent, boost::shared_ptr<ros_workers> workerArg) :
    QWidget(parent),
    ui(new Ui::init_step_one)
{
    ui->setupUi(this);
    worker = workerArg;
    connect(this, &init_step_one::init_nav_filter,
                worker.get(), &ros_workers::run_nav_init_service);
    connect(worker.get(), &ros_workers::nav_init_returned,
                this, &init_step_one::when_nav_init_return);
}

init_step_one::~init_step_one()
{
    //delete ui;
}

void init_step_one::on_skip_init_button_clicked()
{
    ROS_DEBUG("init_step_one:: skip init clicked");
    emit step_one_finished();
}

void init_step_one::on_continue_button_clicked()
{
    ROS_DEBUG("init_step_one:: continue init clicked");
    navInitService.request.northAngle = ui->input_NA_spinbox->value();
    navInitService.request.setNorthAngle = true;
    navInitService.request.sunnyDay = ui->sunny_day_checkbox->isChecked();
    navInitService.request.setSunnyDay = true;
    emit init_nav_filter(navInitService);
}

void init_step_one::when_nav_init_return(const messages::NavFilterControl navResponse,
                                            bool sucessful)
{
    if(sucessful)
    {
        ROS_DEBUG("init_step_one:: Nav Init Sucessful");
        emit step_one_finished();
    }
    ROS_WARN("init_step_one:: Nav Init FAILED! skip step to move on");
}