#include "CameraDistortion.hpp"
#include <iostream>

namespace image_helper {

cv::Mat distortImageRadialAndTangential(const cv::Mat& undistortedImage, const float radialDistortionParams[4],
    const float tangentialDistortionParams[2], const float skew, const float scale)
{
    CV_Assert(undistortedImage.type() == CV_8UC3);

    auto imageWidth = undistortedImage.cols;
    auto imageHeight = undistortedImage.rows;

    double fx = imageWidth;
    double fy = imageHeight;

    double centerX = imageWidth / 2.0;
    double centerY = imageHeight / 2.0;

    cv::Mat outputImage(imageHeight * scale, imageWidth * scale, undistortedImage.type());

    // Code to set original image region to white
    /* cv::Mat originalImageRegion(outputImage, cv::Rect(centerX * scale - centerX, centerY * scale - centerY,
     * imageWidth, imageHeight)); */
    /* originalImageRegion.setTo(cv::Scalar(255, 255, 255)); */

    for (auto y = 0; y < imageHeight; y++) {
        for (auto x = 0; x < imageWidth; x++) {
            /* auto normX = x - centerX; */
            /* auto normY = y - centerY; */
            auto normX = (x - centerX) / fx;
            auto normY = (y - centerY) / fy;

            double r2 = std::pow(normX, 2) + std::pow(normY, 2);
            double r4 = r2 * r2;
            double r6 = r2 * r4;
            double r8 = r4 * r4;

            auto radialDistortionTerm = 1.0 + radialDistortionParams[0] * r2 + radialDistortionParams[1] * r4
                + radialDistortionParams[2] * r6 + radialDistortionParams[3] * r8;

            auto tangentialDistortionTermX = 2 * tangentialDistortionParams[0] * normX * normY
                + tangentialDistortionParams[1] * (r2 + 2 * normX * normX);
            auto tangentialDistortionTermY = tangentialDistortionParams[0] * (r2 + 2 * normY * normY)
                + 2 * tangentialDistortionParams[1] * normX * normY;

            /* int xNew = fx * normX * (radialDistortionTerm + tangentialDistortionTermX) + centerX * scale; */
            /* int yNew = fy * normY * (radialDistortionTerm + tangentialDistortionTermY) + centerY * scale; */
            auto xDistNorm = normX * radialDistortionTerm + tangentialDistortionTermX;
            auto yDistNorm = normY * radialDistortionTerm + tangentialDistortionTermY;
            int xNew = (xDistNorm + skew * yDistNorm) * fx + centerX * scale;
            int yNew = yDistNorm * fy + centerY * scale;

            /* std::cout << xNew << std::endl << yNew << std::endl; */

            if (xNew >= 0 && xNew < imageWidth * scale && yNew >= 0 && yNew < imageHeight * scale) {
                /* std::cout << "new x, y: " << xNew << " " << yNew << std::endl; */
                /* std::cout << "x, y: " << x << " " << y << std::endl; */
                /* std::cout << outputImage.at<cv::Vec3b>(xNew, yNew) << std::endl; */
                /* std::cout << undistortedImage.at<cv::Vec3b>(x, y) << std::endl; */
                outputImage.at<cv::Vec3b>(yNew, xNew) = undistortedImage.at<cv::Vec3b>(y, x);
            }
        }
    }

    return outputImage;
}

cv::Mat distortImageAngular(
    const cv::Mat& undistortedImage, const float distortionParams[4], const float skew, const float scale)
{
    CV_Assert(undistortedImage.type() == CV_8UC3);
    auto imageWidth = undistortedImage.cols;
    auto imageHeight = undistortedImage.rows;

    double fx = imageWidth;
    double fy = imageHeight;

    double centerX = imageWidth / 2.0;
    double centerY = imageHeight / 2.0;

    cv::Mat outputImage(imageHeight * scale, imageWidth * scale, undistortedImage.type());

    // Code to set original image region to white
    /* cv::Mat originalImageRegion(outputImage, cv::Rect(centerX * scale - centerX, centerY * scale - centerY,
     * imageWidth, imageHeight)); */
    /* originalImageRegion.setTo(cv::Scalar(255, 255, 255)); */

    for (auto y = 0; y < imageHeight; y++) {
        for (auto x = 0; x < imageWidth; x++) {
            auto normX = (x - centerX) / fx;
            auto normY = (y - centerY) / fy;

            double radius = std::sqrt(std::pow(normX, 2) + std::pow(normY, 2));

            double theta = std::atan(radius);
            auto theta2 = theta * theta;
            auto theta3 = theta * theta2;
            auto theta5 = theta3 * theta2;
            auto theta7 = theta5 * theta2;
            auto theta9 = theta7 * theta2;

            auto rtheta = theta + distortionParams[0] * theta3 + distortionParams[1] * theta5
                + distortionParams[2] * theta7 + distortionParams[3] * theta9;

            /* auto psi = std::atan(normY/normX); */
            /* auto xDistNorm = rtheta * std::cos(psi); */
            /* auto yDistNorm = rtheta * std::sin(psi); */
            if (radius < 1e-8) {
                radius = 1.0;
            }
            auto xDistNorm = rtheta * normX / radius;
            auto yDistNorm = rtheta * normY / radius;

            int xNew = (xDistNorm + skew * yDistNorm) * fx + centerX * scale;
            int yNew = yDistNorm * fy + centerY * scale;

            if (xNew >= 0 && xNew < imageWidth * scale && yNew >= 0 && yNew < imageHeight * scale) {
                outputImage.at<cv::Vec3b>(yNew, xNew) = undistortedImage.at<cv::Vec3b>(y, x);
            }
        }
    }
    return outputImage;
}

cv::Mat distortImageOmniMeiRives(const cv::Mat& undistortedImage, const float radialDistortionParams[2],
    const float tangentialDistortionParams[2], const float xi, const float skew, const float scale)
{
    CV_Assert(undistortedImage.type() == CV_8UC3);

    auto imageWidth = undistortedImage.cols;
    auto imageHeight = undistortedImage.rows;

    double fx = imageWidth;
    double fy = imageHeight;

    double centerX = imageWidth / 2.0;
    double centerY = imageHeight / 2.0;

    cv::Mat outputImage(imageHeight * scale, imageWidth * scale, undistortedImage.type());

    // Code to set original image region to white
    /* cv::Mat originalImageRegion(outputImage, cv::Rect(centerX * scale - centerX, centerY * scale - centerY,
     * imageWidth, imageHeight)); */
    /* originalImageRegion.setTo(cv::Scalar(255, 255, 255)); */

    for (auto y = 0; y < imageHeight; y++) {
        for (auto x = 0; x < imageWidth; x++) {
            cv::Vec3f undistNormPoint((x - centerX) / fx, (y - centerY) / fy, 1.0);

            double radiusMirror = cv::norm(undistNormPoint);

            cv::Vec3f normUnitSphere = undistNormPoint / radiusMirror;

            cv::Vec2f normImgCoord(normUnitSphere[0], normUnitSphere[1]);
            normImgCoord = normImgCoord / (normUnitSphere[2] + xi);

            double r2 = normImgCoord.dot(normImgCoord);

            double r4 = r2 * r2;

            auto radialDistortionTerm = 1.0 + radialDistortionParams[0] * r2 + radialDistortionParams[1] * r4;

            cv::Vec2f tangentialDistortionTerm(2 * tangentialDistortionParams[0] * normImgCoord[0] * normImgCoord[1]
                    + tangentialDistortionParams[1] * (r2 + 2 * normImgCoord[0] * normImgCoord[0]),
                tangentialDistortionParams[0] * (r2 + 2 * normImgCoord[1] * normImgCoord[1])
                    + 2 * tangentialDistortionParams[1] * normImgCoord[0] * normImgCoord[1]);

            auto distortedNormImgCoord = normImgCoord * radialDistortionTerm + tangentialDistortionTerm;

            int xNew = distortedNormImgCoord[0] * fx + skew * distortedNormImgCoord[1] + centerX * scale;
            int yNew = distortedNormImgCoord[1] * fy + centerY * scale;

            /* std::cout << xNew << std::endl << yNew << std::endl; */

            if (xNew >= 0 && xNew < imageWidth * scale && yNew >= 0 && yNew < imageHeight * scale) {
                /* std::cout << "new x, y: " << xNew << " " << yNew << std::endl; */
                /* std::cout << "x, y: " << x << " " << y << std::endl; */
                /* std::cout << outputImage.at<cv::Vec3b>(xNew, yNew) << std::endl; */
                /* std::cout << undistortedImage.at<cv::Vec3b>(x, y) << std::endl; */
                outputImage.at<cv::Vec3b>(yNew, xNew) = undistortedImage.at<cv::Vec3b>(y, x);
            }
        }
    }

    return outputImage;
}
}
