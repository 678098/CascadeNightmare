#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <algorithm>

std::string cascadePath = "lbpcascade_profileface.xml";
cv::Size minSize(20, 34);
cv::Size maxSize(200, 340);
const int imw = 500;
const int imh = 500;

class Creature
{
public:
    Creature(){}
    Creature(const Creature &c1, const Creature &c2)
    {
        image = c1.image.clone();
        int copySize = 20 + rand() % (std::min(imw, imh)/2);
        int x1 = rand() % (imw - copySize);
        int y1 = rand() % (imh - copySize);
        int x2 = rand() % (imw - copySize);
        int y2 = rand() % (imh - copySize);
        c2.image(cv::Rect(x2, y2, copySize, copySize)).copyTo(image(cv::Rect(x1, y1, copySize, copySize)));
        for (int i = rand() % 6; i >= 0; i--)
        {
           int x = rand() % (imw - 20);
           int y = rand() % (imh - 20);
           cv::Mat noise = cv::Mat::zeros(cv::Size(20, 20), CV_8UC1);
           cv::randn(noise, 127, 10 + rand() % 60);
           noise.copyTo(image(cv::Rect(x, y, 20, 20)));
        }
        for (int i = rand() % 3; i >= 0; i--)
        {
           cv::Rect roi(rand() % imw, rand() % imh, 5 + rand() % 30, 5 + rand() % 30);
           cv::rectangle(image, roi, cv::Scalar(rand() % 256), CV_FILLED);
        }
        for (int i = rand() % 3; i >= 0; i--)
        {
           cv::Point center(rand() % imw, rand() % imh);
           cv::circle(image, center, 2 + rand() % 15, cv::Scalar(rand() % 256), CV_FILLED);
        }
    }

    cv::Mat image;
    int counter = -1;
};

class Generation
{
public:
    Generation(int imagesNumber)
    {
        for (int i = 0; i < imagesNumber; i++)
        {
            Creature c;
            c.image = cv::Mat::zeros(cv::Size(imw, imh), CV_8UC1);
            cv::randn(c.image, 127, 65);
            creatures.push_back(c);
        }
    }

    Generation(const Generation &previous)
    {
        iteration = previous.iteration + 1;
        creatures.push_back(previous.creatures[0]);
        creatures.push_back(previous.creatures[1]);
        int num = previous.creatures.size();
        for (int i = 2; i < num / 2; i++)
        {
            creatures.push_back(Creature(creatures[rand() % 2], previous.creatures[rand() % num]));
        }
        while (creatures.size() < num)
        {
            creatures.push_back(Creature(previous.creatures[rand() % num], previous.creatures[rand() % num]));
        }
    }

    void applyCascade(cv::CascadeClassifier &cascade)
    {
        for (auto &c : creatures)
        {
            if (c.counter >= 0)
                continue;
            std::vector<cv::Rect> objs;
            cascade.detectMultiScale(c.image, objs, 1.1, 0, 0, minSize, maxSize);
            c.counter = objs.size();
        }
        std::sort(creatures.begin(), creatures.end(),
                  [](const Creature &c1, const Creature &c2)
                  { return c1.counter > c2.counter; });
    }
    
    std::vector<Creature> creatures;
    int iteration = 0;
};

class Evolution
{
public:
    Evolution() : current(20)
    {
        cascade.load(cascadePath);
    }

    void run()
    {
        int globalBestNum = -1;
        int savedImages = 0;
        while (true){
            current.applyCascade(cascade);

            int bestNum = current.creatures.front().counter;
            int worstNum = current.creatures.back().counter;
            std::cout << bestNum << " " << worstNum << std::endl;

            if (bestNum > globalBestNum)
            {
                globalBestNum = bestNum;
                cv::imwrite("_img" + std::to_string(savedImages++) + ".png", current.creatures[0].image);
            }

            current = Generation(current);
        }
    }

    cv::CascadeClassifier cascade;
    Generation current;
};

int main()
{
    Evolution evol;
    evol.run();
    return 0;
}