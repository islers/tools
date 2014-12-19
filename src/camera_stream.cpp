#include <ros/ros.h>
#include <ros/console.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sstream>

using namespace std;

ros::Time lastSnapshotTime;
int myit = 0;

void cvdisplay( const sensor_msgs::ImageConstPtr& _toDisplay )
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(_toDisplay, sensor_msgs::image_encodings::BGR8);
    }
    catch( cv_bridge::Exception& e )
    {
      ROS_ERROR("cv_bridge exception: %s",e.what() );
      return;
    }
    
    cv::Mat theImage = cv_ptr->image;
    
    
    cv::imshow("Test screen",theImage);
    
    ros::Time current = ros::Time::now();
    
    if( ( current.sec - lastSnapshotTime.sec ) > 3 )
    { //store image
      std::stringstream conv;std::string it;
      conv<<myit++; conv>>it;
      std::vector<int> params;
      params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      params.push_back(0);
      //cout<<endl<<"writing image"<<endl;
      //cv::imwrite("/home/stewss/semester_project/calibration_images/shot"+it+".png",theImage, params );
      lastSnapshotTime = current;
    }
    cv::waitKey(1);
    return;
}

int main( int argc, char **argv )
{
    ros::init( argc, argv, "camera_stream");
    ros::NodeHandle n;
    
    
    ros::Subscriber sub = n.subscribe("/camera/image_raw", 20, &cvdisplay);
    
    ros::spin();
    
    return 0;
}
