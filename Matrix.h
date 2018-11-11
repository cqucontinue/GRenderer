//
// Created by 田淙宇 on 2018/10/19.
//

#ifndef GRENDER_MATRIX_H
#define GRENDER_MATRIX_H

#include <assert.h>
#include <iostream>
#include <initializer_list>
#include <cmath>

template<class T,int num>
class Vec{
public:
    Vec(){
        value=new T[num];
        for(int i=0;i<num;++i){
            value[i]=0;
        }
    }
    Vec(std::initializer_list<T> l){
        assert(l.size()==num);
        value=new T[num];
        int i=0;
        for(typename std::initializer_list<T>::const_iterator iter=l.begin();i<num;++iter,++i)
            value[i]=*iter;
    }
    explicit Vec(const T *originList){
        value=new T[num];
        for(int i=0;i<num;++i)
        {
            value[i]=originList[i];
        }
    }

    Vec(const Vec<T,num>& r){
        value=new T[num];
        for(int i=0;i<num;++i){
            value[i]=r[i];
        }
    }

    Vec(T x,T y, T z){
        std::cerr<<"false";
        exit(0);
    }

    ~Vec(){
        delete[] value;
    }

    T& operator[](int index){
        return value[index];
    }
    const T& operator[](int idx)const{
        return value[idx];
    }

    T operator*(const Vec<T,num>& r)const{
        T result=0;
        for(int i=0;i<num;++i)
        {
            result+=value[i]*r[i];
        }
        if(std::abs(result)<1e-5)result=0;
        return result;
    }

    Vec<T,num> operator+(const Vec<T,num>& r)const{
        T result[num];
        for(int i=0;i<num;++i){
            result[i]=value[i]+r[i];
        }
        return Vec<T,num>(result);
    }

    Vec<T,num> operator+(T v)const{
        T result[num];
        for(int i=0;i<num;++i){
            result[i]=value[i]-v;
        }
        return Vec<T,num>(result);
    }

    Vec<T,num> operator-()const{
        T result[num];
        for(int i=0;i<num;++i){
            result[i]=0-value[i];
        }
        return Vec<T,num>(result);
    }

    Vec<T,num> operator-(const Vec<T,num>&r)const{
        T result[num];
        for(int i=0;i<num;++i){
            result[i]=value[i]-r[i];
        }
        return Vec<T,num>(result);
    }

    Vec<T,num> operator-(T v)const{
        T result[num];
        for(int i=0;i<num;++i){
            result[i]=value[i]-v;
        }
        return Vec<T,num>(result);
    }

    Vec<T,num>& operator=(const Vec<T,num>&r){
        for(int i=0;i<num;++i){
            value[i]=r[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out,const Vec<T,num>&r){
        for(int i=0;i<num;++i)
        {
            out<<r[i];
            if(i!=num-1)
                out<<" ";
        }
        return out;
    }

    Vec<T,num> normalize(){
        T sum=0;
        for(int i=0;i<num;++i){
            sum+=value[i]*value[i];
        }
        sum=std::sqrt(sum);
        for(int i=0;i<num;++i){
            value[i]/=sum;
        }
        return *this;
    }
private:
    T *value;
};
template <>
Vec<float,3>::Vec(float x, float y, float z) {
    value=new float[3];
    value[0]=x;
    value[1]=y;
    value[2]=z;
}

template<>
Vec<int,3>::Vec(int x,int y,int z) {
    value=new int[3];
    value[0]=x;
    value[1]=y;
    value[2]=z;
}

template<>
Vec<double,3>::Vec(double x, double y, double z) {
    value=new double[3];
    value[0]=x;
    value[1]=y;
    value[2]=z;
}

typedef Vec<float,2> Vec2f;
typedef Vec<float,3> Vec3f;
typedef Vec<int,2> Vec2i;
typedef Vec<int,3> Vec3i;
typedef Vec<double,2> Vec2d;
typedef Vec<double,3> Vec3d;
typedef Vec<int,4> Vec4i;
typedef Vec<float,4> Vec4f;
typedef Vec<double,4> Vec4d;


template<class T>
Vec<T,3> cross(const Vec<T,3>&l,const Vec<T,3>& r){
    T x=l[1]*r[2]-l[2]*r[1];
    T y=l[2]*r[0]-l[0]*r[2];
    T z=l[0]*r[1]-l[1]*r[0];
    return Vec<T,3>(x,y,z);
}

template<class T,int row,int col>
class Matrix{
public:
    Matrix(){
        value=new Vec<T,col>[row];
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                value[i][j]=0;
            }
        }
    }

