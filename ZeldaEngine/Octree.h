#pragma once

#include "Vector3D.h"

#include <vector>
#include <unordered_map>

namespace ZeldaEngine
{
	template<typename DataType>
	class Octree
	{
		// Struct
	private:
		struct DataSet
		{
			ZeldaMath::Vector3D point;
			float radius;
			DataType* dataPtr = nullptr;
		};

		struct OctreeNode
		{
			ZeldaMath::Vector3D minPoint;
			ZeldaMath::Vector3D maxPoint;

			float maxRadius;
			OctreeNode* parentNode;
			std::vector<DataSet> points;
			OctreeNode* children[8];

			OctreeNode(ZeldaMath::Vector3D minpoint, ZeldaMath::Vector3D maxpoint, OctreeNode* parent = nullptr)
				: minPoint(minpoint), maxPoint(maxpoint), maxRadius(0.0f), parentNode(parent)
			{
				for (int i = 0; i < 8; i++)
				{
					children[i] = nullptr;
				}
			}

			// 완전 이진트리에서 데이터가 들어있지 않은 곳은 굳이 만들어 두지 않는 것으로 생각하여 points가 비어 있을 경우 리프노드로 취급하지 않음
			bool IsLeafNode() const {
				return points.size() > 0;
			}
		};

	public:
		Octree(ZeldaMath::Vector3D minpoint, ZeldaMath::Vector3D maxpoint, ZeldaMath::Vector3D minareasize = ZeldaMath::Vector3D::zero)
			: root(new OctreeNode(minpoint, maxpoint)), rootMinPoint(minpoint), rootMaxPoint(maxpoint), minAreaSize(minareasize), autoRadiusUpdate(true)
		{

		}

		~Octree()
		{
			DestroyRecursive(root);
		}

		// 데이터 추가
		void Insert(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr)
		{
			InsertRecursive(center, radius, dataPtr, root);
		}

		// 데이터 제거 (확실하게 데이터를 제거할 수 있도록 center와 radius와 관계없이 삭제하는 함수를 만들거나 삭제성공 여부를 반환하도록 수정하면 좋을듯...)
		void Remove(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr)
		{
			RemoveRecursive(center, radius, dataPtr, root);
		}

		void Remove(DataType* dataPtr)
		{
			assert(nodeTable.count(dataPtr) == 1); // 없는 데이터를 삭제하려 함
			OctreeNode* node = nodeTable[dataPtr];

			for (size_t i = 0; i < node->points.size(); i++)
			{
				if (node->points[i].dataPtr == dataPtr)
				{
					node->points.erase(node->points.begin() + i);
					assert(nodeTable.count(dataPtr) != 0);
					nodeTable.erase(dataPtr);
					if (autoRadiusUpdate)
					{
						UpdateRadiusRecursive(node);
					}

					ClearLeafNode(node);
					return;
				}
			}

			// 못찾았으면 문제가 있는 것임
			assert(0);
			return;
		}

		// 이동
		void Move(const ZeldaMath::Vector3D& position, float radius, DataType* dataPtr)
		{
			assert(nodeTable.count(dataPtr) == 1); // 없는 데이터를 이동하려 함
			OctreeNode* node = nodeTable[dataPtr];

			bool isIntersects = Intersects(position, position, node->minPoint, node->maxPoint);

			if (isIntersects)
			{
				// 현재 노드라면 바로 이동
				for (size_t i = 0; i < node->points.size(); i++)
				{
					if (node->points[i].dataPtr == dataPtr)
					{
						node->points[i].point = position;
						node->points[i].radius = radius;

						// 이동 종료
						return;
					}
				}

				// 못찾았으면 문제가 있는 것임
				assert(0);
				return;
			}
			else
			{
				// Insert의 assert(nodeTable.count(dataPtr) == 0)에서 중단되는 경우가 가끔 발생한다.
				// Remove에서 데이터를 삭제하지 못한 경우로 예상되는데 확인이 필요하다... -> dataPtr로 삭제하는거로 삭제하게 함 아마 고쳐진듯?

				Remove(dataPtr);
				Insert(position, radius, dataPtr);
			}
		}

		// 특정 영역안에 포함되는 데이터의 목록을 가져온다.
		std::vector<DataType*> Query(const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			std::vector<DataType*> result;
			QueryRecursive(minPoint, maxPoint, root, result);
			return result;
		}

