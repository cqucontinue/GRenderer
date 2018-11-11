//
// Created by 田淙宇 on 2018/10/21.
//

#ifndef GRENDER_RENDER_H
#define GRENDER_RENDER_H


#include <climits>
#include <limits>
#include "Object.h"
#include "Shader.h"
typedef unsigned int IUINT32;




typedef struct{
    int width;
    int height;
    IUINT32** framebuffer;
    float **z_buffer;
    IUINT32** texture;
    int tex_width;
    int tex_height;
    float max_u;
    float max_v;
    int render_state;   //渲染状态
    IUINT32 background;
    IUINT32 foreground;
}   device_t;



#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

#include "Color.h"


class Render{
public:
    //初始化设备，fb为外部帧缓存
    void initDevice(int width, int height, void *fb){
        //计算需要的空间
        int need = sizeof(void*) * (height * 2 + 1025) + width * height * 8;
        char *ptr = (char*)malloc(need + 64);
        char *framebuf, *zbuf;
        int j;
        assert(ptr);
        device.framebuffer = (IUINT32**)ptr;
        device.z_buffer = (float**)(ptr + sizeof(void*) * height);
        ptr += sizeof(void*) * height * 2;
        device.texture = (IUINT32**)ptr;
        ptr += sizeof(void*) * 1024;
        framebuf = (char*)ptr;
        zbuf = (char*)ptr + width * height * 4;
        ptr += width * height * 8;
        if (fb != NULL) framebuf = (char*)fb;
        for (j = 0; j < height; j++) {
            device.framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
            device.z_buffer[j] = (float*)(zbuf + width * 4 * j);
        }
        for(int i=0;i<height;++i)
        {
            for(int j=0;j<width;++j){
                device.z_buffer[i][j]=std::numeric_limits<float>::min();
            }
        }
        device.texture[0] = (IUINT32*)ptr;
        device.texture[1] = (IUINT32*)(ptr + 16);
        memset(device.texture[0], 0, 64);
        device.tex_width = 2;
        device.tex_height = 2;
        device.max_u = 1.0f;
        device.max_v = 1.0f;
        device.width = width;
        device.height = height;
        device.background = 0xffffff;
        device.foreground = 0;
        device.render_state = RENDER_STATE_WIREFRAME;
        shader.setViewPort(800,1000);
        shader.setCamera(Vec3f(0,0,3));
        shader.lookat(Vec3f(0,0,0),Vec3f(0,-1.0,0));
        shader.Persepective(-3);
        shader.viewPort();
        shader.updateMatrix();
    }
    //删除设备
    void deleteDevice(){}
    //清除设备
    void clearDevice(int mode){
        int y, x, height = device.height;
        for (y = 0; y < device.height; y++) {
            IUINT32 *dst = device.framebuffer[y];
            IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
            cc = (cc << 16) | (cc << 8) | cc;
            if (mode == 0) cc = device.background;
            for (x = device.width; x > 0; dst++, x--) dst[0] = cc;
        }
        for (y = 0; y < device.height; y++) {
            float *dst = device.z_buffer[y];
            for (x = device.width; x > 0; dst++, x--) dst[0] = std::numeric_limits<float>::min();
        }
    }
    //设置纹理
    void setTexture(void* bits,long pitch,int w,int h){}
    //清除zbuffer



    void Move_Up(){
        n-=0.5;
        shader.Persepective(n);
        shader.updateMatrix();
    }
    void Move_Down(){
        n+=0.5;
        shader.Persepective(n);
        shader.updateMatrix();
    }
    void Reset(){
        n=-3;
        shader.Persepective(n);
        shader.updateMatrix();
    }


    Vec3f vert(const Vec3f& v){
        return shader.vert(v);
    }
    void Line(const Vec3f& begin,const Vec3f& end,Color color=Black){
        Vec3f a,b;
        bool swap=false;
        if(std::abs(end[1]-begin[1])>std::abs(end[0]-begin[0]))
        {
            a[0]=begin[1];
            a[1]=begin[0];
            a[2]=begin[2];
            b[0]=end[1];
            b[1]=end[0];
            b[2]=end[2];
            swap=true;
        } else{
            a[0]=begin[0];
            a[1]=begin[1];
            a[2]=begin[2];
            b[0]=end[0];
            b[1]=end[1];
            b[2]=end[2];
        }
        if(b[0]<a[0])
            std::swap(a,b);

        int dy=b[1]-a[1];
        int dx=b[0]-a[0];
        float dz=(b[2]-a[2])/dx;
        int k=2*dy-dx;
        int y=a[1];
        float z=a[2];
        for(int x=a[0];x<b[0];++x,z+=dz){
            if(swap)
            {
                if(x<device.height&&x>=0&&y>=0&&y<device.width&&device.z_buffer[x][y]<z)
                {
                    setPixel(y,x,color);
                    device.z_buffer[x][y]=z;
                }
            }
            else
            {
                if(y<device.height&&x>=0&&y>=0&&x<device.width&&device.z_buffer[y][x]<z)
                {
                    setPixel(x,y,color);
                    device.z_buffer[y][x]=z;
                }
            }
            k+=2*std::abs(dy);
            if(k>dx)
            {
                y+=b[1]>a[1]?1:-1;
                k-=2*dx;
            }
        }
    }
private:
    void setPixel(int x,int y,Color color=Black){
        if (((IUINT32)x) < (IUINT32)device.width && ((IUINT32)y) < (IUINT32)device.height) {
            //因为坐标系反的
            device.framebuffer[y][x] = color.toIUINT();
        }
    }


    float n=3;
    device_t device;
    VertexShader shader;
};
#endif //GRENDER_RENDER_H
