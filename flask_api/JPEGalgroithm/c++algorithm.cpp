#include<iostream>
#include <math.h>
using namespace std;
extern "C"
{
float *toYuv(int *data, int height,int width) {
    int size = height*width*3;
    float *a = new float[height*width*3];
    for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
            a[i*width*3+j*3+0] = static_cast<float>(data[i*width*3+j*3+0])*0.299+static_cast<float>(data[i*width*3+j*3+1])*0.587+static_cast<float>(data[i*width*3+j*3+2])*0.114;
            a[i*width*3+j*3+1] = static_cast<float>(data[i*width*3+j*3+0])*(-0.169)-static_cast<float>(data[i*width*3+j*3+1])*0.331+static_cast<float>(data[i*width*3+j*3+2])*0.5+128;
            a[i*width*3+j*3+2] = static_cast<float>(data[i*width*3+j*3+0])*0.5-static_cast<float>(data[i*width*3+j*3+1])*0.419-static_cast<float>(data[i*width*3+j*3+2])*0.081+128;
        }
    }
    return a;
}
float *****split_matrix(float *data,int height,int width){
    int new_height = height/8;
    int new_width = width/8;
    float *****new_arr = new float****[new_height];
    for(int i = 0; i < new_height; i++){
        new_arr[i] = new float ***[new_width];
        for(int j = 0; j < new_width; j++){
            new_arr[i][j] = new float **[8];
            for(int k = 0; k < 8; k++){
                new_arr[i][j][k] = new float *[8];
                for(int m = 0; m < 8; m++){
                    new_arr[i][j][k][m] = new float [3];
                }
            }
        }
    }
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            for(int k = 0; k < 8; k++){
                for(int m = 0; m < 8; m++){
                    new_arr[i][j][k][m][0] = data[(i*8+k)*width+(j*8+m)*3+0];
                    new_arr[i][j][k][m][1] = data[(i*8+k)*width+(j*8+m)*3+1];
                    new_arr[i][j][k][m][2] = data[(i*8+k)*width+(j*8+m)*3+2];
                }
            }
        }
    }
    return new_arr;
}
float **dot(float **a,float **b){
    float **result = new float*[8];
    for(int i =0;i<8;i++){
        result[i] = new float [8];
    }
    for(int i =0;i<8;i++){
        for(int j =0;j<8;j++){
            float temp = 0;
            for(int k =0;k<8;k++){
                temp += a[i][k]*b[k][j];
            }
            result[i][j] = temp;
        }
    }
    return result;
}
float* dctTransform(float *data, int height,int width){
    float *****splited_arr = split_matrix(data,height,width);
    int new_height = height/8;
    int new_width = width/8;
    float ****ymatrix = new float ***[new_height];
    float ****umatrix = new float ***[new_height];
    float ****vmatrix = new float ***[new_height];
    for(int i = 0; i < new_height; i++){
        ymatrix[i] = new float **[new_width];
        umatrix[i] = new float **[new_width];
        vmatrix[i] = new float **[new_width];
        for(int j = 0; j < new_width; j++){
            ymatrix[i][j] = new float *[8];
            umatrix[i][j] = new float *[8];
            vmatrix[i][j] = new float *[8];
            for(int k = 0; k < 8; k++){
                ymatrix[i][j][k] = new float [8];
                umatrix[i][j][k] = new float [8];
                vmatrix[i][j][k] = new float [8];
            }
        }
    }
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            for(int k = 0; k < 8; k++){
                for(int m = 0; m < 8; m++){
                    ymatrix[i][j][k][m] = splited_arr[i][j][k][m][0];
                    umatrix[i][j][k][m] = splited_arr[i][j][k][m][1];
                    vmatrix[i][j][k][m] = splited_arr[i][j][k][m][2];
                }
            }
        }
    }
    int x = 8;
    float **dct = new float *[x];
    for(int i =0;i<8;i++){
        dct[i] = new float [x];
    }
    float a;
    float *arr = new float[height*width*3];
    for(int i = 0;i<height*width*3;i++){
        arr[i] = 0;
    }
    for(int i = 0;i<8;i++){
        if(i == 0){
            a = sqrt(0.125);
        }
        else{
            a = sqrt(0.25);
        }
        for(int j = 0;j<8;j++){
            dct[i][j] = a * cos((2 * j + 1) * i * M_PI / 16);
        }
    }
    float **tran_dct = new float *[x];
    for(int i =0;i<8;i++){
        tran_dct[i] = new float [x];
    }
    for(int i =0;i<8;i++){
        for(int j =0;j<8;j++){
            tran_dct[i][j] = dct[j][i];
        }
    }
    for(int i = 0;i<new_height;i++){
        for(int j = 0;j<new_width;j++){
            float **result_y = dot(dct,ymatrix[i][j]);
            ymatrix[i][j] = dot(result_y,tran_dct);
            float **result_u = dot(dct,umatrix[i][j]);
            umatrix[i][j] = dot(result_u,tran_dct);
            float **result_v = dot(dct,vmatrix[i][j]);
            vmatrix[i][j] = dot(result_v,tran_dct);
            for(int i=0;i<8;i++){
                delete[] result_y[i];
                delete[] result_u[i];
                delete[] result_v[i];
            }
            delete[] result_y;
            delete[] result_u;
            delete[] result_v;
        }
    }
    cout<<ymatrix[0][0][0][0];
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            for(int k = 0; k < 8; k++){
                for(int m = 0; m < 8; m++){
                    arr[(i*8+k)*width+(j*8+m)*3+0] = ymatrix[i][j][k][m];
                    arr[(i*8+k)*width+(j*8+m)*3+1] = umatrix[i][j][k][m];
                    arr[(i*8+k)*width+(j*8+m)*3+2] = vmatrix[i][j][k][m];
                }
            }
        }
    }

    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            for(int k = 0; k < 8; k++){
               for(int m = 0;m<8; m++){
                   delete[] splited_arr[i][j][k][m];
               }
            }
        }
    }
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            for(int k = 0; k < 8; k++){
               delete[] splited_arr[i][j][k];
               delete[] ymatrix[i][j][k];
                delete[] umatrix[i][j][k];
                delete[] vmatrix[i][j][k];
            }
        }
    }
    for(int i = 0; i < new_height; i++){
        for(int j = 0; j < new_width; j++){
            delete[] splited_arr[i][j];
            delete[] ymatrix[i][j];
            delete[] umatrix[i][j];
            delete[] vmatrix[i][j];
        }
    }
    for(int i = 0; i < new_height; i++){
        delete [] splited_arr[i];
        delete[] ymatrix[i];
        delete[] umatrix[i];
        delete[] vmatrix[i];

    }
    for(int i = 0;i<8;i++){
        delete[] dct[i];
        delete[] tran_dct[i];
    }
    delete[] dct;
    delete[] tran_dct;
    delete[] ymatrix;
    delete[] umatrix;
    delete[] vmatrix;
    delete[] splited_arr;
    return arr;

}
void change_array(int *data, int height,int width) {
    float *a=toYuv(data,height,width);
    float *b = dctTransform(a,height,width);

    delete []a ;
    delete[]b;

}

void change_array2(int *data, int len) {
    for (int i = 0; i < len; i++) {
        data[i] += len;
    }
}

}