#include "Phase3EventManager.h"

#include "GameManager.h"

const vector<wstring> Phase3EventManager::bossSpriteList =
{
	L"BrushPahse3Boss0",
	L"BrushPahse3Boss1",
	L"BrushPahse3Boss2",
	L"BrushPahse3Boss3",
	L"BrushPahse3Boss4",
	L"BrushPahse3Boss5",
	L"BrushPahse3Boss6"
};

const vector<wstring> Phase3EventManager::playerSpriteList =
{
	L"BrushPahse3Player0",
	L"BrushPahse3Player1",
	L"BrushPahse3Player2",
	L"BrushPahse3Player3",
	L"BrushPahse3Player4",
	L"BrushPahse3Player5",
	L"BrushPahse3Player6"
};

Phase3EventManager::Phase3EventManager() :
	eventDelay(3.0f),
	centerRadiusDecrease(40.0f),
	centerRadius(80.0f),
	bossFirstAttackCount(8),
	bossFirstAttackCycle(0.1f),
	bossAttackCycle(1.0f),
	playerNoHandicapCount(3),
	playerHandicap(4.0f),
	playerHitRate(0.3f),
	playerHitRateIncrease(0.02),
	playerHitRateMax(1.0f),
	playerHandicapDecrease(0.2),
	playerHandicapMin(2.0f),
	animationPlaySpeed(15.0f),
	gameManager(nullptr)
{
	
}

void Phase3EventManager::Awake()
{
	finalEvent = false;
	finalElapsed = 0.0f;
	playerAttackElapsed = 0.0f;
	elapsedTime = 0.0f;
	attackLayer = 300;

	imageWidth = 2500.0f;
	imageHeight = 500.0f;

	keyCount = 8;
	deathCount = 25;

	isGameOver = false;

	random_device rd;
	gen = mt19937(rd());
	dis = uniform_real_distribution<float>(0.0f, 1.0f);
}

void Phase3EventManager::Start()
{
	hithitObject = SceneManager::GetInstance().CreateGameObject(L"hithitObject");
	Image* hithitImage = hithitObject->AddComponent<Image>();
	hithitImage->SetLayer(999);
	hithitImage->SetTexture(L"UIHitHit");
	hithitObject->AddComponent<ZeldaUI>();
	hithitObject->GetTransform()->SetLocalPosition(Vector3D(0, -350));
	hithitObject->GetTransform()->SetLocalScale(Vector3D(1183, 258));
	hithitObject->SetActive(false);
}

void Phase3EventManager::Update()
{
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	elapsedTime += deltaTime;

	if (eventDelay > 0.0f)
	{
		if (eventDelay <= elapsedTime)
		{
			elapsedTime -= eventDelay;
			eventDelay = 0.0f;
		}

		return;
	}

	// Increase, Decrease
	centerRadius -= centerRadiusDecrease * deltaTime;
	playerHitRate += playerHitRateIncrease * deltaTime;
	playerHandicap -= playerHandicapDecrease * deltaTime;
	

	// Attack
	if (bossFirstAttackCount > 0)
	{
		if (elapsedTime >= bossFirstAttackCycle)
		{
			bossFirstAttackCount--;
			elapsedTime -= bossFirstAttackCycle;

			pair<Vector2D, float> randPosRot = CreateRandomPosRot();
			BossAttack(randPosRot.first, randPosRot.second);
		}

		return;
	}
	else if (bossFirstAttackCount == 0)
	{
		bossFirstAttackCount--;

		// 이미지 켜기
		hithitObject->SetActive(true);
	}

	if (bossAttackQueue.size() >= deathCount)
	{
		if (!isGameOver)
		{
			isGameOver = true;

			for (int i = 0; i < 50; i++)
			{
				pair<Vector2D, float> randPosRot = CreateRandomPosRot();
				BossAttack(randPosRot.first, randPosRot.second, 0.02f * i);
			}

			gameManager->ShowGameOverScene(2.0f);
		}

		return;
	}

	// removeBossImage
	for (auto iter = removeBossImageTimerList.begin(); iter != removeBossImageTimerList.end(); iter++)
	{
		(*iter) -= deltaTime;

		if (*iter < deltaTime)
		{

			iter = removeBossImageTimerList.erase(iter);

			assert(!bossAttackErasedQueue.empty());
			pair<SpriteAnimator*, Image*> bossAttack = bossAttackErasedQueue.front();
			bossAttackErasedQueue.pop();
			ReturnToPool(bossAttack);

			// erase이후 iter++하면서 무한루프 방지
			if (iter == removeBossImageTimerList.end()) break;
		}
	}

	if (!bossAttackQueue.empty() && elapsedTime >= bossAttackCycle)
	{
		elapsedTime -= bossAttackCycle;

		pair<Vector2D, float> randPosRot = CreateRandomPosRot();
		BossAttack(randPosRot.first, randPosRot.second);
	}

	if (Input::GetInstance().PressedKeyNow('Z'))
	{
		if (keyCount > 0)
		{
			keyCount--;
		}
		else if (keyCount == 0)
		{
			keyCount--;

			// 이미지 안보이게 하기
			hithitObject->SetActive(false);
		}

		if (playerNoHandicapCount > 0)
		{
			playerAttackElapsed += 1.0f;
			playerNoHandicapCount--;
		}
		else
		{
			playerAttackElapsed += 1.0f / max(playerHandicap, playerHandicapMin);
		}
	}

	while (!bossAttackQueue.empty() && playerAttackElapsed >= 1.0f)
	{
		playerAttackElapsed -= 1.0f;

		float rand = dis(gen);

		if (rand <= min(playerHitRate, playerHitRateMax))
		{
			PlayerAttack(true);
		}
		else
		{
			pair<Vector2D, float> randPosRot = CreateRandomPosRot();
			PlayerAttack(false, randPosRot.first, randPosRot.second);
		}
	}

	if (finalEvent == false && bossAttackQueue.empty())
	{
		finalElapsed += deltaTime;

		if (finalElapsed >= playerSpriteList.size() / animationPlaySpeed)
		{
			// 3페이즈 종료 이벤트
			finalEvent = true;

			gameManager->ShowEndingCutSceneEndEvent();
		}
	}
}

