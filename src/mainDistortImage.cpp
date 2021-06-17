#include "CameraDistortion.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv)
{
  const std::string keys = "{help h usage ?|      |print this message}"
                           "{@inputImage   |<none>|path to input image (*.png, *.jpg, ...)}"
                           "{rotationAngle |0     |rotation angle (0, 90, 180 and 270 degrees allowed)}"
                           "{save          |false |if true, distorted images are stored}";

  cv::CommandLineParser parser(argc, argv, keys);
  int rotationAngle = parser.get<int>("rotationAngle");
  bool save = parser.get<bool>("save");

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  std::string inputImageName = parser.get<std::string>(0);

  if (!parser.check()) {
    parser.printMessage();
    parser.printErrors();
    return 0;
  }

  cv::Mat undistortedImage;
  undistortedImage = cv::imread(argv[1], cv::ImreadModes::IMREAD_COLOR);

  if (undistortedImage.empty()) {
    std::cout << "Couldn't load and open image!" << std::endl;
    return -1;
  }

  int rotationCode = -1;
  switch (rotationAngle) {
  case 90:
    rotationCode = cv::ROTATE_90_CLOCKWISE;
    break;
  case 180:
    rotationCode = cv::ROTATE_180;
    break;
  case 270:
    rotationCode = cv::ROTATE_90_COUNTERCLOCKWISE;
    break;
  case 0:
    rotationCode = -1;
    break;
  default:
    std::cerr << "Rotation angle - " << rotationAngle << " - not supported (0, 90, 180 and 270 valid) => set to 0."
              << std::endl;
    rotationCode = -1;
    break;
  }
  if (rotationCode != -1) {
    cv::rotate(undistortedImage, undistortedImage, rotationCode);
  }

  cv::namedWindow("Undistorted Image", cv::WindowFlags::WINDOW_NORMAL);
  cv::imshow("Undistorted Image", undistortedImage);
  cv::resizeWindow("Undistorted Image", cv::Size(undistortedImage.cols / 2.0, undistortedImage.rows / 2.0));

  /* float distParamsRadial[4] = {-0.2428, 0.0452, -0.0033, 0.0}; */
  // No radial distortion
  /* float distParamsRadial[4] = { 0.0, 0.0, 0.0, 0.0 }; */
  // Pincushion distortion
  /* float distParamsRadial[4] = {0.8, 0.0, 0.0, 0.0}; */
  // Barrel distortion
  float distParamsRadial[4] = { -0.8, 0.0, 0.0, 0.0 };

  // Tangential distortion
  /* float distParamsTangential[2] = { 1.0e-1, 0.0 }; */
  // No tangential distortion
  float distParamsTangential[2] = { 0.0, 0.0 };

  float skewRadial = 0.0f;
  float scaleRadial = 1.5f;

  cv::Mat radialDistortedImage = image_helper::distortImageRadialAndTangential(
      undistortedImage, distParamsRadial, distParamsTangential, skewRadial, scaleRadial);

  /* cv::GaussianBlur(radialDistortedImage, radialDistortedImage, cv::Size(3, 3), 0); */
  /* cv::Mat tmp; */
  /* cv::bilateralFilter(radialDistortedImage, tmp, 0, 100.0, 100.0); */
  /* radialDistortedImage = tmp; */
  /* cv::blur(radialDistortedImage, radialDistortedImage, cv::Size(3, 3)); */

  cv::namedWindow("Radial Distorted Image", cv::WindowFlags::WINDOW_NORMAL);
  cv::imshow("Radial Distorted Image", radialDistortedImage);
  cv::resizeWindow(
      "Radial Distorted Image", cv::Size(radialDistortedImage.cols / 2.0, radialDistortedImage.rows / 2.0));

  float distParamsAngular[4] = { -0.8, 0.0, 0.0, 0.0 };
  float skew = 0.0f;
  float scaleAngular = 1.5f;
  cv::Mat angularDistortedImage
      = image_helper::distortImageAngular(undistortedImage, distParamsAngular, skew, scaleAngular);
  cv::namedWindow("Angular Distorted Image", cv::WindowFlags::WINDOW_NORMAL);
  cv::imshow("Angular Distorted Image", angularDistortedImage);
  cv::resizeWindow(
      "Angular Distorted Image", cv::Size(angularDistortedImage.cols / 2.0, angularDistortedImage.rows / 2.0));

  /* float distParamsRadialOmniMeiRives[2] = {-3.3800738851716483e-01, 1.2640660666010303e-01}; */
  /* float distParamsTangentialOmniMeiRives[2] = {5.0716224294455122e-04, -2.7239144099816025e-03}; */
  /* float xiOmniMeiRives = 1.4646415453703598e+00; */
  float distParamsRadialOmniMeiRives[2] = { -0.8, 0.0 };
  float distParamsTangentialOmniMeiRives[2] = { 0.0, 0.0 };
  float xiOmniMeiRives = 0.25f;
  float skewOmniMeiRives = 0.0f;
  float scaleOmniMeiRives = 1.5f;
  cv::Mat omniMeiRivesDistortedImage
      = image_helper::distortImageOmniMeiRives(undistortedImage, distParamsRadialOmniMeiRives,
          distParamsTangentialOmniMeiRives, xiOmniMeiRives, skewOmniMeiRives, scaleOmniMeiRives);
  cv::namedWindow("OmniMeiRives Distorted Image", cv::WindowFlags::WINDOW_NORMAL);
  cv::imshow("OmniMeiRives Distorted Image", omniMeiRivesDistortedImage);
  cv::resizeWindow("OmniMeiRives Distorted Image",
      cv::Size(omniMeiRivesDistortedImage.cols / 2.0, omniMeiRivesDistortedImage.rows / 2.0));

  if (save) {
    std::string distRadialAndTagentialOuputName = "DistortedRadialTangential-" + inputImageName;
    std::string distAngularOuputName = "DistortedAngular-" + inputImageName;
    std::string distOmniMeiRivesOutputName = "DistortedOmniMeiRives-" + inputImageName;
    cv::imwrite(distRadialAndTagentialOuputName, radialDistortedImage);
    cv::imwrite(distAngularOuputName, angularDistortedImage);
    cv::imwrite(distOmniMeiRivesOutputName, omniMeiRivesDistortedImage);
  }

  cv::waitKey(0);
  return 0;
}
