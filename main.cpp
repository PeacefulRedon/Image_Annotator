#include <iostream>
#include <direct.h>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat src, roi;
int x_0(0), y_0(0), x_1(0), y_1(0);
unsigned long cnt(1), label(1);
bool start_draw( false );
string annot_dir, annot_name;

string int2str( unsigned int x )
{
    char buffer[3];
    sprintf( buffer, "%d", x );
    string str( buffer );
    return str;
}

void on_mouse(int event, int x, int y, int , void * )
{
    if( event == EVENT_LBUTTONDOWN )
    {
        if( !start_draw )
        {
            x_0 = x;
            y_0 = y;
            start_draw = true;
        }
        else
        {
            x_1 = x;
            y_1 = y;
            start_draw = false;
        }
    }
    if( (event == EVENT_MOUSEMOVE) && start_draw )
    {
        Mat current_view;
        current_view = src.clone();
        rectangle( current_view, Point(x_0,y_0), Point(x,y), Scalar(255, 255, 255), 2 );
        imshow( "Image", current_view );
    }

    if( x_1!=0 && y_1!=0 && event==EVENT_RBUTTONDBLCLK )
    {
        roi = src( Range(min(y_0,y_1), max(y_0,y_1)) , Range(min(x_0,x_1), max(x_0,x_1)) );
        imwrite( annot_dir + "\\" + annot_name + "_" + int2str(cnt) + ".png", roi );

        ofstream annot_file( "Database\\Annotations.txt", ios::app );
        annot_file <<"Database\\\\"+annot_name+"\\"+"\\"+annot_name+"_"+int2str(cnt) <<"," <<int2str( label ) <<endl;
        annot_file.close();
        ++cnt;

        imshow( "Image", src );
    }

}

int main( int argc, char* argv[] )
{
    cout <<"================================================================================" <<endl;
    cout <<"How to use this tool :" <<endl;
    cout <<"    1.  Enter the annotation." <<endl;
    cout <<"    2.  Select on the image the area to annotate." <<endl;
    cout <<"    3.  Double-click on the right button on the picture to save the selected" <<endl <<" area to the database." <<endl;
    cout <<"    4.  Press on keyboard to jump to the next image." <<endl;
    cout <<"================================================================================" <<endl <<endl;
    cout <<"Enter the annotation : ";
    cin >>annot_name;

    ifstream label_file( "Database\\nbLabel.txt" );
    if( !label_file )
    {
        mkdir( "Database" );
        ofstream label_file( "Database\\nbLabel.txt" );
        label_file <<label;
    }
    else
    {
        label_file >>label;
        ++label;
    }
    label_file.close();

    annot_dir = "Database\\" + annot_name;
    mkdir( annot_dir.c_str() );

    for( int i(1) ; i<argc ; ++i )
    {
        src = imread( argv[i] );

        namedWindow( "Image" );
        imshow( "Image", src );

        setMouseCallback("Image" , on_mouse);
        waitKey();
    }

    ofstream next_label_file( "Database\\nbLabel.txt" );
    next_label_file.clear();
    next_label_file <<label;
    next_label_file.close();

    return 0;
}
