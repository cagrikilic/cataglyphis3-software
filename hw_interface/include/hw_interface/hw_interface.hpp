

#ifndef HW_INTERFACE_HPP__
#define HW_INTERFACE_HPP__


#include <ros/ros.h>
#include <hw_interface/base_interface.hpp>
#include <pluginlib/class_loader.h>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

namespace interface_worker
{
    void worker(boost::shared_ptr<boost::asio::io_service> ioService);
};

class hw_interface {
    //friend class interface_worker;

private:
    boost::shared_ptr<boost::asio::io_service> interfaceService;
    boost::shared_ptr<boost::asio::io_service::work> interfaceWork;

    std::vector<boost::shared_ptr<base_classes::base_interface> > interfacePluginVector;

    void initInterfacePlugins();
    void initThreadPool();

public:
    boost::thread_group interfaceWorkerGroup;

    hw_interface();
    hw_interface(int maxNumOfThreads);

    void addInterfacePlugins();  //TODO implement

    bool startInterfaces();
    bool stopInterfaces();      //once node has stopped, reset interfaceWork ptr.




};


#endif //HW_INTERFACE_HPP__
