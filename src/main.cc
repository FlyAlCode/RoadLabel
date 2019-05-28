#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include "road_label.h"


int main(){
    RoadLabelor road_labelor(1200, 7);
    cv::Mat mask;
    std::vector<RoadLabelor::RoadLine> labels;
    std::string img_name;
    cv::Mat input_img;
    while (true)    {
        std::cout<<"Please enter the image to label:"<<std::endl;
        std::cin>>img_name;
        input_img = cv::imread(img_name);
        if(input_img.empty()){
            std::cout<<"Cannot read image: "<<img_name<<", please check!!!"<<std::endl;
            continue;
        }

        road_labelor.Run(input_img, mask, labels);
        int point_pos = img_name.find_last_of('.');
        std::string save_img_name = img_name.substr(0,point_pos) + "_mask" + ".png";
        cv::imwrite(save_img_name, mask);
    }
    
    


    return 0;
}