		void UpdateAllRadius()
		{
			UpdateAllRadiusRecursive(root);
		}

		void SetAutoRadiusUpdate(bool value)
		{
			autoRadiusUpdate = value;
		}

	private:
		OctreeNode* root;
		ZeldaMath::Vector3D rootMinPoint;
		ZeldaMath::Vector3D rootMaxPoint;
		ZeldaMath::Vector3D minAreaSize;

		std::unordered_map<DataType*, OctreeNode*> nodeTable;

		// 삽입, 삭제시에 자동으로 반지름 최대값을 업데이트한다.
		bool autoRadiusUpdate;


		// 계속 node가 NULL일 수 있다는 오류가 나는데 나중에 한번 확인해 볼 것
		void DestroyRecursive(OctreeNode* node)
		{
			assert(node != nullptr);
			if (node != nullptr) return;

			// root 노드가 아닌경우 parentNode를 따라가서 children에서 자신을 제거
			if (node->parentNode != nullptr)
			{
				for (size_t i = 0; i < 8; i++)
				{
					if (node->parentNode->children[i] == node)
					{
						node->parentNode->children[i] = nullptr;
					}
				}
			}
			// root 노드인경우 root = nullptr
			else
			{
				root = nullptr;
			}

			// nullptr이 아닌 모든 자식 노드에 대해서 DestroyRecursive
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					DestroyRecursive(node->children[i]);
				}
			}

