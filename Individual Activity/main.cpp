


#include <iostream>
#include <cmath>

#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define ROTATE_90 0
#define ROTATED_180 1
#define ROTATE_270 2
#define ROTATE_360 3




class Tranfsormacoes
{
public:
    bool GaussianBlur;
    bool Canny;
    bool Sobel;
    bool Negative;
    bool GrayScale;
    int Redimensionamento;
    int Rotacao;
    bool espelhamentoVertical;
    bool espelhamentoHorizontal;
   
    Tranfsormacoes()
    {
        GaussianBlur = false;
        Canny = false;
        Sobel = false;
        Negative = false;
        GrayScale = false;
        Redimensionamento = 0;
        Rotacao = ROTATE_360;
        espelhamentoVertical= false;
        espelhamentoHorizontal= false;
       

    }
    void reinicializa()
    {
        GaussianBlur = false;
        Canny = false;
        Sobel = false;
        Negative = false;
        GrayScale = false;
        Redimensionamento = 0;
        Rotacao = ROTATE_360;
        espelhamentoVertical = false;
        espelhamentoHorizontal = false;

    }


};
Mat Transformations(Tranfsormacoes info, Mat frame, int contraste, int brilho, int kernel)
{
    if (info.GaussianBlur)//gblur
    {
        if (kernel % 2 == 0)//tratar exceção de kernel par 
        {
     
       kernel = kernel + 1;
                 
        }
        GaussianBlur(frame, frame, Size(kernel, kernel),BORDER_DEFAULT);//  src,dst,size,border

    }
    if (info.Canny)//canny
    {
        cvtColor(frame, frame, COLOR_BGR2GRAY); // canny necessita conversão para tons de cinza
        Canny(frame, frame, 40, 120); //src,dst,,threshhold1,threshhold2
        cvtColor(frame, frame, COLOR_GRAY2BGR);// trata a execessão que pode crashar o programa caso uma sequencia especifica de comandos forem acionados(ex canny,canny,GrayScale,canny)

    }
    if (info.Sobel)//sobel
    {
      
        Sobel(frame, frame, CV_8U, 1,1,3, 1, 1, BORDER_DEFAULT);// src,dst,depth,dx,dy,ksize,scale,delta,bordertype
    }

    if (info.Negative)//negativo
    {
        frame.convertTo(frame, -1, -1, 255);
    }
    //brilho e contraste em todo programa
    frame.convertTo(frame, -1, contraste, brilho);


    if (info.GrayScale)//tons de cinza
    {
        cvtColor(frame, frame, COLOR_BGR2GRAY);
    }

    if (info.Redimensionamento )//redimensionamento
    {
        int newRow = frame.rows / pow(2, info.Redimensionamento);//2^1, 2^2,etc

        int newCol = frame.cols / pow(2, info.Redimensionamento);//2^1, 2^2,etc

        if (newRow > 1 && newCol > 1)
            resize(frame, frame, Size(newRow, newCol));
    }

    if (info.Rotacao == ROTATE_90)//rotação
    {
       cv::rotate(frame, frame, ROTATE_90_CLOCKWISE);
     
    }
    else 
        if (info.Rotacao == ROTATED_180)
        {
             cv::rotate(frame,frame,ROTATE_180);
      
        }
    else 
        if (info.Rotacao == ROTATE_270) 
        {
             cv::rotate(frame, frame, ROTATE_90_COUNTERCLOCKWISE);
       
        } 
    if (info.espelhamentoHorizontal)//espelhamento horizontal
    {
        flip(frame, frame, 1);
    }

    if (info.espelhamentoVertical)//espelhamento vertical
    {
        flip(frame, frame, 0);
    }
   

    return frame;



}




int main(int argc, char** argv)
{
    
    int camera = 0;  
    Mat frame;
    Mat frameGrav;
    VideoCapture cap;
    VideoWriter writer;


    namedWindow("This is you, smile! :)", 1);
    namedWindow("This is you, smile!", 2);

    Tranfsormacoes info;
    
    int contraste = 0;
    int brilho = 0;
    int kernelSize = 0;

    //gravar e salvar
    bool gravar = false;
    bool salvargrav = false;
   

  
    //Trackbar kernel
    int iSliderValue1 = 5;
    createTrackbar("KernelSize", "This is you, smile! :)", &iSliderValue1, 50);//depois de 50 fica bem pesado

    //Trackbar contraste
    int iSliderValue2 = 1;
    createTrackbar("Contraste", "This is you, smile! :)", &iSliderValue2, 100);

    //Trackbar brilho
    int iSliderValue3 = 0;
    createTrackbar("Brilho", "This is you, smile! :)", &iSliderValue3, 200);


    if (!cap.open(camera))
        return 0;

    cap >> frame;
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return -1;
    }  

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    
    writer.open("out.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height), true);
    if (!writer.isOpened())
        return 0;

    for (;;)
    {
        cap >> frame;

        kernelSize = iSliderValue1;
        contraste = iSliderValue2;
        brilho = iSliderValue3;
     
        char key = waitKey(1);
        if (key == 27) {
            break;
        }
        switch (key)
        {
        case 'a': 
            info.GaussianBlur = true;
            break;
        case 'b':
            info.Canny = !info.Canny;
            break;
        case 'c':
            info.Sobel = !info.Sobel;
            break;

        case 'd':
            info.Negative = !info.Negative;
            break;

        case 'e':
            info.GrayScale = true;
            break;

        case 'f':
            info.espelhamentoHorizontal = !info.espelhamentoHorizontal;
            break;

        case 'g':
            info.espelhamentoVertical = !info.espelhamentoVertical;
            break;

        case 'h':
            if (info.Rotacao == ROTATE_90)
                 info.Rotacao = ROTATED_180;
            else
                if (info.Rotacao == ROTATED_180)
                 info.Rotacao = ROTATE_270;
            else 
                if (info.Rotacao == ROTATE_270)
                 info.Rotacao = ROTATE_360;
            else
                if(info.Rotacao=ROTATE_360)
                 info.Rotacao = ROTATE_90;
            break;
        case 'i':
            info.Redimensionamento += 1;
            break;
        case 'j':
            gravar = true;
            break;
        case 'k':
            salvargrav = true;
            break;
        case '0':
            info.reinicializa();
            break;
        default:
            break;
        }

        imshow("This is you, smile!", frame);
        frameGrav =Transformations(info, frame, contraste, brilho, kernelSize);
        imshow("This is you, smile! :)", frameGrav);
        if (gravar == true)
        {
            if (salvargrav == true)
            {
                gravar = false;
               salvargrav = false;
            }
            else
            {
                if (frameGrav.channels() < 3)
                    cvtColor(frameGrav, frameGrav, COLOR_GRAY2RGB);
                writer.write(frameGrav);
            }
        }

    }
    cap.release();
    return 0;
}