    Matrix(const Matrix<T,row,col>&r){
        value=new Vec<T,col>[row];
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                value[i][j]=r[i][j];
            }
        }
    }

    ~Matrix(){
        delete[] value;
    }

    Vec<T,col>& operator[](int index){
        return value[index];
    }

    const Vec<T,col>&operator[](int idx)const{
        return value[idx];
    }

    Vec<T,col> Row(int index)const{
        return value[index];
    }

    Vec<T,row> Col(int index)const{
        T result[row];
        for(int i=0;i<row;++i){
            result[i]=value[i][index];
        }
        return Vec<T,row>(result);
    }

    void setCol(int index,const Vec<T,row>& l){
        assert(index<col);
        for(int i=0;i<row;++i){
            value[i][index]=l[i];
        }
    }

    void setCol(int index,T* l){
        assert(index<col);
        for(int i=0;i<row;++i){
            value[i][index]=l[i];
        }
    }

    void setRow(int index,const Vec<T,col>& l){
        assert(index<row);
        for(int i=0;i<col;++i){
            value[index][i]=l[i];
        }
    }

    void setRow(int index,T*l){
        assert(index<row);
        for(int i=0;i<col;++i){
            value[index][i]=l[i];
        }
    }

    Matrix<T,row,col> identify(){
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                if(i==j)
                    value[i][j]=1;
                else
                    value[i][j]=0;
            }
        }
        return *this;
    }
    friend Vec<T,col> operator*(const Vec<T,row>&l,const Matrix<T,row,col>& r){
        Vec<T,col> result;
        for(int i=0;i<col;++i){
            result[i]=l*r.Col(i);
        }
        return result;
    }

    Vec<T,row> operator*(const Vec<T,col>&r)const{
        Matrix<T,col,1> matrix;
        matrix.setCol(0,r);
        Matrix<T,row,1> result=(*this)*matrix;
        return result.Col(0);
    }

    template<int n>
    Matrix<T,row,n> operator*(const Matrix<T,col,n>& r)const{
        Matrix<T,row,n> result;
        for(int i=0;i<row;++i){
            for(int j=0;j<n;++j){
                result[i][j]=value[i]*(r.Col(j));
            }
        }
        return result;
    }

    Matrix<T,row,col> operator*(const T& l)const{
        Matrix<T,row,col> result;
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                result[i][j]=value[i][j]*l;
            }
        }
    }

    Matrix<T,row,col> operator/(const T& l)const{
        Matrix<T,row,col> result;
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                result[i][j]=value[i][j]/l;
            }
        }
        return result;
    }

    Matrix<T,row,col> operator+(const T&l)const{
        Matrix<T,row,col> result;
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                result[i][j]=value[i][j]+l;
            }
        }
        return result;
    }

    Matrix<T,row,col> operator-(const T&l)const{
        Matrix<T,row,col> result;
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                result[i][j]=value[i][j]-l;
            }
        }
        return result;
    }

    Matrix<T,row,col>& operator=(const Matrix<T,row,col>& r){
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                value[i][j]=r[i][j];
            }
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out,const Matrix<T,row,col>&r){
        for(int i=0;i<row;++i){
            for(int j=0;j<col;++j){
                out<<r[i][j];
                if(j!=col-1)
                    out<<" ";
            }
            out<<std::endl;
        }
        return out;
    }

    Matrix<T,col,row> transpose()const{
        Matrix<T,col,row> result;
        for(int i=0;i<col;++i){
            result.setRow(i,this->Col(i));
        }
        return result;
    }
private:
    Vec<T,col> *value;
};
typedef Matrix<float,3,3> Matrix3f;
typedef Matrix<float,4,4> Matrix4f;
typedef Matrix<int,3,3> Matrix3i;
typedef Matrix<int,4,4> Matrix4i;


void Rotate(Matrix4f& matrix,int xr,int yr,int zr){
    float x_axis=xr/360,y_axis=yr/360,z_axis=zr/360;
    Matrix4f temp;
    temp.identify();

}

void Translate(Matrix4f& matrix,float ox,float oy,float oz){
    Matrix4f temp;
    temp.identify();
    temp.setCol(3,Vec4f({ox,oy,oz,1}));
    matrix=matrix*temp;
}
void Translate(Matrix4f& matrix,Vec3f o){
    Matrix4f temp;
    temp.identify();
    temp.setCol(3,Vec4f({o[0],o[1],o[2],1}));
    matrix=matrix*temp;
}
void Translate(Matrix4i& matrix,int ox,int oy,int oz);



#endif //GRENDER_MATRIX_H