void Phase3EventManager::SetGameManager(GameManager* gameManager)
{
	this->gameManager = gameManager;
}

pair<Vector2D, float> Phase3EventManager::CreateRandomPosRot()
{
	int cnt = 500;

	pair<Vector2D, float> result;

	while (cnt > 0)
	{
		Vector2D randomPos = Vector2D(1920 * dis(gen) - 960.0f, 1080 * dis(gen) - 540.0f);
		float randomRot = 360 * dis(gen) - 180.0f;
		float radianRot = DegreeToRadian(randomRot);

		result.first = Vector2D(DotProduct(Vector2D(cosf(radianRot), sinf(radianRot)), randomPos), DotProduct(Vector2D(cosf(radianRot + 0.5f * PI), sinf(radianRot + 0.5f * PI)), randomPos));
		// 반시계방향으로 회전하기 때문에 -1을 곱하여 리턴
		result.second = -randomRot;

		float wX = randomPos.x;
		float wY = randomPos.y;

		float slope = tanf(radianRot);
		// 점과 직선사이의 거리 공식 이용
		float a = slope;	// x계수
		float b = -1.0f;	// y계수
		float c = -(slope * wX) + wY;	// 상수항

		float d = Abs(c) / sqrt(a * a + b * b);

		if ((centerRadius <= 0.0f) || (d > centerRadius + (imageHeight * 0.5f)))
		{
			// 화면의 양끝 부분의 중심을 위치로 변경한다.
			// 0.700207f == tan(35)
			if (Abs(slope) < 0.700207f)
			{
				Vector2D newPos = Vector2D(0.0f, -(slope * wX) + wY);
				Vector2D originRight = Vector2D(cosf(radianRot), sinf(radianRot));
				Vector2D originUp = Vector2D(cosf(radianRot + 0.5f * PI), sinf(radianRot + 0.5f * PI));

				result.first = Vector2D(DotProduct(originRight, newPos), DotProduct(originUp, newPos));
			}
			else
			{
				Vector2D newPos = Vector2D(((slope * wX) - wY) / slope, 0.0f);

				// slope == inf
				// atanf(1000) == 89.9427
				if (Abs(Abs(radianRot) - 90.0f) < 0.0573f || Abs(slope) > 1000.0f)
				{
					newPos = Vector2D(wX, 0.0f);
				}

				Vector2D originRight = Vector2D(cosf(radianRot), sinf(radianRot));
				Vector2D originUp = Vector2D(cosf(radianRot + 0.5f * PI), sinf(radianRot + 0.5f * PI));

				result.first = Vector2D(DotProduct(originRight, newPos), DotProduct(originUp, newPos));
			}

			return result;
		}

		cnt--;
	}
	
	assert(0);

	return result;
}

void Phase3EventManager::BossAttack(Vector2D pos, float rot, float delay)
{
	pair<SpriteAnimator*, Image*> bossAttack = PopFromPool();
	bossAttack.first->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(pos.x, pos.y));
	bossAttack.first->GetGameObject()->GetTransform()->SetLocalUIRotation(rot);
	bossAttack.first->SetTexture(bossSpriteList);
	bossAttack.first->SetPreDelay(delay);
	bossAttack.first->Play();
	bossAttackQueue.push(bossAttack);

	bossAttack.second->SetLayer(attackLayer);
	attackLayer++;
}

