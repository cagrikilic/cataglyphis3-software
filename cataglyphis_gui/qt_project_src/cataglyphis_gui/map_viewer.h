#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QWidget>
#include <QPixmapCache>
#include <map_viewer_enums.h>
#include <qgraphicsscenemapviewer.h>
#include <map_view_roi_ellipse.h>
#include <generic_error_dialog.h>
#include <roi_dialog.h>
#include <boost/smart_ptr.hpp>
#include <stdio.h>
#include <ros/ros.h>
#include <ros_workers.h>

#define NUM_OF_MAPS 4

namespace Ui {
class map_viewer_form;
}

class map_viewer : public QWidget
{
    Q_OBJECT

signals:
    void request_map_manager_ROI();
    void run_global_map_request(map_viewer_enums::mapViewerLayers_t requestedLayer);
    void set_map_layer_visibility(map_viewer_enums::mapViewerLayers_t mapLayer, bool visibility);
    void set_scene_setup_alert(bool status);
    void center_on_cataglyphis(bool status);
    void new_path_length(int length);
    void new_path_step_size(double stepSize);
    void confirm_map_changes();
    void discard_map_changes();

public slots:
    void on_global_map_service_returned(messages::GlobalMapFull gridMapFull, map_viewer_enums::mapViewerLayers_t requestedLayer,
                                        bool wasSucessful);

public:
    explicit map_viewer(QWidget *parent = 0, int startIndex = 0);
    ~map_viewer();

    //boost::shared_ptr<Ui::map_viewer> ui;
    Ui::map_viewer_form* ui;

private slots:
    void on_fieldSelector_currentIndexChanged(int index);

//    void on_drawTestShapesButton_clicked();

    void on_keyframeMapLayerButton_clicked(bool checked);

    void on_roiMapLayerButton_clicked(bool checked);

    void on_pathLayerButton_clicked(bool checked);

    void on_gridMapLayerButton_clicked(bool checked);

    void on_initMapButton_clicked();

    void on_satDriveMapLayerButton_clicked(bool checked);

    void on_refresh_layers_button_clicked();

    void on_commit_changes_button_clicked();

    void on_discard_changes_button_clicked();

    void on_center_on_cataglyphis_clicked(bool checked);

    void on_path_length_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    QThread rosWorkerThread;
    boost::shared_ptr<ros_workers> rosWorker;
    boost::shared_ptr<ros::NodeHandle> nh;

    QPointF startPlatformPositionsPixels[10][3];

    const QColor defaultCircleFill;
    const QColor fullTransparentColor;
    boost::shared_array<boost::scoped_ptr<QGraphicsSceneMapViewer> > scene;
};

#endif // MAP_VIEWER_H
