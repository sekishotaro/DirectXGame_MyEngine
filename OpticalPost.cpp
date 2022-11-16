#include "OpticalPost.h"
#include "JsonLoader.h"
#include "Player.h"

std::vector<std::unique_ptr<Object3d>> OpticalPost::OpticalPosts;
Model* OpticalPost::modelOpticalPost = nullptr;

bool OpticalPost::drawFlag = false;

void OpticalPost::Initialize()
{
	modelOpticalPost = Model::LoadFromOBJ("opticalPost");
	XMFLOAT3 pos;

	for (int i = 0; i < JsonLoader::crystalColliderObjects.size(); i++)
	{
		std::unique_ptr<Object3d> objectOpticalPost;
		objectOpticalPost = Object3d::Create();
		objectOpticalPost->SetModel(modelOpticalPost);
		pos = JsonLoader::crystalColliderObjects[i].get()->GetPosition();
	
		pos.y += 100.0f;
		objectOpticalPost->SetPosition(pos);
		objectOpticalPost->SetScale({ 2.0f, 100.0f, 2.0f });
		OpticalPosts.push_back(std::move(objectOpticalPost));
	}
}

void OpticalPost::Update()
{
	//光の柱用の板ポリを常に自機に向ける
	XMFLOAT3 a, b;

	a = Player::GetPos();
	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		//クリスタルの位置
		b = OpticalPosts[i].get()->GetPosition();
		a.x += b.x;
		a.z += b.z;

		b.x += 1.0f;

		//長さ
		float vecLengA = (float)pow((a.x * a.x) + (a.z * a.z), 0.5);
		float vecLengB = (float)pow((b.x * b.x) + (b.z * b.z), 0.5);


		//内積
		float innerProduct = (a.x * b.x) + (a.z + b.z);

		float cos_sita = innerProduct / (vecLengA * vecLengB);

		float sita = acos(cos_sita);

		sita = sita * 180.0f / 3.1415f;
		OpticalPosts[i].get()->SetRotation({ 0, -sita,0 });
	}

	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		OpticalPosts[i].get()->Update();
	}
}

void OpticalPost::Draw()
{
	if (drawFlag != true) return;

	for (int i = 0; i < OpticalPosts.size(); i++)
	{
		OpticalPosts[i].get()->Draw();
	}
}

void OpticalPost::Finalize()
{
	OpticalPosts.clear();
	drawFlag = false;
}

void OpticalPost::Erase(int num)
{
	OpticalPosts.erase(OpticalPosts.begin() + num);
}
