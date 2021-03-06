#include "core_app.h"
#include "ui_core_app_form.h"

core_app::core_app(QWidget *parent, boost::shared_ptr<ros::NodeHandle> nh) :
    QMainWindow(parent),
    ui(new Ui::core_app_form)
{
    ROS_DEBUG("Cataglyphis_GUI:: Core app init");
    ui->setupUi(this);

    guiNhPtr = nh;
    ROS_DEBUG("Cataglypis_GUI:: Starting %d callback theads", NUM_MSG_CALLBACK_THREADS);
    asyncSpinnerPtr = boost::shared_ptr<ros::AsyncSpinner>(new ros::AsyncSpinner(NUM_MSG_CALLBACK_THREADS));
    if(asyncSpinnerPtr->canStart())
    {
        asyncSpinnerPtr->start();
    }
    else
    {
        ROS_INFO("Cataglypis_GUI:: Cannot start callback threads. Is another GUI instance running?");
    }

    if(!nh->ok())
    {
        ROS_ERROR("Node Invalid, Cannot connect to ROS Master");
    }

    //set title bar to a recongizable name
    //sharedMenuBar = boost::shared_ptr<QMenuBar>(new QMenuBar(0));
#ifdef STATIC_BUILD
    setWindowTitle(tr("Cataglyphis GUI - STATIC RELEASE"));
#elif TEST_RELEASE_BUILD
    setWindowTitle(tr("Cataglyphis GUI - TEST_RELEASE"));
#elif DEBUG_BUILD
    setWindowTitle(tr("Cataglyphis GUI - DEBUG"));
#else
    setWindowTitle(tr("Cataglyphis GUI - UNKOWN_BUILD"));
#endif


    cataglyphisStartupFormPtr.reset(new init_container(ui->guiTabber));
    mapViewFormPtr.reset(new map_viewer(ui->guiTabber, 1));
    manualControlFormPtr.reset(new manual_control(ui->guiTabber));
    execInfoFormPtr.reset(new exec_info_queue(ui->guiTabber));
    missionPlanningInfoFormPtr.reset(new mission_planning(ui->guiTabber));

    ui->guiTabber->addTab(cataglyphisStartupFormPtr.get(), "Init/Recovery");
    ui->guiTabber->addTab(mapViewFormPtr.get(), "Map");
    ui->guiTabber->addTab(manualControlFormPtr.get(), "Manual Control");
    ui->guiTabber->addTab(execInfoFormPtr.get(), "Exec Queue");
    ui->guiTabber->addTab(missionPlanningInfoFormPtr.get(), "Mission Planning");

    rosWorker = boost::shared_ptr<ros_workers>(new ros_workers());

    connect(rosWorker.get(), &ros_workers::hsm_global_pose_callback,
                this, &core_app::on_hsm_global_pose_callback);
    connect(missionPlanningInfoFormPtr.get(), &mission_planning::update_mission_timer,
                this, &core_app::on_update_time);

    rosWorker->moveToThread(&rosWorkerThread);
    rosWorker->on_run_hsm_global_pose_subscriber_start();
    rosWorkerThread.start();

}

core_app::~core_app()
{
    asyncSpinnerPtr->stop();
    rosWorkerThread.quit();
    rosWorkerThread.wait();
    delete ui;
    //ui.reset();
}


void core_app::on_hsm_global_pose_callback(const messages::RobotPose hsmRobotPose)
{
    ROS_DEBUG("Core:: HSM Callback %2.6f", QVector2D(lastRobotPose.x, lastRobotPose.y).distanceToPoint(QVector2D(hsmRobotPose.x,hsmRobotPose.y)));
    ui->distance_travelled_spinbox->setValue(ui->distance_travelled_spinbox->value()+
                                                QVector2D(lastRobotPose.x, lastRobotPose.y).distanceToPoint(QVector2D(hsmRobotPose.x,hsmRobotPose.y)));
    ui->x_spinbox->setValue(hsmRobotPose.x);
    ui->y_spinbox->setValue(hsmRobotPose.y);
    ui->heading_spinbox->setValue(hsmRobotPose.humanHeading);
    lastRobotPose = hsmRobotPose;
}

void core_app::on_update_time(double time)
{
    int hours = 0;
    int min = 0;
    int seconds = 0;

    if(time > 3600)
    {
        hours = (int)(time / 3600);
        time-=hours*3600;
    }
    if(time > 60)
    {
        min = (int)(time/60);
        time-=min*60;
    }
    if(time>0)
    {
        seconds = (int)time;
    }

    QTime t(hours, min, seconds);
    ui->mission_time->setTime(t);
}

void core_app::on_reset_distance_travelled_button_clicked()
{
    ui->distance_travelled_spinbox->setValue(0);
}
