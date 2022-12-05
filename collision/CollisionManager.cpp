#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
    static CollisionManager instance;
    return &instance;
}

void CollisionManager::CheckAllCollisions()
{
    std::forward_list<BaseCollider*>::iterator itA;
    std::forward_list<BaseCollider*>::iterator itB;

    //�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
    itA = colliders.begin();
    for (; itA != colliders.end(); ++itA)
    {
        itB = itA;
        ++itB;
        for (; itB != colliders.end(); ++itB)
        {
            BaseCollider* colA = *itA;
            BaseCollider* colB = *itB;

            //�Ƃ��ɋ�
            if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
            {
                Sphere* SphereA = dynamic_cast<Sphere*>(colA);
                Sphere* SphereB = dynamic_cast<Sphere*>(colB);
                DirectX::XMVECTOR inter;
                if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
                {
                    colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_MESH && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
            {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
                Sphere* sphere = dynamic_cast<Sphere*>(colB);
                DirectX::XMVECTOR inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter))
                {
                    colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
                    colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_MESH)
            {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
                Sphere* sphere = dynamic_cast<Sphere*>(colA);
                DirectX::XMVECTOR inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter))
                {
                    if (colA->object3d == nullptr)
                    {
                        colA->OnCollisionF(CollisionInfo(colB->GetObject3d(), colB, inter));
                    }
                    else
                    {
                        colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
                    }
                    
                    if (colB->object3d == nullptr)
                    {
                        colB->OnCollisionF(CollisionInfo(colB->GetObject3d(), colB, inter));
                    }
                    else
                    {
                        colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
                    }
                }
            }

        }
    }
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
    return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
    bool result = false;
    //�����p�̃C�e���[�^
    std::forward_list<BaseCollider*>::iterator it;
    //���܂łōł��߂��R���C�_�[���L�^����ׂ̃C�e���[�^
    std::forward_list<BaseCollider*>::iterator it_hit;
    //���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
    float distance = maxDistance;
    //���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
    XMVECTOR inter;
    //�S�ẴR���C�_�[�Ƒ�������`�F�b�N
    it = colliders.begin();
    for (; it != colliders.end(); ++it)
    {
        BaseCollider* colA = *it;

        if (!(colA->attribute & attribute))
        {
            continue;
        }

        //���̏ꍇ
        if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
        {
            Sphere* sphere = dynamic_cast<Sphere*>(colA);
            float tempDistance;
            XMVECTOR tempInter;
            //������Ȃ���Ώ��O
            if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
            //�������ŏ��łȂ���Ώ��O
            if (tempDistance >= distance) continue;
            //���܂łōł��߂��̂ŋL�^�����
            result = true;
            distance = tempDistance;
            inter = tempInter;
            it_hit = it;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_MESH)
        {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

            float tempDistance;
            DirectX::XMVECTOR tempInter;
            if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
            if (tempDistance >= distance) continue;

            result = true;
            distance = tempDistance;
            inter = tempInter;
            it_hit = it;
        }
    }
    //�ŏI�I�ɉ����ɓ������Ă����猋�ʂ���������
    if (result && hitInfo)
    {
        hitInfo->distance = distance;
        hitInfo->inter = inter;
        hitInfo->collider = *it_hit;
        hitInfo->object = hitInfo->collider->GetObject3d();
    }

    return result;
}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute)
{
    assert(callback);

    std::forward_list<BaseCollider*>::iterator it;

    //�S�ẴR���C�_�[�Ƒ�������`�F�b�N
    it = colliders.begin();
    for (; it != colliders.end(); ++it)
    {
        BaseCollider* col = *it;

        //����������Ȃ���΃X�L�b�v
        if (!(col->attribute & attribute))
        {
            continue;
        }

        //��
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
        {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            XMVECTOR tempInter;
            XMVECTOR tempReject;
            if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

            //��_�����Z�b�g
            QueryHit info;
            info.collider = col;
            info.inter = tempInter;
            info.object = col->GetObject3d();
            info.reject = tempReject;

            //�N�G���\�R�[���o�b�N�Ăяo��
            if (!callback->OnQueryHit(info))
            {
                //�߂�l��false�̏ꍇ�A�p�������I��
                return;
            }
        }

        //���b�V��
        if (col->GetShapeType() == COLLISIONSHAPE_MESH)
        {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            XMVECTOR tempInter;
            XMVECTOR tempReject;
            if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

            //��_�����Z�b�g
            QueryHit info;
            info.collider = col;
            info.inter = tempInter;
            info.object = col->GetObject3d();
            info.reject = tempReject;

            //�N�G���\�R�[���o�b�N�Ăяo��
            if (!callback->OnQueryHit(info))
            {
                //�߂�l��false�̏ꍇ�A�p�������I��
                return;
            }
        }
    }
}
