#include <iostream>
#include <math.h>       /* pow */
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>    // std::sort
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/photo/photo.hpp>

using namespace cv;
using namespace std;


const int MIN_OBJECT_AREA = 100000;
const int MAX_OBJECT_AREA = 10000000;

struct rangeHSV{
    int iLowH;  // menor hue
    int iHighH; // maior hue
    int iLowS;  // menor saturation
    int iHighS; // maior saturation
    int iLowV;  // menor value
    int iHighV; // maior value
};

struct colorFounded {
    double posX;
    double posY;
    double area;
    string color;
};

enum color_code {
    black,
    brown,
    red,
    orange,
    yellow,
    green,
    blue,
    purple,
    gray,
    white,
    golden,
    silver,
};

color_code hashit (std::string const& inString) {
    if (inString == "purple")   return purple;
    if (inString == "black")    return black;
    if (inString == "brown")    return brown;
    if (inString == "red")      return red;
    if (inString == "orange")   return orange;
    if (inString == "yellow")   return yellow;
    if (inString == "green")    return green;
    if (inString == "blue")     return blue;
    if (inString == "gray")     return gray;
    if (inString == "white")    return white;
    if (inString == "golden")   return golden;
    if (inString == "silver")   return silver;
};

struct rangeHSV  getColorRange(string color){
    struct rangeHSV HsvLowAndHigh;

    switch(hashit(color)){
        case black:
            // Defining black color
            HsvLowAndHigh.iLowH  = 0;
            HsvLowAndHigh.iHighH = 179;
            HsvLowAndHigh.iLowS  = 0;
            HsvLowAndHigh.iHighS = 121;
            HsvLowAndHigh.iLowV  = 0;
            HsvLowAndHigh.iHighV = 57;
            break;
        case brown:
            // Defining brown color 
            HsvLowAndHigh.iLowH  = 7;
            HsvLowAndHigh.iHighH = 68;
            HsvLowAndHigh.iLowS  = 0;
            HsvLowAndHigh.iHighS = 253;
            HsvLowAndHigh.iLowV  = 0;
            HsvLowAndHigh.iHighV = 187;
            break;
        case red:
            // Defining red color
            HsvLowAndHigh.iLowH  = 0;
            HsvLowAndHigh.iHighH = 4;
            HsvLowAndHigh.iLowS  = 124;
            HsvLowAndHigh.iHighS = 255;
            HsvLowAndHigh.iLowV  = 74;
            HsvLowAndHigh.iHighV = 255;
            break;
        case orange:
            // Defining orange color
            HsvLowAndHigh.iLowH  = 7;
            HsvLowAndHigh.iHighH = 85;
            HsvLowAndHigh.iLowS  = 216;
            HsvLowAndHigh.iHighS = 255;
            HsvLowAndHigh.iLowV  = 151;
            HsvLowAndHigh.iHighV = 255;
            break;
        case yellow:
            // Defining yellow color
            HsvLowAndHigh.iLowH  = 10;
            HsvLowAndHigh.iHighH = 73;
            HsvLowAndHigh.iLowS  = 250;
            HsvLowAndHigh.iHighS = 255;
            HsvLowAndHigh.iLowV  = 225;
            HsvLowAndHigh.iHighV = 255;
            break;
        case green:
            // Defining green color
            HsvLowAndHigh.iLowH  = 19;
            HsvLowAndHigh.iHighH = 165;
            HsvLowAndHigh.iLowS  = 151;
            HsvLowAndHigh.iHighS = 255;
            HsvLowAndHigh.iLowV  = 165;
            HsvLowAndHigh.iHighV = 194;
            break;
        case blue:
            // Defining blue color
            HsvLowAndHigh.iLowH  = 74;
            HsvLowAndHigh.iHighH = 149;
            HsvLowAndHigh.iLowS  = 111;
            HsvLowAndHigh.iHighS = 255;
            HsvLowAndHigh.iLowV  = 86;
            HsvLowAndHigh.iHighV = 255;
            break;
        case purple:
            // Defining purple color
            HsvLowAndHigh.iLowH  = 121;
            HsvLowAndHigh.iHighH = 150;
            HsvLowAndHigh.iLowS  = 159;
            HsvLowAndHigh.iHighS = 194;
            HsvLowAndHigh.iLowV  = 97;
            HsvLowAndHigh.iHighV = 164;
            break;
        case gray:
            // Defining gray color
            HsvLowAndHigh.iLowH  = 0;
            HsvLowAndHigh.iHighH = 80;
            HsvLowAndHigh.iLowS  = 0;
            HsvLowAndHigh.iHighS = 80;
            HsvLowAndHigh.iLowV  = 34;
            HsvLowAndHigh.iHighV = 139;
            break;
        case white:
            // Defining white color
            HsvLowAndHigh.iLowH  = 0;
            HsvLowAndHigh.iHighH = 85;
            HsvLowAndHigh.iLowS  = 0;
            HsvLowAndHigh.iHighS = 43;
            HsvLowAndHigh.iLowV  = 144;
            HsvLowAndHigh.iHighV = 255;
            break;
        case golden:
            // Defining golden color
            HsvLowAndHigh.iLowH  = 18;
            HsvLowAndHigh.iHighH = 77;
            HsvLowAndHigh.iLowS  = 93;
            HsvLowAndHigh.iHighS = 238;
            HsvLowAndHigh.iLowV  = 81;
            HsvLowAndHigh.iHighV = 255;
            break;
        case silver:
            // Defining silver color
            HsvLowAndHigh.iLowH  = 0;
            HsvLowAndHigh.iHighH = 161;
            HsvLowAndHigh.iLowS  = 45;
            HsvLowAndHigh.iHighS = 123;
            HsvLowAndHigh.iLowV  = 44;
            HsvLowAndHigh.iHighV = 206;
    }
    return HsvLowAndHigh;
};


