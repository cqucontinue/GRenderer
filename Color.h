//
// Created by 田淙宇 on 2018/10/24.
//

#ifndef GRENDER_COLOR_H
#define GRENDER_COLOR_H

#include "Render.h"

class Color{
public:
    Color(float r,float g,float b,float a=1.0):r(r),g(g),b(b),a(a){
    }
    Color(float*color){
        r=color[0];
        g=color[1];
        b=color[2];
        a=color[3];
    }

    float& R(){
        return r;
    }
    float& G(){
        return g;
    }
    float& B(){
        return b;
    }
    float& A(){
        return a;
    }
    IUINT32 toIUINT()const{
        int r0=255*r;
        int g0=255*g;
        int b0=255*b;
        IUINT32 result=(r0<<16)+(g0<<8)+(b0);
        return result;
    }
private:
    float r,g,b,a;
};

#define White Color(1.0,1.0,1.0)
#define Black Color(0,0,0)
#define Green Color(0,1.0,0)
#define Red Color(1.0,0,0)
#define Blue Color(0,0,1.0)



#endif //GRENDER_COLOR_H
