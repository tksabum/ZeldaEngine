#pragma once

namespace ZeldaEngine
{
	class IAnimator
	{
	public:
		// 애니메이션의 작동을 위해 주기적으로 호출되는 함수
		virtual void Execute() abstract;

	};
}