#include "pch.h"
#include "BattleManager.h"
#include "GameSceneManager.h"
#include "DiceDice.h"
#include "EventManager.h"
#include "HistoryEventAttack.h"
#include "DiceDice.h"

void BattleManager::BattleBetween(Character* attacker, Character* defender)
{
	//이것도 슬슬 개선해보자

	SetAttackFormation(attacker);
	SetDefenseFormation(defender);

	int flankBonus = std::abs(std::abs((attacker->getCurrentDirection() - defender->getCurrentDirection())) - 3);
	
	attacker->setAttackPower(flankBonus + attacker->getAttackPower());
	defender->RotateToDirection((DirectionKind)((attacker->getCurrentDirection() + 3) % MAX_DIRECTION));

	PlayerInfo playerAttacker = attacker->GetOwnerPlayer();
	PlayerInfo playerDefender = defender->GetOwnerPlayer();

	while (_CurrentAttackFormation.size() && _CurrentDefenseFormation.size())
	{
		GiveForestBonus(_CurrentAttackFormation.front());
		GiveForestBonus(_CurrentDefenseFormation.back());

		std::list<Character*> *winner = nullptr;
		std::list<Character*> *loser = nullptr;

		if (IsAttackerWin(_CurrentAttackFormation.front(), _CurrentDefenseFormation.front()))
			winner = &_CurrentAttackFormation;
		else
			winner = &_CurrentDefenseFormation;
		loser = (winner == &_CurrentAttackFormation) ? &_CurrentDefenseFormation : &_CurrentAttackFormation;

		EventManager::getInstance()->AddHistory(HistoryEventAttack::Create(_CurrentAttackFormation.front(), _CurrentDefenseFormation.front()));

		bool firstTime = true;

		if (firstTime)
			attacker->setAttackPower(2);

		DirectionKind tempDirection = DIRECTION_ERR;
		DirectionKind prevDirection = loser->front()->getCurrentDirection();
		GM->KillCharacter(loser->front());

		auto iter = loser->begin();
		++iter;
		
		for (; iter != loser->end(); ++iter)
		{
			(*iter)->MovoToTile((*iter)->getCurrentTile()->getNearTile((*iter)->getCurrentDirection()));
			tempDirection = (*iter)->getCurrentDirection();
			(*iter)->RotateToDirection(prevDirection);
			prevDirection = tempDirection;

			if (loser == &_CurrentDefenseFormation)
				break;
		}
		loser->pop_front();
	}

	std::list<Character*> finalWinnerForm = (_CurrentAttackFormation.size()) ? _CurrentAttackFormation : _CurrentDefenseFormation;
	PlayerInfo finalWinnerPlayer = finalWinnerForm.front()->GetOwnerPlayer();

	if (finalWinnerPlayer == playerAttacker)
	{
		Character* pIter = nullptr;
		Character*characterToMove = finalWinnerForm.front();

		characterToMove->MovoToTile(characterToMove->getCurrentTile()->getNearTile(characterToMove->getCurrentDirection()));

		DirectionKind tempDirection = DIRECTION_ERR, prevDirection = characterToMove->getCurrentDirection();
		for (auto iter = ++(finalWinnerForm.begin()); iter != finalWinnerForm.end(); ++iter)
		{
			pIter = *iter;
			pIter->MovoToTile(pIter->getCurrentTile()->getNearTile(pIter->getCurrentDirection()));
			tempDirection = pIter->getCurrentDirection();
			pIter->RotateToDirection(prevDirection);
			prevDirection = tempDirection;
		}
	}
}

bool BattleManager::IsAttackerWin(Character* attacker, Character* defender)
{
	while (true)
	{
		int attackerDice = DiceDice::getInstance()->RollDiceBetween(1, attacker->_AttackPower);
		int defenderDice = DiceDice::getInstance()->RollDiceBetween(1, defender->_AttackPower);

		if (attackerDice == defenderDice)
			continue;
		return (attackerDice > defenderDice) ? true : false;
	}
}

void BattleManager::SetAttackFormation(Character* attacker)
{
	std::list<Character*> tempList;
	_CurrentAttackFormation.clear();
	SearchGraphAndOverwriteAttackFormation(tempList, attacker, 1,0);
}

void BattleManager::SetDefenseFormation(Character* defender)
{
	_CurrentDefenseFormation.clear();
	_CurrentDefenseFormation.push_back(defender);

	for (int i = DIRECTION_DOWN_LEFT; i < MAX_DIRECTION; ++i)
	{
		Character* nearby = defender->GetNearCharacter((DirectionKind)i);
		
		///# shared_ptr은 isnull() 같은거로 널 체크.
		
			if (nearby != nullptr && IsCharFacingMe(defender, nearby) && nearby->GetOwnerPlayer() == defender->GetOwnerPlayer())
				_CurrentDefenseFormation.push_back(nearby);

		
	}
}

bool isHave(std::list<Character*> *checkedNode, Character* node)
{
	for (auto iter : *checkedNode)
	{
		if (iter == node)
			return true;
	}
	return false;
}

int BattleManager::SearchGraphAndOverwriteAttackFormation(std::list<Character*> checkedNode, Character* currentNode, int currentDepth, int maxDepth)
{
	checkedNode.push_back(currentNode);

	if (maxDepth <= currentDepth)
	{
		this->_CurrentAttackFormation = checkedNode;
		maxDepth = currentDepth;
	}

	//왼쪽 아래부터 반시계방향으로 모든 방향을 순회
	for (int i = DIRECTION_DOWN_LEFT; i < MAX_DIRECTION; ++i)
	{
		//i방향으로 인접한 캐릭터
		Character* compareNode = currentNode->GetNearCharacter(static_cast<DirectionKind>(i));

		if (compareNode != nullptr)
			//해당 캐릭터가 자신을 바라보고 있으면
			if (compareNode->GetNearCharacter(compareNode->getCurrentDirection()) == currentNode && compareNode->GetOwnerPlayer() == currentNode->GetOwnerPlayer())
				//이미 탐색했던 node가 아니면
				if (!isHave(&checkedNode, compareNode))
					maxDepth = SearchGraphAndOverwriteAttackFormation(checkedNode, compareNode, currentDepth + 1, maxDepth);
	}

	return maxDepth;
}

void BattleManager::GiveForestBonus(Character* target)
{
	if (target->getCurrentTile()->getTypeOfTile() == TILE_FOREST)
		target->setAttackPower(target->getAttackPower() + 1);
}

bool BattleManager::IsCharFacingMe(Character* me, Character* other)
{
	if (other->GetNearCharacter(other->getCurrentDirection()) == me)
		return true;
	return false;
}