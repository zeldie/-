#pragma once

#ifndef __EXTERN_H__
//////////////////////되도록이면 쓰지 맙시다(렌더할건지 이런거 제외)/////////////////////////////
extern DOUBLE g_dAccumulatedTime;
extern _bool  g_RenderCollider; //F2 누르면
extern _bool  g_RenderNaviMesh; // F3누르면
//아래들 다 지울 에정, Test Code - 인균 -
class CLightCamera;
extern CLightCamera* m_pLightCamera;
extern _vec3 vTest;
extern _double temp;
extern float Gamma;
extern _vec3 vCamPos;
#define __EXTERN_H__
#endif // __EXTERN_H__