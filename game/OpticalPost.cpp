#include "OpticalPost.h"
#include "JsonLoader.h"
#include "Player.h"
#include "stdlib.h"
#include "SafeDelete.h"

//std::vector<std::unique_ptr<Object3d>> OpticalPost::OpticalPosts;
//std::vector<std::unique_ptr<Object3d>> OpticalPost::smallOpticalPosts;
//std::unique_ptr<Object3d> OpticalPost::goalOpticalPostObject;
//Model* OpticalPost::modelOpticalPost = nullptr;
//Model* OpticalPost::modelCrystalEffect = nullptr;
//std::unique_ptr<Object3d> OpticalPost::CrystalEffectObject;
//std::vector<float> OpticalPost::moveQuantitys;
//
//bool OpticalPost::drawFlag = false;
//float OpticalPost::num = 0.0f;
//int OpticalPost::effectNum = 7;
//float OpticalPost::sizeNum = 3.0f;
//float OpticalPost::moveQuantityMax = 3.0f;
//const int OpticalPost::smallOpticalPostNum = 20;
void OpticalPost::Initialize()
{
	modelOpticalPost = Model::LoadFromOBJ("opticalPost");
	modelCrystalEffect = Model::LoadFromOBJ("CrystalSurroundingEffect");
	XMFLOAT3 pos;
	//光の柱1本に対する小さな光の柱の個数

	for (int i = 0; i < JsonLoader::crystalObjects.size(); i++)
	{
		std::unique_ptr<Object3d> objectOpticalPost;
		objectOpticalPost = Object3d::Create();
		objectOpticalPost->SetModel(modelOpticalPost);
		pos = JsonLoader::crystalObjects[i].get()->GetPosition();
		
		pos.y += 100.0f;
		objectOpticalPost->SetPosition(pos);
		objectOpticalPost->SetScale({ 2.0f, 100.0f, 2.0f });
		OpticalPosts.push_back(std::move(objectOpticalPost));
	
		pos.y -= 90.0f;
		XMFLOAT3 posA = {};
		for (int i = 0; i < smallOpticalPostNum; i++)
		{
			std::unique_ptr<Object3d> smallObjectOpticalPost;
			smallObjectOpticalPost = Object3d::Create();
			smallObjectOpticalPost->SetModel(modelOpticalPost);
			posA = pos;
			posA.x += 2.0f - (float)(rand() % 4);
			posA.y += (float)(rand() % 100);
			posA.z += 2.0f - (float)(rand() % 4);
			smallObjectOpticalPost->SetPosition(pos);
			smallObjectOpticalPost->SetScale({ 0.05f, 2.0f, 0.05f });
			smallOpticalPosts.push_back(std::move(smallObjectOpticalPost));

			moveQuantitys.push_back(moveQuantityMax);
		}
	}

	goalOpticalPostObject = Object3d::Create();
	goalOpticalPostObject->SetModel(modelOpticalPost);
	goalOpticalPostObject->SetScale({ 5.0f, 100.0f, 5.0f });

	CrystalEffectObject = Object3d::Create();
	CrystalEffectObject->SetModel(modelCrystalEffect);
	CrystalEffectObject->SetScale({ 3.0f, 3.0f, 3.0f });
}