Scalar getScalar(string color, int HIGH){
    struct rangeHSV rangeColor = getColorRange(color);

    if(HIGH){
        return Scalar(rangeColor.iHighH,rangeColor.iHighS,rangeColor.iHighV);        
    }else {
        return Scalar(rangeColor.iLowH,rangeColor.iLowS,rangeColor.iLowV);
    }
};

//Dilatação e Erosão da imagem
void morphOps(Mat &thresh){
    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(5,5));
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));

    erode(thresh,thresh,erodeElement);
    dilate(thresh,thresh,dilateElement);

    erode(thresh,thresh,erodeElement);
    dilate(thresh,thresh,dilateElement);
}

struct colorFounded getXYColorPosition (string color, Mat imgHSV) {
    Mat imgThresholded;
    inRange(imgHSV, getScalar(color, 0) , getScalar(color, 1) , imgThresholded);

    //Aplicando Dilatação e Erosão na imagem
    morphOps(imgThresholded);

    Moments oMoments = moments(imgThresholded);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;
    
    struct colorFounded found;
    if (dArea > MIN_OBJECT_AREA && dArea < MAX_OBJECT_AREA)
        found = {.posX = dM10 / dArea,.posY = dM01 / dArea,.area = dArea,.color = color};
        return found;
    return found = {.posX = 99999,.posY = 99999,.area = 99999,.color = color};
}

bool orderByXPos(const colorFounded& x, const colorFounded& y) { return x.posX < y.posX; }

bool orderByYPos(const colorFounded& x, const colorFounded& y) { return x.posY < y.posY; }

double getBandValue(double lastValue, int index, string color, int size){
    
    if(index == 0){
        switch(hashit(color)){
            case golden:
                return lastValue;
            case silver:
                return lastValue;
            default:
                if(size == 3 || size == 4) return lastValue + (int) hashit(color)*10;
                else return lastValue + (int) hashit(color)*100;
                break;
        }
    } else if (index == 1){
        switch(hashit(color)){
            case golden:
                return lastValue;
            case silver:
                return  lastValue;
            default:
                if(size == 3 || size == 4) return lastValue + (int)hashit(color);
                else return lastValue + (int) hashit(color)*10;
                break;
        }
    } else if (index == 2){
        switch(hashit(color)){
            case golden:
                if(size == 3 || size == 4)  return lastValue * 0.1;
                else return lastValue;
            case silver:
                if(size == 3 || size == 4) return lastValue * 0.01;
                else return lastValue;
            default:
                if(size == 3 || size == 4) return lastValue * pow(10,(int)hashit(color));
                else return lastValue + (int) hashit(color);
                break;
        }
    } else if (index == 3){
        if(size == 3 || size == 4)  return lastValue;
        else {
            switch(hashit(color)){
                case golden:
                    return lastValue * 0.1;
                case silver:
                    return lastValue * 0.01;
                default:
                    return lastValue * pow(10,(int) hashit(color));
                    break;
            }
        } 
    }
};

