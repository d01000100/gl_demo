#pragma once
#include "eBodyType.h"

namespace phys
{
	class iCollisionBody
	{
	private:
		eBodyType mBodyType;

		// Constructors not to be used.
		iCollisionBody() = delete;
		iCollisionBody(const iCollisionBody& other) = delete;
		iCollisionBody& operator=(const iCollisionBody& other) = delete;
	protected:
		iCollisionBody(const eBodyType& type) :
			mBodyType(type) {}
	public:
		virtual ~iCollisionBody() = default;
		/*
		 * Used by cWorld post integration
		 */
		virtual void ClearAccelerations() = 0;
		/*
		 * Returns the body type of the specific body
		 */
		eBodyType& GetBodyType() { return mBodyType; };
	};
}
