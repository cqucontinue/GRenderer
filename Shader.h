//
// Created by 田淙宇 on 2018/10/25.
//

#ifndef GRENDER_SHADER_H
#define GRENDER_SHADER_H

#include "Matrix.h"


#define G_INFINITE -0xffffffff
//顶点着色器
class VertexShader{
public:
    VertexShader(){
        curMatrix.identify();
        modelView.identify();
        v.identify();
        presepective.identify();
    }
    Vec3f vert(const Vec3f& r){
        Vec4f temp({r[0],r[1],r[2],1});
        Vec4f result=curMatrix*temp;
        return Vec3f(result[0]/result[3],result[1]/result[3],result[2]/result[3]);
    }

    void setViewPort(int w,int h){
        width=w;
        height=h;
    }
    void setCamera(const Vec3f& c){
        camera=c;
    }
    //刷新变换矩阵
    void updateMatrix(){
        curMatrix=v*presepective*modelView;
    }
    //转为屏幕坐标的矩阵
    void viewPort(){
        v.identify();
        v[0][0]=width/2;
        v[0][3]=width/2;
        v[1][1]=height/2;
        v[1][3]=height/2;
    }
    //模型坐标变换矩阵，从世界坐标变为视口坐标
    void lookat(const Vec3f& center,const Vec3f& up){
        modelView.identify();
        Vec3f z=(camera-center).normalize();
        Vec3f x=cross(up,z).normalize();
        Vec3f y=cross(z,x).normalize();
        //设置坐标系旋转
        for(int i=0;i<3;++i){
            modelView[0][i]=x[i];
            modelView[1][i]=y[i];
            modelView[2][i]=z[i];
        }
        //设置坐标系平移
        Translate(modelView,-center);
    }
    //透视投影矩阵
    void Persepective(float n/*近锥面*/,float f=G_INFINITE/*远锥面*/){
        bool infinite=f==G_INFINITE;
        presepective.identify();
        presepective.setCol(2,Vec4f({0,0,infinite?-1/n:-(float)(f+n)/(n*(f-n)),1/n}));
        presepective[2][3]=infinite?2:(float)(2*n*f)/(n*(f-n));
    };
    void Persepective(float l/*左边界*/,float r/*右边界*/,float b/*下边界*/,float t/*上边界*/,float n,float f=G_INFINITE){
        bool infinite=f==G_INFINITE;
        presepective.identify();
        presepective[0][0]=2*n/(r-l);
        presepective[1][1]=2*n/(t-b);
        presepective.setCol(2,Vec4f({(r+l)/(r-l),(t+b)/(t-b),infinite?-1/n:-(float)(f+n)/(n*(f-n)),1/n}));
        presepective[2][3]=infinite?2:(float)(2*n*f)/(n*(f-n));
    }

    void Ortho();

private:
    int width,height;
    Matrix4f presepective,modelView,v;
    //当前用于控制的变换矩阵
    Matrix4f curMatrix;
    Vec3f camera;
};

#endif //GRENDER_SHADER_H
