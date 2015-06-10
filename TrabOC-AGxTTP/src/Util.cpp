
#include "Util.h"

std::vector<std::string> split(std::string str,std::string sep){

    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

float** readMatrix(std::string strMatrix, std::vector<std::string> *header, float** matrix){

    bool first_line = true;


    std::vector<std::string> line = split(strMatrix,"/");

    for (int i=0;i<line.size();i++){

        std::vector<std::string> fields = split(line.at(i),",");


        for (int j=0;j<fields.size();j++){

            if(first_line){

                header->push_back(fields.at(j));

            }
            else{

                if(j==0){

                    matrix[i-1] = new float[header->size()];
                }
                matrix[i-1][j] = atof(fields.at(j).c_str());

            }

        }

        if(first_line){

            matrix = new float*[header->size()];
            first_line = false;
        }
    }

    return matrix;
}

bool GetRdmBool(float p)
{
    return (rand() / (double)RAND_MAX) < p;
}

int  GetRdmInt(int begin, int end){

    return(begin + (rand() % (end - begin)));

}

bool ExistItem(int size,int* v, int value){

    for(int i=0;i<size;i++){
        if(v[i]==value) return true;
    }

    return false;

}