void OpticalPost::Update(const XMFLOAT3& cameraPos)
{
	////光の柱用の板ポリを常に自機に向ける

	//光の柱の法線
	XMFLOAT3 normal = { -1, 0, 0 };

	//ベクトル二つ
	float sita = 270;

	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		sita = RotaUpdate(cameraPos, OpticalPosts[i].get()->GetPosition());

		OpticalPosts[i].get()->SetRotation({ 0, sita,0 });
	}

	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		OpticalPosts[i].get()->Update();
	}
	
	//小さな光の柱
	for (int i = 0; i < smallOpticalPosts.size(); i++)
	{
		sita = RotaUpdate(cameraPos, smallOpticalPosts[i].get()->GetPosition());

		smallOpticalPosts[i].get()->SetRotation({ 0, sita,0 });
	}
	
	SmallOpticalPostsMoveUpdate();
	for (int i = 0; i < smallOpticalPosts.size(); i++)
	{
		smallOpticalPosts[i].get()->Update();
	}

	//クリスタル周囲
	int nearCrystalNum = NearCrystalNum();
	float CrystalEffectObjectRota = RotaUpdate(cameraPos, OpticalPosts[nearCrystalNum].get()->GetPosition());
	CrystalEffectObject->SetRotation({ 0, CrystalEffectObjectRota, 0 });
	CrystalEffectObject->SetPosition(JsonLoader::crystalObjects[nearCrystalNum].get()->GetPosition());
	CrystalEfectSizeNumUpdate();
	CrystalEffectObject->SetScale({ sizeNum, sizeNum, sizeNum });
	CrystalEffectObject->Update();

	//ゴールの処理
	if (JsonLoader::crystalObjects.size() != 0) return;
	sita = RotaUpdate(cameraPos, goalOpticalPostObject.get()->GetPosition());
	goalOpticalPostObject.get()->SetRotation({ 0, sita,0 });
	goalOpticalPostObject.get()->Update();
}

void OpticalPost::Draw()
{
	if (OpticalPosts.size() != 0)
	{
		CrystalEffectObject->Draw();
	}

	if (drawFlag != true) return;

	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		OpticalPosts[i].get()->Draw();
	}

	for (int i = 0; i < smallOpticalPosts.size(); i++)
	{
		smallOpticalPosts[i].get()->Draw();
	}

	if (JsonLoader::crystalObjects.size() != 0) return;
	goalOpticalPostObject->Draw();
}

void OpticalPost::Finalize()
{
	OpticalPosts.clear();
	smallOpticalPosts.clear();
	drawFlag = false;
}

void OpticalPost::Erase(bool getFlag, int num)
{
	if (getFlag == false) return;

	OpticalPosts.erase(OpticalPosts.begin() + num);

	for (int i = 0; i < smallOpticalPostNum; i++)
	{
		smallOpticalPosts.erase(smallOpticalPosts.begin() + ((num * smallOpticalPostNum)));
	}
}

float OpticalPost::RotaUpdate(const XMFLOAT3& cameraPos, const XMFLOAT3& crystalPos)
{
	XMFLOAT3 normal = { -1, 0, 0 };
	//ベクトル二つ
	float sita = 270;

	//クリスタル周辺エフェクト
	XMFLOAT3 crystal_CameraPos = { cameraPos.x, 0, cameraPos.z };
	XMFLOAT3 crystal_normal = { normal.x, 0, normal.z };
	////各取得アイテムの位置をベクトルに
	crystal_CameraPos.x -= crystalPos.x;
	crystal_CameraPos.z -= crystalPos.z;

	//ベクトルの長さ
	float vecLengA = (float)pow((crystal_CameraPos.x * crystal_CameraPos.x) + (crystal_CameraPos.z * crystal_CameraPos.z), 0.5);
	float vecLengB = (float)pow((crystal_normal.x * crystal_normal.x) + (crystal_normal.z * crystal_normal.z), 0.5);
	//内積
	float innerProduct = (crystal_CameraPos.x * crystal_normal.x) + (crystal_CameraPos.z * crystal_normal.z);

	float cos_sita = innerProduct / (vecLengA * vecLengB);

	sita = acos(cos_sita);

	sita = sita * 180.0f / 3.1415f;

	//度数に変換

	if (crystalPos.z > cameraPos.z)
	{
		sita = 360.0f - sita;
	}

	return sita;
}