			delete node;
		}
		
		// 자식노드의 번호 얻기
		int GetOctant(const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			int octant = 0;
			ZeldaMath::Vector3D midPoint = (minPoint + maxPoint) * 0.5f;

			if ((maxPoint.x - minPoint.x) * 0.5f >= minAreaSize.x && point.x > midPoint.x)
				octant |= 1;
			if ((maxPoint.y - minPoint.y) * 0.5f >= minAreaSize.y && point.y > midPoint.y)
				octant |= 2;
			if ((maxPoint.z - minPoint.z) * 0.5f >= minAreaSize.z && point.z > midPoint.z)
				octant |= 4;

			return octant;
		}

		ZeldaMath::Vector3D GetOctantMinPoint(int octant, const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			bool holdx = (maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x;
			bool holdy = (maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y;
			bool holdz = (maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z;

			ZeldaMath::Vector3D octantMinPoint = ZeldaMath::Vector3D(
				(octant & 1) ? (minPoint.x + maxPoint.x) * 0.5f : minPoint.x,
				(octant & 2) ? (minPoint.y + maxPoint.y) * 0.5f : minPoint.y,
				(octant & 4) ? (minPoint.z + maxPoint.z) * 0.5f : minPoint.z
			);

			if (holdx) octantMinPoint.x = minPoint.x;
			if (holdy) octantMinPoint.y = minPoint.y;
			if (holdz) octantMinPoint.z = minPoint.z;

			return octantMinPoint;
		}

		ZeldaMath::Vector3D GetOctantMaxPoint(int octant, const ZeldaMath::Vector3D& point, const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint) const
		{
			bool holdx = (maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x;
			bool holdy = (maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y;
			bool holdz = (maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z;

			ZeldaMath::Vector3D octantMaxPoint = ZeldaMath::Vector3D(
				(octant & 1) ? maxPoint.x : (minPoint.x + maxPoint.x) * 0.5f,
				(octant & 2) ? maxPoint.y : (minPoint.y + maxPoint.y) * 0.5f,
				(octant & 4) ? maxPoint.z : (minPoint.z + maxPoint.z) * 0.5f
			);

			if (holdx) octantMaxPoint.x = maxPoint.x;
			if (holdy) octantMaxPoint.y = maxPoint.y;
			if (holdz) octantMaxPoint.z = maxPoint.z;

			return octantMaxPoint;
		}

		void InsertRecursive(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr, OctreeNode* node)
		{
			const ZeldaMath::Vector3D& minPoint = node->minPoint;
			const ZeldaMath::Vector3D& maxPoint = node->maxPoint;

			// center가 영역 밖에 있는 경우 종료
			if (!Intersects(center, center, minPoint, maxPoint))
				return;

			// LeafNode면 데이터 삽입 후 종료
			if (node->IsLeafNode())
			{
				node->points.push_back({ center, radius, dataPtr });
				if (autoRadiusUpdate)
				{
					UpdateRadiusRecursive(node);
				}
				assert(nodeTable.count(dataPtr) == 0);
				nodeTable[dataPtr] = node;
				return;
			}

			// 더 이상 분할 하면 최소 크기보다 작아지는 경우 데이터 삽입 후 종료
			if (((maxPoint.x - minPoint.x) * 0.5f < minAreaSize.x) && ((maxPoint.y - minPoint.y) * 0.5f < minAreaSize.y) && ((maxPoint.z - minPoint.z) * 0.5f < minAreaSize.z))
			{
				node->points.push_back({ center, radius, dataPtr });
				if (autoRadiusUpdate)
				{
					UpdateRadiusRecursive(node);
				}
				assert(nodeTable.count(dataPtr) == 0);
				nodeTable[dataPtr] = node;
				return;
			}

			int octant = GetOctant(center, minPoint, maxPoint);

			ZeldaMath::Vector3D childMinPoint = GetOctantMinPoint(octant, center, minPoint, maxPoint);
			ZeldaMath::Vector3D childMaxPoint = GetOctantMaxPoint(octant, center, minPoint, maxPoint);

			// 찾고 있는 ocatant가 없다면 새로운 자식 노드를 만든다.
			if (node->children[octant] == nullptr)
			{
				node->children[octant] = new OctreeNode(childMinPoint, childMaxPoint, node);
			}

			// 재귀호출로 자식 노드에 데이터를 삽입한다.
			InsertRecursive(center, radius, dataPtr, node->children[octant]);
		}
		
		void UpdateRadiusRecursive(OctreeNode* node)
		{
			if (node == nullptr)
			{
				return;
			}

			float result = 0.0f;

			// 리프노드라면 모든 데이터의 반지름을 확인하여 최댓값을 구한다.
			if (node->IsLeafNode())
			{
				for (size_t i = 0; i < node->points.size(); i++)
				{
					float currentradius = node->points[i].radius;
					if (result < currentradius)
					{
						result = currentradius;
					}
				}
			}
			// 리프노드가 아니라면 모든 자식 노드에서 최댓값 확인
			else
			{
				for (size_t i = 0; i < 8; i++)
				{
					float currentradius = 0.0f;
					if (node->children[i] == nullptr) continue;

					currentradius = node->children[i]->maxRadius;

					if (result < currentradius)
					{
						result = currentradius;
					}
				}
			}

			float prevMax = node->maxRadius;
			node->maxRadius = result;

			// maxRadius가 변경됐다면 parentNode도 업데이트 한다.
			if (prevMax != result)
			{
				UpdateRadiusRecursive(node->parentNode);
			}
		}

		// 최대 반지름 리턴
		float UpdateAllRadiusRecursive(OctreeNode* node)
		{
			float result = 0.0f;

			// 리프노드라면 모든 데이터의 반지름을 확인하여 최댓값을 node에 저장하고 리턴
			if (node->IsLeafNode())
			{
				for (size_t i = 0; i < node->points.size(); i++)
				{
					float currentradius = node->points[i].radius;
					if (result < currentradius)
					{
						result = currentradius;
					}
				}

				node->maxRadius = result;

				return result;
			}

			// 모든 자식 노드의 값으로 최댓값을 찾는다.
			for (size_t i = 0; i < 8; i++)
			{
				if (node->children[i] == nullptr)
				{
					continue;
				}

				float currentradius = UpdateAllRadiusRecursive(node->children[i]);
				if (result < currentradius)
				{
					result = currentradius;
				}
			}

			node->maxRadius = result;

			return result;
		}

		void RemoveRecursive(const ZeldaMath::Vector3D& center, float radius, DataType* dataPtr, OctreeNode* node)
		{
			const ZeldaMath::Vector3D& minPoint = node->minPoint;
			const ZeldaMath::Vector3D& maxPoint = node->maxPoint;

			// center가 있지 않은 영역이면 종료
			if (!Intersects(center, center, minPoint, maxPoint))
				return;

			// 리프노드라면 dataPtr을 비교하여 데이터를 찾아 삭제 후 종료한다.
			if (node->IsLeafNode())
			{
				for (int i = 0; i < node->points.size(); i++)
				{
					if (node->points[i].dataPtr == dataPtr)
					{
						node->points.erase(node->points.begin() + i);
						assert(nodeTable.count(dataPtr) != 0);
						nodeTable.erase(dataPtr);
						if (autoRadiusUpdate)
						{
							UpdateRadiusRecursive(node);
						}
						break;
					}
				}

				ClearLeafNode(node);
				return;
			}

			int octant = GetOctant(center, minPoint, maxPoint);

			// 자식 노드에서 다시 탐색
			if (node->children[octant] != nullptr)
			{
				RemoveRecursive(center, radius, dataPtr, node->children[octant]);
			}
		}

		// 데이터가 삭제되어 리프노드가 아니게 된 노드를 정리하기 위한 함수
		void ClearLeafNode(OctreeNode* node)
		{
			// 리프노드라면 문제 없음
			if (node->IsLeafNode())
			{
				return;
			}

			// 자식 노드의 개수를 확인한다
			int childCount = 0;
			for (size_t i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					childCount++;
				}
			}

			// 자식노드가 없다면 데이터가 삭제된 리프노드이므로 제거한다. 제거 후 그 부모 오브젝트도 확인해야 한다.
			if (childCount == 0)
			{
				OctreeNode* parent = node->parentNode;
				DestroyRecursive(node);
				ClearLeafNode(parent);
			}
		}

		void QueryRecursive(const ZeldaMath::Vector3D& minPoint, const ZeldaMath::Vector3D& maxPoint, OctreeNode* node, std::vector<DataType*>& result) const
		{
			// node의 영역이 node안의 원소의 최대 반지름 크기만큼 확장된 영역과 탐색 영역이 겹치지 않는다면 종료
			if (!Intersects(minPoint, maxPoint, node->minPoint - ZeldaMath::Vector3D(node->maxRadius, node->maxRadius, node->maxRadius), node->maxPoint + ZeldaMath::Vector3D(node->maxRadius, node->maxRadius, node->maxRadius)))
				return;

			// LeafNode라면 각각의 원소의 반지름으로 조금 더 정확한 판정을 하여 결과에 추가
			if (node->IsLeafNode())
			{
				for (const DataSet& data : node->points)
				{
					if (Intersects(minPoint, maxPoint, data.point - ZeldaMath::Vector3D(data.radius, data.radius, data.radius), data.point + ZeldaMath::Vector3D(data.radius, data.radius, data.radius)))
					{
						result.push_back(data.dataPtr);
					}
				}
				return;
			}

			// 그 외의 경우에 모든 자식 노드들에 대해서 재귀호출
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i] != nullptr)
				{
					QueryRecursive(minPoint, maxPoint, node->children[i], result);
				}
			}
		}
		
		// point1이 이루는 영역과 point2가 이루는 영역이 겹치는지 확인 (겹치는 영역이 있다면 true)
		bool Intersects(ZeldaMath::Vector3D minPoint1, ZeldaMath::Vector3D maxPoint1, ZeldaMath::Vector3D minPoint2, ZeldaMath::Vector3D maxPoint2) const
		{
			if (maxPoint1.x < rootMinPoint.x) maxPoint1.x = rootMinPoint.x;
			if (maxPoint1.y < rootMinPoint.y) maxPoint1.y = rootMinPoint.y;
			if (maxPoint1.z < rootMinPoint.z) maxPoint1.z = rootMinPoint.z;

			if (rootMaxPoint.x < minPoint1.x) minPoint1.x = rootMaxPoint.x;
			if (rootMaxPoint.y < minPoint1.y) minPoint1.y = rootMaxPoint.y;
			if (rootMaxPoint.z < minPoint1.z) minPoint1.z = rootMaxPoint.z;

			if (maxPoint2.x < rootMinPoint.x) maxPoint2.x = rootMinPoint.x;
			if (maxPoint2.y < rootMinPoint.y) maxPoint2.y = rootMinPoint.y;
			if (maxPoint2.z < rootMinPoint.z) maxPoint2.z = rootMinPoint.z;

			if (rootMaxPoint.x < minPoint2.x) minPoint2.x = rootMaxPoint.x;
			if (rootMaxPoint.y < minPoint2.y) minPoint2.y = rootMaxPoint.y;
			if (rootMaxPoint.z < minPoint2.z) minPoint2.z = rootMaxPoint.z;

			if (maxPoint1.x < minPoint2.x || minPoint1.x > maxPoint2.x)
				return false;
			if (maxPoint1.y < minPoint2.y || minPoint1.y > maxPoint2.y)
				return false;
			if (maxPoint1.z < minPoint2.z || minPoint1.z > maxPoint2.z)
				return false;
			return true;
		}
	};
}