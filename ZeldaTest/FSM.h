#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>


/// <summary>
/// FSM
/// 
/// ������Ʈ���� ���¸� ��ȯ�Ҷ� ���
/// ���ø����� ����� �� ������Ʈ�� ������� Ŭ������ �ƴ϶�
/// ��������� �־� ���������� ��밡���ϴ�.
/// 
/// �ʿ��� ������Ʈ�� ���¸� �޾�
/// </summary>
/// <typeparam name="StateEnum"></typeparam>
template <typename StateEnum>
class FSM
{
public:
	typedef std::function<bool()> Condition;
	typedef std::function<void()> Action;
	typedef std::vector<std::pair<StateEnum, Condition>> TransitionList;

	// ���� ��ȯ
	std::unordered_map<StateEnum, TransitionList> transitions;
	// ���� ����
	std::unordered_map<StateEnum, Action> engageAction;
	// ���� ����
	std::unordered_map<StateEnum, Action> updateAction;

	// ������� enum
	StateEnum currentState;
	// �������� enum
	StateEnum previousState;

	// ������
	// �Ű������� ���� ���¸� �޾� �ʱ�ȭ�صд�.
	FSM(StateEnum initialState)
		: currentState(initialState), previousState(initialState)
	{
		// typename���� ���� StateEnum ���� ��� enum�� �׼��� �ʱ�ȭ���ش�.
		for (unsigned int state = 0; state < static_cast<unsigned int>(StateEnum::StateEnd); state++)
		{
			transitions[static_cast<StateEnum>(state)] = TransitionList();
			engageAction[static_cast<StateEnum>(state)] = []() {};
			updateAction[static_cast<StateEnum>(state)] = []() {};
		}
	}

	// update()�� �־ �ֱ������� ��� ������Ʈ ���ش�.
	void UpdateState();
};

template <typename StateEnum>
void FSM<StateEnum>::UpdateState()
{
	bool engaged = false;
	auto& transitionList = transitions[currentState];

	for (auto eachTransition : transitionList)
	{
		// TransitionList�� pair���� �ι�° bool�Լ��� true�� ��� ���¸� �ٲ۴�.
		if (eachTransition.second())
		{
			previousState = currentState;
			currentState = eachTransition.first;
			engaged = true;
			break;
		}
	}

	// �ٲۻ��·� ������ ��
	if (engaged == true)
	{
		engageAction[currentState]();
	}

	// ���°� ������ ��
	updateAction[currentState]();
}