//Efetua somatório do valor do resistor baseado no indice do vetor e a cor em questão
double getResistorValue(vector<colorFounded> bandFounded) {
    double value = 0;
    for (int index = 0, size = bandFounded.size(); index < size; ++index) {
            value = getBandValue(value, index, bandFounded[index].color, size );
	}

    return value;
};

string getBandPrecision(string color ,  int resistorType) {
    switch(resistorType) {
        case 4: case 5: case 6:
            switch(hashit(color)){
                case black:  return "";
                case brown:  return "+/- 1\%";
                case red:    return "+/- 2\%";
                case orange: return "";
                case yellow: return "";
                case green:  return "+/- 0.5\%";
                case blue:   return "+/- 0.25\%";
                case purple: return "+/- 0.1%\%";
                case gray:   return "+/- 0.05\%";
                case white:  return "";
                case golden: return "+/- 5\%";
                case silver: return "+/- 10\%";
                default: return "+/- 20\%";
            }
        default: return "+/- 20\%";
    }

}

string getResistorPrecision(vector<colorFounded> bandFounded) {
    int indexOfPrecision = 0;
    //Descobrindo o indice da faixa de precisão dependendo do tipo do resistor
    switch(bandFounded.size()){
        case 3: indexOfPrecision = -1;  break;
        case 4: indexOfPrecision = 3;   break;
        case 5: indexOfPrecision = 4;   break;
        case 6: indexOfPrecision = 4;   break;
    }
    return getBandPrecision(bandFounded[indexOfPrecision].color , bandFounded.size());
}

int main( int argc, char** argv ){
    //Declarando Vetor com cores validas para resistor de seis faixas
    const char *colors[] = {
        "purple",
        "black",
        "brown",
        "red",
        "orange",
        "yellow",
        "green",
        "blue",
        "gray",
        "white",
        "golden",
        "silver",
    };

    vector<std::string> sixBandColors(colors, colors + 12);

    //Carregando Imagem
    Mat imgOriginal;
    imgOriginal = imread( "resistor2.jpg", 1 );

    //Denoising Image
    fastNlMeansDenoisingColored( imgOriginal, imgOriginal, 3, 3, 7, 21 );

    //Criando uma imagem preta do tamanho da imagem original
    Mat points = Mat::zeros( imgOriginal.size(), CV_8UC3 );
    while(true) {
        vector<colorFounded> bandFounded;

        //Convertendo Imagem de BGR para HSV
        Mat imgHSV;
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
        string color = "none";

        //Encontrando os pontos centrais das cores do resistor
        colorFounded found;
        for (int i = 0; i< sixBandColors.size(); i++){
            color = sixBandColors[i];
            found = getXYColorPosition(color,imgHSV);
            if(found.area > MIN_OBJECT_AREA){
                bandFounded.push_back(found);
            }
        }

        cout << bandFounded.size() << endl;
        //Ordenando Quanto a posição X dos pontos encontrados
        sort(bandFounded.begin(), bandFounded.end(), orderByXPos);  

        for (int i = 0; i< bandFounded.size(); i++){
            found = bandFounded[i];
            if(found.area > MIN_OBJECT_AREA){
                cout << found.color << "  " << found.area << endl;
                circle(points, Point(found.posX, found.posY), 3,  Scalar(0,255,0), 2);
            }
        }

        imgOriginal = imgOriginal + points;
        imshow("Original", imgOriginal); //show the original image
        cout << "Resistor Value" << getResistorValue(bandFounded) << " " << getResistorPrecision(bandFounded) << endl;
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            return 0;
        }
    }

}
