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

    //全ての組み合わせについて総当たりチェック
    itA = colliders.begin();
    for (; itA != colliders.end(); ++itA)
    {
        itB = itA;
        ++itB;
        for (; itB != colliders.end(); ++itB)
        {
            BaseCollider* colA = *itA;
            BaseCollider* colB = *itB;

            //ともに球
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
    //走査用のイテレータ
    std::forward_list<BaseCollider*>::iterator it;
    //今までで最も近いコライダーを記録する為のイテレータ
    std::forward_list<BaseCollider*>::iterator it_hit;
    //今までで最も近いコライダーの距離を記録する変数
    float distance = maxDistance;
    //今までで最も近いコライダーとの交点を記録する変数
    XMVECTOR inter;
    //全てのコライダーと総当たりチェック
    it = colliders.begin();
    for (; it != colliders.end(); ++it)
    {
        BaseCollider* colA = *it;

        if (!(colA->attribute & attribute))
        {
            continue;
        }

        //球の場合
        if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
        {
            Sphere* sphere = dynamic_cast<Sphere*>(colA);
            float tempDistance;
            XMVECTOR tempInter;
            //当たらなければ除外
            if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
            //距離が最小でなければ除外
            if (tempDistance >= distance) continue;
            //今までで最も近いので記録を取る
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
    //最終的に何かに当たっていたら結果を書き込む
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

    //全てのコライダーと総当たりチェック
    it = colliders.begin();
    for (; it != colliders.end(); ++it)
    {
        BaseCollider* col = *it;

        //属性が合わなければスキップ
        if (!(col->attribute & attribute))
        {
            continue;
        }

        //球
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
        {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            XMVECTOR tempInter;
            XMVECTOR tempReject;
            if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

            //交点情報をセット
            QueryHit info;
            info.collider = col;
            info.inter = tempInter;
            info.object = col->GetObject3d();
            info.reject = tempReject;

            //クエリ―コールバック呼び出し
            if (!callback->OnQueryHit(info))
            {
                //戻り値がfalseの場合、継続せず終了
                return;
            }
        }

        //メッシュ
        if (col->GetShapeType() == COLLISIONSHAPE_MESH)
        {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            XMVECTOR tempInter;
            XMVECTOR tempReject;
            if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

            //交点情報をセット
            QueryHit info;
            info.collider = col;
            info.inter = tempInter;
            info.object = col->GetObject3d();
            info.reject = tempReject;

            //クエリ―コールバック呼び出し
            if (!callback->OnQueryHit(info))
            {
                //戻り値がfalseの場合、継続せず終了
                return;
            }
        }
    }
}
