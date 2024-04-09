#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1;
    int thickness = 2;

    VideoCapture cap("C:/Users/Nuta/Documents/Open CV/Practice 26.03/video.mp4");

    if (!cap.isOpened())
    {
        cout << "Error" << endl;
        return -1;
    }

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("output_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));

    if (!video.isOpened())
    {
        cout << "Ошибка при создании видеофайла" << endl;
        return -1;
    }

    Mat frame, gray, gauss, contur;

    while (true)
    {
        cap.read(frame);

        if (frame.empty())
            break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gauss, Size(5, 5), 0);
        Canny(gauss, contur, 50, 200);

        vector<vector<Point>> contours;
        findContours(contur, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);

            if (area > 300)
            {
                Rect rect = boundingRect(contours[i]);
                rectangle(frame, rect, Scalar(0, 255, 0), 2);

                Point textOrg(rect.x, rect.y - 10);
                putText(frame, "rectangle", textOrg, fontFace, fontScale, Scalar(0, 255, 0), thickness);
            }
        }
        imshow("Video", frame);
        video.write(frame);

        if (waitKey(25) == 27)
            break;
    }

    cap.release();
    video.release();
    destroyAllWindows();

    return 0;
}

