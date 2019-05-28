#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "road_label.h"

// debug
#include <iostream>

/******************************************
 *                  public                *
 * ****************************************/
// initial static class number
// double RoadLabelor::line_width_ = 5;
cv::Point RoadLabelor::current_pt_ = cv::Point(0,0);
std::vector<cv::Point> RoadLabelor::current_select_polygon_ = std::vector<cv::Point>() ;
// double RoadLabelor::line_width_step_ = 0.5;                                // 每次中轮滚动线宽调整的值
bool RoadLabelor::is_start_set_ = false;
bool RoadLabelor::new_road_ready_ = false;  

RoadLabelor::RoadLabelor(int max_img_size, double default_line_width){
    max_img_size_ = max_img_size;
    line_width_ = default_line_width;
    scale_ = 1;
    line_width_step_ = 1;                         // set manual
    is_start_set_ = false;
}


void RoadLabelor::Run(const cv::Mat &input_img, cv::Mat &mask, std::vector<RoadLine> &labels){
    // clear state
    current_select_polygon_.clear();
    road_lines_.clear();
    is_start_set_ = false;
    new_road_ready_ = false;

    input_img.copyTo(src_);
    
    // set show image
    if(input_img.rows>max_img_size_ || input_img.cols>max_img_size_){
        double max_length;
        if(input_img.rows>input_img.cols)
            max_length = input_img.rows;
        else 
            max_length = input_img.cols;
        scale_ = max_img_size_ / max_length;

        cv::resize(src_, show_img_, cv::Size(0,0), scale_, scale_);
    }
    else{
        scale_ = 1;
        src_.copyTo(show_img_);
    }

    cv::namedWindow("road label", cv::WINDOW_AUTOSIZE | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_NORMAL);
    cv::setMouseCallback("road label", RoadLabelor::OnMouseCallback);

    cv::imshow("road label", show_img_);
    char current_key = cv::waitKey(10);
    while(current_key!='q'){
        // deal with key
        if(current_key == 'w')
            line_width_ += line_width_step_;
        if(current_key == 's')
            line_width_ -= line_width_step_;
        if(line_width_<1)
            line_width_ = 1;

        
        if(new_road_ready_){
            // save label
            RoadLine tmp;
            tmp.line_width_ = scale_ * line_width_;
            for(int i=0; i<current_select_polygon_.size(); i++){
                tmp.polygon_.push_back(scale_ * cv::Point2d(current_select_polygon_[i].x, current_select_polygon_[i].y));
            }
            road_lines_.push_back(tmp);
            // draw now polygon in show_img
            for(int i=0; i<current_select_polygon_.size()-1; i++){
                cv::line(show_img_, current_select_polygon_[i], current_select_polygon_[i+1], cv::Scalar(0,0,255), line_width_);
            }
            new_road_ready_ = false;
        }

        if(is_start_set_){
            show_img_.copyTo(current_show_img_);
            for(int i=0; i<current_select_polygon_.size()-1; i++){
                cv::line(current_show_img_, current_select_polygon_[i], current_select_polygon_[i+1], cv::Scalar(0,255,0), line_width_);
            }
            cv::line(current_show_img_, current_select_polygon_[current_select_polygon_.size()-1], current_pt_, cv::Scalar(0,255,0), line_width_);
            cv::imshow("road label", current_show_img_);
        }   
        else{
            cv::imshow("road label", show_img_);
        }
        current_key = cv::waitKey(10);
    }

    // draw mask
    mask = cv::Mat::zeros(input_img.rows, input_img.cols, CV_8UC1);
    for(int i=0; i<road_lines_.size(); i++){
        for(int j=0; j<road_lines_[i].polygon_.size()-1; j++)
            cv::line(mask, road_lines_[i].polygon_[j], road_lines_[i].polygon_[j+1], cv::Scalar(255), road_lines_[i].line_width_);
    }
    labels = road_lines_;

    cv::destroyWindow("road label");

}
    


/******************************************
 *                  private                *
 * ****************************************/
void RoadLabelor::OnMouseCallback(int event, int x, int y, int flags, void *userdata){
    switch (event)
    {
    case cv::EVENT_MOUSEMOVE:
        current_pt_.x = x;
        current_pt_.y = y;
        break;

    case cv::EVENT_LBUTTONDOWN:
        if(!is_start_set_){
            is_start_set_ = true;
            current_select_polygon_.clear();
        }
        current_select_polygon_.push_back(cv::Point(x,y));
        // debug
        // std::cout<<"Add point: ["<<x<<", "<<y<<"]"<<std::endl;
        break;

    case cv::EVENT_MBUTTONDOWN:
        if(is_start_set_){
            new_road_ready_ = true;
            is_start_set_ = false;
        }
        break;

    case cv::EVENT_RBUTTONDOWN:
        if(is_start_set_){
            current_select_polygon_.pop_back();
        }
        if(current_select_polygon_.empty())
            is_start_set_ = false;
        break;

    // case cv::EVENT_RBUTTONDBLCLK:
    //     if(is_start_set_){
    //         current_select_polygon_.clear();
    //         is_start_set_ = false;
    //     }
    //     break;

    // case cv::EVENT_MOUSEWHEEL:                               // 由于opencv的中键滚动已经有了响应函数，且无法被重载，因此这里只能放弃使用滚轮进行线宽的调整
    //     if(is_start_set_){
    //         // debug
    //         // std::cout<<"getMouseWheelDelta(flags) = "<<cv::getMouseWheelDelta(flags)<<std::endl;
    //         line_width_ += line_width_step_ * cv::getMouseWheelDelta(flags)/120;
    //         if(line_width_<1)
    //             line_width_ = 1;
    //     }
    //     break;

    default:
        break;
    }
}
