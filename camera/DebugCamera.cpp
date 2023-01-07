#include "DebugCamera.h"
#include "Input.h"
#include "Player.h"
#include "SphereCollider.h"
#include "QueryCallback.h"
#include "CollisionManager.h"

using namespace DirectX;
float DebugCamera::dx = 0;
float DebugCamera::dy = 0;
float DebugCamera::dz = 0;
XMFLOAT3 DebugCamera::eye;
XMFLOAT3 DebugCamera::target;
XMFLOAT3 DebugCamera::distance = { 0, 5.0f, -10.0 };
float DebugCamera::rotaX = 270.0f;
float DebugCamera::rotaY = 70.0f;
float DebugCamera::dis = 10.0f;
std::unique_ptr<Object3d> DebugCamera::Object;
Model* DebugCamera::Model = nullptr;


DebugCamera::DebugCamera(int window_width, int window_height) : Camera(window_width, window_height)
{
	Model = Model::LoadFromOBJ("skydome");
	Object = Object3d::Create();
	Object->SetModel(Model);

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�������S�ɂ���
	Object->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));
	collider = Object->GetBaseCollider();
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	Object->SetBaseCollider(collider);
}

void DebugCamera::Update()
{
	Input::MousePos mpos = Input::GetInstance()->MousePosLoad();

	XMFLOAT3 cameraPos = Player::GetPos();
	//cameraPos.y += 10.0f;

	////�J�����̈ړ�
	//if (Input::GetInstance()->PushKey(DIK_UP)) { distance.y += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_DOWN)) { distance.y -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) { distance.x += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) { distance.x -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_E)) { distance.z += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_Z)) { distance.z -= 1.0f; }
	//if (Input::GetInstance()->RightStickIn(UP)) { distance.y += 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(DOWN)) { distance.y -= 1.0f; }
	//if (Input::GetInstance()->RightStickIn(RIGHT)) { distance.x += 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(LEFT)) { distance.x -= 1.0f; }
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER)) { distance.z += 1.0f; }
	//else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER)) { distance.z -= 1.0f; }

	//cameraPos.x += distance.x;
	//cameraPos.y += distance.y;
	//cameraPos.z += distance.z;

	if (Input::GetInstance()->PushKey(DIK_UP)) { rotaY -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rotaY += 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rotaX += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rotaX -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_E) && dis >= 5.0f ) { dis -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_Z) && dis <= 20.0f) { dis += 1.0f; }

	if(Input::GetInstance()->RightStickIn(UP) && rotaY > 5) { rotaY -= 1.0f; }
	else if (Input::GetInstance()->RightStickIn(DOWN) && rotaY < 175) { rotaY += 1.0f; }
	if (Input::GetInstance()->RightStickIn(RIGHT)) { rotaX += 1.0f; }
	else if (Input::GetInstance()->RightStickIn(LEFT)) { rotaX -= 1.0f; }
	if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER) && dis >= 5.0f) { dis -= 1.0f; }
	else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER) && dis <= 20.0f) { dis += 1.0f; }
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_SHOULDER)) { rotaX = Player::GEtTestFloatNum(); }

	float radiusX = rotaX * 3.14f / 180.0f;
	float radiusY = rotaY * 3.14f / 180.0f;

	//���ʍ��W�n
	cameraPos.y += dis * cos(radiusY);
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	//���R���C�_�[���擾
	Object->SetPosition(cameraPos);
	Object->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//�N�G���\�R�[���o�b�N�N���X
	class CameraQueryCallback :public QueryCallback
	{
	public:
		CameraQueryCallback(Sphere* sphere) : sphere(sphere) {};

		//�Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)
		{
			//���[���h�̏����
			const XMVECTOR up = { 0,1,0,0 };
			//�r�˕���
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//������Ɣr�˕����̊p�x����cos�l
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			//�n�ʔ��肵�����l�p�x
			const float threshold = cosf(XMConvertToRadians(45.0f));

			//�p�x���ɂ���ēV��܂��͒n�ʂƔ��肳�����̂�������
			if (-threshold < cos && cos < threshold)
			{
				//����r�� (�����o��)
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		//�N�G���\�Ɏg�p���鋅
		Sphere* sphere = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	//�N�G���\�R�[���o�b�N�̊֐��I�u�W�F�N�g
	CameraQueryCallback callback(sphereCollider);

	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//�����ɂ��r�˕�������
	cameraPos.x += callback.move.m128_f32[0];
	cameraPos.y += callback.move.m128_f32[1];
	cameraPos.z += callback.move.m128_f32[2];
	//�R���C�_�[�X�V
	Object->UpdateWorldMatrix();
	collider->Update();

	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	//ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	//�ڒn���ێ�
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
		&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
	{
		//onGround = true;
		cameraPos.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//�s��̍X�V�Ȃ�
		//FbxObject3d::Update();
	}

	SetEye(cameraPos);

	XMFLOAT3 targetPos = Player::GetPos();
	targetPos.y += 6.0f;

	Camera::SetTarget(targetPos);
	Camera::Update();
}

void DebugCamera::UpdateOnly()
{
	Camera::Update();
}
