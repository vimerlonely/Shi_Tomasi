#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;

int maxCorners = 9;

class point_gray
{
    private:
        Point location;
        float p_gray;

    public:
        point_gray() {}

        point_gray(Point& corner, float gray)
        {
            location = corner;
            p_gray = gray;
        }

        void out_put()
        {
            cout << location.x << "   " << location.y << "   " << p_gray << endl;
        }

        point_gray& operator=(const point_gray& pd)
        {
            if(&pd != this)
            {
                location = pd.location;
                p_gray = pd.p_gray;
            }

            return *this;
        }

        friend int cmp_gray(point_gray& p1, point_gray& p2);

};

int cmp_gray(point_gray& p1, point_gray& p2)
{
    if(p1.p_gray > p2.p_gray)
        return 1;
    else
        return 0;
}

class Shi_Tomasi
{
    private:
        vector<Point> corners;
        vector<point_gray> corners_gray;
        double qualityLevel;
        double minDistance;
        int blockSize;
        bool useHarrisDetector;
        double k;


    public:
        Shi_Tomasi(): qualityLevel(0.01), minDistance(4), blockSize(3), useHarrisDetector(false), k(0.04) {
        };

        void detect(const cv::Mat &image) {
            // opencv自带得角点响应函数计算函数
            cv::goodFeaturesToTrack(image, corners, maxCorners,
                    qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
        }

        // 用圈圈标记角点
        void drawOnImage(cv::Mat &image,
                int radius = 8.0, int thickness = 4.5) {
            cout << corners.size() << endl;
            int j = 1;
            for(size_t i = 0; i < corners.size(); i++, j++)
            {
                cv::Scalar color = cv::Scalar(25*j, 25*j, 25*j);
                cv::circle(image, corners[i], radius, color, thickness);
            }
        }

        // 计算灰度信息
        void gray_cal(IplImage* &src)
        {
            for(size_t i = 0; i < corners.size(); i++)
            {
                uchar* ptr = (uchar *)(src->imageData + corners[i].y * src->width);
                point_gray test = point_gray(corners[i],
                        ptr[corners[i].x]);
                corners_gray.push_back(test);
            }
        }

        void out_put()
        {
            cout << "  x  " << "  y  " << "  gray" << endl;
            for(size_t i = 0; i < corners.size(); i++) {
                corners_gray[i].out_put();
            }
        }

        void sort()
        {
            point_gray test;
            for(size_t i = 0; i < corners_gray.size(); i++)
            {
                for(size_t j = i+1; j < corners_gray.size(); j++)
                {
                    if(cmp_gray(corners_gray[i], corners_gray[j]))
                    {
                        test = corners_gray[i];
                        corners_gray[i] = corners_gray[j];
                        corners_gray[j] = test;
                    }
                }
            }

        }
};

int main(int argc, char** argv)
{
    Mat image1_gray;
    Mat image2_gray;
    IplImage* src1;
    IplImage* src2;

    if(argv[1] == NULL || argv[2] == NULL) {
        cout << "Use as \"harris_test test_picture1 test_picture2\"" << endl;
        return -1;
    }
    else {
        Mat image1 = imread(argv[1], 1);
        src1 = cvLoadImage(argv[1], 0);
        Mat image2 = imread(argv[2], 1);
        src2 = cvLoadImage(argv[2], 0);

        // 灰度变换
        cvtColor(image1, image1_gray, COLOR_BGR2GRAY);
        cvtColor(image2, image2_gray, COLOR_BGR2GRAY);

        // 经典得harris角点方法
        Shi_Tomasi Shi_Tomasi1;
        Shi_Tomasi Shi_Tomasi2;

        // 计算角点
        Shi_Tomasi1.detect(image1_gray);
        Shi_Tomasi2.detect(image2_gray);

        Shi_Tomasi1.gray_cal(src1);
        Shi_Tomasi2.gray_cal(src2);

        Shi_Tomasi1.sort();
        Shi_Tomasi2.sort();

        cout << "eye_left.png" << endl;
        Shi_Tomasi1.out_put();
        cout << "eye_right.png" << endl;
        Shi_Tomasi2.out_put();

        // 标记角点
        Shi_Tomasi1.drawOnImage(image1);
        Shi_Tomasi2.drawOnImage(image2);

        cv::imshow("eye_left.png", image1);
        cv::imshow("eye_right.png", image2);

        cv::waitKey(0);

        return 0;
    }
}
