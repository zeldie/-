#pragma once

#ifndef __EXTERN_H__
//////////////////////�ǵ����̸� ���� ���ô�(�����Ұ��� �̷��� ����)/////////////////////////////
extern DOUBLE g_dAccumulatedTime;
extern _bool  g_RenderCollider; //F2 ������
extern _bool  g_RenderNaviMesh; // F3������
//�Ʒ��� �� ���� ����, Test Code - �α� -
class CLightCamera;
extern CLightCamera* m_pLightCamera;
extern _vec3 vTest;
extern _double temp;
extern float Gamma;
extern _vec3 vCamPos;
#define __EXTERN_H__
#endif // __EXTERN_H__