void Phase3EventManager::PlayerAttack(bool coverBossAttack, Vector2D pos, float rot)
{
	if (coverBossAttack)
	{
		pair<SpriteAnimator*, Image*> bossAttack = bossAttackQueue.front();
		bossAttackQueue.pop();

		Transform* bossAttackTransform = bossAttack.first->GetGameObject()->GetTransform();
		bossAttackErasedQueue.push(bossAttack);
		removeBossImageTimerList.push_back(bossSpriteList.size() / animationPlaySpeed);

		pair<SpriteAnimator*, Image*> playerAttack = PopFromPool();
		playerAttack.first->GetGameObject()->GetTransform()->SetLocalPosition(bossAttackTransform->GetLocalPosition());
		playerAttack.first->GetGameObject()->GetTransform()->SetLocalUIRotation(bossAttackTransform->GetLocalUIRotation());
		playerAttack.first->SetTexture(playerSpriteList);
		playerAttack.first->Play();

		playerAttack.second->SetLayer(attackLayer);
		attackLayer++;

		return;
	}

	pair<SpriteAnimator*, Image*> playerAttack = PopFromPool();
	playerAttack.first->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(pos.x, pos.y));
	playerAttack.first->GetGameObject()->GetTransform()->SetLocalUIRotation(rot);
	playerAttack.first->SetTexture(playerSpriteList);
	playerAttack.first->Play();

	playerAttack.second->SetLayer(attackLayer);
	attackLayer++;
}

pair<SpriteAnimator*, Image*> Phase3EventManager::PopFromPool()
{
	if (animatorPool.empty())
	{
		GameObject* object = SceneManager::GetInstance().CreateGameObject(L"Phase3Line");
		object->AddComponent<ZeldaUI>();
		Image* image = object->AddComponent<Image>();
		SpriteAnimator* animator = object->AddComponent<SpriteAnimator>();

		object->SetActive(false);
		animatorPool.push({ animator, image });
	}

	pair<SpriteAnimator*, Image*> rtnPair = animatorPool.front();
	animatorPool.pop();
	SpriteAnimator* rtn = rtnPair.first;

	rtn->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D::zero);
	rtn->GetGameObject()->GetTransform()->SetLocalScale(Vector3D(imageWidth, imageHeight));
	rtn->GetGameObject()->GetTransform()->SetLocalUIRotation(0.0f);
	rtn->GetGameObject()->SetActive(true);

	rtn->SetClearAfterPlayEnds(false);
	rtn->SetEventAnimation(false);
	rtn->SetFadeInBeforePlayBegins(false);
	rtn->SetFadeOutAfterPlayEnds(false);
	rtn->SetLoop(false);
	rtn->SetPlaySpeed(animationPlaySpeed);
	rtn->SetPlayWithAwake(false);
	rtn->SetPreDelay(0.0f);
	rtn->SetPostDelay(0.0f);
	rtn->SetRetainAfterPlayEnds(true);

	return rtnPair;
}

void Phase3EventManager::ReturnToPool(pair<SpriteAnimator*, Image*> obj)
{
	obj.first->GetGameObject()->SetActive(false);
	animatorPool.push(obj);
}

void Phase3EventManager::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Phase3EventManager);
	PRESERIALIZE_VALUE(eventDelay);
	PRESERIALIZE_VALUE(centerRadiusDecrease);
	PRESERIALIZE_VALUE(centerRadius);
	PRESERIALIZE_VALUE(bossFirstAttackCount);
	PRESERIALIZE_VALUE(bossFirstAttackCycle);
	PRESERIALIZE_VALUE(bossAttackCycle);
	PRESERIALIZE_VALUE(playerNoHandicapCount);
	PRESERIALIZE_VALUE(playerHandicap);
	PRESERIALIZE_VALUE(playerHitRate);
	PRESERIALIZE_VALUE(playerHitRateIncrease);
	PRESERIALIZE_VALUE(playerHitRateMax);
	PRESERIALIZE_VALUE(playerHandicapDecrease);
	PRESERIALIZE_VALUE(playerHandicapMin);
	PRESERIALIZE_VALUE(animationPlaySpeed);
}

void Phase3EventManager::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Phase3EventManager);
	PREDESERIALIZE_VALUE(eventDelay);
	PREDESERIALIZE_VALUE(centerRadiusDecrease);
	PREDESERIALIZE_VALUE(centerRadius);
	PREDESERIALIZE_VALUE(bossFirstAttackCount);
	PREDESERIALIZE_VALUE(bossFirstAttackCycle);
	PREDESERIALIZE_VALUE(bossAttackCycle);
	PREDESERIALIZE_VALUE(playerNoHandicapCount);
	PREDESERIALIZE_VALUE(playerHandicap);
	PREDESERIALIZE_VALUE(playerHitRate);
	PREDESERIALIZE_VALUE(playerHitRateIncrease);
	PREDESERIALIZE_VALUE(playerHitRateMax);
	PREDESERIALIZE_VALUE(playerHandicapDecrease);
	PREDESERIALIZE_VALUE(playerHandicapMin);
	PREDESERIALIZE_VALUE(animationPlaySpeed);
}

void Phase3EventManager::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(gameManager);
}

void Phase3EventManager::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(gameManager);
}
