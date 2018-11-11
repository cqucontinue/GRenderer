#include <iostream>
#include "Matrix.h"
#include "Object.h"
#include "Painter.h"
#include "Render.h"

void renderObject(const Object& obj,Render *r){
    for(int i=0;i<obj.length()-1;++i)
    {
        Vec3f a[3];
        for(int j=0;j<3;++j){
            a[j]=r->vert(obj.vert(i,j));
        }
        r->Line(a[0],a[1],Green);
        r->Line(a[1],a[2],Green);
        r->Line(a[2],a[0],Green);
    }
}
int main(int argc,char**argv) {
    Object object(argv[1]);



    Render renderer;
    RenderDevice device;
    TCHAR *title=_T("test");
    int result=device.initWindow(800,1000,title);
    std::cout<<result<<std::endl;
    renderer.initDevice(800,1000,device.frameBuffer());
    while(!device.exit())
    {
        device.dispatchWindow();
        renderer.clearDevice(0);
        if(device.keyState(VK_UP))
            renderer.Move_Up();
        else if(device.keyState(VK_DOWN))
            renderer.Move_Down();
        else if(device.keyState(VK_SPACE))
            renderer.Reset();
        renderObject(object,&renderer);
        device.updateWindow();
        Sleep(1);
    }
    return 0;
}