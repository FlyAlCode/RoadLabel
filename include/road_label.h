#ifndef ROAD_LABEL_H_
#define ROAD_LABEL_H_
#include <opencv2/core/core.hpp>


// 道路标注的类：首先通过交互的界面选择所有的道路，保存到road_lines_中；


class RoadLabelor{
public:
    struct RoadLine  {
        std::vector<cv::Point2d> polygon_;
        double line_width_;
    };    

    RoadLabelor(int max_img_size, double default_line_width);
    void Run(const cv::Mat &input_img, cv::Mat &mask, std::vector<RoadLine> &labels);
    

private:
    // mouse callback
    static void OnMouseCallback(int event, int x, int y, int flags, void *userdata);

    cv::Mat src_;

    // show
    cv::Mat show_img_;
    cv::Mat current_show_img_;
    double scale_;
    int max_img_size_;                                      // 最大能显示的图片尺寸，当图片的长/宽大于该值时，需要对图像resize

    // road
    std::vector<RoadLine> road_lines_;

    // draw
    double line_width_;                                     // 这里表示的是显示的线宽，不是真实标注的线宽，考虑的尺度因子，这里选择使用double
    double line_width_step_;                                // 每次中轮滚动线宽调整的值

    // mouse state
    static cv::Point current_pt_;
    static std::vector<cv::Point> current_select_polygon_;

    // flag
    static bool is_start_set_;
    static bool new_road_ready_;                                   // 是否已经选择了一条新的标注道路
    
};


#endif