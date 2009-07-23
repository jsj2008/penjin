#ifndef SKYBOX_H
#define SKYBOX_H

#include "Texture.h"
#include "PenjinTypes.h"
#include "PenjinErrors.h"
class SkyBox
{
    public:
        SkyBox();
        ~SkyBox();

        int loadTexture(CRint index, CRstring file);
        int loadTexture(CRstring file);
        //  Trys to automatically set the texture coords
        void setResolution();
        //  Manually set the texture coords to avoid borders
        void setResolution(CRuint textureResolution);
        //  Set the target that the SkyBox follows
        void setTarget(Vector3df* t){target = t;}
        //  render the skybox to the scene
        void render();

    private:
        Texture textures[6];
        int currentTexture;
        float TEX_MIN;
        float TEX_MAX;
        Vector3df* target;
};

#endif // SKYBOX_H
