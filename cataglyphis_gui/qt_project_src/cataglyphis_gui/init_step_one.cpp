#include "init_step_one.h"
#include "ui_init_step_one_form.h"

init_step_one::init_step_one(QWidget *parent, boost::shared_ptr<ros_workers> workerArg, bool startup) :
    QWidget(parent),
    ui(new Ui::init_step_one_form)
{
    //if not in init, hide skip init button
    ui->setupUi(this);
    worker = workerArg;
    connect(this, &init_step_one::init_nav_filter,
                worker.get(), &ros_workers::on_run_nav_service);
    connect(worker.get(), &ros_workers::nav_service_returned,
                this, &init_step_one::on_nav_init_return);

    connect(this, &init_step_one::start_nav_info_subscriber,
                worker.get(), &ros_workers::on_run_nav_info_subscriber_start);
    connect(this, &init_step_one::stop_nav_info_subscriber,
                worker.get(), &ros_workers::on_run_nav_info_subscriber_stop);

    connect(this, &init_step_one::start_hsm_pose_subscriber,
                worker.get(), &ros_workers::on_run_hsm_global_pose_subscriber_start);
    connect(this, &init_step_one::stop_hsm_pose_subscriber,
                worker.get(), &ros_workers::on_run_hsm_global_pose_subscriber_stop);

    isThisStartup = startup;
    if(!startup)
    {
        ROS_DEBUG("Not Startup");
        ui->skip_init_button->hide();
        ui->kens_angle_spinbox->hide();
        ui->kens_angle_label->hide();
    }

//    connect(worker.get(), &ros_workers::nav_info_callback,
//                this, &init_step_one::on_nav_info_callback);
    connect(worker.get(), &ros_workers::hsm_global_pose_callback,
                this, &init_step_one::on_hsm_pose_callback);

    //emit start_nav_info_subscriber();
    emit start_hsm_pose_subscriber();
}

init_step_one::~init_step_one()
{
    delete ui;
}

void init_step_one::on_skip_init_button_clicked()
{
    ROS_DEBUG("init_step_one:: skip init clicked");
    messages::NavFilterControl navInitService;
    //put nav skip init flag here and send
    navInitService.request.skipInit = true;
    emit init_nav_filter(navInitService);
    emit procedure_finished();
}

void init_step_one::on_continue_button_clicked()
{
    messages::NavFilterControl navInitService;
    if(isThisStartup)
    {
        navInitService.request.setInitNorthAngle = true;
        ROS_DEBUG("init_step_one:: continue init clicked");
        navInitService.request.initNorthAngle = ui->input_NA_spinbox->value();
        navInitService.request.setKensAngle = true;
        navInitService.request.kensAngle = ui->kens_angle_spinbox->value();
    }
    else
    {
        navInitService.request.setNorthAngle = true;
        navInitService.request.northAngle = ui->input_NA_spinbox->value();
    }
    //navInitService.request.sunnyDay = ui->sunny_day_checkbox->isChecked();
    //navInitService.request.setSunnyDay = true;
    emit init_nav_filter(navInitService);
}

void init_step_one::on_nav_init_return(const messages::NavFilterControl navResponse,
                                            bool sucessful)
{
    if(sucessful)
    {
        ROS_DEBUG("init_step_one:: Nav Init Sucessful");
        emit step_one_finished();
    }
    else
    {
        ROS_WARN("init_step_one:: Nav Init FAILED! skip step to move on");
    }

}

void init_step_one::on_nav_info_callback(const messages::NavFilterOut navInfo)
{
    ROS_DEBUG("init_step_one:: nav_info_callback");
    ui->current_NA_spinbox->setValue(navInfo.north_angle);
}

void init_step_one::on_hsm_pose_callback(const messages::RobotPose robotPose)
{
    ROS_DEBUG("init_step_one:: hsm_pose_callback");
    ui->current_NA_spinbox->setValue(robotPose.northAngle);
}
