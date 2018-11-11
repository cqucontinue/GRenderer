//
// Created by 田淙宇 on 2018/10/21.
//

#ifndef GRENDER_OBJECT_H
#define GRENDER_OBJECT_H

#include <vector>
#include <fstream>
#include <cstring>
#include <cmath>
#include "Matrix.h"
using std::vector;
using std::string;
using std::ifstream;
class Triangle{
public:
    Triangle(){}
    Triangle(const Vec3i& vl){
        vertex[0]=vl[0];
        vertex[1]=vl[1];
        vertex[2]=vl[2];
    }
    Triangle(int *v){
        vertex[0]=v[0];
        vertex[1]=v[1];
        vertex[2]=v[2];
    }
    Triangle(int a,int b,int c){
        vertex[0]=a;
        vertex[1]=b;
        vertex[2]=c;
    }

    int& operator[](int index){
        return vertex[index];
    }

    int vert(int index)const {
        return vertex[index];
    }
    int UV(int idx)const{
        return uv[idx];
    }
    int norm(int idx)const{
        return normal[idx];
    }

    void setUv(int a,int b,int c){
        uv[0]=a;
        uv[1]=b;
        uv[2]=c;
    }

    void setNormal(int a,int b,int c){
        normal[0]=a;
        normal[1]=b;
        normal[2]=c;
    }
private:
    //存储顶点序列
    int vertex[3];
    //存储uv坐标
    int uv[3];
    //存储法线坐标
    int normal[3];
};

class Object{
public:
    Object(const string& filename){
       ifstream in(filename,std::ios::in);
       if(!in.is_open()){
           std::cerr<<"read false";
           exit(0);
       }
       while(!in.eof())
       {
           string flag;
           in >>flag;
           float x,y,z;
           if(flag=="v") {
               in >> x >> y >> z;
               vertexList.emplace_back(Vec3f(x, y, z));
           }
           else if(flag=="vt") {
               in >> x >> y >> z;
               uvList.emplace_back(Vec3f(x, y, z));
           }
           else if(flag=="vn") {
               in >> x >> y >> z;
               normalList.emplace_back(Vec3f(x, y, z));
           }
           else if(flag=="f") {
               int v1, v2, v3, uv1, uv2, uv3, n1, n2, n3;
               in >> v1;
               in.ignore();
               in >> uv1;
               in.ignore();
               in >> n1;

               in >> v2;
               in.ignore();
               in >> uv2;
               in.ignore();
               in >> n2;

               in >> v3;
               in.ignore();
               in >> uv3;
               in.ignore();
               in >> n3;

               Triangle temp(v1, v2, v3);
               temp.setUv(uv1, uv2, uv3);
               temp.setNormal(n1, n2, n3);
               faceList.push_back(temp);
           }
       }
       in.close();
    }

    //返回第face个面的第i个顶点
    Vec3f vert(int face,int i)const{
        return vertexList[faceList[face].vert(i)-1];
    }
    //返回uv
    Vec3f uv(int face,int i)const{
        return uvList[faceList[face].UV(i)];
    }
    //返回法线
    Vec3f normal(int face,int i)const{
        return normalList[faceList[face].norm(i)];
    }
    //返回面的数量
    int length()const{
        return faceList.size();
    }

    bool uv_avaliable()const{
        return uvList.size()>0;
    }

    bool normal_avaliable()const{
        return normalList.size()>0;
    }
private:
    //顶点列表
    vector<Vec3f> vertexList;
    //三角面列表
    vector<Triangle> faceList;
    vector<Vec3f> uvList;
    vector<Vec3f>normalList;
};
#endif //GRENDER_OBJECT_H
