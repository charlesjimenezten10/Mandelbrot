#include "./bbrot.h"
#include <time.h> 
#include <random>
#include <complex>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

const float REAL_MIN = -2.0;
const float REAL_MAX = 1.0;
const float IMAG_MIN = -1.5;
const float IMAG_MAX = 1.5;
std::default_random_engine randomEngine;
std::uniform_real_distribution<double> realNum(REAL_MIN, REAL_MAX);
std::uniform_real_distribution<double> imaginaryNum(IMAG_MIN, IMAG_MAX);


double normalize(double min, double max, double value)
{
    //std::cerr << "Max: " << max << " Min: " << min << std::endl;
    assert(max > min);
    // value - min -> calculate how far it is from the minimum -> how much it deviates from the minimum value of the original range
    // max - min -> calculates the span between the max and min range
    // by doing this formula, 
    return (value - min) / (max - min);
}

void update_image(Image& image, const MandelbrotPointInfo& info)
{
    for (const auto& points : info.points_in_path) {
        if ((points.real() < REAL_MIN || points.real() > REAL_MAX) || (points.imag() < IMAG_MIN || points.imag() > IMAG_MAX)) {
            continue;
        }
        else {

            double norm_x = normalize(REAL_MIN, REAL_MAX, points.real());
            double norm_y = normalize(IMAG_MIN, IMAG_MAX, points.imag());

            int x = (norm_x * image.getWidth());

            int y = (norm_y * image.getHeight());


            image.incValue(x, y);
            image.incValue(x, image.getHeight() - y - 1);

        }   
    }
}

void output_image_to_pgm(const Image& image, ostream& os)
{
    int width = image.getWidth();
    int height = image.getHeight();

    int maxPixel = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maxPixel = std::max(maxPixel, image.getValue(x, y));
        }
    }
    os << "P2\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            //image.setValue(i, j, );
            int value = image.getValue(i, j);
            //std::cerr << value;
            int scaledValue = (normalize(0, maxPixel, value) * 255);
            os << scaledValue << " ";
        }
        os << "\n";
    }

}

int main(int argc, char **argv) {
    for (int i = 0; i < 5; i++) {
        std::cerr << argv[i] << std::endl;
    }
    
    int imageSize = std::atoi(argv[1]);
    int numPoints = std::atoi(argv[2]);
    int maxIterations = std::atoi(argv[3]);

    Image image(imageSize, imageSize);
    for (int i = 0; i < numPoints; i++) {
        std::cerr << i << "\n";
        std::complex<double> c(realNum(randomEngine), imaginaryNum(randomEngine));
        //std::cout << c << std::endl;
        MandelbrotPointInfo mpi = compute_mandelbrot(c, maxIterations, true);
        if (mpi.escaped) {
            update_image(image, mpi);
        }
        if (i % 100000 == 0) {
            std::cerr << ".\n";
        }
    }
    //std::ofstream newFile("test.pgm");
    output_image_to_pgm(image, std::cout);
    //newFile.close();

    return 0;
}