int OpticalPost::NearCrystalNum()
{
	XMFLOAT3 playerPos = Player::GetPos();
	XMFLOAT3 crystalPos = {};
	float length = 0.0f;
	float minLength = 0.0f;
	int minLengthCrystalNum = 0;
	for (int i = 0; i < JsonLoader::crystalObjects.size(); i++)
	{
		crystalPos = JsonLoader::crystalObjects[i].get()->GetPosition();

		float X = (crystalPos.x - playerPos.x) * (crystalPos.x - playerPos.x);
		float Y = (crystalPos.y - playerPos.y) * (crystalPos.y - playerPos.y);
		float Z = (crystalPos.z - playerPos.z) * (crystalPos.z - playerPos.z);

		length = sqrtf(X + Y + Z);

		if (i != 0)
		{
			if (length < minLength)
			{
				minLength = length;
				minLengthCrystalNum = i;
			}
		}
		else
		{
			minLength = length;
			minLengthCrystalNum = 0;
		}
	}

	return minLengthCrystalNum;
}

void OpticalPost::CrystalEfectSizeNumUpdate()
{
	if (sizeNum >= 2.0f)
	{
		sizeNum = 0.01f;
	}
	else
	{
		sizeNum += 0.05f;
	}
}

void OpticalPost::SmallOpticalPostsMoveUpdate()
{
	XMFLOAT3 pos = {};

	for (int i = 0; i < JsonLoader::crystalObjects.size(); i++)
	{
		for (int j = 0; j < smallOpticalPostNum; j++)
		{
			//上昇処理
			int crystalNumA = (i * smallOpticalPostNum) + j;
			pos = smallOpticalPosts[crystalNumA].get()->GetPosition();
			pos.y += 0.05f;
			moveQuantitys[crystalNumA] += 0.05f;
			smallOpticalPosts[crystalNumA].get()->SetPosition(pos);
			//上昇終了しきった時の位置を戻す
			if (moveQuantityMax > moveQuantitys[crystalNumA]) continue;

			pos = JsonLoader::crystalObjects[i].get()->GetPosition();
			pos.x += 2.0f - (float)(rand() % 4);
			pos.y += (float)(float)(rand() % 100);;
			pos.z += 2.0f - (float)(rand() % 4);
			moveQuantitys[crystalNumA] = 0.0f;
			smallOpticalPosts[crystalNumA].get()->SetPosition(pos);
		}
	}

}

void OpticalPost::Restart()
{
	OpticalPosts.clear();
	smallOpticalPosts.clear();


	XMFLOAT3 pos;
	//光の柱1本に対する小さな光の柱の個数

	for (int i = 0; i < JsonLoader::crystalObjects.size(); i++)
	{
		std::unique_ptr<Object3d> objectOpticalPost;
		objectOpticalPost = Object3d::Create();
		objectOpticalPost->SetModel(modelOpticalPost);
		pos = JsonLoader::crystalObjects[i].get()->GetPosition();

		pos.y += 100.0f;
		objectOpticalPost->SetPosition(pos);
		objectOpticalPost->SetScale({ 2.0f, 100.0f, 2.0f });
		OpticalPosts.push_back(std::move(objectOpticalPost));

		pos.y -= 90.0f;
		XMFLOAT3 posA = {};
		for (int i = 0; i < smallOpticalPostNum; i++)
		{
			std::unique_ptr<Object3d> smallObjectOpticalPost;
			smallObjectOpticalPost = Object3d::Create();
			smallObjectOpticalPost->SetModel(modelOpticalPost);
			posA = pos;
			posA.x += 2.0f - (float)(rand() % 4);
			posA.y += (float)(rand() % 100);
			posA.z += 2.0f - (float)(rand() % 4);
			smallObjectOpticalPost->SetPosition(pos);
			smallObjectOpticalPost->SetScale({ 0.05f, 2.0f, 0.05f });
			smallOpticalPosts.push_back(std::move(smallObjectOpticalPost));

			moveQuantitys.push_back(moveQuantityMax);
		}
	}
}
